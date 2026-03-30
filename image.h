#ifndef IMAGE_H
#define IMAGE_H

unsigned char *allocate_image(int width, int height);
void free_image(unsigned char *img);

void initialize_image(unsigned char *img, int width, int height);
void add_salt_pepper_noise(unsigned char *img, int width, int height, float prob);

#endif