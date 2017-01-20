#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int A = 0x01234567;
	int B = 0x89ABCDEF;
	int C = 0xFEDCBA89;
	int D = 0x76543210;
	
	char *string = "abcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcde";
	printf("Input %s\n", string);
	
	long length = strlen(string) - 1;
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
		printf("%#02x", message[index]);
	}
}
