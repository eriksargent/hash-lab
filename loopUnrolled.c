#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "md5_constants.h"

int main(int argc, char *argv[])
{
	int A = 0x01234567;
	int B = 0x89ABCDEF;
	int C = 0xFEDCBA89;
	int D = 0x76543210;

	char *passwd = "abcdefghij";

    union Chunk message;						//message is 512 bits long
	memset(&message.C8[0], 0, 64);				//initialize
	int len = strlen(passwd);					//passwd length

	memcpy(&message, passwd, len); 				//place passwd into Chunk Message
	message.C8[len] = 0x80;						//set last bit = 1
	message.C64[7] = len * 8;					//set bit size 

	for (int index = 0; index < 63; index++) 
	{
		printf("%#02x \n", message.C8[index]);
	}
}

void functionCall(union Block *b, int H0, int H1, int H2, int H3)
{
	A = H0;
	B = H1;
	C = H2;
	D = H3;

	/* Round1 */
	Round1(A, B, C, D, b->C32[0],  7, (0xd76aa478)); //1
	Round1(D, A, B, C, b->C32[1], 12, (0xe8c7b756)); //2
    Round1(C, D, A, B, b->C32[2], 17, (0x242070db)); //3
    Round1(B, C, D, A, b->C32[3], 22, (0xc1bdceee)); //4
    
    Round1(A, B, C, D, b->C32[4],  7, (0xf57c0faf)); //5
    Round1(D, A, B, C, b->C32[5], 12, (0x4787c62a)); //6
    Round1(C, D, A, B, b->C32[6], 17, (0xa8304613)); //7
    Round1(B, C, D, A, b->C32[7], 22, (0xfd469501)); //8
    
    Round1(A, B, C, D, b->C32[8],  7, (0x698098d8)); //9
    Round1(D, A, B, C, b->C32[9], 12, (0x8b44f7af)); //10
    Round1(C, D, A, B, b->C32[10],17, (0xffff5bb1)); //11
    Round1(B, C, D, A, b->C32[11],22, (0x895cd7be)); //12
    
    Round1(A, B, C, D, b->C32[12], 7, (0x6b901122)); //13
    Round1(D, A, B, C, b->C32[13],12, (0xfd987193)); //14
    Round1(C, D, A, B, b->C32[14],17, (0xa679438e)); //15
    Round1(B, C, D, A, b->C32[15],22, (0x49b40821)); //16

	/* Round2 */
	Round2(A, B, C, D, b->C32[ 1], 5, (0xf61e2562)); //17
    Round2(D, A, B, C, b->C32[ 6] ,9, (0xc040b340)); //18 
    Round2(C, D, A, B, b->C32[11],14, (0x265e5a51)); //19
    Round2(B, C, D, A, b->C32[ 0],20, (0xe9b6c7aa)); //20 

    Round2(A, B, C, D, b->C32[ 5], 5, (0xd62f105d)); //21 
    Round2(D, A, B, C, b->C32[10], 9, (0x02441453)); //22 
    Round2(C, D, A, B, b->C32[15],14, (0xd8a1e681)); //23
    Round2(B, C, D, A, b->C32[ 4],20, (0xe7d3fbc8)); //24

    Round2(A, B, C, D, b->C32[ 9], 5, (0x21e1cde6)); //25 
    Round2(D, A, B, C, b->C32[14], 9, (0xc33707d6)); //26 
    Round2(C, D, A, B, b->C32[ 3],14, (0xf4d50d87)); //27
    Round2(B, C, D, A, b->C32[ 8],20, (0x455a14ed)); //28

    Round2(A, B, C, D, b->C32[13], 5, (0xa9e3e905)); //29 
    Round2(D, A, B, C, b->C32[ 2], 9, (0xfcefa3f8)); //30 
    Round2(C, D, A, B, b->C32[ 7],24, (0x676f02d9)); //31
    Round2(B, C, D, A, b->C32[12],20, (0x8d2a4c8a)); //32

	/* Round3 */
	Round3(A, B, C, D, b->C32[ 5], 4, (0xfffa3942)); // 33 
    Round3(D, A, B, C, b->C32[ 8],11, (0x8771f681)); // 34 
    Round3(C, D, A, B, b->C32[11],16, (0x6d9d6122)); // 35 
    Round3(B, C, D, A, b->C32[14],23, (0xfde5380c)); // 36 

    Round3(A, B, C, D, b->C32[ 1], 4, (0xa4beea44)); // 37 
    Round3(D, A, B, C, b->C32[ 4],11, (0x4bdecfa9)); // 38 
    Round3(C, D, A, B, b->C32[ 7],16, (0xf6bb4b60)); // 39 
    Round3(B, C, D, A, b->C32[10],23, (0xbebfbc70)); // 40 

    Round3(A, B, C, D, b->C32[13], 4, (0x289b7ec6)); // 41 
    Round3(D, A, B, C, b->C32[ 0],11, (0xeaa127fa)); // 42 
    Round3(C, D, A, B, b->C32[ 3],16, (0xd4ef3085)); // 43 
    Round3(B, C, D, A, b->C32[ 6],23, (0x04881d05)); // 44 

    Round3(A, B, C, D, b->C32[ 9], 4, (0xd9d4d039)); // 45 
    Round3(D, A, B, C, b->C32[12],11, (0xe6dB99e5)); // 46 
    Round3(C, D, A, B, b->C32[15],16, (0x1fa27cf8)); // 47 
    Round3(B, C, D, A, b->C32[ 2],23, (0xc4ac5665)); // 48 

	/* Round4 */
	Round4(A, B, C, D, b->C32[ 0], 6, (0xf4292244)); // 49 
    Round4(D, A, B, C, b->C32[ 7],10, (0x432aff97)); // 50 
    Round4(C, D, A, B, b->C32[14],15, (0xab9423a7)); // 51 
    Round4(B, C, D, A, b->C32[ 5],21, (0xfc93a039)); // 52 

    Round4(A, B, C, D, b->C32[12], 6, (0x655b59c3)); // 53 
    Round4(D, A, B, C, b->C32[ 3],10, (0x8f0ccc92)); // 54 
    Round4(C, D, A, B, b->C32[10],15, (0xffeff47d)); // 55 
    Round4(B, C, D, A, b->C32[ 1],21, (0x85845dd1)); // 56 

    Round4(A, B, C, D, b->C32[ 8], 6, (0x6fa87e4f)); // 57 
    Round4(D, A, B, C, b->C32[15],10, (0xfe2ce6e0)); // 58 
    Round4(C, D, A, B, b->C32[ 6],15, (0xa3014314)); // 59 
    Round4(B, C, D, A, b->C32[13],21, (0x4e0811a1)); // 60 

    Round4(A, B, C, D, b->C32[ 4], 6, (0xf7537e82)); // 61 
    Round4(D, A, B, C, b->C32[11],10, (0xbd3af235)); // 62 
    Round4(C, D, A, B, b->C32[ 2],15, (0x2ad7d2bb)); // 63 
    Round4(B, C, D, A, b->C32[ 9],21, (0xeb86d391)); // 64 
