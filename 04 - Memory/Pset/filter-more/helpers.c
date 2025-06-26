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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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

    // Sobel operator kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Loop through each pixel
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            int GxRed = 0, GxGreen = 0, GxBlue = 0;
            int GyRed = 0, GyGreen = 0, GyBlue = 0;

            // Apply Sobel operator
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    int currentRow = row + i;
                    int currentCol = column + j;

                    // Treat out-of-bounds pixels as black (0)
                    int red = 0, green = 0, blue = 0;
                    if (currentRow >= 0 && currentRow < height && currentCol >= 0 &&
                        currentCol < width)
                    {
                        red = copy[currentRow][currentCol].rgbtRed;
                        green = copy[currentRow][currentCol].rgbtGreen;
                        blue = copy[currentRow][currentCol].rgbtBlue;
                    }

                    // Apply Gx kernel
                    GxRed += red * Gx[i + 1][j + 1];
                    GxGreen += green * Gx[i + 1][j + 1];
                    GxBlue += blue * Gx[i + 1][j + 1];

                    // Apply Gy kernel
                    GyRed += red * Gy[i + 1][j + 1];
                    GyGreen += green * Gy[i + 1][j + 1];
                    GyBlue += blue * Gy[i + 1][j + 1];
                }
            }

            // Calculate final values using Sobel formula
            int finalRed = round(sqrt(GxRed * GxRed + GyRed * GyRed));
            int finalGreen = round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            int finalBlue = round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));

            // Cap values at 255
            image[row][column].rgbtRed = finalRed > 255 ? 255 : finalRed;
            image[row][column].rgbtGreen = finalGreen > 255 ? 255 : finalGreen;
            image[row][column].rgbtBlue = finalBlue > 255 ? 255 : finalBlue;
        }
    }

    return;
}
