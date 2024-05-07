/*
    *** DO NOT MODIFY THIS FILE ***
    When I test your code, it will be with my own 
    unmodified version.

    Functions in sort.cpp will be called as seen below.
    However - you may write as many helper functions as
    you want in sort.cpp.
*/

#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "sort.hpp"

using namespace std::chrono; // Namespace for fancy ms timing

bool verify_sorted(int arr[], int n);
void fill_rand_array(int *arr, int n);

int main()
{
	int n2avg = 10;
	
	printf("\nCCPS616 - Lab 1");
	printf("Each result is the average of %d trials\n\n", n2avg);
	srand((int)time(NULL));

	/****************************************/
	/************ SELECTION SORT ************/
	/****************************************/
	{
		int sel_test_sizes[] = { 2000, 4000, 8000, 16000 };
		
		for (int n : sel_test_sizes)
		{
			int *nums = new int[n];
			int *sel_nums = new int[n];

			fill_rand_array(nums, n);

			printf("SELECTION    %5d nums: ", n);
			int time_sum = 0;
			for (int k = 0; k < n2avg; k++)
			{
				std::copy(nums, nums + n, sel_nums);
				auto begin = high_resolution_clock::now();

				selection_sort(sel_nums, n);
				
				auto dur = high_resolution_clock::now() - begin;
				time_sum += (int)duration_cast<milliseconds>(dur).count();

				assert(verify_sorted(sel_nums, n));
				std::cout << "." << std::flush;
			}
			printf(" Avg: %dms\n", time_sum / n2avg);

			delete[] nums;
			delete[] sel_nums;
		}
	}
	/****************************************/
	/********** END SELECTION SORT **********/
	/****************************************/


	/****************************************/
	/******* MERGE & MERGE-SEL SORT *********/
	/****************************************/
	{
		int merge_test_sizes[] = { 128000, 256000, 512000, 1024000 };
		
		for (int n : merge_test_sizes)
		{
			int *nums = new int[n];
			int *merge_nums = new int[n];
			int *merge_sel_nums = new int[n];

			fill_rand_array(nums, n);

			printf("MERGE      %7d nums: ", n);
			int time_sum = 0;
			for (int k = 0; k < n2avg; k++)
			{
				std::copy(nums, nums + n, merge_nums);
				auto begin = high_resolution_clock::now();

				merge_sort(merge_nums, n);
				
				auto dur = high_resolution_clock::now() - begin;
				time_sum += (int)duration_cast<milliseconds>(dur).count();

				assert(verify_sorted(merge_nums, n));
				std::cout << "." << std::flush;
			}
			printf(" Avg: %dms\n", time_sum / n2avg);
			

			printf("MERGE-SEL  %7d nums: ", n);
			int ax = 9;
			time_sum = 0;
			for (int k = 0; k < n2avg; k++)
			{
				std::copy(nums, nums + n, merge_sel_nums);
				auto begin = high_resolution_clock::now();

				merge_sel_sort(merge_sel_nums, n);
				
				auto dur = high_resolution_clock::now() - begin;
				time_sum += (int)duration_cast<milliseconds>(dur).count();

				assert(verify_sorted(merge_sel_nums, n));
				std::cout << "." << std::flush;
			}
			printf(" Avg: %dms\n", time_sum / n2avg);

			delete[] nums;
			delete[] merge_nums;
			delete[] merge_sel_nums;
		}
	}
	/****************************************/
	/****** END MERGE & MERGE-SEL SORT ******/
	/****************************************/

	printf("\n");
}

bool verify_sorted(int arr[], int n)
{
	for (int i = 0; i < n - 1; i++)
		if (arr[i] > arr[i + 1])
			return false;
	return true;
}

void fill_rand_array(int *arr, int n)
{
	for (int i = 0; i < n; i++)
		arr[i] = rand() % (n * 4) + 1;
}
