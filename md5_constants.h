#include <stdint.h>
#include <stdlib.h>

#ifndef _MD5_CONSTANTS
#define _MD5_CONSTANTS

union Chunk
{
	uint8_t		C8[64];
	uint32_t	C32[16];
	uint64_t	C64[8];

//	Chunk()
//	{
		//fills the first n bytes of the memory area pointed to by s with the constant byte c.
//		memset(&this->C8[0], 0 ,64);
//	};
};

#define F(x, y, z) ((x&y) | (~x&z))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define Round1(a, b, c, d, x, s, T) { \
        a += F(b,c,d) + x + (uint32_t)(T); \
        ROT_L (a, s); \
        a += b; \
    }
#define Round2(a, b, c, d, x, s, T) { \
        a += G (b,c,d) + x + (uint32_t)(T); \
        ROT_L (a, s); \
        a += b; \
    }
#define Round3(a, b, c, d, x, s, T) { \
        a += H (b,c,d) + x + (uint32_t)(T); \
        ROT_L (a, s); \
        a += b; \
    }
#define Round4(a, b, c, d, x, s, T) { \
        a += I (b,c,d) + x + (uint32_t)(T); \
        ROT_L (a, s); \
        a += b; \
    }

#define ROT_L(x, shift){\
 		((x<<shift)|(x>>(32-shift))); \
	}

#endif
