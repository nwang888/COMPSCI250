#include <stdio.h>
#include <stdlib.h>

int log2(int n) {
    int r=0;
    while (n>>=1) r++;
    return r;
}

int ones(int n) {
    return ((1<<n)-1);
}

int main(int argc, char * argv[]) {

    //read address as hex
    int address;
    sscanf(argv[2], "%x", &address);

    //start to read the inputted file
    FILE * f = fopen(argv[1], "r");

    // scan address bits and page size
    int address_bits;
    int page_size;
    fscanf(f, "%d", &address_bits);
    fscanf(f, "%d", &page_size);

    // calculate page offset
    int page_offset = log2(page_size);

    // get the vpn of the address by throwing away pageoffset bits
    int vpn = address >> page_offset;

    // plug those bits into the page table, iterate through values in the page table until we hit the vpn
    int i = 0;
    int cur_ppn;
    //iterate through the page table until we get the vpn we want
    while (true) {
        //DEPRECATED: if (fscanf(f, "%x", &cur_ppn) == EOF) return "PAGEFAULT\n";
        fscanf(f, "%d", &cur_ppn);
        if (i == vpn) break;
        i++;
    }

    // if the bits we get back the page table are -1, PAGEFAULT 
    if (cur_ppn == -1) printf("PAGEFAULT\n");

    // append the page offset bits from the address onto the bits we get from the page table
    else printf("%x\n",(cur_ppn << page_offset) | (ones(page_offset) & address));
}