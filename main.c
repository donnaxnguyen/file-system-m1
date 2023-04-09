#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#define BLOCK_SIZE 512
#define MAGIC_NUM 0x12345678
#define STORAGE_CAPACITY (10* 1024 * 1024) // 10MB in bytes

typedef struct Entry {
    char filename[256]; 
    int size; 
    int blockLocation; 
    time_t dateCreated; 
    time_t lastAccessed;
    time_t lastModified;
} Entry; 

// creating volume control block struct 
typedef struct VolumeControlBlock { 
    int magicNum;
    int totalBlocks; 
    int blockSize; 
    int free_blocks; 
    int root_directory;
} VolumeControlBlock;

// creating free space struct 
typedef struct FreeSpace { 
    int blockNumber; 
    struct FreeSpace* next;
} FreeSpace;

// setting global variable delcarations 
VolumeControlBlock vcb; // using to store info about the VCB's
FreeSpace* get_free_space = NULL; // using to store info about free blocks.. setting to null bc list is empty.

// initializing values in the vcb
void initializeVCB() {
    vcb.magicNum = MAGIC_NUM; 
    vcb.totalBlocks = STORAGE_CAPACITY / BLOCK_SIZE; // 20480 blocks
    vcb.blockSize = BLOCK_SIZE; 
    vcb.free_blocks = 6; // setting the starting block number to 6 since 1-5 are reserved in the VCB
    vcb.root_directory = 33; // root directory starts from block 33
}

// creating a global variable for the nodes of the free space linked list.
FreeSpace* free_space_node = NULL;

// initializing free space 
void initializeFreeSpace(){
    int totalBlocks = vcb.totalBlocks;
    int first_free_block = 6; // setting the first free space in linked list

    printf("default size (total blocks): %d\n", totalBlocks); // creating a print statement to show a starting block number for free space

    FreeSpace* current = NULL; // creating and initializing free space linked list

    for (int i = first_free_block; i < totalBlocks; i++) {
        // first need to allocate memory for free space
        FreeSpace* new_block = (FreeSpace*)malloc(sizeof(FreeSpace));
        if (new_block == NULL) {
            printf("error allocating memory for free space block %d\n", i);
            break;
        }

    // setting the block number and initializing the next pointer 
    (*new_block).blockNumber = i;
    (*new_block).next = NULL;
    
    // if first node, then set as the start of linked list, 
    // if not, then add to the end of the linked list
    if (free_space_node == NULL) {
        free_space_node = new_block;
    } else {
            (*current).next = new_block;
            }

    // update current node
    current = new_block;
    }

    vcb.free_blocks = first_free_block;
    printf("setting the starting block number of the free space list in the VCB: %d\n", first_free_block);
    printf("free space starts at block: %d\n", free_space_node->blockNumber);
}

// initializing file system 
void initFileSystem() {
    // malloc a block of memory 
    VolumeControlBlock *vcb;
    vcb = (struct VolumeControlBlock*) malloc(sizeof(BLOCK_SIZE));
    if(vcb == NULL){
        printf("Error in mallocing initfilesystem \n");
       //error
    }
    //reading the first block
    //int storeread = LBAread(vcb, 1, 0);brings an error?

    // look at the magic number 
    if(vcb->magicNum == MAGIC_NUM){//initialized already
        
    // if magic number matches, copy to vcb 
    
    }else{
    // if doesnt match, format the volume
    initializeVCB();
    initializeFreeSpace();
    }  
}

int main() {
    int entrySize = sizeof(Entry); //size of our Entry struct
    int entries = 60; //from what prof used as an example 

    //calculating total size of directory
    int directorySize = entries * entrySize; 
    printf("directory size: %d\n", directorySize);

    // calculating how many blocks we need for directory 
    int blocks = (directorySize + BLOCK_SIZE - 1) / BLOCK_SIZE;

    // calculating the actual directory size
    int actual_size = blocks * BLOCK_SIZE;

    // recalculate the no. of entries that can fit in the actual size of the directory 
    int actual_entries = actual_size / entrySize;

    // this will allocate memory for array of 'entry' structure with the size calculated of actual number of entry
    Entry* directory = (Entry*) malloc(actual_entries * directorySize);
   
    //prints out 33 blocks, when calculated it's 33.75...
    printf("number of blocks: %d\n", blocks); 

    //create a directory entry for each of the blocks using for loop
    for (int i = 0; i < blocks; i++) {
        strcpy(directory[i].filename, "hi");
        directory[i].size = 0; 
        directory[i].blockLocation = 0; 
        directory[i].dateCreated = time(NULL); 
        directory[i].lastAccessed = time(NULL); 
        directory[i].lastModified = time(NULL);
    }

    initFileSystem();

    //initialize first directory entry to "."
    strcpy(directory[0].filename, ".");
    directory[0].size = directorySize; 
    directory[0].blockLocation = 0; 
    directory[0].dateCreated = time(NULL);
    directory[0].lastAccessed = time(NULL);
    directory[0].lastModified = time(NULL);

    //initialize second directory entry to ".."
    strcpy(directory[1].filename, "..");
    directory[1].size = directorySize; 
    directory[1].blockLocation = 1; 
    directory[1].dateCreated = time(NULL);
    directory[1].lastAccessed = time(NULL);
    directory[1].lastModified = time(NULL);

    // allocating root directory starting from the block that was returned by free space allocation
    vcb.root_directory = (*free_space_node).blockNumber;
    printf("root directory starting block: %d\n", vcb.root_directory);

    // allocating the blocks for the root directory
    FreeSpace *current_block = free_space_node;
    for (int i = 0; i < 6; i++) {
    if (current_block == NULL) {
        printf("error: not enough free blocks for root directory.\n");
        break;
    }

    // updating the free space list and the VCB
    free_space_node = current_block->next;
    vcb.free_blocks = free_space_node->blockNumber;
    current_block = free_space_node;
}
    free(directory);
}