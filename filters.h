#ifndef FILTERS_H
#define FILTERS_H

void box_blur(unsigned char *in, unsigned char *out, int w, int h);
void gaussian_blur(unsigned char *in, unsigned char *out, int w, int h);
void median_filter(unsigned char *in, unsigned char *out, int w, int h);

#endif