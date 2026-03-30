#include <stdio.h>
#include "image.h"

#define WIDTH 3000
#define HEIGHT 3000

int main()
{
    unsigned char **image = allocate_image(WIDTH, HEIGHT);

    initialize_image(image, WIDTH, HEIGHT);
    add_salt_pepper_noise(image, WIDTH, HEIGHT, 0.05);

    printf("Image initialized + noise added\n");

    free_image(image, HEIGHT);
    return 0;
}