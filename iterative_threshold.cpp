// Course: CS4491
// Student name: Syed Hassaan Ali
// Student ID: 000675546
// Assignment #: #5
// Due Date: 10/21/2015
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

int total_pixels;

// This array holds the amount of pixels for each intensity
int histogram_array[256] = { 0 };

// Calculate the histogram for the input image
void calc_histogram(Mat input_img)
{
	// pixel_scalar is for storing the pixel
	Scalar pixel_scalar;

	// pixel_value is for storing the intensity value at a pixel
	int pixel_value;

	// Loop through the image matrix
	for (int i = 0; i < input_img.rows; i++)
	{
		for (int j = 0; j < input_img.cols; j++)
		{
			// Get the intensity value of the pixel through the pixel_scalar object
			pixel_scalar = input_img.at<uchar>(Point(j, i));
			pixel_value = pixel_scalar.val[0];

			// Add one to the array at the pixel value
			histogram_array[pixel_value]++;
		}
	}

	// The histogram array now contains the amount of pixel for 	
	// every intensity
}

double calc_mean(vector<int>& data , int start, int end)
{
	double mean = 0;

	for (int i = start; i < end + 1; i++)
	{
		mean += ((data[i]/(double)total_pixels) * i);
	}

	return mean;
}

void iterative_thresh(Mat image)
{
	bool same_mean_flag = false;
	double current_mean;
	double class1_mean = 0;
	double class2_mean = 0;
	int array_length = (sizeof histogram_array / sizeof histogram_array[0]);
	vector<int> histogram_vec(histogram_array, histogram_array + array_length);
	double threshold;

	// pixel_scalar is for storing the pixel
	Scalar pixel_scalar;

	// pixel_value is for storing the intensity value at a pixel
	int pixel_value;

	current_mean = calc_mean(histogram_vec, 0, 255);


	while (same_mean_flag == false)
	{
		class1_mean = calc_mean(histogram_vec, 0, floor(current_mean));
		class2_mean = calc_mean(histogram_vec, floor(current_mean) + 1, (array_length-1));

		if (((class1_mean + class2_mean) / 2) == current_mean)
		{
			same_mean_flag = true;
		}
		else
		{
			current_mean = ((class1_mean + class2_mean) / 2);
		}
	}

	threshold = current_mean;

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			// Get the intensity value of the pixel through the pixel_scalar object
			pixel_value = image.at<uchar>(i, j);

			if (pixel_value <= threshold)
			{
				image.at<uchar>(i, j) = 0;
			}
			else
			{
				image.at<uchar>(i, j) = 255;
			}
		}
	}

	imshow("Thresholded Image", image);
}

void main()
{
	input_image = imread("C:\\Users\\Hassaan\\Pictures\\car.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	total_pixels = input_image.cols * input_image.rows;

	calc_histogram(input_image);
	iterative_thresh(input_image.clone());

	imshow("Input Image", input_image);
	waitKey(0);
}