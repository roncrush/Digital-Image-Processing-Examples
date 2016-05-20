// Course: CS4491
// Student name: Syed Hassaan Ali
// Student ID: 000675546
// Assignment #: Test 1
// Due Date: 10/05/2015
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

// Matrix of the input image
Mat input_image;

int noise_level = 0;

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

double vector_distance(Vec3b vector_1, Vec3b vector_2)
{
	double element1;
	double element2;
	double element3;
	double distance;

	element1 = vector_1[0] - vector_2[0];
	element2 = vector_1[1] - vector_2[1];
	element3 = vector_1[2] - vector_2[2];

	distance = sqrt(pow(element1, 2) + pow(element2, 2) + pow(element3, 2));

	return distance;
}

void vector_median_filter(Mat input)
{
	int column_limit = input.cols;
	int row_limit = input.rows;
	double min_distance = 0;
	int minR;
	int minG;
	int minB;
	Mat new_image(input.size(), CV_8UC3, Scalar(0, 0, 0));

	// pixel_value is for storing the intensity value at a pixel
	double current_distance = 0;

	// Loops through every pixel and uses a 3x3 Kernel median filter
	for (int j = 1; j < row_limit - 1; j++)
	{
		for (int i = 1; i < column_limit - 1; i++)
		{
			// Top middle square
			current_distance = vector_distance(input.at<Vec3b>(j, i - 1), input.at<Vec3b>(j, i))
				+ vector_distance(input.at<Vec3b>(j, i - 1), input.at<Vec3b>(j - 1, i))
				+ vector_distance(input.at<Vec3b>(j, i - 1), input.at<Vec3b>(j + 1, i))
				+ vector_distance(input.at<Vec3b>(j, i - 1), input.at<Vec3b>(j, i + 1));
			min_distance = current_distance;
			minR = input.at<Vec3b>(j, i - 1)[0];
			minG = input.at<Vec3b>(j, i - 1)[1];
			minB = input.at<Vec3b>(j, i - 1)[2];

			// Middle left square
			current_distance = vector_distance(input.at<Vec3b>(j - 1, i), input.at<Vec3b>(j, i - 1))
				+ vector_distance(input.at<Vec3b>(j - 1, i), input.at<Vec3b>(j, i))
				+ vector_distance(input.at<Vec3b>(j - 1, i), input.at<Vec3b>(j + 1, i))
				+ vector_distance(input.at<Vec3b>(j - 1, i), input.at<Vec3b>(j, i + 1));
			if (current_distance < min_distance)
			{
				min_distance = current_distance;
				minR = input.at<Vec3b>(j - 1, i)[0];
				minG = input.at<Vec3b>(j - 1, i)[1];
				minB = input.at<Vec3b>(j - 1, i)[2];
			}

			// Center square
			current_distance = vector_distance(input.at<Vec3b>(j, i), input.at<Vec3b>(j, i - 1))
				+ vector_distance(input.at<Vec3b>(j, i), input.at<Vec3b>(j - 1, i))
				+ vector_distance(input.at<Vec3b>(j, i), input.at<Vec3b>(j + 1, i))
				+ vector_distance(input.at<Vec3b>(j, i), input.at<Vec3b>(j, i + 1));
			if (current_distance < min_distance)
			{
				min_distance = current_distance;
				minR = input.at<Vec3b>(j, i)[0];
				minG = input.at<Vec3b>(j, i)[1];
				minB = input.at<Vec3b>(j, i)[2];
			}

			// Middle right square
			current_distance = vector_distance(input.at<Vec3b>(j + 1, i), input.at<Vec3b>(j, i - 1))
				+ vector_distance(input.at<Vec3b>(j + 1, i), input.at<Vec3b>(j - 1, i))
				+ vector_distance(input.at<Vec3b>(j + 1, i), input.at<Vec3b>(j, i))
				+ vector_distance(input.at<Vec3b>(j + 1, i), input.at<Vec3b>(j, i + 1));
			if (current_distance < min_distance)
			{
				min_distance = current_distance;
				minR = input.at<Vec3b>(j + 1, i)[0];
				minG = input.at<Vec3b>(j + 1, i)[1];
				minB = input.at<Vec3b>(j + 1, i)[2];
			}

			// Bottom middle square
			current_distance = vector_distance(input.at<Vec3b>(j, i + 1), input.at<Vec3b>(j, i - 1))
				+ vector_distance(input.at<Vec3b>(j, i + 1), input.at<Vec3b>(j - 1, i))
				+ vector_distance(input.at<Vec3b>(j, i + 1), input.at<Vec3b>(j, i))
				+ vector_distance(input.at<Vec3b>(j, i + 1), input.at<Vec3b>(j + 1, i));
			if (current_distance < min_distance)
			{
				min_distance = current_distance;
				minR = input.at<Vec3b>(j, i + 1)[0];
				minG = input.at<Vec3b>(j, i + 1)[1];
				minB = input.at<Vec3b>(j, i + 1)[2];
			}

			input.at<Vec3b>(j, i)[0] = minR;
			input.at<Vec3b>(j, i)[1] = minG;
			input.at<Vec3b>(j, i)[2] = minB;

			min_distance = 0;
			current_distance = 0;

		}
	}

	imshow("Vector Median Filter", input);
}

void on_trackbar(int, void *)
{
	Mat noise_image = add_noise(noise_level, input_image.clone());
	vector_median_filter(noise_image.clone());
}

void main()
{
	input_image = imread("C:\\Users\\Hassaan\\Pictures\\Noise.png");

	imshow("Noise Image", input_image.clone());
	createTrackbar("Sigma", "Noise Image", &noise_level, 256, on_trackbar);

	//imshow("Input Image", input_image);
	waitKey(0);
}