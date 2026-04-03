#include "filters.h"
#include <omp.h>

#define PIXEL(img, i, j, w) img[(i)*(w) + (j)]

void box_blur_seq(unsigned char *in, unsigned char *out, int w, int h)
{
    for (int i = 1; i < h - 1; i++)
    {
        for (int j = 1; j < w - 1; j++)
        {
            int sum = 0;

            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++)
                    sum += PIXEL(in, i + dx, j + dy, w);

            PIXEL(out, i, j, w) = sum / 9;
        }
    }
}

void box_blur_omp(unsigned char *in, unsigned char *out, int w, int h)
{
    #pragma omp parallel for schedule(static)
    for (int i = 1; i < h - 1; i++)
    {
        for (int j = 1; j < w - 1; j++)
        {
            int sum = 0;

            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++)
                    sum += PIXEL(in, i + dx, j + dy, w);

            PIXEL(out, i, j, w) = sum / 9;
        }
    }
}