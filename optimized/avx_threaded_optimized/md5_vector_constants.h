#include <stdint.h>
#include <stdlib.h>
#include <immintrin.h>

#ifndef _MD5_CONSTANTS
#define _MD5_CONSTANTS

//Store a single chunk of the padded message
union Chunk
{
	uint8_t		C8[64];
	uint32_t	C32[16];
	uint64_t	C64[8];
};

//Store the hashed value
union Hash
{
    uint8_t     C8[16];
    uint32_t    C32[4];
    uint64_t    C64[2];
};

union Hash Known;

unsigned int T[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,

0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,

0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,

0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

#define NumLetters 26
char alphabet[NumLetters] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

unsigned int S[64] = {
7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,

5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,

4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,

6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

#define EXPANDCHUNK(c, i) _mm256_set_epi32(c[0].C32[i], c[1].C32[i], c[2].C32[i], c[3].C32[i], c[4].C32[i], c[5].C32[i], c[6].C32[i], c[7].C32[i])

#define MAKE256(a) _mm256_set1_epi32(a)
#define AND256(a, b) _mm256_and_si256(a, b) //not a, then and a and b
#define ANDNOT256(a, b) _mm256_andnot_si256(a, b)
#define OR256(a, b) _mm256_or_si256(a, b)
#define XOR256(a, b) _mm256_xor_si256(a, b)
#define ADD256(a, b) _mm256_add_epi32(a, b)
#define SHIFTL256(a, b) _mm256_slli_epi32(a, b)
#define SHIFTR256(a, b) _mm256_srli_epi32(a, b)

#define MASK MAKE256(0xFFFFFFFF)

#define F(x, y, z) OR256(AND256(x, y), ANDNOT256(x, z))
#define G(x, y, z) OR256(AND256(x, z), ANDNOT256(z, y))
#define H(x, y, z) XOR256(XOR256(x, y), z)
#define I(x, y, z) XOR256(y, OR256(x, ANDNOT256(z, MASK)))

#define ROT_L(x, shift) OR256(SHIFTL256(x, shift), SHIFTR256(x, (32 - shift)))

#define Round1(a, b, c, d, x, s, t) \
a = ADD256(ADD256(a, F(b,c,d)), ADD256(x, MAKE256(t))); \
a = ROT_L(a, s); \
a = ADD256(a, b); \

#define Round2(a, b, c, d, x, s, t) \
a = ADD256(ADD256(a, G(b,c,d)), ADD256(x, MAKE256(t))); \
a = ROT_L(a, s); \
a = ADD256(a, b); \

#define Round3(a, b, c, d, x, s, t)  \
a = ADD256(ADD256(a, H(b,c,d)), ADD256(x, MAKE256(t))); \
a = ROT_L(a, s); \
a = ADD256(a, b); \

#define Round4(a, b, c, d, x, s, t)  \
a = ADD256(ADD256(a, I(b,c,d)), ADD256(x, MAKE256(t))); \
a = ROT_L(a, s); \
a = ADD256(a, b); \


#endif
