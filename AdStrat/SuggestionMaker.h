#ifndef SUGGESTIONMAKER_H
#define SUGGESTIONMAKER_H

#include <string>
#include <vector>
#include "global.h"
#include "RandomForest.h"

using namespace std;

// Function to suggest a better ad placement
string suggestAdPlacement(const DataPoint& userPoint, const vector<string>& possiblePlacements, RandomForest& rf);

#endif // SUGGESTIONMAKER_H

