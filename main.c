#include <stdio.h>
#include <stdint.h>

#define MASK48BITS(v) (v & 0xFFFFFFFFFFFF)
#define EXTRACTBIT(input,bitPosition) ((input >> bitPosition) & 1)


void printHex(uint64_t input) {
    printf("Hexadecimal: 0x%llx \n", input);  // Correct syntax
}

//! Constants 
static const char expansionTable[] = {
    32,  1,  2,  3,  4,  5,  
     4,  5,  6,  7,  8,  9,  
     8,  9, 10, 11, 12, 13, 
    12, 13, 14, 15, 16, 17, 
    16, 17, 18, 19, 20, 21, 
    20, 21, 22, 23, 24, 25, 
    24, 25, 26, 27, 28, 29, 
    28, 29, 30, 31, 32,  1
};

static const char postSBoxPermutation[] = {
    16,  7, 20, 21, 
    29, 12, 28, 17, 
     1, 15, 23, 26, 
     5, 18, 31, 10, 
     2,  8, 24, 14, 
    32, 27,  3,  9, 
    19, 13, 30,  6, 
    22, 11,  4, 25
};

static const char SBoxTable[8][64] = {
    /* S1 */
    {
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,  
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,  
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0, 
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
    },
    /* S2 */
    {
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,  
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,  
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
    },
    {
    /* S3 */
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,  
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,  
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
    },
    {
    /* S4 */
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,  
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,  
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
    },
    {
    /* S5 */
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6, 
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14, 
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
    },
    {
    /* S6 */
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
    },
    {
    /* S7 */
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
    },
    {
    /* S8 */
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
    }
};

static const char PC1[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

static const char PC2[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

static const char iterationShiftTable[] = {
 /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
    1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
};

static const char InitialPermutationTable[] = {
    58, 50, 42, 34, 26, 18, 10,  2, 
    60, 52, 44, 36, 28, 20, 12,  4, 
    62, 54, 46, 38, 30, 22, 14,  6, 
    64, 56, 48, 40, 32, 24, 16,  8, 
    57, 49, 41, 33, 25, 17,  9,  1, 
    59, 51, 43, 35, 27, 19, 11,  3, 
    61, 53, 45, 37, 29, 21, 13,  5, 
    63, 55, 47, 39, 31, 23, 15,  7
};
//! structs
struct Unsigned48Bits {
    uint64_t value: 48;
};

void initUnsigned48Bits(struct Unsigned48Bits* u, uint64_t v) {
    u->value = MASK48BITS(v);
}

uint64_t getUnsigned48Bits(const struct Unsigned48Bits* u) {
    return u->value;
}

void setUnsigned48Bits(struct Unsigned48Bits* u, uint64_t v) {
    u->value = MASK48BITS(v);
}


//! Prototypes
//! *****************Rounds************************
struct Unsigned48Bits expansionPermutation(uint32_t RiPrev);
struct Unsigned48Bits XOR48Bits(struct Unsigned48Bits expandedPermuted, struct Unsigned48Bits Ki);
uint32_t SBox(struct Unsigned48Bits XORedExpandedPermutedWithKey);
uint32_t permutation(uint32_t SBoxed);
uint32_t XOR32Bits(uint32_t permuted, uint32_t LiPrev);
uint64_t Round(uint64_t input,struct Unsigned48Bits Ki);
uint64_t ExecuteAllRounds(uint64_t input,struct Unsigned48Bits Keys[]);

//! *****************Key manipulation************************
uint64_t PermutedChoiceOne(uint64_t key);
uint64_t LeftShiftKeys(int iteration,uint64_t key);
struct Unsigned48Bits PermutedChoiceTwo(uint64_t key);
void GenerateKeys(struct Unsigned48Bits subkeys[],uint64_t key);

int main() {
    /* struct Unsigned48Bits key;
    key.value = 0xB02679B49A5;
    struct Unsigned48Bits keys[16] = {
    {0xB02679B49A5},  // K1
    {0x69A659256A26},  // K2
    {0x45D48AB428D2},  // K3
    {0x7289D2A58257},  // K4
    {0x3CE80317A6C2},   // K5
    {0x23251E3C8545},     // K6
    {0x6C04950AE4C6},  // K7
    {0x5788386CE581},      // K8
    {0xC0C9E926B839},  // K9
    {0x91E307631D72},  // K10
    {0x211F830D893A},  // K11
    {0x7130E5455C54},  // K12
    {0x91C4D04980FC},  // K13
    {0x5443B681DC8D},  // K14
    {0xB691050A16B5},  // K15
    {0xCA3D03B87032}   // K16
};
    // printHex(Round(0xCC00CCFFF0AAF0AA,key));
    printHex(ExecuteAllRounds(0xCC00CCFFF0AAF0AA,keys)); */
    //printHex(PermutedChoiceOne(0x123456789ABCDEF));
    //printHex(PermutedChoiceTwo(LeftShiftKeys(0,PermutedChoiceOne(0x123456789ABCDEF))).value);
    struct Unsigned48Bits keys[16];
    GenerateKeys(keys,0x123456789ABCDEF);
    printHex(PermutedChoiceTwo(0xC332A83AB33C02).value);
    //for(int i =0;i<16;i++)
        //printHex(keys[i].value);
    return 0;
}

//! *****************Rounds************************
struct Unsigned48Bits expansionPermutation(uint32_t RiPrev) {
    struct Unsigned48Bits expanded;
    initUnsigned48Bits(&expanded, 0);

    for(int i = 0; i < 48; i++) {
        int bitPosition = expansionTable[i] - 1; 
        uint64_t bit = EXTRACTBIT(RiPrev, 31 - bitPosition);
        expanded.value = expanded.value | (bit << (47 - i));
    }
    return expanded;
}

struct Unsigned48Bits XOR48Bits(struct Unsigned48Bits expandedPermuted, struct Unsigned48Bits Ki) {
    struct Unsigned48Bits result;
    
    result.value = MASK48BITS(expandedPermuted.value^Ki.value);

    return result;
}



uint32_t SBox(struct Unsigned48Bits XORedExpandedPermutedWithKey) {
    uint64_t inputValue = XORedExpandedPermutedWithKey.value;
    
    uint32_t Sboxed=0;
    for(int i =0;i<8;i++){
        uint16_t currentSBoxPartition = (inputValue >> (48 - (i+1)*6))& 0x3F; 

        uint16_t sboxRow = ((currentSBoxPartition & 0x21) >> 5) <<1 | currentSBoxPartition & 1 ;

        uint16_t sboxColumn = (currentSBoxPartition& 0x1E) >>1;
        
        uint16_t index= sboxRow*16 + sboxColumn;

        //printf("Row: %d, Column: %d, Index: %d, Value:%d\n",sboxRow,sboxColumn,index,SBoxTable[i][index]);
        
        Sboxed |= SBoxTable[i][index] << (32-(i+1)*4);        
    }
    return Sboxed; 
}

uint32_t permutation(uint32_t SBoxed) {
    uint32_t permuted = 0;
    for(int i = 0; i < 32; i++) {
        int bitPosition = postSBoxPermutation[i] - 1; 
        uint32_t bit = EXTRACTBIT(SBoxed, 31 - bitPosition);
        permuted |= bit << (31 - i);
    }
    return permuted;
}

uint32_t XOR32Bits(uint32_t permuted, uint32_t LiPrev) {
    return permuted ^ LiPrev;
}

uint64_t Round(uint64_t input,struct Unsigned48Bits Ki){
    uint32_t RiPrev = input & 0x00000000FFFFFFFF;
    uint32_t LiPrev = input >> 32;
    
    //! 1) Expansion
    struct Unsigned48Bits pipeLineVariableOne = expansionPermutation(RiPrev);
    
    //! 2) XOR 48 bits
    pipeLineVariableOne = XOR48Bits(pipeLineVariableOne,Ki);

    //! 3) SBox
    uint32_t pipeLineVariableTwo = SBox(pipeLineVariableOne);
    
    //! 4) Permutation
    pipeLineVariableTwo = permutation(pipeLineVariableTwo);
    
    uint32_t Ri = XOR32Bits(LiPrev,pipeLineVariableTwo);
    uint32_t Li = RiPrev;
    
    return ((uint64_t)(Li)<<32) | Ri;
}

uint64_t ExecuteAllRounds(uint64_t input,struct Unsigned48Bits Keys[]){
    uint64_t pipeLineVariable = input;
    for(int i =0;i<16;i++)
        pipeLineVariable = Round(pipeLineVariable,Keys[i]);
    return pipeLineVariable;
}

//! *****************Key manipulation************************
uint64_t PermutedChoiceOne(uint64_t key){
    uint64_t permuted = 0;
    for(int i = 0; i < 56; i++) {
        int bitPosition = PC1[i] - 1; 
        uint64_t bit = EXTRACTBIT(key, 63 - bitPosition);
        permuted |=  (bit << (55 - i));
    }
    return permuted;
}

uint64_t LeftShiftKeys(int iteration,uint64_t key){
    uint32_t leftKeyPart = (key >> 28);
    uint32_t rightKeyPart = key & 0xFFFFFFF;
    int shiftBy = iterationShiftTable[iteration];
    //! We mask with 0FFF FFFF to maintain the 28 bits
    leftKeyPart = ((leftKeyPart << shiftBy) | (leftKeyPart >>(28-shiftBy)))&0x0FFFFFFF;
    rightKeyPart = ((rightKeyPart << shiftBy) | (rightKeyPart >>(28-shiftBy))) & 0x0FFFFFFF;
    return ((uint64_t)leftKeyPart) << 28 | rightKeyPart;
}

struct Unsigned48Bits PermutedChoiceTwo(uint64_t key){
    struct Unsigned48Bits permuted;

    for(int i = 0; i < 48; i++) {
        int bitPosition = PC2[i] - 1; 
        uint64_t bit = EXTRACTBIT(key, 55 - bitPosition);
        permuted.value |=  (bit << (47 - i));
    }
    return permuted;
}
//C3 32A8 3AB3 3C02
void GenerateKeys(struct Unsigned48Bits subkeys[],uint64_t key){
    uint64_t currentKey = PermutedChoiceOne(key);

    for(int i =0;i<16;i++){
        uint64_t temp = LeftShiftKeys(i,currentKey);
        // printHex(temp);
        subkeys[i] = PermutedChoiceTwo(temp);
        currentKey = temp;
    }
}
