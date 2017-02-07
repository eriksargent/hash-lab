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

int checkOutput(union Hash Output, union Hash Known)
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

int main()
{
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
	//cl_kernel kernel[4] = { NULL, NULL, NULL, NULL };
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
	int err;

	char *input1 = new char;
	char *input2 = new char;
	char *input3 = new char;
	char *input4 = new char;
	char *input5 = new char;
	char *input6 = new char;

	unsigned int *output1 = new unsigned int;
	unsigned int *output2 = new unsigned int;
	unsigned int *output3 = new unsigned int;
	unsigned int *output4 = new unsigned int;

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

	union Hash hashedOutput;
	*input1 = 'H';
	*input2 = 'a';
	*input3 = 's';
	*input4 = 'h';
	*input5 = 'e';
	*input6 = 'd';

	*output1 = 'AAAA';
	*output2 = 'BBBB';
	*output3 = 'CCCC';
	*output4 = 'DDDD';

	/* Display Result */
	printf("Input Password to be Hashed: ");
	printf("%c", *input1);
	printf("%c", *input2);
	printf("%c", *input3);
	printf("%c", *input4);
	printf("%c", *input5);
	printf("%c\n", *input6);

	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	if (ret != CL_SUCCESS)
	{
		printf("Error 1\n");
	}
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
	if (ret != CL_SUCCESS)
	{
		printf("Error 2\n");
	}

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/* Create Memory Buffer */
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char), NULL, &ret);
	memobj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char), NULL, &ret);
	memobj3 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char), NULL, &ret);
	memobj4 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char), NULL, &ret);
	memobj5 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char), NULL, &ret);
	memobj6 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char), NULL, &ret);
	memobj7 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(unsigned int), NULL, &ret);
	memobj8 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(unsigned int), NULL, &ret);
	memobj9 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(unsigned int), NULL, &ret);
	memobj10 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(unsigned int), NULL, &ret);

	ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, sizeof(char), input1, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, memobj2, CL_TRUE, 0, sizeof(char), input2, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, memobj3, CL_TRUE, 0, sizeof(char), input3, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, memobj4, CL_TRUE, 0, sizeof(char), input4, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, memobj5, CL_TRUE, 0, sizeof(char), input5, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, memobj6, CL_TRUE, 0, sizeof(char), input6, 0, NULL, NULL);


	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
		(const size_t *)&source_size, &ret);

	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("Error 3\n");
	}

	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, "md5Search", &ret);
	
	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memobj2);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memobj3);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&memobj4);
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&memobj5);
	ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&memobj6);
	ret = clSetKernelArg(kernel, 6, sizeof(cl_mem), (void *)&memobj7);
	ret = clSetKernelArg(kernel, 7, sizeof(cl_mem), (void *)&memobj8);
	ret = clSetKernelArg(kernel, 8, sizeof(cl_mem), (void *)&memobj9);
	ret = clSetKernelArg(kernel, 9, sizeof(cl_mem), (void *)&memobj10);

	
	/* Execute OpenCL Kernel */ 
	//ret = clEnqueueNDRangeKernel(command_queue, kernel, 0, NULL, NULL);
	size_t global_item_size = 1;
	size_t local_item_size = 1;
	clock_t start, finish;
	start = clock();

	/* Execute OpenCL kernel as data parallel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("Execute OpenCL Error\n");
	}

	/* Copy results from the memory buffer */
	ret = clEnqueueReadBuffer(command_queue, memobj7, CL_TRUE, 0, sizeof(unsigned int), output1, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, memobj8, CL_TRUE, 0, sizeof(unsigned int), output2, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, memobj9, CL_TRUE, 0, sizeof(unsigned int), output3, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, memobj10, CL_TRUE, 0, sizeof(unsigned int), output4, 0, NULL, NULL);


	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;

	hashedOutput.C32[0] = *output1;
	hashedOutput.C32[1] = *output2;
	hashedOutput.C32[2] = *output3;
	hashedOutput.C32[3] = *output4;

	printf("0x%08x \n", *output1);
	printf("0x%08x \n", *output2);
	printf("0x%08x \n", *output3);
	printf("0x%08x \n", *output4);
	
	printOutputHash(hashedOutput);
	printf("Hash Duration = %5f seconds\n", duration);

	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
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










