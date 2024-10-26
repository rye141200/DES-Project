#include <stdio.h>
#include <stdint.h>
#include <inttypes.h> // just to use PRIu64 for printing uint64_t on the screen

/*******************************************************************************************************
Function: swap_left_and_right_halves
Return Type: uint64_t

Arguments:
    - uint64_t data_block_64bit:
        + Unsigned constant 64 bit.
        + The 64-bit data before swapping occurs on it.

Returned Value:
    + Unsigned 64 bit.
    + The 64-bit data after swapping occured on it.

Test:
    + Run test_swap_left_and_right_halves() and compare the results.
    + Output on console should be: "The swapped 64-bit block in decimal is: 9920249030613615975".
********************************************************************************************************/
uint64_t swap_left_and_right_halves(uint64_t data_block_64bit);
void test_swap_left_and_right_halves();
/*******************************************************************************************************/

int main() {
    
    //uncomment to test
    //test_swap_left_and_right_halves();

    return 0;
}


uint64_t swap_left_and_right_halves(uint64_t data_block_64bit){
    return (0xFFFFFFFF00000000 & (data_block_64bit << 32)) | (0x00000000FFFFFFFF & (data_block_64bit >> 32));
}

void test_swap_left_and_right_halves(){

    // pre-defined 64-bit data used to test
    static const uint64_t data_block_64bit =
        0b0000000100100011010001010110011110001001101010111100110111101111;

    uint64_t swapped_64bit_data = swap_left_and_right_halves(data_block_64bit);

    printf("The swapped 64-bit block in decimal is: %" PRIu64 "\n", swapped_64bit_data);

    /*
    original: 0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
    swapped should be: 1111 0000 1010 1010 1111 0000 1010 1010 0000 0001 0010 0011 0100 0101 0110 0111
    swapped in decimal should be: 9920249030613615975

    So output should be >> "The swapped 64-bit block in decimal is: 9920249030613615975"
    */
}
