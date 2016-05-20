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

void roberts_operator(Mat input)
{
	int column_limit = input.cols;
	int row_limit = input.rows;

	// h1 and h2 are the two templates for roberts operator
	double h1 = 0;
	double h2 = 0;

	// This is where the edge image will go to
	Mat gradient = input.clone();

	// gradient_pixel will hold the final gradient value for the current pixel
	double gradient_pixel;

	// Loops through every pixel and uses a 3x3 Kernel median filter
	for (int i = 0; i < row_limit - 1; i++)
	{
		for (int j = 0; j < column_limit - 1; j++)
		{
			// Calculate the first template, top left element minus bottom right element, and then square it
			h1 = pow(input.at<uchar>(Point(j, i)) - input.at<uchar>(Point(j + 1, i + 1)), 2);

			// Calculate the second template, top right element minus the bottom left element, and then square it
			h2 = pow(input.at<uchar>(Point(j + 1, i)) - input.at<uchar>(Point(j, i + 1)), 2);
			
			// Calculate the final pixel value
			gradient_pixel = sqrt(h1 + h2);

			// Set the new pixel value in the gradient image
			gradient.at<uchar>(Point(j, i)) = round(gradient_pixel);

		}
	}

	// Display edge image
	imshow("Gradient", gradient);
}

void main()
{
	input_image = imread("", CV_LOAD_IMAGE_GRAYSCALE);

	roberts_operator(input_image);

	imshow("Input Image", input_image);
	waitKey(0);
}