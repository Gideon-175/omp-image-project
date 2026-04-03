#ifndef OPENCL_H
#define OPENCL_H

double run_opencl_box_blur(unsigned char *input,
                          unsigned char *output,
                          int width,
                          int height);

#endif