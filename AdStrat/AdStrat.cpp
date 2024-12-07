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

    RandomForest rf(10);
    rf.train(trainingData, attributes);

    DataPoint newPoint = { 30, "Male", "Mobile", "Side", "Shopping", "Night", -1 }; //-1 means we want to test not train
    int prediction = rf.predict(newPoint);

    cout << "Prediction: " << (prediction == 1 ? "Good Ad Placement" : "Bad Ad Placement") << endl;

    return 0;
}
