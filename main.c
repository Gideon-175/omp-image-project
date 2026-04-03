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
    unsigned char *temp  = allocate_image(WIDTH, HEIGHT);
    unsigned char *output = allocate_image(WIDTH, HEIGHT);

    initialize_image(input, WIDTH, HEIGHT);
    add_salt_pepper_noise(input, WIDTH, HEIGHT, 0.05);

    double start, end;

    printf("Image is of size %d x %d \n", WIDTH, HEIGHT);

    printf("\n========== BOX BLUR ==========\n");

    // 🔹 Sequential Box
    start = omp_get_wtime();
    box_blur_seq(input, temp, WIDTH, HEIGHT);
    end = omp_get_wtime();
    double seq_box = end - start;

    // 🔹 OpenMP Box
    omp_set_num_threads(THREADS);
    start = omp_get_wtime();
    box_blur_omp(input, temp, WIDTH, HEIGHT);
    end = omp_get_wtime();
    double omp_box = end - start;

    double speedup_box = seq_box / omp_box;
    double eff_box = speedup_box / THREADS;

    // /* 🔹 OpenCL */
    double gpu_time = run_opencl_box_blur(input, output, WIDTH, HEIGHT);
    double gpu_speedup = seq_box / gpu_time;


    printf("Sequential: %f sec\n", seq_box);
    printf("OpenMP: %f sec | Speedup: %f | Efficiency: %f\n",
           omp_box, speedup_box, eff_box);
    printf("OpenCL: %f sec | Speedup: %f\n\n",
           gpu_time, gpu_speedup);

    printf("\n========== GAUSSIAN BLUR ==========\n");

    // 🔹 Sequential Gaussian
    start = omp_get_wtime();
    gaussian_blur_seq(input, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    double seq_gauss = end - start;

    // 🔹 OpenMP Gaussian
    omp_set_num_threads(THREADS);
    start = omp_get_wtime();
    gaussian_blur_omp(input, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    double omp_gauss = end - start;

    double speedup_gauss = seq_gauss / omp_gauss;
    double eff_gauss = speedup_gauss / THREADS;

    printf("Sequential: %f sec\n", seq_gauss);
    printf("OpenMP: %f sec | Speedup: %f | Efficiency: %f\n",
           omp_gauss, speedup_gauss, eff_gauss);


    printf("\n========== PIPELINE (BOX → GAUSSIAN) ==========\n");

    // 🔹 Sequential Pipeline
    start = omp_get_wtime();
    box_blur_seq(input, temp, WIDTH, HEIGHT);
    gaussian_blur_seq(temp, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    double seq_pipe = end - start;

    // 🔹 OpenMP Pipeline
    omp_set_num_threads(THREADS);
    start = omp_get_wtime();
    box_blur_omp(input, temp, WIDTH, HEIGHT);
    gaussian_blur_omp(temp, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    double omp_pipe = end - start;

    double speedup_pipe = seq_pipe / omp_pipe;
    double eff_pipe = speedup_pipe / THREADS;

    printf("Sequential: %f sec\n", seq_pipe);
    printf("OpenMP: %f sec | Speedup: %f | Efficiency: %f\n",
           omp_pipe, speedup_pipe, eff_pipe);


    printf("\n========== FINAL SUMMARY ==========\n");

    printf("Case\t\tModel\t\tTime\t\tSpeedup\t\tEfficiency\n");

    printf("Box\t\tSeq\t\t%f\t1.00\t\tN/A\n", seq_box);
    printf("Box\t\tOMP\t\t%f\t%f\t%f\n", omp_box, speedup_box, eff_box);
    printf("Box\t\tOCL\t\t%f\t%f\tN/A\n", gpu_time, gpu_speedup);

    printf("Gaussian\tSeq\t\t%f\t1.00\t\tN/A\n", seq_gauss);
    printf("Gaussian\tOMP\t\t%f\t%f\t%f\n", omp_gauss, speedup_gauss, eff_gauss);

    printf("Pipeline\tSeq\t\t%f\t1.00\t\tN/A\n", seq_pipe);
    printf("Pipeline\tOMP\t\t%f\t%f\t%f\n", omp_pipe, speedup_pipe, eff_pipe);

    free_image(input);
    free_image(output);

    return 0;
}