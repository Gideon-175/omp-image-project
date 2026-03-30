#include <stdio.h>
#include <omp.h>
#include "image.h"
#include "filters.h"

#define WIDTH 3000
#define HEIGHT 3000

int main()
{
    unsigned char *input = allocate_image(WIDTH, HEIGHT);
    unsigned char *output = allocate_image(WIDTH, HEIGHT);

    initialize_image(input, WIDTH, HEIGHT);
    add_salt_pepper_noise(input, WIDTH, HEIGHT, 0.05);

    double start, end;

    // Box Blur
    start = omp_get_wtime();
    box_blur(input, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    printf("Box Blur Time: %f sec\n", end - start);

    // Gaussian
    start = omp_get_wtime();
    gaussian_blur(input, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    printf("Gaussian Blur Time: %f sec\n", end - start);

    // Median
    start = omp_get_wtime();
    median_filter(input, output, WIDTH, HEIGHT);
    end = omp_get_wtime();
    printf("Median Filter Time: %f sec\n", end - start);

    free_image(input);
    free_image(output);

    return 0;
}