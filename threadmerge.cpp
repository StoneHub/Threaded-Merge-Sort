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


// A function to merge the two half into a sorted data.
void merge(int *a, int low, int high, int mid)
{
	// We have low to mid and mid+1 to high already sorted.
	int i, j, k, temp[high-low+1];
	i = low;
	k = 0;
	j = mid + 1;
 
	// Merge the two parts into temp[].
	while (i <= mid && j <= high)
	{
		if (a[i] < a[j])
		{
			temp[k] = a[i];
			k++;
			i++;
		}
		else
		{
			temp[k] = a[j];
			k++;
			j++;
		}
	}
 
	// Insert all the remaining values from i to mid into temp[].
	while (i <= mid)
	{
		temp[k] = a[i];
		k++;
		i++;
	}
 
	// Insert all the remaining values from j to high into temp[].
	while (j <= high)
	{
		temp[k] = a[j];
		k++;
		j++;
	}
 
 
	// Assign sorted data stored in temp[] to a[].
	for (i = low; i <= high; i++)
	{
		a[i] = temp[i-low];
	}
}

void * mergeSortThreaded(void * arg){

	//set up arguments 
	ArgStruct *args = (ArgStruct *) arg;
	int left = args->left;
	int right = args->right;
	int * A = args->array;
	int * B = args->target;

	//base case 
	if (args->left < args->right){	
		int midd = (left + right)/2;				
		//set up args for the left thread
		ArgStruct Largs;
		Largs.left = left;
		Largs.right = midd;
		Largs.array = args->array;
		int middL = (Largs.left + Largs.right)/2;				
		cout << "Creating thread for left half of list index " << Largs.left << "-" << Largs.right << endl; 
		int ret = 0; 	
		pthread_t leftThread;
		ret = pthread_create(&leftThread, NULL, mergeSortThreaded, (void *) &Largs);
		cout << "Merging for index " << Largs.left << "-" << Largs.right << endl;
		pthread_join(leftThread, NULL);	
		
		// merge(Largs.array, Largs.left, Largs.right, middL);		
		

		//set up args for the right thread
		ArgStruct Rargs;
		Rargs.left = midd+1;
		Rargs.right = right;
		Rargs.array = args->array;
		int midd2 = (Rargs.left + Rargs.right)/2;				
		cout << "Creating thread for right half of list index " << Rargs.left << "-" << Rargs.right << endl; 				
		pthread_t rightThread;
		ret = pthread_create(&rightThread, NULL, mergeSortThreaded, (void *) &Rargs);
		cout << "Merging for index " << Rargs.left << "-" << Rargs.right << endl;		
		pthread_join(rightThread, NULL);
	
		// merge(Rargs.array, Rargs.left, Rargs.right, midd2);	
		merge(args->array, left, right, midd);	

		cout << "Exiting Thread for index " << left << "- " << right << endl;
		pthread_exit(NULL);	
	}	
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
