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



#endif
