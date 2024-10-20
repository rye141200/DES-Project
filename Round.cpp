#include <iostream>
#include <math.h>
using namespace std;
#define MASK48BITS(v) {v & 0xFFFFFFFFFFFF};
#define EXTRACTBIT(input,bitPosition) {(input>>bitPosition)&1}
void printHex(uint64_t input){
    std::cout << "Hexadecimal: " << "0x" << std::hex << input << std::endl;
}

//! Constants 
static char expansionTable[] = {
    32,  1,  2,  3,  4,  5,  
     4,  5,  6,  7,  8,  9,  
     8,  9, 10, 11, 12, 13, 
    12, 13, 14, 15, 16, 17, 
    16, 17, 18, 19, 20, 21, 
    20, 21, 22, 23, 24, 25, 
    24, 25, 26, 27, 28, 29, 
    28, 29, 30, 31, 32,  1
};


//! Type declarations
struct Unsigned48Bits{
    uint64_t value: 48;
    public:
        Unsigned48Bits(uint64_t v = 0){
            value = MASK48BITS(v); //! Masking to 48 bits
        } 
        uint64_t get() const {return value;}
        void set(uint64_t v) { value = MASK48BITS(v);}
};
/*
    * DES Round pipeline (...inputs)=>output notation:
    *    1) Expansion permutation: (R(i-1) - 32 bits) => Expanded R(i-1) 48 bits
    *    2) XOR: (Expanded R(i-1) - 48 bits, K(i) - 48 bits ) => XORED Output - 48 bits
    *    3) S-box: (XORed input - 48 bits) => Substituted output 32 bits
    *    4) Permutation: (Substituded input - 32 bits) => Permuted 32 bits
    *    5) XOR: (Permuted - 32 bits, L(i-1) ) => R(i)
 */

Unsigned48Bits expansionPermutation(uint32_t RiPrev);
Unsigned48Bits XOR48Bits(Unsigned48Bits expandedPermuted,Unsigned48Bits Ki);
uint32_t SBox(Unsigned48Bits XORedExpandedPermutedWithKey);
uint32_t permutation(uint32_t SBoxed);
uint32_t XOR32Bits(uint32_t permuted,uint32_t LiPrev);

int main(){
    // Optionally, print with "0x" prefix
    printHex(expansionPermutation(0x53214011).value);
}

Unsigned48Bits expansionPermutation(uint32_t RiPrev){
    Unsigned48Bits expanded;
    for(int i =0;i<48;i++){
        int bitPosition = expansionTable[i]-1; //! for 0 based indexing
        uint64_t bit = EXTRACTBIT(RiPrev,bitPosition);
        expanded.set(expanded.get() | (bit<<(47-i)));
    }
    return expanded;
}