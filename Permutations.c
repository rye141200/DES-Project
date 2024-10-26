#include <stdio.h>
#include <stdint.h>
#include <inttypes.h> // just to use PRIu64 for printing uint64_t on the screen

#define ROTATE_RIGHT_64BIT(value, amount) (((value >> amount) | (value << (64-amount))))

/*******************************************************************************************************
Function: permut_64bit_block
Return Type: uint64_t

Arguments:
    - const uint8_t permutation_table[]:
        + Unsigned constant array.
        + The 64 bit permutation table that will be used (for ex. IP or INV_IP).
    - const uint64_t non_permuted_64bit_block:
        + Unsigned constant 64 bit.
        + The 64-bit data before permutation occurs on it.

Returned Value:
    + Unsigned constant 64 bit.
    + The 64-bit data after permutation occured on it (permuted_64bit_block).

Test:
    + Run test_permut_64bit_block() and compare the results.
    + Output on console should be: "The permuted 64-bit block in decimal is: 14699974583363760298".
********************************************************************************************************/
uint64_t permut_64bit_block(const uint8_t permutation_table[], const uint64_t non_permuted_64bit_block);
void test_permut_64bit_block();
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
    //test_permut_64bit_block();


    return 0;
}


uint64_t permut_64bit_block(const uint8_t permutation_table[], const uint64_t non_permuted_64bit_block){

    uint64_t permuted_64bit_block = 0xFFFFFFFFFFFFFFFF;

    for (int i = 0; i < 64; i++){

        if ((permutation_table[i]-1-i) > 0){
            permuted_64bit_block &= ((non_permuted_64bit_block << (permutation_table[i]-i-1)) | 
                ROTATE_RIGHT_64BIT(0x7FFFFFFFFFFFFFFF, i));
        }
        else {
            permuted_64bit_block &= ((non_permuted_64bit_block >> (i-permutation_table[i]+1)) | 
                ROTATE_RIGHT_64BIT(0x7FFFFFFFFFFFFFFF, i));
        }

    }

    return permuted_64bit_block;

}

void test_permut_64bit_block(){

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
    
    // calling the function with the predefined arguments to test it
    uint64_t permuted_plaintext_block = permut_64bit_block(permutation_table, plaintext_block);

    printf("The permuted 64-bit block in decimal is: %" PRIu64 "\n", permuted_plaintext_block);

    /*
    original: 0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
    permuted should be: 1100 1100 0000 0000 1100 1100 1111 1111 1111 0000 1010 1010 1111 0000 1010 1010
    permuted in decimal should be: 14699974583363760298

    So output should be >> "The permuted 64-bit block in decimal is: 14699974583363760298"
    */

}
