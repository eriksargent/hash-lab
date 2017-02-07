#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "md5_vector_constants.h"
#include <arpa/inet.h>
#include <sys/time.h>
#include <immintrin.h>
#include <math.h>


// Initial first block values
unsigned int At = 0x67452301;
unsigned int Bt = 0xefcdab89;
unsigned int Ct = 0x98badcfe;
unsigned int Dt = 0x10325476;

//char *inputHash = "e80b5017098950fc58aad83c8c14978e";
char * inputHash = "453e41d218e071ccfb2d1c99ce23906a";//zzzzzz

/*Function Definitions*/
long md5Search();
void setHashAnswer(char *);
int checkOutput(union Hash *Output);
void md5LoopUnrolled(union Chunk *c, union Hash *output);
void printPaddedChunk(union Chunk Input);
void printOutputHash(union Hash Output);
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
	union Chunk Input[8];
    union Hash Output[8];

    for (int input = 0; input < 8; input++)
    {
        memset(&Input[input].C8[0],0,64);
        Input[input].C8[6] = 0x80;
        Input[input].C64[7] = 0x30;
    }

	for (int i = 0; i < NumLetters; i++)
	{
        for (int input = 0; input < 8; input++)
    		Input[input].C8[5] = alphabet[i];

		for (int j = 0; j < NumLetters; j++)
		{
            for (int input = 0; input < 8; input++)
                Input[input].C8[4] = alphabet[j];

			for (int k = 0; k < NumLetters; k++)
			{
                for (int input = 0; input < 8; input++)
                    Input[input].C8[3] = alphabet[k];

				for (int l = 0; l < NumLetters / 2; l += 2)
				{
                    Input[0].C8[2] = alphabet[l];
                    Input[1].C8[2] = alphabet[l];
                    Input[2].C8[2] = alphabet[l];
                    Input[3].C8[2] = alphabet[l];
                    Input[4].C8[2] = alphabet[l + 1];
                    Input[5].C8[2] = alphabet[l + 1];
                    Input[6].C8[2] = alphabet[l + 1];
                    Input[7].C8[2] = alphabet[l + 1];

					for (int m = 0; m < NumLetters / 2; m += 2)
					{
						Input[0].C8[1] = alphabet[m];
                        Input[1].C8[1] = alphabet[m];
                        Input[2].C8[1] = alphabet[m + 1];
                        Input[3].C8[1] = alphabet[m + 1];
                        Input[4].C8[1] = alphabet[m];
                        Input[5].C8[1] = alphabet[m];
                        Input[6].C8[1] = alphabet[m + 1];
                        Input[7].C8[1] = alphabet[m + 1];

						for (int n = 0; n < NumLetters / 2; n += 2)
						{
                            Input[0].C8[1] = alphabet[n];
                            Input[2].C8[1] = alphabet[n + 1];
                            Input[1].C8[1] = alphabet[n];
                            Input[3].C8[1] = alphabet[n + 1];
                            Input[4].C8[1] = alphabet[n];
                            Input[6].C8[1] = alphabet[n + 1];
                            Input[5].C8[1] = alphabet[n];
                            Input[7].C8[1] = alphabet[n + 1];

							md5LoopUnrolled(Input, Output);

                            for (int vecIndex = 0; vecIndex < 8; vecIndex++)
                            {
								if((Output->C8[0]^Known.C8[15])==0)
								{
	 	   							if (checkOutput(&Output[vecIndex]) == 1)
    								{
    									printOutputHash(Output[vecIndex]);

                	                    printf("The password was: ");
                    	                for (int index = 0; index < 6; index++)
                        	            {
                            	            printf("%c", (uint8_t) Input[vecIndex].C8[index]);
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
		}
	}

    return pow(NumLetters, 6);
}

int checkOutput(union Hash *Output)
{
/*    if ((Output->C8[0] == Known.C8[15]) &&
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
	*/
	 if(((Output->C8[0] ^ Known.C8[15]) ||
         (Output->C8[1] ^ Known.C8[14]) ||
         (Output->C8[7] ^ Known.C8[8]) ||
         (Output->C8[15] ^ Known.C8[0]))==0)
     {
        printf("Found the password!!\n\n");
    	return 1;
     }	

    return 0;
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

void md5LoopUnrolled(union Chunk c[8], union Hash Output[8])
{
    __m256i A = MAKE256(At);
    __m256i B = MAKE256(Bt);
    __m256i C = MAKE256(Ct);
    __m256i D = MAKE256(Dt);

    // Round1
    Round1(A, B, C, D, EXPANDCHUNK(c, 0),  S[0],  T[0]); // 1
    Round1(D, A, B, C, EXPANDCHUNK(c, 1),  S[1],  T[1]); // 2
    Round1(C, D, A, B, EXPANDCHUNK(c, 2),  S[2],  T[2]); // 3
    Round1(B, C, D, A, EXPANDCHUNK(c, 3),  S[3],  T[3]); // 4

    Round1(A, B, C, D, EXPANDCHUNK(c, 4),  S[4],  T[4]); // 5
    Round1(D, A, B, C, EXPANDCHUNK(c, 5),  S[5],  T[5]); // 6
    Round1(C, D, A, B, EXPANDCHUNK(c, 6),  S[6],  T[6]); // 7
    Round1(B, C, D, A, EXPANDCHUNK(c, 7),  S[7],  T[7]); // 8

    Round1(A, B, C, D, EXPANDCHUNK(c, 8),  S[8],  T[8]); // 9
    Round1(D, A, B, C, EXPANDCHUNK(c, 9),  S[9],  T[9]); // 10
    Round1(C, D, A, B, EXPANDCHUNK(c, 10), S[10], T[10]); // 11
    Round1(B, C, D, A, EXPANDCHUNK(c, 11), S[11], T[11]); // 12

    Round1(A, B, C, D, EXPANDCHUNK(c, 12), S[12], T[12]); // 13
    Round1(D, A, B, C, EXPANDCHUNK(c, 13), S[13], T[13]); // 14
    Round1(C, D, A, B, EXPANDCHUNK(c, 14), S[14], T[14]); // 15
    Round1(B, C, D, A, EXPANDCHUNK(c, 15), S[15], T[15]); // 16

    // Round2
    Round2(A, B, C, D, EXPANDCHUNK(c, 1),  S[16], T[16]); // 17
    Round2(D, A, B, C, EXPANDCHUNK(c, 6),  S[17], T[17]); // 18
    Round2(C, D, A, B, EXPANDCHUNK(c, 11), S[18], T[18]); // 19
    Round2(B, C, D, A, EXPANDCHUNK(c, 0),  S[19], T[19]); // 20

    Round2(A, B, C, D, EXPANDCHUNK(c, 5),  S[20], T[20]); // 21
    Round2(D, A, B, C, EXPANDCHUNK(c, 10), S[21], T[21]); // 22
    Round2(C, D, A, B, EXPANDCHUNK(c, 15), S[22], T[22]); // 23
    Round2(B, C, D, A, EXPANDCHUNK(c, 4),  S[23], T[23]); // 24

    Round2(A, B, C, D, EXPANDCHUNK(c, 9),  S[24], T[24]); // 25
    Round2(D, A, B, C, EXPANDCHUNK(c, 14), S[25], T[25]); // 26
    Round2(C, D, A, B, EXPANDCHUNK(c, 3),  S[26], T[26]); // 27
    Round2(B, C, D, A, EXPANDCHUNK(c, 8),  S[27], T[27]); // 28

    Round2(A, B, C, D, EXPANDCHUNK(c, 13), S[28], T[28]); // 29
    Round2(D, A, B, C, EXPANDCHUNK(c, 2),  S[29], T[29]); // 30
    Round2(C, D, A, B, EXPANDCHUNK(c, 7),  S[30], T[30]); // 31 
    Round2(B, C, D, A, EXPANDCHUNK(c, 12), S[31], T[31]); // 32

    // Round3
    Round3(A, B, C, D, EXPANDCHUNK(c, 5),  S[32], T[32]); // 33
    Round3(D, A, B, C, EXPANDCHUNK(c, 8),  S[33], T[33]); // 34
    Round3(C, D, A, B, EXPANDCHUNK(c, 11), S[34], T[34]); // 35
    Round3(B, C, D, A, EXPANDCHUNK(c, 14), S[35], T[35]); // 36

    Round3(A, B, C, D, EXPANDCHUNK(c, 1),  S[36], T[36]); // 37
    Round3(D, A, B, C, EXPANDCHUNK(c, 4),  S[37], T[37]); // 38
    Round3(C, D, A, B, EXPANDCHUNK(c, 7),  S[38], T[38]); // 39
    Round3(B, C, D, A, EXPANDCHUNK(c, 10), S[39], T[39]); // 40

    Round3(A, B, C, D, EXPANDCHUNK(c, 13), S[40], T[40]); // 41
    Round3(D, A, B, C, EXPANDCHUNK(c, 0),  S[41], T[41]); // 42
    Round3(C, D, A, B, EXPANDCHUNK(c, 3),  S[42], T[42]); // 43
    Round3(B, C, D, A, EXPANDCHUNK(c, 6),  S[43], T[43]); // 44

    Round3(A, B, C, D, EXPANDCHUNK(c, 9),  S[44], T[44]); // 45
    Round3(D, A, B, C, EXPANDCHUNK(c, 12), S[45], T[45]); // 46
    Round3(C, D, A, B, EXPANDCHUNK(c, 15), S[46], T[46]); // 47
    Round3(B, C, D, A, EXPANDCHUNK(c, 2),  S[47], T[47]); // 48

    // Round4
    Round4(A, B, C, D, EXPANDCHUNK(c, 0),  S[48], T[48]); // 49
    Round4(D, A, B, C, EXPANDCHUNK(c, 7),  S[49], T[49]); // 50
    Round4(C, D, A, B, EXPANDCHUNK(c, 14), S[50], T[50]); // 51
    Round4(B, C, D, A, EXPANDCHUNK(c, 5),  S[51], T[51]); // 52

    Round4(A, B, C, D, EXPANDCHUNK(c, 12), S[52], T[52]); // 53
    Round4(D, A, B, C, EXPANDCHUNK(c, 3),  S[53], T[53]); // 54
    Round4(C, D, A, B, EXPANDCHUNK(c, 10), S[54], T[54]); // 55
    Round4(B, C, D, A, EXPANDCHUNK(c, 1),  S[55], T[55]); // 56

    Round4(A, B, C, D, EXPANDCHUNK(c, 8),  S[56], T[56]); // 57
    Round4(D, A, B, C, EXPANDCHUNK(c, 15), S[57], T[57]); // 58
    Round4(C, D, A, B, EXPANDCHUNK(c, 6),  S[58], T[58]); // 59
    Round4(B, C, D, A, EXPANDCHUNK(c, 13), S[59], T[59]); // 60

    Round4(A, B, C, D, EXPANDCHUNK(c, 4),  S[60], T[60]); // 61
    Round4(D, A, B, C, EXPANDCHUNK(c, 11), S[61], T[61]); // 62
    Round4(C, D, A, B, EXPANDCHUNK(c, 2),  S[62], T[62]); // 63
    Round4(B, C, D, A, EXPANDCHUNK(c, 9),  S[63], T[63]); // 64

    A = ADD256(A, MAKE256(At));
    B = ADD256(B, MAKE256(Bt));
    C = ADD256(C, MAKE256(Ct));
    D = ADD256(D, MAKE256(Dt));

    unsigned int * Aout = (unsigned int *)&A;
    unsigned int * Bout = (unsigned int *)&B;
    unsigned int * Cout = (unsigned int *)&C;
    unsigned int * Dout = (unsigned int *)&D;

    for (int index = 0; index < 8; index++) 
    {
        Output[index].C32[0] = Aout[index];
        Output[index].C32[1] = Bout[index];
        Output[index].C32[2] = Cout[index];
        Output[index].C32[3] = Dout[index];
    }
}
