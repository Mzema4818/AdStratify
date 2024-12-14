#include <iostream>
#include <vector>
#include "ImportedData.h"
#include "DataImputer.h"
#include "RegressionTests.h"  // Include the header for regression tests
#include "SuggestionMaker.h"  // Include the SuggestionMaker header

using namespace std;

int main() {
    // Load the dataset
    ImportedData loader("../ad_click_dataset.csv");
    if (!loader.loadData()) {
        cerr << "Data loading failed!" << endl;
        return 1;
    }

    DataImputer imputer;
    vector<DataPoint>& dataPoints = loader.getDataPoints();
    imputer.impute(dataPoints);

    // Define attributes for training
    vector<string> attributes = { "gender", "deviceType", "adPosition", "browsingHistory", "timeOfDay" };

    // Train the RandomForest model once and pass it to the test function
    RandomForest rf = trainRandomForest(dataPoints, attributes, 1);

    // Define test cases
    vector<DataPoint> testCases = {
        {25, "Male", "Desktop", "Bottom", "News", "Morning", -1},        // Test case 1
        {47, "Female", "Tablet", "Side", "Education", "Afternoon", -1},   // Test case 2
        {18, "Non-Binary", "Mobile", "Top", "Entertainment", "Night", -1}, // Test case 3
        {30, "Male", "Mobile", "Top", "Shopping", "Evening", -1},         // Test case 4
        {42, "Female", "Desktop", "Side", "Technology", "Morning", -1},   // Test case 5
        {51, "Non-Binary", "Tablet", "Bottom", "Health", "Afternoon", -1},// Test case 6
        {29, "Male", "Desktop", "Top", "News", "Night", -1},              // Test case 7
        {21, "Non-Binary", "Mobile", "Top", "Gaming", "Afternoon", -1}    // Test case 8
    };

    vector<int> expectedPredictions = { 0, 1, 1, 0, 1, 1, 1, 1 };
    vector<string> expectedSuggestions = { "Top", "None", "None", "Side", "None", "Bottom", "Top", "None" };

    // Run the regression tests
    runRegressionTests(rf, testCases, expectedPredictions, expectedSuggestions);

    // Now, let's use static data for a single prediction (as per your request)
    vector<string> possiblePlacements = { "Top", "Side", "Bottom" };
    DataPoint userPoint = { 21, "Non-Binary", "Mobile", "Top", "Gaming", "Afternoon", -1 };

    int prediction = rf.predict(userPoint);

    cout << "Prediction: " << prediction;

    if (prediction == 0) {
        string suggestion = suggestAdPlacement(userPoint, possiblePlacements, rf);
        cout << " | Suggested placement: " << suggestion << endl;
    }
    else {
        cout << endl;
    }

    return 0;
}
