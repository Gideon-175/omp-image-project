#include <stdio.h>
#include <omp.h>
#include "image.h"
#include "filters.h"
#include "opencl.h"

#define WIDTH 2048
#define HEIGHT 2048

#define THREADS 8

int main()
{
    unsigned char *input = allocate_image(WIDTH, HEIGHT);
    unsigned char *output = allocate_image(WIDTH, HEIGHT);

    initialize_image(input, WIDTH, HEIGHT);
    add_salt_pepper_noise(input, WIDTH, HEIGHT, 0.05);

    printf("\n===== Box Blur Comparison =====\n\n");

    double start, end;

    /* 🔹 Sequential */
    start = omp_get_wtime();
    box_blur_seq(input, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    double seq_time = end - start;

    /* 🔹 OpenMP */
    omp_set_num_threads(THREADS);
    start = omp_get_wtime();
    box_blur_omp(input, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    double omp_time = end - start;

    // /* 🔹 OpenCL */
    double gpu_time = run_opencl_box_blur(input, output, WIDTH, HEIGHT);

    // /* 🔹 Metrics */
    double omp_speedup = seq_time / omp_time;
    double omp_eff = omp_speedup / THREADS;
    double gpu_speedup = seq_time / gpu_time;

    printf("Sequential Time: %f sec\n", seq_time);

    printf("OpenMP Time: %f sec\n", omp_time);
    printf("Speedup: %f | Efficiency: %f\n\n", omp_speedup, omp_eff);

    printf("OpenCL Time: %f sec\n", gpu_time);
    printf("GPU Speedup: %f\n\n", gpu_speedup);

    printf("=========== FINAL TABLE ===========\n");
    printf("Model\t\tTime\t\tSpeedup\t\tEfficiency\n");

    printf("Sequential\t%f\t1.00\t\tN/A\n", seq_time);
    printf("OpenMP(8)\t%f\t%f\t%f\n", omp_time, omp_speedup, omp_eff);
    printf("OpenCL\t\t%f\t%f\tN/A\n", gpu_time, gpu_speedup);

    free_image(input);
    free_image(output);

    return 0;
}