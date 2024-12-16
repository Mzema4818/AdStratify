#include <iostream>
#include <chrono>
#include <algorithm> // For transform
#include <cctype>    // For tolower
#include <limits>    // For numeric_limits
#include "ImportedData.h"
#include "DataImputer.h"
#include "RegressionTests.h"
#include "SuggestionMaker.h"

using namespace std;

// Function Declarations
void testEfficiency(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees);
void testScalability(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees);
void testAccuracy(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees);
bool isValidBrowsingHistory(const string& browsingHistory);
void userAdInteraction(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees);

// Helper function to convert a string to lowercase
string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Function to validate browsing history
bool isValidBrowsingHistory(const string& browsingHistory) {
    vector<string> validOptions = { "shopping", "news", "entertainment", "education", "social media" }; // Lowercase options
    string lowerInput = toLowerCase(browsingHistory);
    return find(validOptions.begin(), validOptions.end(), lowerInput) != validOptions.end();
}

// Function to test system efficiency
void testEfficiency(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees) {
    cout << "\n--- Efficiency Testing ---" << endl;

    auto start = chrono::high_resolution_clock::now();

    // Train the RandomForest model
    RandomForest rf = trainRandomForest(dataPoints, attributes, numTrees);

    auto end = chrono::high_resolution_clock::now();
    cout << "Training time: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;

    // Measure prediction time for the entire dataset
    start = chrono::high_resolution_clock::now();
    for (const auto& dp : dataPoints) {
        rf.predict(dp);
    }
    end = chrono::high_resolution_clock::now();
    cout << "Prediction time for " << dataPoints.size() << " impressions: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;
}

// Function to test system scalability
void testScalability(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees) {
    cout << "\n--- Scalability Testing ---" << endl;

    vector<size_t> datasetSizes = { dataPoints.size() / 4, dataPoints.size() / 2, (3 * dataPoints.size()) / 4, dataPoints.size() };

    for (size_t datasetSize : datasetSizes) {
        vector<DataPoint> subset(dataPoints.begin(), dataPoints.begin() + datasetSize);

        auto start = chrono::high_resolution_clock::now();
        RandomForest rf = trainRandomForest(subset, attributes, numTrees);
        auto end = chrono::high_resolution_clock::now();
        cout << "Dataset size: " << subset.size()
            << " | Training time: "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << " ms" << endl;

        start = chrono::high_resolution_clock::now();
        for (const auto& dp : subset) {
            rf.predict(dp);
        }
        end = chrono::high_resolution_clock::now();
        cout << "Dataset size: " << subset.size()
            << " | Prediction time: "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << " ms" << endl;
    }
}

// Function to test accuracy
void testAccuracy(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees) {
    cout << "\n--- Accuracy Testing ---" << endl;

    RandomForest rf = trainRandomForest(dataPoints, attributes, numTrees);

    int correctPredictions = 0;
    int totalPredictions = dataPoints.size();

    for (const auto& dp : dataPoints) {
        int prediction = rf.predict(dp);
        if (prediction == dp.click) { // Assuming 'click' holds the actual outcome (1 for click, 0 for no click)
            correctPredictions++;
        }
    }

    double accuracy = (static_cast<double>(correctPredictions) / totalPredictions) * 100.0;
    cout << "Accuracy of the Random Forest model: " << accuracy << "%" << endl;
}

// Function to interact with the user and provide ad suggestions
void userAdInteraction(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees) {
    cout << "\n--- User Ad Interaction ---" << endl;

    RandomForest rf = trainRandomForest(dataPoints, attributes, numTrees);

    char repeat = 'y';
    while (repeat == 'y' || repeat == 'Y') {
        DataPoint userPoint;
        cout << "Enter the following details:\n";

        // Validate age
        do {
            cout << "Age (18-64): ";
            cin >> userPoint.age;
            if (userPoint.age < 18 || userPoint.age > 64) {
                cout << "Invalid age! Please enter a value between 18 and 64.\n";
            }
        } while (userPoint.age < 18 || userPoint.age > 64);

        cout << "Gender (Male/Female/Non-Binary): ";
        cin >> userPoint.gender;

        cout << "Device Type (Desktop/Tablet/Mobile): ";
        cin >> userPoint.deviceType;

        cout << "Current Ad Position (Top/Side/Bottom): ";
        cin >> userPoint.adPosition;

        do {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Browsing History (Shopping, News, Entertainment, Education, Social Media): ";
            getline(cin, userPoint.browsingHistory);
            if (!isValidBrowsingHistory(userPoint.browsingHistory)) {
                cout << "Invalid browsing history! Choose from Shopping, News, Entertainment, Education, or Social Media.\n";
            }
        } while (!isValidBrowsingHistory(userPoint.browsingHistory));

        cout << "Time of Day (Morning/Afternoon/Evening/Night): ";
        cin >> userPoint.timeOfDay;

        int prediction = rf.predict(userPoint);
        cout << "Prediction: " << (prediction == 1 ? "Click (ad is effective)" : "No Click (ad is not effective)") << endl;

        if (prediction == 0) {
            vector<string> possiblePlacements = { "Top", "Side", "Bottom" };
            string suggestion = suggestAdPlacement(userPoint, possiblePlacements, rf);
            cout << "Suggested better ad placement: " << (suggestion != "None" ? suggestion : "No better ad placement found.") << endl;
        }

        cout << "\nWould you like to enter another configuration? (y/n): ";
        cin >> repeat;
    }
}

int main() {
    string filePath;
    cout << "Enter the path to the dataset file (e.g., ../ad_click_dataset.csv): ";
    cin >> filePath;

    ImportedData loader(filePath);
    if (!loader.loadData()) {
        cerr << "Data loading failed! Check the file path and try again." << endl;
        return 1;
    }

    int numTrees;
    cout << "Enter the number of trees to train (max 100): ";
    cin >> numTrees;

    if (numTrees < 1 || numTrees > 100) {
        cerr << "Invalid number of trees. Please enter a number between 1 and 100." << endl;
        return 1;
    }

    DataImputer imputer;
    vector<DataPoint>& dataPoints = loader.getDataPoints();
    imputer.impute(dataPoints);

    vector<string> attributes = { "gender", "deviceType", "adPosition", "browsingHistory", "timeOfDay" };

    testEfficiency(dataPoints, attributes, numTrees);
    testScalability(dataPoints, attributes, numTrees);
    testAccuracy(dataPoints, attributes, numTrees);

    userAdInteraction(dataPoints, attributes, numTrees);

    return 0;
}






