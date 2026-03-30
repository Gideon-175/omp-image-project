#include <stdio.h>
#include <omp.h>
#include "image.h"
#include "filters.h"

#define WIDTH 3000
#define HEIGHT 3000
#define FRAMES 100
#define DEADLINE 0.016   // 16 ms

int main()
{
    unsigned char *input = allocate_image(WIDTH, HEIGHT);
    unsigned char *output = allocate_image(WIDTH, HEIGHT);

    initialize_image(input, WIDTH, HEIGHT);

    int threads = 8;
    omp_set_num_threads(threads);

    printf("\n=== Phase 4: Real-Time Frame Processing ===\n\n");
    printf("Threads Used: %d\n", threads);
    printf("Frames: %d\n\n", FRAMES);

    double total_time = 0.0;
    int deadline_miss = 0;

    for (int f = 0; f < FRAMES; f++)
    {
        // Add noise per frame (simulate new frame)
        add_salt_pepper_noise(input, WIDTH, HEIGHT, 0.05);

        double start = omp_get_wtime();

        // Process frame (OpenMP box blur)
        box_blur_omp(input, output, WIDTH, HEIGHT);

        double end = omp_get_wtime();

        double frame_time = end - start;
        total_time += frame_time;

        // Deadline check
        if (frame_time > DEADLINE)
            deadline_miss++;

        // Print first few frames only (avoid clutter)
        if (f < 5)
        {
            printf("Frame %d Time: %f sec\n", f, frame_time);
        }
    }

    double avg_time = total_time / FRAMES;
    double fps = FRAMES / total_time;

    printf("\n===== SUMMARY =====\n");
    printf("Average Frame Time: %f sec\n", avg_time);
    printf("FPS: %f\n", fps);
    printf("Deadline Misses: %d / %d\n", deadline_miss, FRAMES);

    free_image(input);
    free_image(output);

    return 0;
}