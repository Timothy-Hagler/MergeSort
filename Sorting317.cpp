// 
//  Author: Timmy Hagler
//  Date: November 4, 2020
//  Section: CS 317-03
//  Description: The MergeSort program shall take input and output files from the user. The input file will contain lines of strings
//  that will be sorted line by line into the output file using a mergesort. The program can handle more than 200,000 lines from the input file.
//

//Define this for ctime use
#define _CRT_SECURE_NO_WARNINGS

//Include all required headers
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>


//Include the standard library
using namespace std;

//Declare all functions
void MergeSort(string lines[], int left, int right);
void Merge(string lines[], int left, int middle, int right);
void PrintArray(string lines[], int length);



//Output file. It is global to avoid some parameters
ofstream outfile;


//Main function
int main()
{

    time_t startTime;   //Starting time of algorithms
    time_t endTime;     //Ending time of algorithms

    

    string *lines = NULL;    //Create a set of memory for all the lines in the file
    string input;       //The user input for the input file
    string output;      //The user input for the output file
    int size;           //Size of the number of lines. It will later be set to the number of lines in the file.
    ifstream infile;    //The input file that the sort will read from  
    string temp;        //temp string to store lines for counting

    bool goodFile = false;  //Determines if the input file is in existence
    
    int index = 0;  //Index to count number of lines in the file

    //Loop to determine if the input file is valid. If it is not, ask the user for another file.
    while (!goodFile)
    {
        //User inputs the file here
        cout << "Enter your input file for the MergeSort" << endl;
        cin >> input;

        //Open the user file
        infile.open(input.c_str());

        //Checks if the file is open/good
        if (infile.is_open())
        {
            cout << "Input file Opened" << endl << endl;
            goodFile = true;    //Ends the loop
        }
        else
            cout << "Enter a valid file" << endl;   //Restarts the loop since there was an invalid file


    }
    
    //User inputs the output file
    cout << "Enter your output file for the MergeSort" << endl;
    cin >> output;
    
    
    //Opens the output file. If that file does not exist, create a new one.
    outfile.open(output.c_str());

    //Counts all lines to find how big the dynamic array should be
    while (!infile.eof())
    {
        getline(infile, temp);  //Constantly replaces temp so it does not take much storage
        index++;    //Increase the index so the lines index is not replacing the previous. Also counts the total lines.
       
    }

    //Set the size to use in other functions as the number of lines.
    size = index;

    //Set the amount of memory to use for the lines array
    lines = new string[size + 1];

    //Reset the index
    index = 0;

    //Reset the input file
    infile.close();
    infile.open(input.c_str());

  
    //Append all lines to the lines array to prepare for sorting
    while (!infile.eof())
    {
        getline(infile, lines[index]);  //Place each line into the lines array
        index++;    //Increase the index so the lines index is not replacing the previous. Also counts the total lines.

    }


    //Get the start time
    startTime = time(&startTime);  //Measures current time to print to screen
    auto start = chrono::high_resolution_clock::now();  //Measures time to use in total time equation

    //Call MergeSort function
    MergeSort(lines, 0, size - 1);

    //Get ending time of MergeSort. Do not count PrintArray in time since it is not part of the sort
    endTime = time(&endTime);   //Measures ending time to print to screen
    auto end = chrono::high_resolution_clock::now();    //Measures time to use in total time equation

    //Call PrintArray function
    PrintArray(lines, size);

    //Delete the lines array to avoid a memory leak
    delete[] lines;

    //Close all files
    infile.close();
    outfile.close();

    //Calculate time duration of algorithm
    chrono::duration<float> duration = end - start;

    //Print starting, ending, and total times
    cout << endl;
    cout << "Starting time: " << ctime(&startTime) << endl;
    cout << "Ending time: " << ctime(&endTime) << endl;
    cout << "Approximate total elapsed time: " << duration.count() * 1000.0 << " milliseconds." << endl; //Multiplying by 1000.0 to put into miliseconds



    //Return 0 since the program has ended
    return 0;

}



//Splits the data into half of its previous self. Recursively splits the array so there is very little cost.
//Finds the middle point and mergesorts from the left to the middle and from one past the middle to the right
//This is repeated until no more splitting can be done so either left > right or left == right
void MergeSort(string lines[], int left, int right)
{
    //middle is used to find the middle index to split the array for the merge. Default is 0
    int middle = 0;

    //Check if the left variable is left of the right variable and then start the split
    if (left < right)
    {
        //Finds the middle of the array
        middle = left + (right - left) /2;
       
        //MergeSort the left side of the array
        MergeSort(lines, left, middle);
        
        //MergeSort the right side of the array
        MergeSort(lines, middle + 1, right);

        
        //Call Merge with the split arrays
        Merge(lines, left, middle, right);

        

    }
}

//Merge all of the divided data back together. This function will compare data in the left subarray to the right subarray
//and sort them from least to greatest. It uses two temp arrays to store the data for sorting.
//All of the cost comes from the Merge since it is not recursive. 
void Merge(string lines[], int left, int middle, int right)
{
    int t1 = middle - left + 1;     //Finds starting position of left subarray
    int t2 = right - middle;        //Finds starting position of right subarray

    int j = 0;      //index to use for the left side of the array comparison
    int k = 0;      //index to use for the right side of the array comparison
    int c = left;   //index to put each element into place. Starts on left of array since array is read left to right

   

    string *tempL = new string[(right - left) + 1], *tempR = new string[(right - left) + 1];      //Set up temporary arrays for each side of the array. 

    //Copy the lines from the lines left subarray into the left temp array for sorting
    for (int i = 0; i < t1; i++)
    {
        tempL[i] = lines[left + i];
    }

    //Copy the lines from the lines right subarray into the right temp array for sorting 
    for (int i = 0; i < t2; i++)
    {
        tempR[i] = lines[middle + i + 1];
    }


    //Compare the strings at each index between the two temp arrays.
    while (j < t1 && k < t2)
    {
        if (tempL[j] <= tempR[k])
        {
            lines[c] = tempL[j];    //if the string at index j in the left array is less than or equal to the right array at index k, place the string in the lines array at index c 
            j++;    //Increment index j since its content was placed in the lines array
        }
       else
        {
            lines[c] = tempR[k];    //if the string of the left array is not less than or equal to the right array, place the string from the right index in lines at index c
            k++;    //Increment index k since that content was placed into the lines array
        }
        c++;    //Increment index c so the strings are not replaced in the lines array
   }

    
    //Place all remaining strings from the left array into the lines array
    while (j < t1)
    {
        lines[c] = tempL[j];
        j++;    
        c++; 
    }

    //Check if there are any strings left in the right array. If so, copy those into the lines array at the end.
    if (k < t2)
    {
        while (k < t2)
        {
            lines[c] = tempR[k];
            k++;
            c++;
        }
    }

    //Delete the temp arrays to avoid a memory leak
    delete[] tempL, tempR;

}

//Function to print out the values of the array
void PrintArray(string lines[], int length)
{
    //Go through the lines array and print out the result of the MergeSort
    for (int i = 0; i < length; i++)
    { 
        outfile << lines[i] << endl;    //Print lines to the output file
    }
   
}