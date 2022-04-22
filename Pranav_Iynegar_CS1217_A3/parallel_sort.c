/*===================================================
What's the main thing that we're doing?
1. Create a giant array of all 10000 ints
2. Split them into 5 sub-arrays of len 2000 each.
3. Each each of them to a separate process to sort
4. A final process merges these sorted lists at the end.
==================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define n 2000

struct args {
    int *array;
    int low;
    int high;
};

//================================================================================
// Quicksort from Programiz
// https://www.programiz.com/dsa/quick-sort
//================================================================================
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int array[], int low, int high) {
  
  // select the rightmost element as pivot
  int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void *quickSort(void *input) {
  if ( ((struct args*)input)->low < ((struct args*)input)->high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(((struct args*)input)->array, ((struct args*)input)->low, ((struct args*)input)->high);
    
    // recursive call on the left of pivot
    struct args *a = (struct args *)malloc(sizeof(struct args));
    a->array = ((struct args*)input)->array;
    a->low = ((struct args*)input)->low;
    a->high = pi-1;
    quickSort((void *)a);
    
    // recursive call on the right of pivot
    a->array = ((struct args*)input)->array;
    a->low = pi + 1;
    a->high = ((struct args*)input)->high;
    quickSort((void *)a);
  }
}
//================================================================================
//================================================================================


//================================================================================
// The code below for merging K sorted arrays has been taken from GeeksForGeeks
// https://www.geeksforgeeks.org/merge-k-sorted-arrays/
//================================================================================
void mergeArrays(int arr1[], int arr2[], int n1,
                             int n2, int arr3[])
{
    int i = 0, j = 0, k = 0;
   
    // Traverse both array
    while (i<n1 && j <n2)
    {
        // Check if current element of first
        // array is smaller than current element
        // of second array. If yes, store first
        // array element and increment first array
        // index. Otherwise do same with second array
        if (arr1[i] < arr2[j])
            arr3[k++] = arr1[i++];
        else
            arr3[k++] = arr2[j++];
    }
   
    // Store remaining elements of first array
    while (i < n1)
        arr3[k++] = arr1[i++];
   
    // Store remaining elements of second array
    while (j < n2)
        arr3[k++] = arr2[j++];
}
 
// A utility function to print array elements
void printArray(int arr[], int size)
{
   for (int i=0; i < size; i++)
       printf("%d ",arr[i]);
}
 
// This function takes an array of arrays as an argument and
// All arrays are assumed to be sorted. It merges them together
// and prints the final sorted output.
void mergeKArrays(int arr[][n],int i,int j, int output[])
{
    //if one array is in range
    if(i==j)
    {
        for(int p=0; p < n; p++)
        output[p]=arr[i][p];
        return;
    }
     
    //if only two arrays are left them merge them
    if(j-i==1)
    {
        mergeArrays(arr[i],arr[j],n,n,output);
        return;
    }
     
    //output arrays
    int out1[n*(((i+j)/2)-i+1)],out2[n*(j-((i+j)/2))];
     
    //divide the array into halves
    mergeKArrays(arr,i,(i+j)/2,out1);
    mergeKArrays(arr,(i+j)/2+1,j,out2);
     
    //merge the output array
    mergeArrays(out1,out2,n*(((i+j)/2)-i+1),n*(j-((i+j)/2)),output);
     
}
//================================================================================
//================================================================================

int main(void) {
    
    // Open file
    char *filename = "randomNum.txt";
    FILE *fp = fopen(filename, "r");

    // If file cannot be opened, then do this
    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    int num[10000];

    int subnum[5][2000];

    int i = 0;

    // Take all the inputs in a huge array
    while(fscanf(fp, "%d", &num[i]) != EOF) {
        i++;
    }

    int first_list_index = 0;

    // Split it into 5 different arrays
    for (int sublist = 0; sublist < 5; sublist++) {
        for (int index = 0; index < 2000; index++) {
            subnum[sublist][index] = num[first_list_index];
            first_list_index++;
        }
    }

    pthread_t thread_id[5];
    struct args *arg[5];

    for (int i = 0; i < 5; i++) {
        arg[i] = (struct args *)malloc(sizeof(struct args));
        arg[i]->array = subnum[i];
        arg[i]->low = 0;
        arg[i]->high = 2000;
    }

    // Send these 5 arrays into different processes to sort using the sort that c has
    for (int sublist = 0; sublist < 5; sublist++) {
        pthread_create(&thread_id[sublist], NULL, quickSort, (void *)arg[sublist]);
    }

    for (int sublist = 0; sublist < 5; sublist++) {
        pthread_join(thread_id[sublist], NULL);
    }

    // Once those lists are sorted, merge these two lists
    int k = 5;
    int output[10000];
    mergeKArrays(subnum, 0, k-1, output);

    FILE *fptr = fopen("sorted.txt", "w");

    for (int i = 0; i < 10000; i++) {
        fprintf(fptr, "%d\n", output[i]);
    }

    fclose(fptr);

    exit(0);
}