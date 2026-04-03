__kernel void box_blur(
    __global unsigned char *input,
    __global unsigned char *output,
    int width,
    int height)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    if (i == 0 || j == 0 || i == height-1 || j == width-1)
        return;

    int sum = 0;

    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
            sum += input[(i + dx) * width + (j + dy)];

    output[i * width + j] = sum / 9;
}