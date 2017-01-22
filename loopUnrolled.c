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
