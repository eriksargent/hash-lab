#include <stdio.h>  
#include <stdlib.h>  
#include <time.h> 
#include <string.h>
#include <iostream>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (32)
#define Char_MEM_SIZE (8)
#define MAX_SOURCE_SIZE (0x100000)

union Hash
{
	unsigned char   C8[16];
	unsigned int	C32[4];
	unsigned long   C64[2];
};

union Hash Known;

int checkOutput(union Hash Output)
{
	if ((Output.C8[0] == Known.C8[15]) &&
		(Output.C8[1] == Known.C8[14]) &&
		(Output.C8[2] == Known.C8[13]) &&
		(Output.C8[3] == Known.C8[12]) &&
		(Output.C8[4] == Known.C8[11]) &&
		(Output.C8[5] == Known.C8[10]) &&
		(Output.C8[6] == Known.C8[9]) &&
		(Output.C8[7] == Known.C8[8]) &&
		(Output.C8[8] == Known.C8[7]) &&
		(Output.C8[9] == Known.C8[6]) &&
		(Output.C8[10] == Known.C8[5]) &&
		(Output.C8[11] == Known.C8[4]) &&
		(Output.C8[12] == Known.C8[3]) &&
		(Output.C8[13] == Known.C8[2]) &&
		(Output.C8[14] == Known.C8[1]) &&
		(Output.C8[15] == Known.C8[0]))
	{
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

void setHashAnswer(char *input)
{
	//sscanf(input, "%016llx%016llx", &Known.C64[1], &Known.C64[0]);

	Known.C8[15] = 0x11;
	Known.C8[14] = 0x64;
	Known.C8[13] = 0x9b;
	Known.C8[12] = 0x43;
	Known.C8[11] = 0x94;
	Known.C8[10] = 0xd0;
	Known.C8[9] = 0x9e;
	Known.C8[8] = 0x4a;
	Known.C8[7] = 0xba;
	Known.C8[6] = 0x13;
	Known.C8[5] = 0x2a;
	Known.C8[4] = 0xd4;
	Known.C8[3] = 0x9b;
	Known.C8[2] = 0xd1;
	Known.C8[1] = 0xe7;
	Known.C8[0] = 0xdb;
	printf("\nInput hash: %016llx%016llx\n\n", Known.C64[1], Known.C64[0]);
}

char *inputHash = "4e8645994a6f75c7a2ad4959061230c4"; //lmnopq

int main()
{
	setHashAnswer(inputHash);
	system("pause");

	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobj = NULL;
	cl_mem memobj2 = NULL;
	cl_mem memobj3 = NULL;
	cl_mem memobj4 = NULL;
	cl_mem memobj5 = NULL;
	cl_mem memobj6 = NULL;
	cl_mem memobj7 = NULL;
	cl_mem memobj8 = NULL;
	cl_mem memobj9 = NULL;
	cl_mem memobj10 = NULL;
	cl_program program = NULL;
	cl_kernel kernel[4] = { NULL, NULL, NULL, NULL };
	//cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;

	unsigned char input1a[4];
	unsigned char input2a[4];
	unsigned char input3a[4];
	unsigned char input4a[4];
	unsigned char input5a[4];
	unsigned char input6a[4];

	unsigned int output1a[4];
	unsigned int output2a[4];
	unsigned int output3a[4];
	unsigned int output4a[4];

	FILE *fp;
	char fileName[] = "./kernel.cl";
	char *source_str;
	size_t source_size;

	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/* Create Memory Buffer */
	memobj = clCreateBuffer(context, CL_MEM_READ_ONLY, 4*sizeof(char), NULL, &ret);
	memobj2 = clCreateBuffer(context, CL_MEM_READ_ONLY, 4 * sizeof(char), NULL, &ret);
	memobj3 = clCreateBuffer(context, CL_MEM_READ_ONLY, 4 * sizeof(char), NULL, &ret);
	memobj4 = clCreateBuffer(context, CL_MEM_READ_ONLY, 4 * sizeof(char), NULL, &ret);
	memobj5 = clCreateBuffer(context, CL_MEM_READ_ONLY, 4 * sizeof(char), NULL, &ret);
	memobj6 = clCreateBuffer(context, CL_MEM_READ_ONLY, 4 * sizeof(char), NULL, &ret);
	memobj7 = clCreateBuffer(context, CL_MEM_READ_WRITE, 4 * MEM_SIZE, NULL, &ret);
	memobj8 = clCreateBuffer(context, CL_MEM_READ_WRITE, 4 * MEM_SIZE, NULL, &ret);
	memobj9 = clCreateBuffer(context, CL_MEM_READ_WRITE, 4 * MEM_SIZE, NULL, &ret);
	memobj10 = clCreateBuffer(context, CL_MEM_READ_WRITE, 4 * MEM_SIZE, NULL, &ret);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
		(const size_t *)&source_size, &ret);

	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* Create OpenCL Kernel */
	kernel[0] = clCreateKernel(program, "md5Search", &ret);
	kernel[1] = clCreateKernel(program, "md5Search", &ret);
	kernel[2] = clCreateKernel(program, "md5Search", &ret);
	kernel[3] = clCreateKernel(program, "md5Search", &ret);

	/* Set OpenCL Kernel Parameters */
	for (int i = 0; i < 4; i++) {
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj2);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj3);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj4);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj5);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj6);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj7);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj8);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj9);
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *)&memobj10);
	}


	clock_t start, finish;
	int hashesTotal;
	int numLetters = 52;
	char alphabet[52] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

	union Hash hashedOutput[4];

	start = clock();

	for (int i = 0; i<numLetters; i+=4)
	{
		input6a[0] = alphabet[i];
		input6a[1] = alphabet[i+1];
		input6a[2] = alphabet[i+2];
		input6a[3] = alphabet[i+3];
		for (int j = 0; j<numLetters; j+=4)
		{
			input5a[0] = alphabet[j];
			input5a[1] = alphabet[j + 1];
			input5a[2] = alphabet[j + 2];
			input5a[3] = alphabet[j + 3];
			for (int k = 0; k<numLetters; k+=4)
			{
				input4a[0] = alphabet[k];
				input4a[1] = alphabet[k + 1];
				input4a[2] = alphabet[k + 1];
				input4a[3] = alphabet[k + 1];
				for (int l = 0; l<numLetters; l+=4)
				{
					input3a[0] = alphabet[l];
					input3a[1] = alphabet[l + 1];
					input3a[2] = alphabet[l + 2];
					input3a[3] = alphabet[l + 3];
					for (int m = 0; m<numLetters; m+=4)
					{
						input2a[0] = alphabet[m];
						input2a[1] = alphabet[m + 1];
						input2a[2] = alphabet[m + 2];
						input2a[3] = alphabet[m + 3];
						for (int n = 0; n<numLetters; n+=4)
						{
							input1a[0] = alphabet[n];
							input1a[1] = alphabet[n + 1];
							input1a[2] = alphabet[n + 2];
							input1a[3] = alphabet[n + 3];

							//printf("%c", input1);
							//printf("%c", input2);
							//printf("%c", input3);
							//printf("%c", input4);
							//printf("%c", input5);
							//printf("%c\n", input6);

							ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, sizeof(unsigned char), &input1a, 0, NULL, NULL);
							ret = clEnqueueWriteBuffer(command_queue, memobj2, CL_TRUE, 0, sizeof(unsigned char), &input2a, 0, NULL, NULL);
							ret = clEnqueueWriteBuffer(command_queue, memobj3, CL_TRUE, 0, sizeof(unsigned char), &input3a, 0, NULL, NULL);
							ret = clEnqueueWriteBuffer(command_queue, memobj4, CL_TRUE, 0, sizeof(unsigned char), &input4a, 0, NULL, NULL);
							ret = clEnqueueWriteBuffer(command_queue, memobj5, CL_TRUE, 0, sizeof(unsigned char), &input5a, 0, NULL, NULL);
							ret = clEnqueueWriteBuffer(command_queue, memobj6, CL_TRUE, 0, sizeof(unsigned char), &input6a, 0, NULL, NULL);

							/* Execute OpenCL Kernel */
							for (int index = 0; index < 4; index++) {
								ret = clEnqueueTask(command_queue, kernel[index], 0, NULL, NULL);
							}

							/* Copy results from the memory buffer */
							ret = clEnqueueReadBuffer(command_queue, memobj7, CL_TRUE, 0, sizeof(unsigned int), &output1a, 0, NULL, NULL);
							ret = clEnqueueReadBuffer(command_queue, memobj8, CL_TRUE, 0, sizeof(unsigned int), &output2a, 0, NULL, NULL);
							ret = clEnqueueReadBuffer(command_queue, memobj9, CL_TRUE, 0, sizeof(unsigned int), &output3a, 0, NULL, NULL);
							ret = clEnqueueReadBuffer(command_queue, memobj10, CL_TRUE, 0, sizeof(unsigned int), &output4a, 0, NULL, NULL);

							for (int index = 0; index < 4; index++)
							{
								hashedOutput[index].C32[0] = output1a[index];
								hashedOutput[index].C32[1] = output2a[index];
								hashedOutput[index].C32[2] = output3a[index];
								hashedOutput[index].C32[3] = output4a[index];
							}
							
							for(int index=0; index<4;index++)
							{
								if (checkOutput(hashedOutput[index]) == 1)
								{
									hashesTotal = (i * pow(numLetters, 5)) +
									(j * pow(numLetters, 4)) +
									(k * pow(numLetters, 3)) +
									(l * pow(numLetters, 2)) +
									(m * numLetters) + n;
									break;
								}
							}
							
						}
					}
				}
			}
		}
	}

	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;

	double hashesPerSec = hashesTotal / duration;

	printf("Hashes per second = %f \n", hashesPerSec);

	for (int index = 0; index < 4; index++)
	{
		/* Display Result */
		printf("%c", input1a[index]);
		printf("%c", input2a[index]);
		printf("%c", input3a[index]);
		printf("%c", input4a[index]);
		printf("%c", input5a[index]);
		printf("%c\n", input6a[index]);
	}

	for (int index = 0; index < 4; index++)
	{
		printOutputHash(hashedOutput[index]);
	}



	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel[0]);
	ret = clReleaseKernel(kernel[1]);
	ret = clReleaseKernel(kernel[2]);
	ret = clReleaseKernel(kernel[3]);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj);
	ret = clReleaseMemObject(memobj2);
	ret = clReleaseMemObject(memobj3);
	ret = clReleaseMemObject(memobj4);
	ret = clReleaseMemObject(memobj5);
	ret = clReleaseMemObject(memobj6);
	ret = clReleaseMemObject(memobj7);
	ret = clReleaseMemObject(memobj8);
	ret = clReleaseMemObject(memobj9);
	ret = clReleaseMemObject(memobj10);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);

	system("pause");
	return 0;
}










