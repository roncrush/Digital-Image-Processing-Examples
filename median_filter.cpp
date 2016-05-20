// Course: CS4491
// Student name: Syed Hassaan Ali
// Student ID: 000675546
// Assignment #: #3
// Due Date: 09/21/2015
// Signature: ________________________
//			  (The signature means that the work is your own, not from somewhere else)
// Score: ______________

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


// These variables hold the minimum and maximum percentages,
// respectivley
int noise_level = 0;

// Matrix of the input image
Mat input_image;

// This function takes in a sigma and then creates a noisy image
Mat add_noise(int sigma, Mat input)
{
	Mat noise_filter = input.clone();
	RNG rng;

	// Create a random noise image based on the sigma
	rng.fill(input, RNG::NORMAL, 10, sigma);

	// Overlay random noise image onto the input image
	add(input, noise_filter, input);

	// Display new noisy image
	imshow("Noise Image", input);

	return input;
}

void median_filter(Mat input)
{
	int column_limit = input.cols;
	int row_limit = input.rows;
	int median_array[9];

	// pixel_scalar is for storing the pixel
	Scalar pixel_scalar;

	// pixel_value is for storing the intensity value at a pixel
	int pixel_value;

	// Loops through every pixel and uses a 3x3 Kernel median filter
	for (int i = 0; i < row_limit; i++)
	{
		for (int j = 0; j < column_limit; j++)
		{
			// Top left square
			if (j - 1 >= 0 && i - 1 >= 0)
			{
				pixel_scalar = input.at<uchar>(Point(j - 1, i - 1));
				median_array[0] = (pixel_scalar.val[0]);
			}
			else
			{
				median_array[0] = 0;
			}

			// Top middle square
			if (i - 1 >= 0)
			{
				pixel_scalar = input.at<uchar>(Point(j, i - 1));
				median_array[1] = pixel_scalar.val[0];
			}
			else
			{
				median_array[1] = 0;
			}

			// Top right square
			if (i - 1 >= 0 && j + 1 < column_limit)
			{
				pixel_scalar = input.at<uchar>(Point(j + 1, i - 1));
				median_array[2] = pixel_scalar.val[0];
			}
			else
			{
				median_array[2] = 0;
			}

			// Middle left square
			if (j - 1 >= 0)
			{
				pixel_scalar = input.at<uchar>(Point(j - 1, i));
				median_array[3] = pixel_scalar.val[0];
			}
			else
			{
				median_array[3] = 0;
			}

			// Center square
			pixel_scalar = input.at<uchar>(Point(j, i));
			median_array[4] = pixel_scalar.val[0];

			// Middle right square
			if (j + 1 < column_limit)
			{
				pixel_scalar = input.at<uchar>(Point(j + 1, i));
				median_array[5] = pixel_scalar.val[0];
			}
			else
			{
				median_array[5] = 0;
			}

			// Bottom left square
			if (j - 1 >= 0 && i + 1 < row_limit)
			{
				pixel_scalar = input.at<uchar>(Point(j - 1, i + 1));
				median_array[6] = pixel_scalar.val[0];
			}
			else
			{
				median_array[6] = 0;
			}

			// Bottom middle square
			if (i + 1 < row_limit)
			{
				pixel_scalar = input.at<uchar>(Point(j, i + 1));
				median_array[7] = pixel_scalar.val[0];
			}
			else
			{
				median_array[7] = 0;
			}

			// Bottom right square
			if (i + 1 < row_limit && j + 1 < column_limit)
			{
				pixel_scalar = input.at<uchar>(Point(j + 1, i + 1));
				median_array[8] = pixel_scalar.val[0];
			}
			else
			{
				median_array[8] = 0;
			}

			sort(median_array, median_array + 9);
			input.at<uchar>(Point(j, i)) = median_array[4];

		}
	}

	imshow("Denoised", input);
}

void on_trackbar(int, void *)
{
	Mat noise_image = add_noise(noise_level, input_image.clone());
	median_filter(noise_image);
}

void main()
{
	input_image = imread("C:\\Users\\Hassaan\\Pictures\\grass.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	imshow("Noise Image", input_image.clone());
	createTrackbar("Sigma", "Noise Image", &noise_level, 256, on_trackbar);

	imshow("Input Image", input_image);
	waitKey(0);
}