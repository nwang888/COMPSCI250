#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int log2(int n) {
    int r=0;
    while (n>>=1) r++;
    return r;
}

int ones(int n) {
    return ((1<<n)-1);
}

struct cell {
    int valid;
    int tag;
    int recency;
};

// intialize memory
unsigned char memory[1<<24] = {0};

int main(int argc, char * argv[]) {
    // grab all the command line inputs
    // given vars
    int cache_size;
    int associativity;
    int block_size;
    FILE * f = fopen(argv[1], "r");
    sscanf(argv[2], "%d", &cache_size);
    sscanf(argv[3], "%d", &associativity);
    sscanf(argv[4], "%d", &block_size);

    // calculate the size of the cache
    int num_frames = cache_size*1024/block_size;
    int num_sets = num_frames/associativity;
    //printf("cache_size: %d, associativity: %d, blockSize: %d, num_frames: %d, num_sets: %d\n", cache_size, associativity, block_size, num_frames, num_sets);
    int offset_bits = log2(block_size);
    int index_bits = log2(num_sets);
    int tag_bits = block_size - index_bits - offset_bits;
    //printf("Initial Bit Calc off Bits: %d, Index Bits: %d, tagBits: %d\n", offset_bits, index_bits, tag_bits);
    // define the cache
    struct cell cache[num_sets][associativity];

    // intialize cache
    for (int i = 0 ; i < num_sets ; i++) {
        for (int j = 0 ; j < associativity ; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = -1;
            cache[i][j].recency = 0;
        }
    }

    // doing it properly lol - ok this segfaults everything
    // struct cell **cache = (struct cell**) calloc(sizeof(struct cell**), num_sets);
    
    // for(int i = 0; i < num_sets; i++) {
    //     cache[i] = (struct cell *) calloc(sizeof(struct cell *), associativity);
    //     for(int j = 0; j < associativity; j++) {
    //         cache[i][j].valid = 0;
    //         cache[i][j].tag = 0;
    //         cache[i][j].recency = 0;
    //     }
    // }

    char instruction[5];

    // iterate through the trace file
    while (fscanf(f, "%s", instruction) != EOF) {

        unsigned int address; // 24-bit hex
        int access_size;

        // scans the address
        fscanf(f, "%x", &address);
        // scans the access size
        fscanf(f, "%d", &access_size);

        // break the address into its parts
        int tag = address >> (index_bits + offset_bits);
        int index = (address >> offset_bits) & ones(index_bits);
        //printf("shift: %d, ones: %d indexbits: %d\n", address >> offset_bits, ones(index_bits), index_bits);
        int offset = address & ones(offset_bits);

        // miss or hit?
        int found = 0;
        int way;
        for (int i = 0 ; i < associativity ; i++) {
            if (cache[index][i].tag == tag && cache[index][i].valid == 1) {
                found = 1;
                way = i;
                break;
            }
        }

        // scans the stored value only if it's a store
        if (strcmp(instruction, "store") == 0) {
            // scan in the stored value to memory 
            for (int i = 0 ; i < access_size ; i++) {
                fscanf(f, "%2hhx", &memory[address + i]);
            }
            // probable debug: check to see if we are storing the right value into the address

            // STORE MISS
            if (found == 0) {
                printf("store 0x%x miss\n", address);
            }
            // STORE HIT: need to update recency
            else {
                cache[index][way].recency = -1;
                for (int i = 0 ; i < associativity ; i++) {
                    cache[index][i].recency++;
                }
                // if (index == 0) {
                
                printf("store 0x%x hit", address);
                // printf("tag %d, index %d\n", tag, index);
                // printf("cache at this index:\n");
                // for (int i = 0 ; i < associativity ; i++) {
                //     printf("%d ", cache[index][i].tag);
                // }
                // printf("\nrecency at this index:\n");
                // for (int i = 0 ; i < associativity ; i++) {
                //     printf("%d ", cache[index][i].recency);
                // }
                printf("\n");
                // }
            }
        }
        else { // if its a load

            // LOAD MISS: need to update recency, and bring in a new tag into the cache
            if (found == 0) {

                // remove the oldest block in the cache or any invalid block
                int largest_recency = -1;
                int largest_way = 0;
                for (int i = 0 ; i < associativity ; i++) {
                    if (cache[index][i].valid == 0) {
                        largest_way = i;
                        break;
                    }
                    if (cache[index][i].recency > largest_recency) {
                        largest_recency = cache[index][i].recency;
                        largest_way = i;
                    }
                }
                // bring a new block into the cache
                cache[index][largest_way].tag = tag;
                cache[index][largest_way].valid = 1;
                cache[index][largest_way].recency = -1;

                // update recency
                for (int i = 0 ; i < associativity ; i++) {
                    cache[index][i].recency++;
                }
                // sus code that just pulls from memory - fix later
                // if (index == 0) {
                printf("load 0x%x miss ", address);
                for (int i = 0 ; i < access_size ; i++) {
                    printf("%02hhx", memory[address + i]);
                }
                
                // printf("tag %d, index %d\n", tag, index);
                // printf("cache at this index:\n");
                // for (int i = 0 ; i < associativity ; i++) {
                //     printf("%d ", cache[index][i].tag);
                // }
                // printf("\nrecency at this index:\n");
                // for (int i = 0 ; i < associativity ; i++) {
                //     printf("%d ", cache[index][i].recency);
                // }
                printf("\n");
                // }
            }
            // LOAD HIT: need to update recency, return data from the recovered tag
            else {

                for (int i = 0 ; i < associativity ; i++) {
                    if (i != way) {
                        cache[index][i].recency++;
                    }
                }
                cache[index][way].recency = 0;

                // sus code that just pulls from memory - fix later
                // unsigned char retrieved_data[access_size];
                // if (index == 0) {
                printf("load 0x%x hit ", address);
                for (int i = 0 ; i < access_size ; i++) {
                    printf("%02hhx", memory[address + i]);
                }
                
                // printf("tag %d, index %d\n", tag, index);
                // printf("cache at this index:\n");
                // for (int i = 0 ; i < associativity ; i++) {
                //     printf("%d ", cache[index][i].tag);
                // }
                // printf("\nrecency at this index:\n");
                // for (int i = 0 ; i < associativity ; i++) {
                //     printf("%d ", cache[index][i].recency);
                // }
                printf("\n");
                // }
            }
        }   
    }       
    return 0;
}