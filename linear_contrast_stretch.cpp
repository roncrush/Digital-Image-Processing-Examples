// Course: CS4491
// Student name: Syed Hassaan Ali
// Student ID: 000675546
// Assignment #: #2
// Due Date: 09/14/2015
// Signature: ________________________
//			  (The signature means that the work is your own, not from somewhere else)
// Score: ______________

#include <iostream>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

// The histogram_array object holds number pixels for every
// intensity value 0-255
// It is a instantiated as a array of holding only zeroes
int histogram_array[256] = { 0 };

// The total number of pixels, which is used to calculate entropy
int total_pixels;

// These variables hold the minimum and maximum percentages,
// respectivley
int b_low_perc = 0;
int b_high_perc = 100;

// Matrix of the input image
Mat input_image;

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
			// Get the intensity value of the pixel through 				// the pixel_scalar object
			pixel_scalar = input_img.at<uchar>(Point(j, i));
			pixel_value = pixel_scalar.val[0];

			// Add one to the array at the pixel value
			histogram_array[pixel_value]++;
		}
	}

	// The histogram array now contains the amount of pixel for 	// every intensity
}

// This function takes in a percentage and then calculates the
// pixel value at that percentage
int calc_hist_perc(double percent)
{
	// Convert percentage to decimal form and store it as the 		// target number of pixels we are aiming to cover
	double target = total_pixels * (percent / 100);

	// current_total is used in the while loop to track how many 	// pixels have been counted thus far
	int current_total = 0;

	// target_value is the pixel value that will increment until 	// we reach the percentage of pixels we are targeting
	int target_value = 0;

	// Loop through the histogram until we cover the percentage 	// we are targeting
	while (target > current_total)
	{
		current_total += histogram_array[target_value];
		target_value++;
	}

	// Return the pixel value that covers the number of pixels 		// we are targeting
	return target_value;
}

// This uses the linear contrast stretch equation to calculate
// the new pixel values, writes them to a new matrix and then
// displays it
void calc_stretch(int b_low, int b_high, Mat input)
{
	// pixel_value holds the value of the current pixel
	int pixel_value;
	double stretch_value;

	for (int i = 0; i < input.rows; i++)
	{
		for (int j = 0; j < input.cols; j++)
		{
			pixel_value = input.at<uchar>(i, j);

			// If pixel value is less than b-low, the pixel value becomes 0
			if (pixel_value < b_low)
			{
				input.at<uchar>(i, j) = 0;
			}

			// If pixel value is greater than b-high, the pixel value becomes 255
			else if ((pixel_value > b_high))
			{
				input.at<uchar>(i, j) = 255;
			}

			// If the pixel value is between b-low and b-high
			// calculate the contrast stretch and set that as the new pixel value
			else
			{
				//Calculate contrast stretch
				stretch_value = (double(pixel_value - b_low) / double(b_high - b_low)) * 255;

				// Round down
				input.at<uchar>(i, j) = (int)round(stretch_value);
			}
		}
	}

	// Display the new image
	imshow("Linear Contrast Stretch", input);
}

void on_trackbar(int, void *)
{
	int b_low = calc_hist_perc(b_low_perc);
	int b_high = calc_hist_perc(b_high_perc);

	calc_stretch(b_low, b_high, input_image.clone());
}

void main()
{
	input_image = imread("C:\\Users\\Hassaan\\Pictures\\LowContrast.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	total_pixels = input_image.cols * input_image.rows;

	calc_histogram(input_image.clone());

	imshow("Linear Contrast Stretch", input_image);
	createTrackbar("Max %", "Linear Contrast Stretch", &b_high_perc, 100, on_trackbar);
	createTrackbar("Min %", "Linear Contrast Stretch", &b_low_perc, 100, on_trackbar);

	imshow("Input Image", input_image);
	waitKey(0);
}