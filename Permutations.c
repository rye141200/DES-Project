#include <stdio.h>
#include <stdint.h>
#include <inttypes.h> // just to use PRIu64 for printing uint64_t on the screen

#define ROTATE_RIGHT(value, amount, size) (((value >> amount) | (value << (size-amount))))

/*******************************************************************************************************
Function: permut_block
Return Type: uint64_t

Arguments:
    - const uint8_t permutation_table[]:
        + Unsigned constant array.
        + The permutation table that will be used (for ex. IP or INV_IP).
    - int permutation_table_size:
        + size of the permutaion table
    - const uint64_t non_permuted_block:
        + Unsigned constant 64 bit.
        + The data before permutation occurs on it.

Returned Value:
    + Unsigned constant.
    + The data after permutation occured on it (permuted_block).

Test:
    + Run test_permut_block() and compare the results.
    + Output on console should be: "The permuted 64-bit block in decimal is: 14699974583363760298".
********************************************************************************************************/
uint64_t permut_block(const uint8_t permutation_table[], int permutation_table_size, const uint64_t non_permuted_block);
void test_permut_block();
/*******************************************************************************************************/

int main() {
    
    // The Initial Permutation (IP)
    //// applied on the 64-bit block of plaintext
    static const uint8_t IP[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };
    


    // The Final Permutation (Inverse of The Initial Permutation (IP))
    //// applied on the 64-bit block result from the 16 rounds (after swapping L and R step)
    static const uint8_t INV_IP[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    //uncomment to test
    //test_permut_block();


    return 0;
}


uint64_t permut_block(const uint8_t permutation_table[], int permutation_table_size, const uint64_t non_permuted_block){

    uint64_t permuted_block = 0xFFFFFFFFFFFFFFFF;

    for (int i = 0; i < permutation_table_size; i++){

        if ((permutation_table[i]-1-i) > 0){
            permuted_block &= ((non_permuted_block << (permutation_table[i]-i-1)) | 
                ROTATE_RIGHT(0x7FFFFFFFFFFFFFFF, i, permutation_table_size));
        }
        else {
            permuted_block &= ((non_permuted_block >> (i-permutation_table[i]+1)) | 
                ROTATE_RIGHT(0x7FFFFFFFFFFFFFFF, i, permutation_table_size));
        }
        
    }

    return (permuted_block >> (64-permutation_table_size));
}

void test_permut_block(){
    
    // pre-defined 64-bit plaintext_block used to test
    static const uint64_t plaintext_block =
        0b0000000100100011010001010110011110001001101010111100110111101111;

    // pre-defined 64-bit permutation table used to test
    static const uint8_t permutation_table[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    static const int permutation_table_size = 64;
    
    // calling the function with the predefined arguments to test it
    uint64_t permuted_plaintext_block = permut_block(permutation_table, permutation_table_size, plaintext_block);

    printf("The permuted 64-bit block in decimal is: %" PRIu64 "\n", permuted_plaintext_block);

    /*
    original: 0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
    permuted should be: 1100 1100 0000 0000 1100 1100 1111 1111 1111 0000 1010 1010 1111 0000 1010 1010
    permuted in decimal should be: 14699974583363760298

    So output should be >> "The permuted 64-bit block in decimal is: 14699974583363760298"
    */
}
