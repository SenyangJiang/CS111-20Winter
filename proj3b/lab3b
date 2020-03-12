#!/usr/local/cs/bin/python3

#NAME: Senyang Jiang, Max Zhang
#EMAIL: senyangjiang@yahoo.com, pipi-max@hotmail.com
#ID: 505111806, 205171726

import csv
import math
import sys

block_dict = {}
inode_dict = {}
inconst_flag = False

class Inode:
    def __init__(self, free, allocated, link_count, dir_ref_list):
        self.free = free # on free list (bool)
        self.allocated = allocated # has an INODE entry (bool)
        self.link_count = link_count # link count
        self.dir_ref_list = dir_ref_list # info about directory that contains entry referencing this inode (directory inode number, name of the entry)

class Block:
    def __init__(self, free, inode_ref_list):
        self.free = free # on free list (bool)
        self.inode_ref_list = inode_ref_list  # info about inodes that refers to this block (level, inode number, offset)

def main():
    if len(sys.argv) != 2:
        sys.stderr.write("wrong number of arguments\n")
        exit(1)
        
    try:
        fid =  open(sys.argv[1], 'r')
    except:
        sys.stderr.write("fail to open file\n")
        exit(1)

    csv_reader = csv.reader(fid, delimiter=',')
    data = [[int(element) if element.isdigit() else element for element in row] for row in csv_reader]
    fid.close()
    
    # fill the data structure
    for row in data:
        if row[0] == "SUPERBLOCK":
            total_num_blocks = row[1]
            total_num_inodes = row[2]
            first_inode = row[7]
            block_size = row[3]
            inode_size = row[4]
        if row[0] == "GROUP":
            first_data_block = row[8] + math.ceil(total_num_inodes*inode_size/block_size)
        if row[0] == "BFREE":
            if row[1] not in block_dict:
                block_dict[row[1]] = Block(True, [])
            else:
                block_dict[row[1]].free = True
        if row[0] == "IFREE":
            if row[1] not in inode_dict:
                inode_dict[row[1]] = Inode(True, False, 0, [])
            else:
                inode_dict[row[1]].free = True
        if row[0] == "INODE":
            if row[1] not in inode_dict:
                inode_dict[row[1]] = Inode(False, True, row[6], [])
            else:
                inode_dict[row[1]].allocated = True
                inode_dict[row[1]].link_count = row[6]
            if len(row) == 27:
                for i in range(12,24): # check direct blocks referenced by the inode
                    if row[i] == 0:
                        continue
                    if row[i] not in block_dict:
                        block_dict[row[i]] = Block(False, [[0, row[1], i-12]])
                    else:
                        block_dict[row[i]].inode_ref_list.append([0, row[1], i-12])
                if row[24] != 0: # check indirect blocks
                    if row[24] not in block_dict:
                        block_dict[row[24]] = Block(False, [[1, row[1], 12]])
                    else:
                        block_dict[row[24]].inode_ref_list.append([1, row[1], 12])
                if row[25] != 0:
                    if row[25] not in block_dict:
                        block_dict[row[25]] = Block(False, [[2, row[1], 12+256]])
                    else:
                        block_dict[row[25]].inode_ref_list.append([2, row[1], 12+256])
                if row[26] != 0:
                    if row[26] not in block_dict:
                        block_dict[row[26]] = Block(False, [[3, row[1], 12+256+256*256]])
                    else:
                        block_dict[row[26]].inode_ref_list.append([3, row[1], 12+256+256*256])
        if row[0] == "INDIRECT":
            if row[5] not in block_dict:
                block_dict[row[5]] = Block(False, [[row[2]-1, row[1], row[3]]])
            else:
                block_dict[row[5]].inode_ref_list.append([row[2]-1, row[1], row[3]])
        if row[0] == "DIRENT":
            if row[3] not in inode_dict:
                inode_dict[row[3]] = Inode(False, False, 0, [[row[1], row[6]]])
            else:        
                inode_dict[row[3]].dir_ref_list.append([row[1], row[6]])

    # Block consistency Audit
    for block_num in block_dict:
        block = block_dict[block_num]

        #Invalid block number
        if block_num < 0 or block_num >= total_num_blocks:
            inconst_flag = True
            for inode_ref in block.inode_ref_list:
                print("INVALID", end=" ")
                if inode_ref[0] == 1:
                    print("INDIRECT", end=" ")
                elif inode_ref[0] == 2:
                    print("DOUBLE INDIRECT", end=" ")
                elif inode_ref[0] == 3:
                    print("TRIPLE INDIRECT", end=" ")
                print("BLOCK " + str(block_num) + " IN INODE " + str(inode_ref[1]) + " AT OFFSET " + str(inode_ref[2]))
        #Reserved block number
        elif block_num < first_data_block:
            inconst_flag = True
            for inode_ref in block.inode_ref_list:
                print("RESERVED", end=" ")
                if inode_ref[0] == 1:
                    print("INDIRECT", end=" ")
                elif inode_ref[0] == 2:
                    print("DOUBLE INDIRECT", end=" ")
                elif inode_ref[0] == 3:
                    print("TRIPLE INDIRECT", end=" ")
                print("BLOCK " + str(block_num) + " IN INODE " + str(inode_ref[1]) + " AT OFFSET " + str(inode_ref[2]))
    #Check all legal blocks
    for block_num in range(first_data_block, total_num_blocks):
        # unreferenced block
        if block_num not in block_dict:
            inconst_flag = True
            print("UNREFERENCED BLOCK " + str(block_num))
        else:
            block = block_dict[block_num]
            # allocated block on free list
            if block.free and len(block.inode_ref_list) != 0:
                inconst_flag = True
                print("ALLOCATED BLOCK " + str(block_num) + " ON FREELIST")
            # duplicate block
            if len(block.inode_ref_list) > 1:
                inconst_flag = True
                for inode_ref in block.inode_ref_list:
                    print("DUPLICATE", end=" ")
                    if inode_ref[0] == 1:
                        print("INDIRECT", end=" ")
                    elif inode_ref[0] == 2:
                        print("DOUBLE INDIRECT", end=" ")
                    elif inode_ref[0] == 3:
                        print("TRIPLE INDIRECT", end=" ")
                    print("BLOCK " + str(block_num) + " IN INODE " + str(inode_ref[1]) + " AT OFFSET " + str(inode_ref[2]))

    # Inode Allocation Audits
    if 2 not in inode_dict:
        inconst_flag = True
        print("UNALLOCATED INODE 2 NOT ON FREELIST")
    elif not inode_dict[2].free and not inode_dict[2].allocated:
        inconst_flag = True
        print("UNALLOCATED INODE 2 NOT ON FREELIST")
    elif inode_dict[2].free and inode_dict[2].allocated:
        inconst_flag = True
        print("ALLOCATED INODE 2 ON FREELIST")
        
    for inode_num in range(first_inode, total_num_inodes+1):
        if inode_num not in inode_dict:
            inconst_flag = True
            print("UNALLOCATED INODE " + str(inode_num) + " NOT ON FREELIST")
        elif not inode_dict[inode_num].free and not inode_dict[inode_num].allocated:
            inconst_flag = True
            print("UNALLOCATED INODE " + str(inode_num) + " NOT ON FREELIST")
        elif inode_dict[inode_num].free and inode_dict[inode_num].allocated:
            inconst_flag = True
            print("ALLOCATED INODE " + str(inode_num) + " ON FREELIST")

    # Directory Consistency Audits
    for inode_num in inode_dict:
        inode = inode_dict[inode_num]
        if inode_num < 1 or inode_num > total_num_inodes:
            inconst_flag = True
            for dir_ref in inode.dir_ref_list:
                print("DIRECTORY INODE " + str(dir_ref[0]) + " NAME " + dir_ref[1] + " INVALID INODE " + str(inode_num))
        elif not inode.allocated:
            inconst_flag = True
            for dir_ref in inode.dir_ref_list:
                print("DIRECTORY INODE " + str(dir_ref[0]) + " NAME " + dir_ref[1] + " UNALLOCATED INODE " + str(inode_num))
        elif inode.allocated:
            num_links = len(inode.dir_ref_list)
            if inode.link_count != num_links:
                inconst_flag = True
                print("INODE " + str(inode_num) + " HAS " + str(num_links) + " LINKS BUT LINKCOUNT IS " + str(inode.link_count))
        for dir_ref in inode.dir_ref_list:
            if dir_ref[0] == 2 and dir_ref[1] == "'..'" and inode_num != 2:
                inconst_flag = True
                print("DIRECTORY INODE 2 NAME '..' LINK TO INODE " + str(inode_num) + " SHOULD BE 2")
            if dir_ref[1] == "'.'" and dir_ref[0] != inode_num:
                inconst_flag = True
                print("DIRECTORY INODE " + str(dir_ref[0]) + " NAME '.' LINK TO INODE " + str(inode_num) + " SHOULD BE " + str(dir_ref[0]))
            if dir_ref[1] == "'..'" and not dir_ref[0] == 2:
                parent_inode_num = dir_ref[0]
                if parent_inode_num not in inode_dict:
                    continue
                parent_inode = inode_dict[parent_inode_num]
                for parent_dir_ref in parent_inode.dir_ref_list:
                    if parent_dir_ref[1] != "'.'" and parent_dir_ref[1] != "'..'":
                        if inode_num != parent_dir_ref[0]:
                            inconst_flag = True
                            print("DIRECTORY INODE " + str(parent_inode_num) + " NAME '..' LINK TO INODE " + str(inode_num) + " SHOULD BE " + str(parent_dir_ref[0]))
    if inconst_flag:
        exit(2)
    else:
        exit(0)
        
if __name__ == "__main__":
    main()
