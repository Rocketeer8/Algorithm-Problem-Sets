
/*
    Fill in this block comment before submitting.
    Name: Hai Ning Liu
*/

#include <iostream>
#include <stdlib.h>

// You are not limited to these functions. 
// Write as many helpers as you want
// You might find it useful to overload and
// pass additional arguments, for example.
void merge_sort_helper(int *arr, int start, int end);
void merge(int *arr, int start, int end, int mid);
void merge_sel_sort_helper(int *arr, int start, int end);

void selection_sort(int *arr, int n)
{
	// Your code here

    for (int i = 0; i < n; i++) {
        int min_index = i;

        for (int j = i; j < n; j++) {
            if (arr[j] < arr[min_index]) {min_index = j;}
        }
        int temp = arr[i];
        // swap minimum to the left
        arr[i] = arr[min_index];
        arr[min_index] = temp;
    }
}

void merge_sort(int *arr, int n)
{
	// Your code here
    merge_sort_helper(arr, 0, n-1);
}

void merge_sort_helper(int *arr, int start, int end) {
        if (start >= end)
            return; // Returns recursively
        
        // same as (end+start)/2 but avoid overflow for large end and start
        int mid = start + (end - start) / 2;
        merge_sort_helper(arr, start, mid);
        merge_sort_helper(arr, mid + 1, end);
        merge(arr, start, end, mid);
}

void merge (int *arr, int start, int end, int mid) {
    int arrayOneSize = mid - start + 1;
    int arrayTwoSize = end - mid;
 
    // Create temperary left and right array
    int *leftArray = new int[arrayOneSize];
    int *rightArray = new int[arrayTwoSize];
 
    // copy left half to left array, right half to right array
    // if total elments is odd number, left will take one more element than right
    for (int i = 0; i < arrayOneSize; i++)
        leftArray[i] = arr[start + i];
    for (int j = 0; j < arrayTwoSize; j++)
        rightArray[j] = arr[mid + 1 + j];
 
    int indexL = 0;
    int indexR = 0;
    // index of the orginal array, starting index is the start of left array
    int indexMerge = start;

    // Merge the left and right array back into array[start..end], in sorted fassion
    while (indexL < arrayOneSize && indexR < arrayTwoSize) {
        if (leftArray[indexL] <= rightArray[indexR]) {
            arr[indexMerge] = leftArray[indexL];
            indexL++;
        } else {
            arr[indexMerge] = rightArray[indexR];
            indexR++;
        }
        indexMerge++;
    }

    // copy the rest of the left array into arr (if any elements left)
    while (indexL < arrayOneSize) {
        arr[indexMerge] = leftArray[indexL];
        indexL++;
        indexMerge++;
    }
    
    // copy the rest of the right array into arr (if any elements left)  
    while (indexR < arrayTwoSize) {
        arr[indexMerge] = rightArray[indexR];
        indexR++;
        indexMerge++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

void merge_sel_sort(int *arr, int n)
{
    // Your code here
    merge_sel_sort_helper(arr, 0, n - 1); 

}

void merge_sel_sort_helper(int *arr, int start, int end) { 
        // size of array is end index - start index + 1
        // switch to selection sort when size is <= 12 seems to be the fastest
        if (end - start + 1 <= 12) {
            selection_sort(arr+start, end - start + 1);
            return; // Returns recursively
        }
       
        // same as (end+start)/2 but avoid overflow for large end and start
        int mid = start + (end - start) / 2;
        merge_sel_sort_helper(arr, start, mid);
        merge_sel_sort_helper(arr, mid + 1, end);
        merge(arr, start, end, mid);
}



