#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue
            int avg =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Update pixel values
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // 存储RGB值
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            // Compute sepia values
            int sepiaRed =
                round(0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue);
            int sepiaGreen =
                round(0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue);
            int sepiaBlue =
                round(0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue);

            // 判断是否超过上限
            sepiaRed = (sepiaRed > 255) ? 255 : sepiaRed;
            sepiaGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            sepiaBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;

            // Update pixel with sepia values
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Swap pixels
            if (j < width / 2)
            {
                RGBTRIPLE temp = image[i][j];
                image[i][j] = image[i][width - 1 - j];
                image[i][width - 1 - j] = temp;
            }
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loop through rows
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // Initialize the variables
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int counter = 0;

            // Loop through the surrounding pixels for each pixel (3x3 grid)
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    // Check for the pixel to be in the grid and skip if it isn't
                    if (row + i < 0 || row + i >= height)
                    {
                        // 这里直接break，因为i的值在内层的循环没有变化，在内层的后续执行中不可能处于正确的范围
                        break;
                    }

                    if (column + j < 0 || column + j >= width)
                    {
                        continue;
                    }

                    // Add to sum
                    sumRed += copy[row + i][column + j].rgbtRed;
                    sumGreen += copy[row + i][column + j].rgbtGreen;
                    sumBlue += copy[row + i][column + j].rgbtBlue;

                    // 计数器++
                    counter++;
                }
            }

            // Add the blur effect to the original image
            image[row][column].rgbtRed = round((float) sumRed / counter);
            image[row][column].rgbtGreen = round((float) sumGreen / counter);
            image[row][column].rgbtBlue = round((float) sumBlue / counter);
        }
    }

    return;
}
