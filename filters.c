#include "filters.h"

#define PIXEL(img, i, j, w) img[(i)*(w) + (j)]

void box_blur(unsigned char *in, unsigned char *out, int w, int h)
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

void gaussian_blur(unsigned char *in, unsigned char *out, int w, int h)
{
    int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };

    for (int i = 1; i < h - 1; i++)
    {
        for (int j = 1; j < w - 1; j++)
        {
            int sum = 0;

            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++)
                    sum += PIXEL(in, i + dx, j + dy, w) *
                           kernel[dx + 1][dy + 1];

            PIXEL(out, i, j, w) = sum / 16;
        }
    }
}

static void sort9(unsigned char *arr)
{
    for (int i = 0; i < 9; i++)
        for (int j = i + 1; j < 9; j++)
            if (arr[j] < arr[i])
            {
                unsigned char temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}

void median_filter(unsigned char *in, unsigned char *out, int w, int h)
{
    unsigned char window[9];

    for (int i = 1; i < h - 1; i++)
    {
        for (int j = 1; j < w - 1; j++)
        {
            int k = 0;

            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++)
                    window[k++] = PIXEL(in, i + dx, j + dy, w);

            sort9(window);

            PIXEL(out, i, j, w) = window[4];
        }
    }
}