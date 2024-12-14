#include "RegressionTests.h"
#include <iostream>
#include <vector>
#include "SuggestionMaker.h"  // Make sure to include this if needed

using namespace std;

// Function to train the RandomForest model once
RandomForest trainRandomForest(const vector<DataPoint>& dataPoints, const vector<string>& attributes, int num) {
    RandomForest rf(num);
    rf.train(dataPoints, attributes);
    return rf;
}

// Function to run all regression tests
void runRegressionTests(RandomForest& rf, const vector<DataPoint>& testCases, const vector<int>& expectedPredictions, const vector<string>& expectedSuggestions) {
    int failedTests = 0;

    // Loop through the test cases and check predictions and suggestions
    for (size_t i = 0; i < testCases.size(); ++i) {
        int prediction = rf.predict(testCases[i]);
        if (prediction != expectedPredictions[i]) {
            cerr << "Test case " << i << " failed: Expected prediction "
                << expectedPredictions[i] << ", but got " << prediction << endl;
            failedTests++;
        }
        else {
            cout << "Test case " << i << " passed: Prediction " << prediction << endl;
        }

        if (prediction == 0) {
            vector<string> possiblePlacements = { "Top", "Side", "Bottom" };
            string suggestion = suggestAdPlacement(testCases[i], possiblePlacements, rf);
            if (suggestion != expectedSuggestions[i]) {
                cerr << "Suggestion mismatch for test case " << i << ": Expected "
                    << expectedSuggestions[i] << ", but got " << suggestion << endl;
                failedTests++;
            }
            else {
                cout << "Suggestion for test case " << i << " passed: " << suggestion << endl;
            }
        }
    }

    if (failedTests == 0) {
        cout << "All regression tests passed!" << endl;
    }
    else {
        cout << failedTests << " tests failed!" << endl;
    }
}
