#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


//From the definitions here: https://en.wikipedia.org/wiki/MD5#Algorithm
#define F(B, C, D) ((B & C) | (~B & D))
#define G(B, C, D) ((B & C) | (C & ~D))
#define H(B, C, D) (B ^ C ^ D)
#define I(B, C, D) (C ^ (B | ~D))

#define LEFTROT(val, amt) val = (val << amt) | (val >> (32 - amt))

#define R1(A, B, C, D, x, s, T) \
			A += F(B, C, D) + x + T; \
			LEFTROT(A, s); \
			A += B; 

#define R2(A, B, C, D, x, s, T); \
			A += G(B, C, D) + x + T; \
			LEFTROT(A, s); \
			A += B;

#define R3(A, B, C, D, x, s, T); \
			A += H(B, C, D) + x + T; \
			LEFTROT(A, s); \
			A += B;

#define R4(A, B, C, D, x, s, T); \
			A += I(B, C, D) + x + T; \
			LEFTROT(A, s); \
			A += B;


#define A0 0x67452301;
#define B0 0xefcdab89;
#define C0 0x98badcfe;
#define D0 0x10325476;


int main(int argc, char *argv[]) {
	char *string = "The quick brown fox jumps over the lazy dog";
	printf("Input: %s\n", string);
	
	long length = strlen(string);
	long bits = length * 8;
	
	int numBlocks = bits / 512;
	int blockSize = bits % 512;
	int paddingLength = 512 - 64 - 1 - blockSize;
	
	printf("length: %ld\n", bits);
	printf("blocks: %d\n", numBlocks);
	printf("blocksize: %d\n", blockSize);
	printf("paddingLength: %d\n", paddingLength);
	
	int blankChars = paddingLength / 8;
	
	int messageLength = length + blankChars + 1 + 8;
	uint8_t message[messageLength];
	memcpy(message, string, length);
	
	message[length] = (uint8_t)0x80;
	
	long newLength = (numBlocks + 1) * 64 - 8;
	long offset = length + 1;
	for (; offset < newLength; offset++) {
		message[offset] = 0;
	}
	
	message[offset] = (bits >> 56) & 0xFF;
	message[offset + 1] = (bits >> 48) & 0xFF;
	message[offset + 2] = (bits >> 40) & 0xFF;
	message[offset + 3] = (bits >> 32) & 0xFF;
	message[offset + 4] = (bits >> 24) & 0xFF;
	message[offset + 5] = (bits >> 16) & 0xFF;
	message[offset + 6] = (bits >> 8) & 0xFF;
	message[offset + 7] = bits & 0xFF;
	
	printf("Final message: \n");
	
	for (int index = 0; index < messageLength; index++) {
		printf("%02x ", message[index]);
	}
	
	printf("\n");
	
	//Initialize the total values to the initial conditions
	uint32_t At = A0;
	uint32_t Bt = B0;
	uint32_t Ct = C0;
	uint32_t Dt = D0;
	
	for (int chunk = 0; chunk < messageLength; chunk += 64) {
		printf("\nChunk: %d\n", chunk);
		//Get a chunk of the message 
		//Unfortunately...a direct cast to an array of uint32_t's doesn't work
		//because the bytes are in the wrong order...so it takes this mess of shifting
		//There should be a much cleaner way to do this, but this seems to work for now
		uint32_t M[16] = {
			(message[chunk] << 24) | (message[chunk + 1] << 16) | (message[chunk + 2] << 8) | message[chunk + 3],
			(message[chunk + 4] << 24) | (message[chunk + 5] << 16) | (message[chunk + 6] << 8) | message[chunk + 7],
			(message[chunk + 8] << 24) | (message[chunk + 9] << 16) | (message[chunk + 10] << 8) | message[chunk + 11],
			(message[chunk + 12] << 24) | (message[chunk + 13] << 16) | (message[chunk + 14] << 8) | message[chunk + 15],
			(message[chunk + 16] << 24) | (message[chunk + 17] << 16) | (message[chunk + 18] << 8) | message[chunk + 19],
			(message[chunk + 20] << 24) | (message[chunk + 21] << 16) | (message[chunk + 22] << 8) | message[chunk + 23],
			(message[chunk + 24] << 24) | (message[chunk + 25] << 16) | (message[chunk + 26] << 8) | message[chunk + 27],
			(message[chunk + 28] << 24) | (message[chunk + 29] << 16) | (message[chunk + 30] << 8) | message[chunk + 31],
			(message[chunk + 32] << 24) | (message[chunk + 33] << 16) | (message[chunk + 34] << 8) | message[chunk + 35],
			(message[chunk + 36] << 24) | (message[chunk + 37] << 16) | (message[chunk + 38] << 8) | message[chunk + 39],
			(message[chunk + 40] << 24) | (message[chunk + 41] << 16) | (message[chunk + 42] << 8) | message[chunk + 43],
			(message[chunk + 44] << 24) | (message[chunk + 45] << 16) | (message[chunk + 46] << 8) | message[chunk + 47],
			(message[chunk + 48] << 24) | (message[chunk + 49] << 16) | (message[chunk + 50] << 8) | message[chunk + 51],
			(message[chunk + 52] << 24) | (message[chunk + 53] << 16) | (message[chunk + 54] << 8) | message[chunk + 55],
			(message[chunk + 56] << 24) | (message[chunk + 57] << 16) | (message[chunk + 58] << 8) | message[chunk + 59],
			(message[chunk + 60] << 24) | (message[chunk + 61] << 16) | (message[chunk + 62] << 8) | message[chunk + 63],
		};
		
		printf("Start of M: %x %x %x %x ... %x", M[0], M[1], M[2], M[3], M[15]);
		
		//Initialize the round A, B, C, D values to the total values		
		uint32_t A = At;
		uint32_t B = Bt;
		uint32_t C = Ct;
		uint32_t D = Dt;
		
		//First 16 rounds
		R1(A, B, C, D, M[0],  7,  0xd76aa478);
		R1(D, A, B, C, M[1],  12, 0xe8c7b756);
		R1(C, D, A, B, M[2],  17, 0x242070db);
		R1(B, C, D, A, M[3],  22, 0xc1bdceee);
		R1(A, B, C, D, M[4],  7,  0xf57c0faf);
		R1(D, A, B, C, M[5],  12, 0x4787c62a);
		R1(C, D, A, B, M[6],  17, 0xa8304613);
		R1(B, C, D, A, M[7],  22, 0xfd469501);
		R1(A, B, C, D, M[8],  7,  0x698098d8);
		R1(D, A, B, C, M[9],  12, 0x8b44f7af);
		R1(C, D, A, B, M[10], 17, 0xffff5bb1);
		R1(B, C, D, A, M[11], 22, 0x895cd7be);
		R1(A, B, C, D, M[12], 7,  0x6b901122);
		R1(D, A, B, C, M[13], 12, 0xfd987193);
		R1(C, D, A, B, M[14], 17, 0xa679438e);
		R1(B, C, D, A, M[15], 22, 0x49b40821);
		printf("\nIntermediate hashed value is: %x%x%x%x", A, B, C, D);

		//Second 16 rounds
		R2(A, B, C, D, M[1],  5,  0xf61e2562);
		R2(D, A, B, C, M[6],  9,  0xc040b340);
		R2(C, D, A, B, M[11], 14, 0x265e5a51);
		R2(B, C, D, A, M[0],  20, 0xe9b6c7aa);
		R2(A, B, C, D, M[5],  5,  0xd62f105d);
		R2(D, A, B, C, M[10], 9,  0x02441453);
		R2(C, D, A, B, M[15], 14, 0xd8a1e681);
		R2(B, C, D, A, M[4],  20, 0xe7d3fbc8);
		R2(A, B, C, D, M[9],  5,  0x21e1cde6);
		R2(D, A, B, C, M[14], 9,  0xc33707d6);
		R2(C, D, A, B, M[3],  14, 0xf4d50d87);
		R2(B, C, D, A, M[8],  20, 0x455a14ed);
		R2(A, B, C, D, M[13], 5,  0xa9e3e905);
		R2(D, A, B, C, M[2],  9,  0xfcefa3f8);
		R2(C, D, A, B, M[7],  14, 0x676f02d9);
		R2(B, C, D, A, M[12], 20, 0x8d2a4c8a);
		printf("\nIntermediate hashed value is: %x%x%x%x", A, B, C, D);

		//Third 16 rounds
		R3(A, B, C, D, M[5],  4,  0xfffa3942);
		R3(D, A, B, C, M[8],  11, 0x8771f681);
		R3(C, D, A, B, M[11], 16, 0x6d9d6122);
		R3(B, C, D, A, M[14], 23, 0xfde5380c);
		R3(A, B, C, D, M[1],  4,  0xa4beea44);
		R3(D, A, B, C, M[4],  11, 0x4bdecfa9);
		R3(C, D, A, B, M[7],  16, 0xf6bb4b60);
		R3(B, C, D, A, M[10], 23, 0xbebfbc70);
		R3(A, B, C, D, M[13], 4,  0x289b7ec6);
		R3(D, A, B, C, M[0],  11, 0xeaa127fa);
		R3(C, D, A, B, M[3],  16, 0xd4ef3085);
		R3(B, C, D, A, M[6],  23, 0x04881d05);
		R3(A, B, C, D, M[9],  4,  0xd9d4d039);
		R3(D, A, B, C, M[12], 11, 0xe6db99e5);
		R3(C, D, A, B, M[15], 16, 0x1fa27cf8);
		R3(B, C, D, A, M[2],  23, 0xc4ac5665);
		printf("\nIntermediate hashed value is: %x%x%x%x", A, B, C, D);

		//Fourth 16 rounds
		R4(A, B, C, D, M[0],  6,  0xf4292244);
		R4(D, A, B, C, M[7],  10, 0x432aff97);
		R4(C, D, A, B, M[14], 15, 0xab9423a7);
		R4(B, C, D, A, M[5],  21, 0xfc93a039);
		R4(A, B, C, D, M[12], 6,  0x655b59c3);
		R4(D, A, B, C, M[3],  10, 0x8f0ccc92);
		R4(C, D, A, B, M[10], 15, 0xffeff47d);
		R4(B, C, D, A, M[1],  21, 0x85845dd1);
		R4(A, B, C, D, M[8],  6,  0x6fa87e4f);
		R4(D, A, B, C, M[15], 10, 0xfe2ce6e0);
		R4(C, D, A, B, M[6],  15, 0xa3014314);
		R4(B, C, D, A, M[13], 21, 0x4e0811a1);
		R4(A, B, C, D, M[4],  6,  0xf7537e82);
		R4(D, A, B, C, M[11], 10, 0xbd3af235);
		R4(C, D, A, B, M[2],  15, 0x2ad7d2bb);
		R4(B, C, D, A, M[9],  21, 0xeb86d391);
		
		At += A;
		Bt += B;
		Ct += C;
		Dt += D;
		
		printf("\n\nIntermediate hashed value is: %x%x%x%x\n", A, B, C, D);
	}
	
	printf("\n\nHashed value is: %x%x%x%x\n", At, Bt, Ct, Dt);
}
