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
	while (i < 100000)
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


//combine half(s) of array into one array
void merge(int *A, int left, int right){
	cout << "Merging for index " << left << "-" << right << endl;
	
		int L = left;
		int R = right; 
		int midd = L + ((R - L) / 2);
		int midPlusOne = midd + 1;		
		int index = 0;
		int *B = new int[100000];
		while((L <= midd) && (midPlusOne <= R))
		{
		    if(A[L] <= A[midPlusOne])
		    {
		        B[index++] = A[L++];
		    }
		    else
		    {
				B[index++] = A[(midPlusOne)++];
		    }
		}

		if (L == midd + 1) {
			while ((midPlusOne) <= R){
				B[index++] = A[(midPlusOne)++];
			}
		} 
		else {
			while (L <= midd){
				B[index++] = A[L++];
			}
		}

		L = left;
		index = 0;
		while (L <= R){
			A[L++] = B[index++];
		}

		return;
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
		//lock
		// pthread_mutex_lock(&lockNumThread);		
			Largs.Tid = numThread++;
		// pthread_mutex_unlock(&lockNumThread);
		int ret = 0; 	
		pthread_t leftThread;
		ret = pthread_create(&leftThread, NULL, mergeSortThreaded, (void *) &Largs);
		pthread_join(leftThread, NULL);		

		//set up args for the right thread
		ArgStruct Rargs;
		Rargs.left = midd+1;
		Rargs.right = right;
		Rargs.array = args->array;
		cout << "Creating thread for right half of list index " << Rargs.left << "-" << Rargs.right << endl; 	
		//lock
		// pthread_mutex_lock(&lockNumThread);		
			Rargs.Tid = numThread++;
		// pthread_mutex_unlock(&lockNumThread);			
		pthread_t rightThread;
		ret = pthread_create(&rightThread, NULL, mergeSortThreaded, (void *) &Rargs);
		pthread_join(rightThread, NULL);
	
		cout << "NUM THREADS::: " << numThread << endl;
		
		// merge(args->array, left, right);		
	}
	pthread_exit(NULL);
	print_array(A);
	
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

    return 0;
}
