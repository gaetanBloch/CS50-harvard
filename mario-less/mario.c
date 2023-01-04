#include <cs50.h>
#include <stdio.h>

void print_right_pyramid(int height);
void print_left_pyramid(int height);

int main(void)
{
    const int MAX_HEIGHT = 8;
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > MAX_HEIGHT);
    print_right_pyramid(height);
}

void print_right_pyramid(int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 1; j <= height; j++)
        {
            if (j < height - i)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("\n");
    }
}

void print_left_pyramid(int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = height - i; j <= height; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
