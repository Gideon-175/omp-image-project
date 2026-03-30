#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"

unsigned char **allocate_image(int width, int height)
{
    unsigned char **img = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; i++)
    {
        img[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }
    return img;
}

void free_image(unsigned char **img, int height)
{
    for (int i = 0; i < height; i++)
        free(img[i]);
    free(img);
}

void initialize_image(unsigned char **img, int width, int height)
{
    srand(time(NULL));

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            img[i][j] = rand() % 256;
}

void add_salt_pepper_noise(unsigned char **img, int width, int height, float prob)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float r = (float)rand() / RAND_MAX;

            if (r < prob)
                img[i][j] = (rand() % 2) ? 255 : 0;
        }
    }
}