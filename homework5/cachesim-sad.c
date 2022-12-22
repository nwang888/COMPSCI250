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

// given vars
int cache_size;
int associativity;
int block_size;

char instruction[5];
unsigned int address; // 24-bit hex
unsigned int access_size;

// calculated vars
int num_frames;
int num_sets;
int offset_bits;
int index_bits;
int tag_bits;

// intialize memory
unsigned char memory[1<<24] = {0};

// Linked List node
struct frame {
    int valid;
    int tag; // int because size always smaller or equal to 24 bits
    struct frame* next_frame;
    char data[1024]; // 2^10 max block size and then 2 hex + 1 null terminator
};

// Linked List head
struct head {
    int size;
    struct frame* first_frame;
};

// create a new frame at the start of the linked list, add a whole block of data from memory into it ; happens on load miss
void new_frame(struct head* h, int tag, int index, int offset) {
    h->size++;
    struct frame* current = h->first_frame;
    // if the list is too large, then need to chuck the last node in the list as per LRU
    if (h->size > associativity) {
        h->size--; // undo the size increment since we hit the associativity limit already
        // iterate through loop until the next next node is NULL
        while (current->next_frame->next_frame != NULL) {
            current->next_frame = NULL;
        }
        current = NULL; // make the next node null
    }
    // reset current to the top of the list
    current = h->first_frame;
    
    // if the list isnt empty, add to the top of the linked list and initialize
    if (current != NULL) {
        struct frame* temp = h->first_frame;
        h->first_frame = (struct frame*) calloc(1, sizeof(struct frame));
        h->first_frame->valid = 1;
        h->first_frame->tag = tag;
        h->first_frame->next_frame = temp;
        // insert two hex values per byte in the data array
        for (int i = 0 ; i < block_size ; i++) {
            // strcpy(h->first_frame->data[i], memory[address-offset + i]);
            h->first_frame->data[i] = memory[address-offset + i];
        }
        return;
    }
    else { // if the list is currently empty, add and intialize the frame
        h->first_frame = (struct frame*) calloc(1, sizeof(struct frame));
        h->first_frame->valid = 1;
        h->first_frame->tag = tag;
        // insert two hex values per byte in the data array
        for (int i = 0 ; i < block_size ; i++) {
            // strcpy(h->first_frame->data[i], memory[address-offset + i]);
            h->first_frame->data[i] = memory[address-offset + i];
        }
    }
    return;
}

// get the whole block of data from a frame, if data doesn't exist return 0 ; happens on load hit
char* get_data(struct head* h, int tag, int index, int offset) {
    struct frame* current = h->first_frame;
    struct frame* temp = h->first_frame;
    char result[access_size];
    if (current == NULL) return 0;
    if (current->tag == tag) {
        // iterate through the cache block till access size and then concatenate those values into a string
        for (int i = 0 ; i < access_size ; i++) {
            // need to grab from current->data[i] and put it into result at the relevant index
            // strcpy(result[i], current->data[i]);
            result[i] = current->data[i];
        }
        return result;
    }
    while (current->next_frame != NULL) {
        if (current->next_frame->tag == tag && current->next_frame->valid == 1) // found the data
            h->first_frame = current;
            current->next_frame = h->first_frame;
            current->next_frame->next_frame = temp;
            // iterate through the cache block till access size and then concatenate those values into a string
            for (int i = 0 ; i < access_size ; i++) {
                // need to grab from current->data[i] and put it into result at the relevant index
                // strcpy(result[i], current->next_frame->data[i]);
                result[i] = current->next_frame->data[i];
            }
        return result;
        current = current->next_frame;
    }
    return 0;
}

// // find a tag, if it exists, change its data, move it to the top else return 0 ; if store hit, then changes data, if not then return 0
// int change_data(FILE * f, struct head* h, int access_size, int tag, int index, int offset) { // write in may be a string
//     int found = 0;
//     struct frame* current = h->first_frame;
//     struct frame* previous = current;
//     int flag = 0;
//     unsigned char store_value[access_size]; //hex, value stored

//     while (current != NULL) {
//         if (current->tag == tag && current->valid == 1) {
            
//             // scan in the stored value
//             for (int i = 0 ; i < access_size ; i++) {
//                 fscanf(f, "%2hhx", store_value[i]);
//             }

//             // save the current frame's data
//             char* temp_data;

//             temp_data = current->data;

//             // delete this frame
//             previous->next_frame = current->next_frame;

//             // add a new frame to the top with the same data
//             new_frame(&cache[index], tag, index, offset);

//             // edit the new frame's data to reflect changes in data
//             for (int i = 0 ; i < 1024 ; i ++) {
//                 h->first_frame->data[i] = temp_data[i];
//             }
//             for (int i = 0 ; i < access_size ; i++) {
//                 h->first_frame->data[offset + i] = store_value[i];
//             }
//             found = 1;
//             return found;
//         }
//         // iterate through loop
//         current = current -> next_frame;
//         if (flag != 0) {
//             previous-> next_frame;
//         }
//         flag++;
//     }
//     // change memory
//     for (int i = 0 ; i < access_size ; i++) {
//         memory[address+i] = store_value[i];
//     }
//     return found;
// }

int main(int argc, char * argv[]) {

    // parts of the address
    int tag;
    int index;
    int offset;

    // grab all the command line inputs
    FILE * f = fopen(argv[1], "r");
    sscanf(argv[2], "%d", &cache_size);
    sscanf(argv[3], "%d", &associativity);
    sscanf(argv[4], "%d", &block_size);

    // calculate the size of the cache
    num_frames = cache_size/block_size;
    num_sets = num_frames/associativity;
    offset_bits = log2(block_size);
    index_bits = log2(num_sets);
    tag_bits = block_size - index_bits - offset_bits;

    // define cache
    struct head* cache = (struct head*) calloc(sizeof(struct head), num_sets);

    // initialize the cache as an array of linked list heads with size = 0
    // for (int i = 0 ; i < num_sets ; i++) {
    //     struct head* h = &cache[i];
    //     h->size = 0;
    // }

    // iterate through the trace file
    while (fscanf(f, "%s", instruction) != EOF) {
        // If Empty File, end the loop, also scans the command name

        // scans the address
        fscanf(f, "%x", &address);

        // break the address into its parts
        tag = address >> (index_bits + offset_bits);
        index = (address >> offset_bits) & ones(index_bits);
        offset = address & ones(offset_bits);

        // scans the access size
        fscanf(f, "%d", &access_size);

        // scans the stored value only if it's a store
        if (strcmp(instruction, "store") == 0) {
            struct head * h = &cache[index];
            int found = 0;
            struct frame* current = h->first_frame;
            struct frame* previous = current;
            int flag = 0;
            unsigned char store_value[access_size]; //hex, value stored

            while (current != NULL) {
                if (current->tag == tag && current->valid == 1) {
                    
                    // scan in the stored value
                    for (int i = 0 ; i < access_size ; i++) {
                        fscanf(f, "%2hhx", store_value[i]);
                    }

                    // save the current frame's data
                    char* temp_data;

                    temp_data = current->data;

                    // delete this frame
                    previous->next_frame = current->next_frame;

                    // add a new frame to the top with the same data
                    new_frame(h, tag, index, offset);

                    // edit the new frame's data to reflect changes in data
                    for (int i = 0 ; i < 1024 ; i ++) {
                        h->first_frame->data[i] = temp_data[i];
                    }
                    for (int i = 0 ; i < access_size ; i++) {
                        h->first_frame->data[offset + i] = store_value[i];
                    }
                    found = 1;
                    break;
                }
                // iterate through loop
                current = current -> next_frame;
                if (flag != 0) {
                    previous-> next_frame;
                }
                flag++;
            }
            // change memory
            for (int i = 0 ; i < access_size ; i++) {
                memory[address+i] = store_value[i];
            }

            if (found == 0){
                printf("store 0x%x miss\n", address);
            }
            else{
                printf("store 0x%x hit\n", address);
            }
        }
        else { // if its a load
            char* data = get_data(&cache[index], tag, index, offset);
            char* retrieved_data;
            if (data == 0) { // this means load miss
                new_frame(&cache[index], tag, index, offset);
                printf("load 0x%x miss %x\n", address, retrieved_data);
            }
            else {
                // update the linked list
                // -----------------------
                struct head * h = &cache[index];
                struct frame* current = h->first_frame;
                struct frame* previous = current;
                int flag = 0;

                while (current != NULL) {
                    if (current->tag == tag && current->valid == 1) {

                        // save the current frame's data
                        char* temp_data;

                        temp_data = current->data;

                        // delete this frame
                        previous->next_frame = current->next_frame;

                        // add a new frame to the top with the same data
                        new_frame(h, tag, index, offset);

                        // edit the new frame's data to reflect changes in data
                        for (int i = 0 ; i < 1024 ; i ++) {
                            h->first_frame->data[i] = temp_data[i];
                        }
                        break;
                    }
                    // iterate through loop
                    current = current -> next_frame;
                    if (flag != 0) {
                        previous-> next_frame;
                    }
                    flag++;
                }

                retrieved_data = data;
                // Return data
                printf("load 0x%x hit %x\n", address, retrieved_data);
            }
        }    
    }
    return 0;
}