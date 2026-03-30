#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"

#define PIXEL(img, i, j, w) img[(i) * (w) + (j)]

unsigned char *allocate_image(int width, int height)
{
    unsigned char *img = (unsigned char *)malloc(width * height * sizeof(unsigned char));

    if (!img)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    return img;
}

void free_image(unsigned char *img)
{
    free(img);
}

void initialize_image(unsigned char *img, int width, int height)
{
    srand(time(NULL));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            PIXEL(img, i, j, width) = rand() % 256;
        }
    }
}

void add_salt_pepper_noise(unsigned char *img, int width, int height, float prob)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float r = (float)rand() / RAND_MAX;

            if (r < prob)
            {
                PIXEL(img, i, j, width) = (rand() % 2) ? 255 : 0;
            }
        }
    }
}