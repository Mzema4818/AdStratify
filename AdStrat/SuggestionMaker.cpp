#include <string>
#include <vector>
#include "global.h"
#include "RandomForest.h"

using namespace std;

// Function to suggest a better ad placement
string suggestAdPlacement(const DataPoint& userPoint, const vector<string>& possiblePlacements, RandomForest& rf) {
    for (const auto& placement : possiblePlacements) {
        // Create a copy of the user's data point with a modified ad placement
        DataPoint modifiedPoint = userPoint;
        modifiedPoint.adPosition = placement;

        // Predict the click outcome for the modified data point
        int prediction = rf.predict(modifiedPoint);

        // If the modified placement predicts a click (1), return the suggestion
        if (prediction == 1) {
            return placement;
        }
    }
    // If no better placement is found, return the original placement
    return userPoint.adPosition;
}
