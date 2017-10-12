/*
Threaded Merge Sort 
Clemson University CPSC 3220
Monroe Stone
Mason Strohl 
*/

#include <fstream>
#include <iostream>
#include <pthread.h>
using namespace std;
pthread_mutex_t lockNumThread;
int numThread;
//struct for passing arguments into threads
typedef struct
{
	int left, right; 
	int *array;
	int *target;
	int Tid;
}ArgStruct;

//Global's 
int array[100000];
int finArray[100000];

//Functions 
//Read from file called "input.txt"
void read_input() {
	int i, index = 0;
    fstream cin;
    cin.open("input.txt");
	while (cin >> i)
	{
		array[index++] = i;
	}
	cin.close();
}

//Print function
void print_array(int* A) {
	int i = 0;
	while (i < 500)
	{
		cout << A[i++] << endl;
	}
}

//Array copy
int* array_cpy(int* OG){
	int i = 0;
	int* CP = new int[100000];
	while (i < 100000)
	{
		CP[i] = OG[i];
		// cout << CP[i] << "    " << OG[i] << endl;
		i++;
	}
	return CP;
}


void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void * mergeSortThreaded(void * arg){

	//set up arguments 
	ArgStruct *args = (ArgStruct *) arg;
	int left = args->left;
	int right = args->right;
	int * A = args->array;
	int * B = args->target;
	int midd;

	//base case 
	if (args->left < args->right){	
		midd = left+(right-left)/2;

		//set up args for the left thread
		ArgStruct Largs;
		Largs.left = left;
		Largs.right = midd;
		Largs.array = args->array;
		cout << "Creating thread for left half of list index " << Largs.left << "-" << Largs.right << endl; 
		int ret = 0; 	
		pthread_t leftThread;
		ret = pthread_create(&leftThread, NULL, mergeSortThreaded, (void *) &Largs);
		cout << "Merging for index " << Largs.left << "-" << Largs.right << endl;
		pthread_join(leftThread, NULL);	
		
		merge(args->array, left, midd, right);		
		

		//set up args for the right thread
		ArgStruct Rargs;
		Rargs.left = midd+1;
		Rargs.right = right;
		Rargs.array = args->array;
		cout << "Creating thread for right half of list index " << Rargs.left << "-" << Rargs.right << endl; 				
		pthread_t rightThread;
		ret = pthread_create(&rightThread, NULL, mergeSortThreaded, (void *) &Rargs);
		cout << "Merging for index " << Rargs.left << "-" << Rargs.right << endl;		
		pthread_join(rightThread, NULL);
	
		merge(args->array, left, midd, right);		
	}	
	cout << "Exiting Thread for index " << left << "- " << right << endl;
	pthread_exit(NULL);	
}

void mergeSort(int *array, int left, int right){
	ArgStruct args;
	args.array = array;
	args.left = left;
	args.right = right;

	cout <<"Remember threads execute out of order" << endl;
	cout << "=======================================" << endl;

	pthread_t thread;
	int id = pthread_create(&thread, NULL, mergeSortThreaded, &args);
	pthread_join(thread, NULL);
	return; 
}

int main(int argc, char *argv[]) {
	//build global array
	read_input();
	
	//launch into mergeSort
	mergeSort(array, 0, 500);
	print_array(array);
	
    return 0;
}
