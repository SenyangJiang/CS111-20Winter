#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "ext2_fs.h"

int fd;
__u32 block_size;

unsigned long block_address(unsigned long blockNum) {
  return blockNum*block_size;
}

void read_dir_entry(__u32 group_block, __u32 inode_number, __u32 dir_entry_block) {
  __u32 entry_address = block_address(group_block+(dir_entry_block-1));
  __u32 offset_within_dir = 0;
  __u32 file_inode = 0;
  __u16 rec_len = 0;
  struct ext2_dir_entry dir_entry;
  while(1){
    if(offset_within_dir == block_size) {
      break;
    }
    pread(fd, &file_inode, 4, entry_address);
    pread(fd, &rec_len, 2, entry_address + 4);
    if(file_inode == 0) {
      entry_address += rec_len;
      offset_within_dir += rec_len;
      continue;
    }
    pread(fd, &dir_entry, sizeof(dir_entry), entry_address);
    fprintf(stdout, "DIRENT,%u,%u,%u,%hu,%hhu,'%.*s'\n", inode_number, offset_within_dir, file_inode, rec_len, dir_entry.name_len, dir_entry.name_len, dir_entry.name);
    entry_address += rec_len;
    offset_within_dir += rec_len;
  }
}

void read_indirect_block(int level, __u32 group_block, __u32 inode_number, __u32 logical_block_offset, __u32 indirect_block_entry, char type) {
   __u32 offset_within_block = 0; 
   __u32 referenced_block_entry = 0;
   __u32 indirect_block_addr = block_address(group_block+(indirect_block_entry-1));
   while(1){
     if(offset_within_block == block_size) {
       break;
     }
     pread(fd, &referenced_block_entry, 4, indirect_block_addr+offset_within_block);
     if(referenced_block_entry == 0){
       offset_within_block += 4;
       if(level == 1) {
	 logical_block_offset += 1;
       } else if(level == 2) {
	 logical_block_offset += block_size/4;
       } else if(level == 3) {
	 logical_block_offset += (block_size/4)*(block_size/4);
       }
       continue;
     }
     // read directory entries
     if(level == 1 && type == 'd') {
       read_dir_entry(group_block, inode_number, referenced_block_entry);
     }
     
     fprintf(stdout, "INDIRECT,%u,%d,%u,%u,%u\n", inode_number, level, logical_block_offset, indirect_block_entry, referenced_block_entry);
     // make recursive call if level 2 or 3
     if(level == 2) {
       read_indirect_block(1, group_block, inode_number, logical_block_offset, referenced_block_entry, type);
     }
     if(level == 3) {
       read_indirect_block(2, group_block, inode_number, logical_block_offset, referenced_block_entry, type);
     }
     // increase parameters
     offset_within_block += 4;
     if(level == 1) {
       logical_block_offset += 1;
     } else if(level == 2) {
       logical_block_offset += block_size/4;
     } else if(level == 3) {
       logical_block_offset += (block_size/4)*(block_size/4);
     }
   }
}


int main(int argc, char **argv)
{
  if(argc < 2) {
    fprintf(stderr, "file system image not specified");
    exit(1);
  }
  if(argc > 2) {
    fprintf(stderr, "too many arguments");
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);
  if(fd == -1) {
    fprintf(stderr, "fail to open file system image");
    exit(1);
  }

  // super block summary
  struct ext2_super_block sb;
  pread(fd, &sb, sizeof(sb), 1024);
  __u32 block_count = sb.s_blocks_count;
  __u32 inode_count = sb.s_inodes_count;
  block_size = EXT2_MIN_BLOCK_SIZE << sb.s_log_block_size;
  __u16 inode_size = sb.s_inode_size;
  __u32 blocks_per_group = sb.s_blocks_per_group;
  __u32 inodes_per_group = sb.s_inodes_per_group;
  __u32 first_data_block = sb.s_first_data_block;
  __u32 first_ino = sb.s_first_ino;
  fprintf(stdout, "SUPERBLOCK,%u,%u,%u,%hu,%u,%u,%u\n", block_count, inode_count, block_size, inode_size, blocks_per_group, inodes_per_group, first_ino);

  struct ext2_group_desc gd;
  __u16 group_number = 0;
  for(; group_number <= block_count/blocks_per_group; group_number++){
    __u32 group_block = first_data_block + group_number*blocks_per_group;
    // group summary
    pread(fd, &gd, sizeof(gd), block_address(group_block+1));
    __u16 free_blocks_count = gd.bg_free_blocks_count;
    __u16 free_inodes_count = gd.bg_free_inodes_count;
    __u32 block_bitmap_block_id = gd.bg_block_bitmap;
    __u32 inode_bitmap_block_id = gd.bg_inode_bitmap;
    __u32 inode_table_block_id = gd.bg_inode_table;
    __u32 temp_blocks_per_group = blocks_per_group;
    if(group_number == block_count/blocks_per_group){
      temp_blocks_per_group = block_count - group_number * blocks_per_group;
    }
    fprintf(stdout, "GROUP,%hu,%u,%u,%hu,%hu,%u,%u,%u\n", group_number, temp_blocks_per_group, inodes_per_group, free_blocks_count, free_inodes_count, block_bitmap_block_id, inode_bitmap_block_id, inode_table_block_id);
    // free block entries
    char* bitmap = malloc(block_size);
    pread(fd, bitmap, block_size, block_address(group_block+(block_bitmap_block_id-1)));
    for(size_t i = 0; i < temp_blocks_per_group/8; i++) {
      for(size_t j = 0; j < 8; j++) {
	      if(!(bitmap[i] & (1 << j))){
	        fprintf(stdout, "BFREE,%lu\n", i*8+j);
	      }
      }
    }

    // read I-node bitmap
    pread(fd, bitmap, block_size, block_address(group_block+(inode_bitmap_block_id-1)));
    struct ext2_inode current_inode;
    __u32 current_inode_number = 0;
    __u32 global_inode_number = 0;
    for(size_t i = 0; i < inodes_per_group/8; i++) {
      for(size_t j = 0; j < 8; j++) {
        current_inode_number = i * 8 + j + 1;
	global_inode_number = group_number * inodes_per_group + current_inode_number;
	if(!(bitmap[i] & (1 << j))){
	  // free I-node entries
	  fprintf(stdout, "IFREE,%u\n", global_inode_number);
	}
	else {
	  // I-node summary
	  pread(fd, &current_inode, inode_size, block_address(group_block+inode_table_block_id-1) + inode_size * (current_inode_number - 1));
          __u8 type;
          __u16 file_mode = current_inode.i_mode;
	  if (current_inode.i_mode == 0 || current_inode.i_links_count == 0) {
	    continue;
	  }
          if((file_mode & 0xF000) == 0x8000){
            type = 'f';
          } else if ((file_mode & 0xF000) == 0x4000){
            type = 'd';
          } else if ((file_mode & 0xF000) == 0xA000){
            type = 's';
          } else {
            type = '?';
          }
          __u16 mode = file_mode & 0x0FFF;
          __u16 uid = current_inode.i_uid;
          __u16 gid = current_inode.i_gid;
          __u16 links_count = current_inode.i_links_count;
          struct tm* change_time;
	  time_t ctime = current_inode.i_ctime;
          change_time = localtime(&ctime);
          struct tm* mod_time;
	  time_t mtime = current_inode.i_mtime;
          mod_time = localtime(&mtime);
          struct tm* acc_time;
	  time_t atime = current_inode.i_atime;
          acc_time = localtime(&atime);
          __u32 size = current_inode.i_size;
          __u32 blocks = current_inode.i_blocks;
	  char time_report[32];
          fprintf(stdout, "INODE,%u,%c,%o,%hu,%hu,%hu,", current_inode_number, type, mode, uid, gid, links_count);
	  strftime(time_report, 32, "%m/%d/%y %H:%M:%S", change_time); 
          fprintf(stdout, "%s,", time_report);
          strftime(time_report, 32, "%m/%d/%y %H:%M:%S", mod_time); 
          fprintf(stdout, "%s,", time_report);
	  strftime(time_report, 32, "%m/%d/%y %H:%M:%S", acc_time); 
          fprintf(stdout, "%s,", time_report);
          fprintf(stdout, "%u,%u", size, blocks);
	  if(type == 'f' || type == 'd') {
	    for(int i = 0; i < 15; i++) {
	      fprintf(stdout, ",%lu", block_address(group_block+current_inode.i_block[i]));
	    }
	  } else if(type == 's' && size >= sizeof(current_inode.i_block)) {
	    for(size_t i = 0; i < 15; i++){
              fprintf(stdout, ",%lu", block_address(group_block+current_inode.i_block[i]));
            }
	  }
	  fprintf(stdout, "\n");
	  // directory entries
	  if(type == 'd'){
            for (int i = 0; i < 12; i++) {
	      if(current_inode.i_block[i] != 0) {
		read_dir_entry(group_block, global_inode_number, current_inode.i_block[i]);
	      }
            }
          }
	  // indirect block references
	  if(type == 'f' || type == 'd') {
	    // single indirect block
	    if(current_inode.i_block[12] != 0) {
	      read_indirect_block(1, group_block, current_inode_number, 12, current_inode.i_block[12], type);
	    }
	    // double indirect block
	    if(current_inode.i_block[13] != 0) {
	      read_indirect_block(2, group_block, current_inode_number, 12+256, current_inode.i_block[13], type);
            }
	    // triple indirect blocks
	    if(current_inode.i_block[14] != 0) {
	      read_indirect_block(3, group_block, current_inode_number, 12+256+256*256, current_inode.i_block[14], type);
	    }
	  }

        }
      }
    }
  }

  exit(0);
}
