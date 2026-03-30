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

    printf("\n=== Phase 3: OpenMP Box Blur Performance ===\n\n");

    double seq_time, par_time;

    /* ================= SEQUENTIAL BASELINE ================= */
    double start = omp_get_wtime();
    box_blur_seq(input, output, WIDTH, HEIGHT);
    double end = omp_get_wtime();

    seq_time = end - start;

    printf("Sequential Time: %f sec\n\n", seq_time);

    /* ================= PARALLEL TEST ================= */
    int thread_counts[] = {1, 2, 4, 8};
    int n = 4;

    for (int t = 0; t < n; t++)
    {
        int threads = thread_counts[t];

        omp_set_num_threads(threads);

        start = omp_get_wtime();
        box_blur_omp(input, output, WIDTH, HEIGHT);
        end = omp_get_wtime();

        par_time = end - start;

        double speedup = seq_time / par_time;
        double efficiency = speedup / threads;

        printf("Threads: %d\n", threads);
        printf("Time: %f sec\n", par_time);
        printf("Speedup: %f\n", speedup);
        printf("Efficiency: %f\n", efficiency);
        printf("-----------------------------\n");
    }

    free_image(input);
    free_image(output);

    return 0;
}