#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <map>
#include "ImportedData.h"
#include "global.h"

namespace std {
    // Define the structure for decision tree nodes
    struct TreeNode {
        string splitAttribute;
        string splitValue;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        int prediction = -1; // -1 for non-leaf nodes, 0 or 1 for leaf nodes
    };

    // Function to calculate Gini Impurity for a split
    double calculateGini(const vector<DataPoint>& data);

    // Split data based on an attribute and value
    pair<vector<DataPoint>, vector<DataPoint>> splitData(
        const vector<DataPoint>& data,
        const string& attribute,
        const string& value);

    // Build a decision tree
    TreeNode* buildDecisionTree(vector<DataPoint>& data, const vector<string>& attributes);

    // Predict using a single tree
    int predictTree(TreeNode* node, const DataPoint& point);

    // Random forest class
    class RandomForest {
        int numTrees;
        vector<TreeNode*> trees;

    public:
        RandomForest(int n); // Constructor

        // Train the Random Forest model
        void train(const vector<DataPoint>& data, const vector<string>& attributes);

        // Predict the outcome for a data point
        int predict(const DataPoint& point);

        // Getter for number of trees (debugging purposes)
        int getNumTrees() const { return trees.size(); }

        // Predict using a specific tree (debugging purposes)
        int predictWithTree(const DataPoint& point, int treeIndex) const;
    };

} // namespace std

#endif // RANDOM_FOREST_H

