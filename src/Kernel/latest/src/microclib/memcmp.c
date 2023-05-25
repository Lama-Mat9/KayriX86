#include <stddef.h>

int memcmp(const void* ptr1, const void* ptr2, size_t size) {
/*
    Compares the contents of two blocks of memory whose size is specified in the size parameter.

    If return value is
    0: The two blocks have the same contents
    positive: The first byte that differs is smaller in ptr1 than in ptr2
    negative:  The first byte that differs is smaller in ptr2 than in ptr1
*/

    //Make copies of the pointers to have char versions of them, performing byte-sized comparaisons
    const unsigned char* cptr1 = (const unsigned char*) ptr1;
    const unsigned char* cptr2 = (const unsigned char*) ptr2;


    //Each byte gets compared
    for (size_t i = 0; i < size; i++) {
        
        if (cptr1[i] < cptr2[i]) return -1;
        else if (cptr2[i] < cptr1[i]) return 1;
        //Else they are the same so we continue

    }
    
    //Since we haven't returned yet, they are the same blocks
    return 0;
}