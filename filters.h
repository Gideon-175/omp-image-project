#ifndef FILTERS_H
#define FILTERS_H

void box_blur_seq(unsigned char *in, unsigned char *out, int w, int h);
void box_blur_omp(unsigned char *in, unsigned char *out, int w, int h);

void gaussian_blur_seq(unsigned char *in, unsigned char *out, int w, int h);
void gaussian_blur_omp(unsigned char *in, unsigned char *out, int w, int h);

#endif