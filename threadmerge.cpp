/*
Threaded Merge Sort 
Clemson University CPSC 3220
Monroe Stone
Mason Strohl 
*/

#include <fstream>
#include <iostream>
using namespace std;

typedef struct
{
	int left, right; 
	int *array;
	int *target;
}args;

	int array[100000];


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

void print_array() {
	int i = 0;
	while (i < 100000)
	{
		cout << array[i++] << endl;
	}
}

int main(int argc, char *argv[]) {
	int* arrayptr; 
	arrayptr = array; 
	read_input();


    return 0;
}