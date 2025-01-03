/*
 * Filename: AkiFS.h
 * Author: Your Name
 * Description:
 */

#ifndef AKIFS_H
#define AKIFS_H

typedef struct BootBlock {
    const int group_num;
    const int block_size;
} BootBlock;

typedef struct BlockGroup {
    int inode_size;
} BlockGroup;

#endif /* AKIFS_H */
