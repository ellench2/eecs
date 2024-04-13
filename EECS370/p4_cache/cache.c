/*
 * EECS 370, University of Michigan
 * Project 4: LC-2K Cache Simulator
 * Instructions are found in the project spec.
 */

#include <stdio.h>
#include <math.h>
#define MAX_CACHE_SIZE 256
#define MAX_BLOCK_SIZE 256

extern int mem_access(int addr, int write_flag, int write_data);
extern int get_num_mem_accesses(void);

//Use this when calling printAction. Do not modify the enumerated type below.
enum actionType
{
    cacheToProcessor,
    processorToCache,
    memoryToCache,
    cacheToMemory,
    cacheToNowhere
};

typedef struct blockStruct
{
    int data[MAX_BLOCK_SIZE];
    int dirty;
    int valid;
    int lruLabel;
    int set;
    int tag;
} blockStruct;

typedef struct cacheStruct
{
    blockStruct blocks[MAX_CACHE_SIZE];
    int blockSize;
    int numSets;
    int blocksPerSet;
} cacheStruct;

/* Global Cache variable */
cacheStruct cache;

void printAction(int, int, enum actionType);
void printCache(void);

/*
 * Set up the cache with given command line parameters. This is
 * called once in main(). You must implement this function.
 */
void cache_init(int blockSize, int numSets, int blocksPerSet){
    cache.blockSize = blockSize;
    cache.numSets = numSets;
    cache.blocksPerSet = blocksPerSet;
    for (int i = 0; i < MAX_CACHE_SIZE; i++) {
        cache.blocks[i].dirty = 0;
        cache.blocks[i].valid = 0;
        cache.blocks[i].lruLabel = -1;
    }
    //initialize all valid bits to 0 and dirty bits to 0
    return;
}

int extract_set(int addr) {
    int shift_bits = log(cache.blockSize)/log(2);
    return (addr >> shift_bits) & (cache.numSets - 1);
    //return int corresponding to given address using bit extraction according to cache objects member vars
    //0 indexed
}

int extract_blockoffset(int addr) {
    return addr & (cache.blockSize - 1);
}

int extract_tag(int addr) {
    int shift_bits = log(cache.numSets)/log(2) + log(cache.blockSize)/log(2);
    return addr >> shift_bits;
    //extract MOST significant 16 - log(cache.numSets) - log(cache.blockSize)
}

int search_cache(int addr) {
    int block = -1;
    for (int i = extract_set(addr) * cache.blocksPerSet;
         i < (extract_set(addr) + 1) * cache.blocksPerSet; i++) {
        if (cache.blocks[i].valid == 1 && cache.blocks[i].tag == extract_tag(addr)) {
            block = i;
        }
    }
    return block;
    //search cache array of blocks in appropriate set
    //ie in blocks[] from extract_set(addr) * blocksPerSet to (extract_set(addr) + 1) * blocksPerSet
    //search cache for given address
}

int get_lru(int set) {
    //loop over given set, find which block has lru == log(blocks per set) - 1
    for (int i = set * cache.blocksPerSet;
         i < (set+1) * cache.blocksPerSet; i++) {
        if (cache.blocks[i].lruLabel == (cache.blocksPerSet - 1)) {
            return i;
        }
    }
    return -1;
    //get lru of given set in cache_array but looping over lru bits
}

int get_dirty(int block) {
    return cache.blocks[block].dirty;
    //bit extract dirty bit according to cache objects member vars
}

int empty_space(int set) {
    //return first line in cache in given set which has valid bit 0
    for (int i = set * cache.blocksPerSet;
         i < (set+1) * cache.blocksPerSet; i++) {
        if (cache.blocks[i].valid == 0) {
            return i;
        }
    }
    return -1;
    //check valid bits in given set, if all 1 return 0 ie no empty space
}

void update_lru(int set, int block) {
    int curr = cache.blocks[block].lruLabel;
    if (curr > 0) {
        for (int i = set * cache.blocksPerSet;
             i < (set+1) * cache.blocksPerSet; i++) {
            if (cache.blocks[i].valid != 0 && cache.blocks[i].lruLabel < curr) {
                cache.blocks[i].lruLabel++;
            }
            // if lru is less than that of block, increment
        }
    }
    else if (curr == -1) {
        for (int i = set * cache.blocksPerSet;
             i < (set+1) * cache.blocksPerSet; i++) {
            if (cache.blocks[i].valid != 0) {
                cache.blocks[i].lruLabel++;
            }
        }
    }
    cache.blocks[block].lruLabel = 0;
    //set lru of block to 0
    //update lru bits of all blocks in set if given block was used last
}

void pull_from_mem(int block, int addr) {
    for (int i = 0; i < cache.blockSize; i++) {
        int subtract = extract_blockoffset(addr) - i;
        cache.blocks[block].data[i] = mem_access(addr - subtract, 0, 0);
    }
    cache.blocks[block].valid = 1;
    cache.blocks[block].dirty = 0;
    cache.blocks[block].tag = extract_tag(addr);
    printAction(addr - extract_blockoffset(addr), cache.blockSize, 2);
    //pull appropriate block of data, according to addr, from memory, and write at cache.blocks[block]
}

void evict(int block, int set) {
    if (cache.blocks[block].dirty != 0) {
        for (int i = 0; i < cache.blockSize; i++) {
            int block_bits = log(cache.blockSize)/log(2);
            int set_bits = log(cache.numSets)/log(2);
            int both = block_bits + set_bits;
            int shift_tag = cache.blocks[block].tag << both;
            int shift_set = set << block_bits;
            int addr = shift_tag + shift_set + i;
            if (i == 0) {
                printAction(addr, cache.blockSize, 3);
            }
            mem_access(addr, 1, cache.blocks[block].data[i]);
        }
        //write back!
    }
    else {
        int block_bits = log(cache.blockSize)/log(2);
        int set_bits = log(cache.numSets)/log(2);
        int both = block_bits + set_bits;
        int shift_tag = cache.blocks[block].tag << both;
        int shift_set = set << block_bits;
        int addr = shift_tag + shift_set;
        printAction(addr, cache.blockSize, 4);
    }
    
}

void write_cache(int block, int addr, int write_data) {
    cache.blocks[block].data[extract_blockoffset(addr)] = write_data;
    cache.blocks[block].dirty = 1;
    //write write_data to cache at addr, in block
}
/*
 
 * Access the cache. This is the main part of the project,
 * and should call printAction as is appropriate.
 * It should only call mem_access when absolutely necessary.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads (fetch/lw) and 1 for writes (sw).
 * write_data is a word, and is only valid if write_flag is 1.
 * The return of mem_access is undefined if write_flag is 1.
 * Thus the return of cache_access is undefined if write_flag is 1.
 */
int cache_access(int addr, int write_flag, int write_data) {
    //search cache
    //if hit, either write or return data
    //if miss, check if there is empty space; if there is, write there; if not, find lru and write there
    //if valid bit 0, set to 1
    //if evicting, check if dirty bit is 1; if so, write back to memory
    //if read, return data
    //if write, write data and set dirty bit to  1
    //update lru in set give block that was used last
    
    int block = search_cache(addr);
    if (block == -1) {
        int empty = empty_space(extract_set(addr));
        int write_block = -1;
        if (empty != -1) {
            write_block = empty;
        }
        else {
            write_block = get_lru(extract_set(addr));
            evict(write_block, extract_set(addr));
        }
        pull_from_mem(write_block, addr);
        block = search_cache(addr);
    }
    update_lru(extract_set(addr), block);
    if (write_flag == 0) {
        printAction(addr, 1, 0);
        return cache.blocks[block].data[extract_blockoffset(addr)];
    }
    else {
        printAction(addr, 1, 1);
        write_cache(block, addr, write_data);
        return 0;
    }
}



/*
 * print end of run statistics like in the spec. **This is not required**,
 * but is very helpful in debugging.
 * This should be called once a halt is reached.
 * DO NOT delete this function, or else it won't compile.
 * DO NOT print $$$ in this function
 */
void printStats(void){
    return;
}

/*
 * Log the specifics of each cache action.
 *
 *DO NOT modify the content below.
 * address is the starting word address of the range of data being transferred.
 * size is the size of the range of data being transferred.
 * type specifies the source and destination of the data being transferred.
 *  -    cacheToProcessor: reading data from the cache to the processor
 *  -    processorToCache: writing data from the processor to the cache
 *  -    memoryToCache: reading data from the memory to the cache
 *  -    cacheToMemory: evicting cache data and writing it to the memory
 *  -    cacheToNowhere: evicting cache data and throwing it away
 */
void printAction(int address, int size, enum actionType type)
{
    printf("$$$ transferring word [%d-%d] ", address, address + size - 1);

    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    }
    else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    }
    else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    }
    else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    }
    else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
}

/*
 * Prints the cache based on the configurations of the struct
 * This is for debugging only and is not graded, so you may
 * modify it, but that is not recommended.
 */
void printCache(void)
{
    printf("\ncache:\n");
    for (int set = 0; set < cache.numSets; ++set) {
        printf("\tset %i:\n", set);
        for (int block = 0; block < cache.blocksPerSet; ++block) {
            printf("\t\t[ %i ]: {", block);
            for (int index = 0; index < cache.blockSize; ++index) {
                printf(" %i", cache.blocks[set * cache.blocksPerSet + block].data[index]);
            }
            printf(" }\n");
        }
    }
    printf("end cache\n");
}
