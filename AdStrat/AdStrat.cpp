#include <iostream>
#include "RandomForest.h"

// Declare the function prototype here
void displayCSVData(const char* filename);

using namespace std;

int main() {
    //const char* filename = "../ad_click_dataset.csv";  // Path to your CSV file
    //displayCSVData(filename);  // Call the function to display the CSV data

    vector<DataPoint> trainingData = {
        {25, "Male", "Mobile", "Top", "Shopping", "Morning", 1},
        {34, "Female", "Desktop", "Side", "News", "Afternoon", 0},
        {45, "Non-Binary", "Tablet", "Bottom", "Entertainment", "Evening", 1},
    };

    vector<string> attributes = { "gender", "deviceType", "adPosition", "browsingHistory", "timeOfDay" };

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


<<<<<<< HEAD
=======

>>>>>>> 922163a3dfe6a60ce2a74c561b805b88a7b1fb33
