#define F(x, y, z) ((x&y) | (~x&z))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define Round1(a, b, c, d, x, s, t) \
	a = (a + F(b, c, d) + x + t); \
	a = (ROT_L(a, s)); \
	a = (a + b); \

#define Round2(a, b, c, d, x, s, t) \
	a = (a + G(b, c, d) + x + t); \
	a = (ROT_L(a, s)); \
	a = (a + b); \

#define Round3(a, b, c, d, x, s, t)  \
	a = (a + H(b, c, d) + x + t); \
	a = (ROT_L(a, s)); \
	a = (a + b); \

#define Round4(a, b, c, d, x, s, t)  \
	a = (a + I(b, c, d) + x + t); \
	a = (ROT_L(a, s)); \
	a = (a + b); \

#define ROT_L(x, shift) ((x<<shift)|(x>>(32-shift)))

__kernel void md5Search(__global char * i1, __global char * i2, __global char * i3, __global char * i4, __global char * i5, __global char * i6, __global uint * c1, __global uint * c2, __global uint * c3, __global uint * c4)
{

uint id = get_global_id(0);
__constant union Chunk
{
	uchar   C8[64];
	uint    C32[16];
	ulong	C64[8];
};

//Store the hashed value
__constant union Hash
{
	uchar   C8[16];
	uint	C32[4];
	ulong   C64[2];
};


__constant uint T[64] = {
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

__constant int S[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,

	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,

	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,

	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

	union Chunk c;
	for(int i=0;i<64;i++)
	{
		c.C8[i] = 0x00;	
	}
	c.C8[0] = *i1;
	c.C8[1] = *i2;
	c.C8[2] = *i3;
	c.C8[3] = *i4;
	c.C8[4] = *i5;
	c.C8[5] = *i6;

	c.C8[6] = 0x80;
	c.C64[7] = 0x30;

	uint At = 0x67452301;
	uint Bt = 0xefcdab89;
	uint Ct = 0x98badcfe;
	uint Dt = 0x10325476;

	uint A = At;
	uint B = Bt;
	uint C = Ct;
	uint D = Dt;

	// Round1
	Round1(A, B, C, D, c.C32[0], S[0], T[0]); // 1
	Round1(D, A, B, C, c.C32[1], S[1], T[1]); // 2
	Round1(C, D, A, B, c.C32[2], S[2], T[2]); // 3
	Round1(B, C, D, A, c.C32[3], S[3], T[3]); // 4

	Round1(A, B, C, D, c.C32[4], S[4], T[4]); // 5
	Round1(D, A, B, C, c.C32[5], S[5], T[5]); // 6
	Round1(C, D, A, B, c.C32[6], S[6], T[6]); // 7
	Round1(B, C, D, A, c.C32[7], S[7], T[7]); // 8

	Round1(A, B, C, D, c.C32[8], S[8], T[8]); // 9
	Round1(D, A, B, C, c.C32[9], S[9], T[9]); // 10
	Round1(C, D, A, B, c.C32[10], S[10], T[10]); // 11
	Round1(B, C, D, A, c.C32[11], S[11], T[11]); // 12

	Round1(A, B, C, D, c.C32[12], S[12], T[12]); // 13
	Round1(D, A, B, C, c.C32[13], S[13], T[13]); // 14
	Round1(C, D, A, B, c.C32[14], S[14], T[14]); // 15
	Round1(B, C, D, A, c.C32[15], S[15], T[15]); // 16

	// Round2
	Round2(A, B, C, D, c.C32[1], S[16], T[16]); // 17
	Round2(D, A, B, C, c.C32[6], S[17], T[17]); // 18
	Round2(C, D, A, B, c.C32[11], S[18], T[18]); // 19
	Round2(B, C, D, A, c.C32[0], S[19], T[19]); // 20

	Round2(A, B, C, D, c.C32[5], S[20], T[20]); // 21
	Round2(D, A, B, C, c.C32[10], S[21], T[21]); // 22
	Round2(C, D, A, B, c.C32[15], S[22], T[22]); // 23
	Round2(B, C, D, A, c.C32[4], S[23], T[23]); // 24

	Round2(A, B, C, D, c.C32[9], S[24], T[24]); // 25
	Round2(D, A, B, C, c.C32[14], S[25], T[25]); // 26
	Round2(C, D, A, B, c.C32[3], S[26], T[26]); // 27
	Round2(B, C, D, A, c.C32[8], S[27], T[27]); // 28

	Round2(A, B, C, D, c.C32[13], S[28], T[28]); // 29
	Round2(D, A, B, C, c.C32[2], S[29], T[29]); // 30
	Round2(C, D, A, B, c.C32[7], S[30], T[30]); // 31 
	Round2(B, C, D, A, c.C32[12], S[31], T[31]); // 32

	// Round3
	Round3(A, B, C, D, c.C32[5], S[32], T[32]); // 33
	Round3(D, A, B, C, c.C32[8], S[33], T[33]); // 34
	Round3(C, D, A, B, c.C32[11], S[34], T[34]); // 35
	Round3(B, C, D, A, c.C32[14], S[35], T[35]); // 36

	Round3(A, B, C, D, c.C32[1], S[36], T[36]); // 37
	Round3(D, A, B, C, c.C32[4], S[37], T[37]); // 38
	Round3(C, D, A, B, c.C32[7], S[38], T[38]); // 39
	Round3(B, C, D, A, c.C32[10], S[39], T[39]); // 40

	Round3(A, B, C, D, c.C32[13], S[40], T[40]); // 41
	Round3(D, A, B, C, c.C32[0], S[41], T[41]); // 42
	Round3(C, D, A, B, c.C32[3], S[42], T[42]); // 43
	Round3(B, C, D, A, c.C32[6], S[43], T[43]); // 44

	Round3(A, B, C, D, c.C32[9], S[44], T[44]); // 45
	Round3(D, A, B, C, c.C32[12], S[45], T[45]); // 46
	Round3(C, D, A, B, c.C32[15], S[46], T[46]); // 47
	Round3(B, C, D, A, c.C32[2], S[47], T[47]); // 48

	// Round4
	Round4(A, B, C, D, c.C32[0], S[48], T[48]); // 49
	Round4(D, A, B, C, c.C32[7], S[49], T[49]); // 50
	Round4(C, D, A, B, c.C32[14], S[50], T[50]); // 51
	Round4(B, C, D, A, c.C32[5], S[51], T[51]); // 52

	Round4(A, B, C, D, c.C32[12], S[52], T[52]); // 53
	Round4(D, A, B, C, c.C32[3], S[53], T[53]); // 54
	Round4(C, D, A, B, c.C32[10], S[54], T[54]); // 55
	Round4(B, C, D, A, c.C32[1], S[55], T[55]); // 56

	Round4(A, B, C, D, c.C32[8], S[56], T[56]); // 57
	Round4(D, A, B, C, c.C32[15], S[57], T[57]); // 58
	Round4(C, D, A, B, c.C32[6], S[58], T[58]); // 59
	Round4(B, C, D, A, c.C32[13], S[59], T[59]); // 60

	Round4(A, B, C, D, c.C32[4], S[60], T[60]); // 61
	Round4(D, A, B, C, c.C32[11], S[61], T[61]); // 62
	Round4(C, D, A, B, c.C32[2], S[62], T[62]); // 63
	Round4(B, C, D, A, c.C32[9], S[63], T[63]); // 64

	c1[id] = At + A;
	c2[id] = Bt + B;
	c3[id] = Ct + C;
	c4[id] = Dt + D;	
}

