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

    // Define four specific dataset sizes for testing
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

// Function to interact with the user and provide ad suggestions
void userAdInteraction(vector<DataPoint>& dataPoints, vector<string>& attributes, int numTrees) {
    cout << "\n--- User Ad Interaction ---" << endl;

    // Train the RandomForest model on the full dataset
    RandomForest rf = trainRandomForest(dataPoints, attributes, numTrees);

    char repeat = 'y';
    while (repeat == 'y' || repeat == 'Y') {
        // Prompt the user for input
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

        // Gender input
        cout << "Gender (Male/Female/Non-Binary): ";
        cin >> userPoint.gender;

        // Device type input
        cout << "Device Type (Desktop/Tablet/Mobile): ";
        cin >> userPoint.deviceType;

        // Current ad position input
        cout << "Current Ad Position (Top/Side/Bottom): ";
        cin >> userPoint.adPosition;

        // Browsing history input with validation
        do {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            cout << "Browsing History (Shopping, News, Entertainment, Education, Social Media): ";
            getline(cin, userPoint.browsingHistory);
            if (!isValidBrowsingHistory(userPoint.browsingHistory)) {
                cout << "Invalid browsing history! Please choose from Shopping, News, Entertainment, Education, or Social Media.\n";
            }
        } while (!isValidBrowsingHistory(userPoint.browsingHistory));

        // Time of day input
        cout << "Time of Day (Morning/Afternoon/Evening/Night): ";
        cin >> userPoint.timeOfDay;

        // Predict the click outcome for the user's input
        int prediction = rf.predict(userPoint);
        cout << "Prediction: " << (prediction == 1 ? "Click (ad is effective)" : "No Click (ad is not effective)") << endl;

        // Suggest a better ad placement if the prediction is "No Click"
        if (prediction == 0) {
            vector<string> possiblePlacements = { "Top", "Side", "Bottom" };
            string suggestion = suggestAdPlacement(userPoint, possiblePlacements, rf);
            if (suggestion != "None") {
                cout << "Suggested better ad placement: " << suggestion << endl;
            }
            else {
                cout << "No better ad placement found." << endl;
            }
        }

        // Ask if the user wants another suggestion
        cout << "\nWould you like to enter another configuration? (y/n): ";
        cin >> repeat;
    }
}

int main() {
    // Prompt user for dataset file path
    string filePath;
    cout << "Enter the path to the dataset file (e.g., ../ad_click_dataset.csv): ";
    cin >> filePath;

    // Load the dataset
    ImportedData loader(filePath);
    if (!loader.loadData()) {
        cerr << "Data loading failed! Check the file path and try again." << endl;
        return 1;
    }

    // Prompt user for the number of trees
    int numTrees;
    cout << "Enter the number of trees to train (max 10): ";
    cin >> numTrees;

    // Validate the number of trees
    if (numTrees < 1 || numTrees > 10) {
        cerr << "Invalid number of trees. Please enter a number between 1 and 10." << endl;
        return 1;
    }

    // Preprocess the dataset
    DataImputer imputer;
    vector<DataPoint>& dataPoints = loader.getDataPoints();
    imputer.impute(dataPoints);

    // Define attributes for RandomForest
    vector<string> attributes = { "gender", "deviceType", "adPosition", "browsingHistory", "timeOfDay" };

    // Perform non-functional requirement tests
    testEfficiency(dataPoints, attributes, numTrees);
    testScalability(dataPoints, attributes, numTrees);

    // Allow user interaction for personalized predictions and suggestions
    userAdInteraction(dataPoints, attributes, numTrees);

    return 0;
}




