#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE blue = image[i][j].rgbtBlue;
            BYTE green = image[i][j].rgbtGreen;
            BYTE red = image[i][j].rgbtRed;

            BYTE avg = lrint((blue + green + red) / 3.0);

            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

BYTE min(int color)
{
    return (color > 255) ? 255 : color;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE blue = image[i][j].rgbtBlue;
            BYTE green = image[i][j].rgbtGreen;
            BYTE red = image[i][j].rgbtRed;

            int sepiaRed = lrint(.393 * red + .769 * green + .189 * blue);
            int sepiaGreen = lrint(.349 * red + .686 * green + .168 * blue);
            int sepiaBlue = lrint(.272 * red + .534 * green + .131 * blue);

            // printf("R:%i G:%i B:%i\n", sepiaRed, sepiaGreen, sepiaBlue);

            image[i][j].rgbtBlue = min(sepiaBlue);
            image[i][j].rgbtGreen = min(sepiaGreen);
            image[i][j].rgbtRed = min(sepiaRed);
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp_pixel = image[i][j];
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][width - j - 1].rgbtBlue = tmp_pixel.rgbtBlue;
            image[i][width - j - 1].rgbtGreen = tmp_pixel.rgbtGreen;
            image[i][width - j - 1].rgbtRed = tmp_pixel.rgbtRed;
        }
    }
}

BYTE compute_average(BYTE values[], int length)
{
    int sum = 0;
    for (int i = 0; i < length; i++)
    {
        sum += values[i];
    }
    // printf("%li\n", lrint(sum / length));
    return round(sum / (double) length);
}

void compute_blur(int i,  int j, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE original_image[height][width])
{
    BYTE blue_values[9];
    BYTE green_values[9];
    BYTE red_values[9];
    int idx = 0;
    if (i - 1 >= 0)
    {
        blue_values[idx] = original_image[i - 1][j].rgbtBlue;
        green_values[idx] = original_image[i - 1][j].rgbtGreen;
        red_values[idx] = original_image[i - 1][j].rgbtRed;
        idx++;
    }
    if (i + 1 < height)
    {
        blue_values[idx] = original_image[i + 1][j].rgbtBlue;
        green_values[idx] = original_image[i + 1][j].rgbtGreen;
        red_values[idx] = original_image[i + 1][j].rgbtRed;
        idx++;
    }
    if (j - 1 >= 0)
    {
        blue_values[idx] = original_image[i][j - 1].rgbtBlue;
        green_values[idx] = original_image[i][j - 1].rgbtGreen;
        red_values[idx] = original_image[i][j - 1].rgbtRed;
        idx++;
    }
    if (j + 1 < width)
    {
        blue_values[idx] = original_image[i][j + 1].rgbtBlue;
        green_values[idx] = original_image[i][j + 1].rgbtGreen;
        red_values[idx] = original_image[i][j + 1].rgbtRed;
        idx++;
    }
    if (i - 1 >= 0 && j - 1 >= 0)
    {
        blue_values[idx] = original_image[i - 1][j - 1].rgbtBlue;
        green_values[idx] = original_image[i - 1][j - 1].rgbtGreen;
        red_values[idx] = original_image[i - 1][j - 1].rgbtRed;
        idx++;
    }
    if (i + 1 < height && j - 1 >= 0)
    {
        blue_values[idx] = original_image[i + 1][j - 1].rgbtBlue;
        green_values[idx] = original_image[i + 1][j - 1].rgbtGreen;
        red_values[idx] = original_image[i + 1][j - 1].rgbtRed;
        idx++;
    }
    if (i - 1 >= 0 && j + 1 < width)
    {
        blue_values[idx] = original_image[i - 1][j + 1].rgbtBlue;
        green_values[idx] = original_image[i - 1][j + 1].rgbtGreen;
        red_values[idx] = original_image[i - 1][j + 1].rgbtRed;
        idx++;
    }
    if (i + 1 < height && j + 1 < width)
    {
        blue_values[idx] = original_image[i + 1][j + 1].rgbtBlue;
        green_values[idx] = original_image[i + 1][j + 1].rgbtGreen;
        red_values[idx] = original_image[i + 1][j + 1].rgbtRed;
        idx++;
    }

    blue_values[idx] = original_image[i][j].rgbtBlue;
    green_values[idx] = original_image[i][j].rgbtGreen;
    red_values[idx] = original_image[i][j].rgbtRed;
    idx++;

    image[i][j].rgbtBlue = compute_average(blue_values, idx);
    image[i][j].rgbtGreen = compute_average(green_values, idx);
    image[i][j].rgbtRed = compute_average(red_values, idx);
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Deep copy of the original image
    RGBTRIPLE original_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original_image[i][j].rgbtBlue = image[i][j].rgbtBlue;
            original_image[i][j].rgbtGreen = image[i][j].rgbtGreen;
            original_image[i][j].rgbtRed = image[i][j].rgbtRed;
            // printf("Address o: %p, Address i: %p\n", &original_image[i][j].rgbtBlue, &image[i][j].rgbtBlue);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            compute_blur(i, j, height, width, image, original_image);
        }
    }
}
