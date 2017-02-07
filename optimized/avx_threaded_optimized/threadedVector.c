#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "md5_vector_constants.h"
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>


// Initial first block values
unsigned int At = 0x67452301;
unsigned int Bt = 0xefcdab89;
unsigned int Ct = 0x98badcfe;
unsigned int Dt = 0x10325476;

// char *inputHash = "e80b5017098950fc58aad83c8c14978e"; //abcdef
// char *inputHash = "4e8645994a6f75c7a2ad4959061230c4"; //lmnopq
char *inputHash = "453e41d218e071ccfb2d1c99ce23906a"; //zzzzzz
//char *inputHash = "d6a280b23327bfbf909a3b44ee9b0891"; //Target hash


typedef struct {
    uint8_t first;
    uint8_t second;
    uint8_t third;
    uint8_t fourth;
    uint8_t fifth;
    uint8_t sixth;
} LetterBlock;

LetterBlock letterIndexes;

pthread_mutex_t nextBlockMutex;

/*Function Definitions*/
void setHashAnswer(char *);
int checkOutput(union Hash *Output);
void md5LoopUnrolled(union Chunk *c, union Hash *output);
void printOutputHash(union Hash Output);
void *thread();
uint8_t getNextLetterBlock(LetterBlock *);
/*End Function Definitions*/

uint8_t foundPassword = 0;


int main(int argc, char *argv[])
{
    //Load in the hash to find
    setHashAnswer(inputHash);
    
    //Get the number of threads available
    int numThreads = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t threads[numThreads];

    //Record the starting time
    struct timeval start, end;
    gettimeofday(&start, NULL);

    //Dispatch that many threads
    for (int i = 0; i < numThreads; i++) 
    {
        pthread_create(&threads[i], NULL, thread, NULL);
    }
    
    //Wait for all threads to finish
    for (int i = 0; i < numThreads; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    //Record the ending time after all threads have finished
    gettimeofday(&end, NULL);

    double secDiff = end.tv_sec - start.tv_sec;
    double nsecDiff = end.tv_usec - start.tv_usec;
    double elapsedTime = secDiff + nsecDiff / 1000000;

    long hashes = (letterIndexes.first * pow(NumLetters, 5)) +
        (letterIndexes.second * pow(NumLetters, 4)) +
        (letterIndexes.third * pow(NumLetters, 3)) +
        pow(NumLetters, 3) +
        pow(NumLetters, 2) +
        NumLetters;
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

void *thread() 
{
    union Chunk input[8];
    for (int index = 0; index < 8; index++)
    {
        memset(&input[index].C8[0],0,64);
        input[index].C8[6] = 0x80;
        input[index].C64[7] = 0x30;
    }

    union Hash output[8];

    LetterBlock letters;

    while (getNextLetterBlock(&letters) == 0) 
    {
        for (int index = 0; index < 8; index++)
        {
            input[index].C8[0] = alphabet[letters.first];
            input[index].C8[1] = alphabet[letters.second];
            input[index].C8[2] = alphabet[letters.third];
        }

        while (letters.fourth < NumLetters)
        {
            input[0].C8[3] = alphabet[letters.fourth];
            input[1].C8[3] = alphabet[letters.fourth];
            input[2].C8[3] = alphabet[letters.fourth];
            input[3].C8[3] = alphabet[letters.fourth];
            input[4].C8[3] = alphabet[letters.fourth + 1];
            input[5].C8[3] = alphabet[letters.fourth + 1];
            input[6].C8[3] = alphabet[letters.fourth + 1];
            input[7].C8[3] = alphabet[letters.fourth + 1];

            while (letters.fifth < NumLetters)
            {
                input[0].C8[4] = alphabet[letters.fifth];
                input[1].C8[4] = alphabet[letters.fifth];
                input[2].C8[4] = alphabet[letters.fifth + 1];
                input[3].C8[4] = alphabet[letters.fifth + 1];
                input[4].C8[4] = alphabet[letters.fifth];
                input[5].C8[4] = alphabet[letters.fifth];
                input[6].C8[4] = alphabet[letters.fifth + 1];
                input[7].C8[4] = alphabet[letters.fifth + 1];

                while (letters.sixth < NumLetters)
                {
                    input[0].C8[5] = alphabet[letters.sixth];
                    input[1].C8[5] = alphabet[letters.sixth + 1];
                    input[2].C8[5] = alphabet[letters.sixth];
                    input[3].C8[5] = alphabet[letters.sixth + 1];
                    input[4].C8[5] = alphabet[letters.sixth];
                    input[5].C8[5] = alphabet[letters.sixth + 1];
                    input[6].C8[5] = alphabet[letters.sixth];
                    input[7].C8[5] = alphabet[letters.sixth + 1];

                    md5LoopUnrolled(input, output);

                    for (int vecIndex = 0; vecIndex < 8; vecIndex++)
                    {
                        if (checkOutput(&output[vecIndex]) == 1)
                        {
                            foundPassword = 1;

                            printOutputHash(output[vecIndex]);
                            printf("The password was: ");

                            printf("%c", (uint8_t) input[vecIndex].C8[0]);
                            printf("%c", (uint8_t) input[vecIndex].C8[1]);
                            printf("%c", (uint8_t) input[vecIndex].C8[2]);
                            printf("%c", (uint8_t) input[vecIndex].C8[3] + 1);
                            printf("%c", (uint8_t) input[vecIndex].C8[4] - 1);
                            printf("%c", (uint8_t) input[vecIndex].C8[5] + 1);
                        }
                    }

                    letters.sixth += 2;
                }

                letters.sixth = 0;
                letters.fifth += 2;
            }

            letters.fifth = 0;
            letters.fourth += 2;
        }

        letters.fourth = 0;
    }

    return 0;
}

uint8_t getNextLetterBlock(LetterBlock *threadLetters) 
{
    if (foundPassword != 0)
        return 1;

    pthread_mutex_lock(&nextBlockMutex);
    
    letterIndexes.third++;
    if (letterIndexes.third >= NumLetters) 
    {
        letterIndexes.third = 0;
        letterIndexes.second++;
        
        if (letterIndexes.second >= NumLetters) 
        {
            letterIndexes.second = 0;
            letterIndexes.first++;

            if (letterIndexes.first >= NumLetters)
            {
                return 2;
            }
        }
    }
    
    threadLetters->first = letterIndexes.first;
    threadLetters->second = letterIndexes.second;
    threadLetters->third = letterIndexes.third;
    
    pthread_mutex_unlock(&nextBlockMutex);

    return 0;

}

int checkOutput(union Hash *Output)
{
   /* if ((Output->C8[0] == Known.C8[15]) &&
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
    }*/
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
