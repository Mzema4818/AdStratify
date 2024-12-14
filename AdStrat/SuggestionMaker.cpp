#include <string>
#include <vector>
#include "global.h"
#include "RandomForest.h"

using namespace std;

// Function to suggest a better ad placement
string suggestAdPlacement(const DataPoint& userPoint, const vector<string>& possiblePlacements, RandomForest& rf) {
    for (const auto& placement : possiblePlacements) {
        // Skip the current ad position to avoid redundant suggestions
        if (placement == userPoint.adPosition) {
            continue;
        }

        // Create a modified data point with a new ad placement
        DataPoint modifiedPoint = userPoint;
        modifiedPoint.adPosition = placement;

        // Predict the click outcome for the modified placement
        int prediction = rf.predict(modifiedPoint);

        // If the modified placement predicts a click (1), return the suggestion
        if (prediction == 1) {
            return placement;
        }
    }
    // Return "None" if no better placement is found
    return "None";
}

