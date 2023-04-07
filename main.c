#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#define BLOCK_SIZE 512
#define MAGIC_NUM 0x12345678

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
FreeSpace* getFreeSpace = NULL; // using to store info about free blocks.. setting to null bc list is empty.

// initializing values in the vcb
void initializeVCB() {
    vcb.magicNum = MAGIC_NUM; 
    vcb.totalBlocks = 10000; // random number of total blocks
    vcb.blockSize = BLOCK_SIZE; 
    vcb.free_blocks = 3; // random number of free space starting at block 3
    vcb.root_directory = 33; // root directory starts from block 33
}

// initializing free space 
void initializeFreeSpace(){
    //assuming we are using a bitmap
    //assuming the default size will have 19531 blocks,
    //so we need 19531 bits or 2442 bytes or 5 blocks
    
    char *free_space_pointer;
    //this holds 2560 bytes
    free_space_pointer = malloc(BLOCK_SIZE * 5);
    //set first 6 bits to used/1 and mark the rest as free/0
    
    //This is wrong?... might need to create new struct for bitmap,
    //along with helper functions to set everything
    //memset(free_space_pointer, '1', 6);   
    //printf("After bit change: %d \n", free_space_pointer);

}




// initialize root directory 


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

// allocation of free space 


int main() {
    int entrySize = sizeof(Entry); //size of our Entry struct
    int entries = 60; //from what prof used as an example 

    //calculating total size of directory
    int directorySize = entries * entrySize; 
    printf("directory size: %d\n", directorySize);

    Entry* directory = (Entry*) malloc(entries * directorySize);
   
    //calculate how many blocks we need for the directory 
    int blocks = directorySize / BLOCK_SIZE;
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

    /* 
        Things left to do 
        - ask free space for remaining .75 blocks? or extra 1 block? 
        
        - set the first entry to "." directory
        - set the second entry to ".." directory 

        - write the directory to disk? 

        - return starting block number of the root directory (this is what we get when we ask free space for the blocks?)
    */
    initFileSystem();


}