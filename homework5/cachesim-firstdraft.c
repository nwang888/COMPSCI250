#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char memory[1<<24]

// Linked List node
struct frame {
    int valid;
    int tag; // int because size always smaller or equal to 24 bits
    struct frame* next_frame;
    char* data; // char so that each ptr points to 1 byte
};

// Linked List head
struct head {
    int size;
    struct frame* first_frame;
};

// based on t
int change_data(struct head* hd, int tag, int write_in) { // write in may be a string
    int found = 0;
    struct frame* current = hd->first_frame
    while (current != NULL) {
        if (current->tag == tag && current->valid == 1) {
            current->data = write_in;
            found = 1;
            return found;
        }
        // iterate through loop
        current = current -> next_frame;
    }
    return found;
}

void new_frame(struct head* h, int tag, int write_in) {
    h->size++;
    struct frame* current = h->first_frame;
    if (current == NULL) {
        h->first_frame = (struct frame*) calloc(1, sizeof(struct frame));
        h->first_frame->valid = 1;
        h->first_frame->tag = tag;
        h->first_frame->data = write_in;
        return;
    }
}

int get_data(struct head* h, int tag) {
    struct frame* current = h->first_frame;
    while (current != NULL) {
        if (current->tag == tag && current->valid == 1)
            return current->data;
        current = current->next_frame;
    }
    return 0;
}

int main(int argc, char * argv[]) {
    int cache_size;
    int associativity;
    int block_size;

    // grab all the command line inputs
    FILE * f = fopen(argv[1], "r");
    sscanf(argv[2], "%d", &cache_size);
    sscanf(argv[3], "%d", &associativity);
    sscanf(argv[4], "%d", &block_size);

    // iterate through the trace file
    while (true) {
        char command[5];
        unsigned int address; // 24-bit hex
        unsigned int access_size;

        // If Empty File, end the loop, also scans the command name
        if (fscanf(f, "%s", command) == EOF) // stores "store" or "load" into command
            break;

        // scans the address
        fscanf(f, "%x", &address);

        // scans the access size
        fscanf(f, "%d", &access_size);

        unsigned char store_value[access_size]; //hex, value stored

        // scans the stored value only if it's a store
        if (strcmp(command, "store") == 0) {
            for(int i = 0 ; i < access_size ; i++) {
                fscanf(f, "%2hhx", &store_value[i]);

                // Cache hit?
                if in cache or something

                // Write data into lower memory
            }
            // Return data
            printf("%s 0x%x %d", command, address, access_size)
        }
        else { // if its a load

            // Not cache hit?
            if not in cache
                // Locate a cache block to use

                // Read data from lower memory into cache block

            // Return data
            printf("%s 0x%x %s %x", command, address, hitormiss, retrieved_data)
        }    
    }
}
