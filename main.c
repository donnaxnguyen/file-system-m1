#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 512;

typedef struct Entry {
    char filename[256]; 
    int size; 
    int blockLocation; 
    time_t dateCreated; 
    time_t lastAccessed;
    time_t lastModified;
} Entry; 

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
        directory[i].dateCreated = time(null); 
        directory[i].lastAccessed = time(null); 
        directory[i].lastModified = time(null);
    }

    /* 
        Things left to do 
        - ask free space for remaining .75 blocks? or extra 1 block? 

        - set the first entry to "." directory
        - set the second entry to ".." directory 

        - write the directory to disk? 

        - return starting block number of the root directory (this is what we get when we ask free space for the blocks?)
    */
    


}