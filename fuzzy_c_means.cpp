// Course: CS4491
// Student name: Syed Hassaan Ali
// Student ID: 000675546
// Assignment #: #6
// Due Date: 11/30/2015
// Signature: ________________________
//			  (The signature means that the work is your own, not from somewhere else)
// Score: ______________

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <random>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int c_value = 2;
double m_value = 2;
double epsilon_value = .01;
Mat input_image;

// Calculates a uniformly random value
double random_double(double from, double upto)
 {
	 random_device rd;
	 mt19937 gen(rd());
	 uniform_real_distribution<> dis(from, upto);

	 return dis(gen);
}

// Used to calculate a random membership matrix
vector<double> random_vec_double(int size)
{
	vector<double> vector_hold;
	vector<double> uniform_random_vec;

	for (int i = 0; i < size - 1; i++)
	{
		vector_hold.push_back(random_double(0, 1));
	}

	vector_hold.push_back(0);
	vector_hold.push_back(1);

	sort(vector_hold.begin(), vector_hold.end());

	for (int i = 0; i < vector_hold.size() - 1; i++)
	{
		uniform_random_vec.push_back(vector_hold[i + 1] - vector_hold[i]);
	}

	return uniform_random_vec;
}

// Used in the calc_membership function
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

double calc_membership(vector<Vec3b> cluster_vector, Vec3b target_cluster, Vec3b pixel, double m)
{
	double cluster_sum = 0.0;
	double membership;
	double target_distance = vector_distance(pixel, target_cluster);

	for (int i = 0; i < cluster_vector.size(); i++)
	{
		cluster_sum += pow(target_distance / vector_distance(pixel, cluster_vector[i]), 2 / (m - 1));
	}

	membership = 1 / cluster_sum;

	return membership;
}

void fuzzy_c_means(Mat input_image, int c, double m, double epsilon)
{
	bool has_converged = false;
	int row_limit = input_image.rows;
	int column_limit = input_image.cols;
	vector<Vec3b> cluster_vector;
	Vec3b* pixel;
	vector<vector<vector<double>>> membership_array;
	vector<vector<double>> mem_sum_pixel;
	vector<double> mem_squared_sum;
	double prev_membership;
	int loop_times = 0;
	vector<double>::iterator largest_mem_it;


	/*----------------------------------------------------------------
	--------Setup membership matrix and cluster center vector---------
	----------------------------------------------------------------*/
	
	mem_sum_pixel.resize(c);
	mem_squared_sum.resize(c);

	for (int i = 0; i < mem_sum_pixel.size(); i++)
	{
		mem_sum_pixel[i].resize(3);
		
		mem_sum_pixel[i][0] = 0;
		mem_sum_pixel[i][1] = 0;
		mem_sum_pixel[i][2] = 0;

		mem_squared_sum[i] = 0;
	}

	membership_array.resize(row_limit);
	for (int i = 0; i < row_limit; i++)
	{
		membership_array[i].resize(column_limit);

		for (int j = 0; j < column_limit; j++)
		{
			membership_array[i][j].resize(c);
		}
	}

	cluster_vector.resize(c);


	/*----------------------------------------------------------------
	----Intitalize membership matrix and calculate cluster center-----
	----------------------------------------------------------------*/

	// Loop through membership matrix and intialize it with random values
	for (int i = 0; i < membership_array.size(); i++)
	{
		pixel = input_image.ptr<Vec3b>(i);

		for (int j = 0; j < column_limit; j++)
		{
			membership_array[i][j] = random_vec_double(c);
			
			for (int k = 0; k < cluster_vector.size(); k++)
			{
				mem_sum_pixel[k][0] += pow(membership_array[i][j][k], m) * (double)pixel[j][0];
				mem_sum_pixel[k][1] += pow(membership_array[i][j][k], m) * (double)pixel[j][1];
				mem_sum_pixel[k][2] += pow(membership_array[i][j][k], m) * (double)pixel[j][2];

				mem_squared_sum[k] += pow(membership_array[i][j][k], m);
			}
		}
	}

	// Calculate cluster centers
	for (int i = 0; i < mem_sum_pixel.size(); i++)
	{
		cout << mem_squared_sum[i] << endl;
		cluster_vector[i] = Vec3b(mem_sum_pixel[i][0] / mem_squared_sum[i], mem_sum_pixel[i][1] / mem_squared_sum[i], mem_sum_pixel[i][2] / mem_squared_sum[i]);

		cout << cluster_vector[i] << endl;
	}


	/*----------------------------------------------------------------
	-------Keep updating matrix and centers until they converge-------
	----------------------------------------------------------------*/

	while (has_converged == false)
	{
		has_converged = true;

		// Reset cluster center calculation values
		for (int i = 0; i < mem_sum_pixel.size(); i++)
		{
			mem_sum_pixel[i][0] = 0;
			mem_sum_pixel[i][1] = 0;
			mem_sum_pixel[i][2] = 0;

			mem_squared_sum[i] = 0;
		}

		for (int i = 0; i < row_limit; i++)
		{
			pixel = input_image.ptr<Vec3b>(i);

			for (int j = 0; j < column_limit; j++)
			{
				for (int k = 0; k < cluster_vector.size(); k++)
				{
					prev_membership = membership_array[i][j][k];

					membership_array[i][j][k] = calc_membership(cluster_vector, cluster_vector[k], pixel[j], m);

					mem_sum_pixel[k][0] += pow(membership_array[i][j][k], m) * (double)pixel[j][0];
					mem_sum_pixel[k][1] += pow(membership_array[i][j][k], m) * (double)pixel[j][1];
					mem_sum_pixel[k][2] += pow(membership_array[i][j][k], m) * (double)pixel[j][2];

					mem_squared_sum[k] += pow(membership_array[i][j][k], m);

					// Check to see if the menbership has converged
					if (abs(prev_membership - membership_array[i][j][k]) > epsilon)
					{
						has_converged = false;
					}

				}

			}
		}

		cout << "---------------------" << endl;

		// Calculate cluster centers
		for (int i = 0; i < cluster_vector.size(); i++)
		{
			cluster_vector[i] = Vec3b(mem_sum_pixel[i][0] / mem_squared_sum[i], mem_sum_pixel[i][1] / mem_squared_sum[i], mem_sum_pixel[i][2] / mem_squared_sum[i]);
			cout << cluster_vector[i] << endl;
		}

		cout << "---------------------" << endl;

		loop_times++;
	}

	cout << "Times looped: " << loop_times << endl;

	for (int i = 0; i < cluster_vector.size(); i++)
	{
		cout << cluster_vector[i] << endl;
	}


	/*----------------------------------------------------------------
	----------------Draw the clusters onto the image------------------
	----------------------------------------------------------------*/

	for (int i = 0; i < row_limit; i++)
	{
		pixel = input_image.ptr<Vec3b>(i);

		for (int j = 0; j < column_limit; j++)
		{
			largest_mem_it = max_element(membership_array[i][j].begin(), membership_array[i][j].end());
			pixel[j] = cluster_vector[distance(membership_array[i][j].begin(), largest_mem_it)];
		}
	}

	// Show new image
	imshow("Fuzzy C-Means", input_image);
}	

void on_trackbar(int, void*)
{
	fuzzy_c_means(input_image.clone(), c_value, m_value, epsilon_value);
}

void main()
{
	input_image = imread("C:\\Users\\Hassaan\\Pictures\\Lenna.png");
	imshow("Input", input_image);

	srand(time(0));
	fuzzy_c_means(input_image.clone(), 3, 2, .01);

	while (waitKey(0) < 1)
	{

	}
}