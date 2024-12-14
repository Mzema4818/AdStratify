#ifndef REGRESSIONTESTS_H
#define REGRESSIONTESTS_H

#include <vector>
#include <string>
#include "RandomForest.h"
#include "ImportedData.h"

// Function to train the RandomForest model
RandomForest trainRandomForest(const std::vector<DataPoint>& dataPoints, const std::vector<std::string>& attributes, int num);

// Function to run all regression tests
void runRegressionTests(RandomForest& rf, const std::vector<DataPoint>& testCases, const std::vector<int>& expectedPredictions, const std::vector<std::string>& expectedSuggestions);

#endif // REGRESSIONTESTS_H
