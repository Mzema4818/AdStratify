#include "RandomForest.h"
#include "global.h"
#include <iostream> // For displaying progress

namespace std {

    // Function to calculate Gini Impurity for a split
    double calculateGini(const vector<DataPoint>& data) {
        if (data.empty()) return 0.0;

        int total = data.size();
        int countClick = count_if(data.begin(), data.end(), [](const DataPoint& dp) { return dp.click == 1; });
        double pClick = static_cast<double>(countClick) / total;
        double pNoClick = 1.0 - pClick;

        return 1.0 - (pClick * pClick + pNoClick * pNoClick);
    }

    // Split data based on an attribute and value
    pair<vector<DataPoint>, vector<DataPoint>> splitData(
        const vector<DataPoint>& data,
        const string& attribute,
        const string& value) {

        vector<DataPoint> leftSplit, rightSplit;
        for (const auto& dp : data) {
            if ((attribute == "gender" && dp.gender == value) ||
                (attribute == "deviceType" && dp.deviceType == value) ||
                (attribute == "adPosition" && dp.adPosition == value) ||
                (attribute == "browsingHistory" && dp.browsingHistory == value) ||
                (attribute == "timeOfDay" && dp.timeOfDay == value)) {
                leftSplit.push_back(dp);
            }
            else {
                rightSplit.push_back(dp);
            }
        }
        return { leftSplit, rightSplit };
    }

    // Build a decision tree
    TreeNode* buildDecisionTree(vector<DataPoint>& data, const vector<string>& attributes) {
        if (data.empty()) return nullptr;

        // Check if all data points have the same target value
        bool allSame = true;
        int firstClick = data[0].click;
        for (const auto& point : data) {
            if (point.click != firstClick) {
                allSame = false;
                break;
            }
        }

        if (allSame) {
            TreeNode* leaf = new TreeNode();
            leaf->prediction = firstClick;
            return leaf;
        }

        if (attributes.empty()) {
            // Majority class leaf node
            int countClick = count_if(data.begin(), data.end(), [](const DataPoint& dp) { return dp.click == 1; });
            TreeNode* leaf = new TreeNode();
            leaf->prediction = (countClick >= data.size() / 2) ? 1 : 0;
            return leaf;
        }

        // Find the best split
        double bestGini = 1.0;
        string bestAttribute, bestValue;
        vector<DataPoint> bestLeftSplit, bestRightSplit;

        for (const auto& attr : attributes) {
            vector<string> uniqueValues;
            for (const auto& point : data) {
                if (attr == "gender") uniqueValues.push_back(point.gender);
                else if (attr == "deviceType") uniqueValues.push_back(point.deviceType);
                else if (attr == "adPosition") uniqueValues.push_back(point.adPosition);
                else if (attr == "browsingHistory") uniqueValues.push_back(point.browsingHistory);
                else if (attr == "timeOfDay") uniqueValues.push_back(point.timeOfDay);
            }
            sort(uniqueValues.begin(), uniqueValues.end());
            uniqueValues.erase(unique(uniqueValues.begin(), uniqueValues.end()), uniqueValues.end());

            for (const auto& value : uniqueValues) {
                auto splitResult = splitData(data, attr, value);
                auto& leftSplit = splitResult.first;
                auto& rightSplit = splitResult.second;

                double gini = (static_cast<double>(leftSplit.size()) / data.size()) * calculateGini(leftSplit) +
                    (static_cast<double>(rightSplit.size()) / data.size()) * calculateGini(rightSplit);

                if (gini < bestGini) {
                    bestGini = gini;
                    bestAttribute = attr;
                    bestValue = value;
                    bestLeftSplit = leftSplit;
                    bestRightSplit = rightSplit;
                }
            }
        }

        if (bestLeftSplit.empty() || bestRightSplit.empty()) {
            TreeNode* leaf = new TreeNode();
            leaf->prediction = count_if(data.begin(), data.end(), [](const DataPoint& dp) { return dp.click == 1; }) >= data.size() / 2 ? 1 : 0;
            return leaf;
        }

        TreeNode* root = new TreeNode();
        root->splitAttribute = bestAttribute;
        root->splitValue = bestValue;
        root->left = buildDecisionTree(bestLeftSplit, attributes);
        root->right = buildDecisionTree(bestRightSplit, attributes);

        return root;
    }

    // Predict using a single tree
    int predictTree(TreeNode* node, const DataPoint& point) {
        if (!node->left && !node->right) return node->prediction;

        if ((node->splitAttribute == "gender" && point.gender == node->splitValue) ||
            (node->splitAttribute == "deviceType" && point.deviceType == node->splitValue) ||
            (node->splitAttribute == "adPosition" && point.adPosition == node->splitValue) ||
            (node->splitAttribute == "browsingHistory" && point.browsingHistory == node->splitValue) ||
            (node->splitAttribute == "timeOfDay" && point.timeOfDay == node->splitValue)) {
            return predictTree(node->left, point);
        }
        else {
            return predictTree(node->right, point);
        }
    }

    // Random forest class
    RandomForest::RandomForest(int n) : numTrees(n) {}

    void RandomForest::train(const vector<DataPoint>& data, const vector<string>& attributes) {
        mt19937 rng(random_device{}());
        for (int i = 0; i < numTrees; ++i) {
            vector<DataPoint> sample;
            for (size_t j = 0; j < data.size(); ++j) {
                sample.push_back(data[rng() % data.size()]);
            }

            vector<string> selectedAttributes = attributes;
            shuffle(selectedAttributes.begin(), selectedAttributes.end(), rng);
            selectedAttributes.resize(3); // Choose a subset of attributes

            trees.push_back(buildDecisionTree(sample, selectedAttributes));

            // Display progress after each tree is built
            double progress = static_cast<double>(i + 1) / numTrees * 100;
            std::cout << "Training progress: " << progress << "% (" << i + 1 << " out of " << numTrees << " trees trained)\r";
            std::cout.flush();
        }
        std::cout << std::endl; // Move to the next line after progress display
    }

    int RandomForest::predict(const DataPoint& point) {
        vector<int> predictions;
        for (const auto& tree : trees) {
            predictions.push_back(predictTree(tree, point));
        }

        int ones = count(predictions.begin(), predictions.end(), 1);
        return (ones > predictions.size() / 2) ? 1 : 0;
    }

    int RandomForest::predictWithTree(const DataPoint& point, int treeIndex) const {
        if (treeIndex < 0 || treeIndex >= trees.size()) {
            cerr << "Error: Tree index out of range!" << endl;
            return -1; // Indicating an invalid prediction
        }
        return predictTree(trees[treeIndex], point);
    }

} // namespace std

