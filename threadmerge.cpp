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

//struct for passing arguments into threads
typedef struct
{
	int left, right; 
	int *array;
	int *target;
	int *Vreturn;
}Args;

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

//
void * mergeSort(void * arg){
	cout <<"Remember threads execute out of order" << endl;
	cout << "=======================================" << endl;
	Args *args = (Args *) arg;
	cout << args->array[100000-1] << endl;
	// print_array(args->array);
}

//combine half(s) of array into one array
int merge(){
	printf("Merging!!!!!!!!\n");
}

int main(int argc, char *argv[]) {
	//build global array
	read_input();
	//create a copy of the array and make a pointer to it
	int *carray = array_cpy(&array[0]);
	//create args struct and fill
	Args args;
	args.left = 0;
	args.right = array[100000-1];
	args.array = &carray[0];
	//start main thread
	pthread_t mainThread;
	int ret; 	
	ret = pthread_create(&mainThread, NULL, mergeSort, &args);
	//join threads  //if i didnt join the main would finish before merge() could do anything.
	void *status;
	pthread_join(mainThread, &status);
	// print_array(carray);

    return 0;
}