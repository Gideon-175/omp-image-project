#define CL_TARGET_OPENCL_VERSION 110
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "opencl.h"

#define KERNEL_FILE "kernel.cl"

char* load_kernel_source(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) { printf("Kernel file not found\n"); exit(1); }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char* source = (char*)malloc(size + 1);
    fread(source, 1, size, fp);
    source[size] = '\0';

    fclose(fp);
    return source;
}

double run_opencl_box_blur(unsigned char *input,
                          unsigned char *output,
                          int width,
                          int height)
{
    cl_int err;

    cl_platform_id platform;
    cl_device_id device;

    cl_context context;
    cl_command_queue queue;

    cl_program program;
    cl_kernel kernel;

    cl_mem input_buf, output_buf;

    size_t size = width * height;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueue(context, device, 0, NULL);

    input_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, NULL);
    output_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size, NULL, NULL);

    char* source = load_kernel_source(KERNEL_FILE);
    program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error building program\n");
        exit(1);
    }

    kernel = clCreateKernel(program, "box_blur", NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buf);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buf);
    clSetKernelArg(kernel, 2, sizeof(int), &width);
    clSetKernelArg(kernel, 3, sizeof(int), &height);

    size_t global[2] = {height - 2, width - 2};
    size_t local[2] = {16, 16};

    // TOTAL TIME (correct measurement)
    double start = omp_get_wtime();

    clEnqueueWriteBuffer(queue, input_buf, CL_TRUE, 0, size, input, 0, NULL, NULL);

    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global, local, 0, NULL, NULL);
    clFinish(queue);

    clEnqueueReadBuffer(queue, output_buf, CL_TRUE, 0, size, output, 0, NULL, NULL);

    double end = omp_get_wtime();

    clReleaseMemObject(input_buf);
    clReleaseMemObject(output_buf);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    free(source);

    return end - start;
}