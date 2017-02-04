#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "md5_constants.h"
#include <arpa/inet.h>
#include <sys/time.h>

// Initial first block values
uint32_t At = 0x67452301;
uint32_t Bt = 0xefcdab89;
uint32_t Ct = 0x98badcfe;
uint32_t Dt = 0x10325476;

char *inputHash = "e80b5017098950fc58aad83c8c14978e";


/*Function Definitions*/
long md5Search();
void setHashAnswer(char *);
int checkOutput(union Hash *Output);
void padSingleChunk(union Chunk *input, char *str);
void md5LoopUnrolled(union Chunk *c, union Hash *output);
void printPaddedChunk(union Chunk Input);
void printOutputHash(union Hash Output);
void runMD5(union Chunk message, char * passwd);
/*End Function Definitions*/


int main(int argc, char *argv[])
{
    setHashAnswer(inputHash);

    union Chunk message;

    struct timeval start, end;
    gettimeofday(&start, NULL);

	long hashes = md5Search();

    gettimeofday(&end, NULL);

    double secDiff = end.tv_sec - start.tv_sec;
    double nsecDiff = end.tv_usec - start.tv_usec;
    double elapsedTime = secDiff + nsecDiff / 1000000;
    double hashRate = (double)hashes / elapsedTime;

    printf("\n\nTotal elapsed time: %f\n", elapsedTime);
    printf("Hashes per second: %f\n", hashRate);

    return 0;
}

void setHashAnswer(char *input)
{
    sscanf(input, "%016llx%016llx", &Known.C64[1], &Known.C64[0]);
    
    printf("\nInput hash: %016llx%016llx\n\n", Known.C64[1], Known.C64[0]);
}

long md5Search()
{
	union Chunk Input;
    union Hash Output;
	memset(&Input.C8[0],0,64);
	Input.C8[6] = 0x80;
	Input.C64[7] = 0x30;

	for (int i = 0; i < NumLetters; i++)
	{
		Input.C8[5] = alphabet[i];
		for (int j = 0; j < NumLetters; j++)
		{
			Input.C8[4] = alphabet[j];
			for (int k = 0; k < NumLetters; k++)
			{
				Input.C8[3] = alphabet[k];
				for (int l = 0; l < NumLetters; l++)
				{
					Input.C8[2] = alphabet[l];
					for (int m = 0; m < NumLetters; m++)
					{
						Input.C8[1] = alphabet[m];
						for (int n = 0; n < NumLetters; n++)
						{
							Input.C8[0] = alphabet[n];
							md5LoopUnrolled(&Input, &Output);
							if (checkOutput(&Output) == 1)
							{
								printOutputHash(Output);

                                printf("The password was: ");
                                for (int index = 0; index < 6; index++)
                                {
                                    printf("%c", (uint8_t) Input.C8[index]);
                                }
								return (i * pow(NumLetters, 5)) + 
                                        (j * pow(NumLetters, 4)) + 
                                        (k * pow(NumLetters, 3)) + 
                                        (l * pow(NumLetters, 2)) + 
                                        (m * NumLetters) + n;
							}
						}
					}
				}
			}
		}
	}

    return 308915776; //26^6
}

int checkOutput(union Hash *Output)
{
    if ((Output->C8[0] == Known.C8[15]) &&
            (Output->C8[1] == Known.C8[14]) &&
            (Output->C8[2] == Known.C8[13]) &&
            (Output->C8[3] == Known.C8[12]) &&
            (Output->C8[4] == Known.C8[11]) &&
            (Output->C8[5] == Known.C8[10]) &&
            (Output->C8[6] == Known.C8[9]) &&
            (Output->C8[7] == Known.C8[8]) &&
            (Output->C8[8] == Known.C8[7]) &&
            (Output->C8[9] == Known.C8[6]) &&
            (Output->C8[10] == Known.C8[5]) &&
            (Output->C8[11] == Known.C8[4]) &&
            (Output->C8[12] == Known.C8[3]) &&
            (Output->C8[13] == Known.C8[2]) &&
            (Output->C8[14] == Known.C8[1]) &&
            (Output->C8[15] == Known.C8[0]))
    {
        printf("Found the password!!\n\n");
        return 1;   
    }

    return 0;
}

void runMD5(union Chunk message, char * passwd)
{
    //Where to store final hash
	union Hash output;

	padSingleChunk(&message, passwd);
	printPaddedChunk(message);
	md5LoopUnrolled(&message,&output);
	printOutputHash(output);
}

void printPaddedChunk(union Chunk Input)
{
	for (int index = 0; index < 63; index++)
	{
		 printf("%#02x ", (uint8_t) Input.C8[index]);

		 if(((index+1)%8)==0)
		 {
		 	printf("\n");
		 }
	}
	printf("\n");
}

void printOutputHash(union Hash Output)
{
    printf("Final Hash: ");
    for (int index = 0; index < 16; index++)
    {
        printf("%02x", Output.C8[index]);
    }
    printf("\n");
}

void padSingleChunk(union Chunk *input, char * str)
{
	memset(&input->C8[0],0,64);			//Initialize message with 0's
	int len = strlen(str);				//Get length of string
	memcpy(input, str, len);			//Copy Password Guess into union Chunk
	input->C8[len] = 0x80;				//Set Bit after Guess to 1
	input->C64[7] = len*8;				//Set Size of Guess in last 64 bits
}

void md5LoopUnrolled(union Chunk *c, union Hash *Output)
{
	uint32_t A = At;
	uint32_t B = Bt;
	uint32_t C = Ct;
	uint32_t D = Dt;

    // Round1
    Round1(A, B, C, D, c->C32[0],  S[0],  T[0]); // 1
    Round1(D, A, B, C, c->C32[1],  S[1],  T[1]); // 2
    Round1(C, D, A, B, c->C32[2],  S[2],  T[2]); // 3
    Round1(B, C, D, A, c->C32[3],  S[3],  T[3]); // 4

    Round1(A, B, C, D, c->C32[4],  S[4],  T[4]); // 5
    Round1(D, A, B, C, c->C32[5],  S[5],  T[5]); // 6
    Round1(C, D, A, B, c->C32[6],  S[6],  T[6]); // 7
    Round1(B, C, D, A, c->C32[7],  S[7],  T[7]); // 8

    Round1(A, B, C, D, c->C32[8],  S[8],  T[8]); // 9
    Round1(D, A, B, C, c->C32[9],  S[9],  T[9]); // 10
    Round1(C, D, A, B, c->C32[10], S[10], T[10]); // 11
    Round1(B, C, D, A, c->C32[11], S[11], T[11]); // 12

    Round1(A, B, C, D, c->C32[12], S[12], T[12]); // 13
    Round1(D, A, B, C, c->C32[13], S[13], T[13]); // 14
    Round1(C, D, A, B, c->C32[14], S[14], T[14]); // 15
    Round1(B, C, D, A, c->C32[15], S[15], T[15]); // 16

    // Round2
    Round2(A, B, C, D, c->C32[1],  S[16], T[16]); // 17
    Round2(D, A, B, C, c->C32[6],  S[17], T[17]); // 18
    Round2(C, D, A, B, c->C32[11], S[18], T[18]); // 19
    Round2(B, C, D, A, c->C32[0],  S[19], T[19]); // 20

    Round2(A, B, C, D, c->C32[5],  S[20], T[20]); // 21
    Round2(D, A, B, C, c->C32[10], S[21], T[21]); // 22
    Round2(C, D, A, B, c->C32[15], S[22], T[22]); // 23
    Round2(B, C, D, A, c->C32[4],  S[23], T[23]); // 24

    Round2(A, B, C, D, c->C32[9],  S[24], T[24]); // 25
    Round2(D, A, B, C, c->C32[14], S[25], T[25]); // 26
    Round2(C, D, A, B, c->C32[3],  S[26], T[26]); // 27
    Round2(B, C, D, A, c->C32[8],  S[27], T[27]); // 28

    Round2(A, B, C, D, c->C32[13], S[28], T[28]); // 29
    Round2(D, A, B, C, c->C32[2],  S[29], T[29]); // 30
    Round2(C, D, A, B, c->C32[7],  S[30], T[30]); // 31 
    Round2(B, C, D, A, c->C32[12], S[31], T[31]); // 32

    // Round3
    Round3(A, B, C, D, c->C32[5],  S[32], T[32]); // 33
    Round3(D, A, B, C, c->C32[8],  S[33], T[33]); // 34
    Round3(C, D, A, B, c->C32[11], S[34], T[34]); // 35
    Round3(B, C, D, A, c->C32[14], S[35], T[35]); // 36

    Round3(A, B, C, D, c->C32[1],  S[36], T[36]); // 37
    Round3(D, A, B, C, c->C32[4],  S[37], T[37]); // 38
    Round3(C, D, A, B, c->C32[7],  S[38], T[38]); // 39
    Round3(B, C, D, A, c->C32[10], S[39], T[39]); // 40

    Round3(A, B, C, D, c->C32[13], S[40], T[40]); // 41
    Round3(D, A, B, C, c->C32[0],  S[41], T[41]); // 42
    Round3(C, D, A, B, c->C32[3],  S[42], T[42]); // 43
    Round3(B, C, D, A, c->C32[6],  S[43], T[43]); // 44

    Round3(A, B, C, D, c->C32[9],  S[44], T[44]); // 45
    Round3(D, A, B, C, c->C32[12], S[45], T[45]); // 46
    Round3(C, D, A, B, c->C32[15], S[46], T[46]); // 47
    Round3(B, C, D, A, c->C32[2],  S[47], T[47]); // 48

    // Round4
    Round4(A, B, C, D, c->C32[0],  S[48], T[48]); // 49
    Round4(D, A, B, C, c->C32[7],  S[49], T[49]); // 50
    Round4(C, D, A, B, c->C32[14], S[50], T[50]); // 51
    Round4(B, C, D, A, c->C32[5],  S[51], T[51]); // 52

    Round4(A, B, C, D, c->C32[12], S[52], T[52]); // 53
    Round4(D, A, B, C, c->C32[3],  S[53], T[53]); // 54
    Round4(C, D, A, B, c->C32[10], S[54], T[54]); // 55
    Round4(B, C, D, A, c->C32[1],  S[55], T[55]); // 56

    Round4(A, B, C, D, c->C32[8],  S[56], T[56]); // 57
    Round4(D, A, B, C, c->C32[15], S[57], T[57]); // 58
    Round4(C, D, A, B, c->C32[6],  S[58], T[58]); // 59
    Round4(B, C, D, A, c->C32[13], S[59], T[59]); // 60

    Round4(A, B, C, D, c->C32[4],  S[60], T[60]); // 61
    Round4(D, A, B, C, c->C32[11], S[61], T[61]); // 62
    Round4(C, D, A, B, c->C32[2],  S[62], T[62]); // 63
    Round4(B, C, D, A, c->C32[9],  S[63], T[63]); // 64

    Output->C32[0] = At+A;
    Output->C32[1] = Bt+B;
    Output->C32[2] = Ct+C;
    Output->C32[3] = Dt+D;
}

