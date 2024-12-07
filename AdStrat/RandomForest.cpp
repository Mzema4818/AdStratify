#include "RandomForest.h"

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

        vector<DataPoint> left, right;
        for (const auto& dp : data) {
            if ((attribute == "gender" && dp.gender == value) ||
                (attribute == "deviceType" && dp.deviceType == value) ||
                (attribute == "adPosition" && dp.adPosition == value) ||
                (attribute == "browsingHistory" && dp.browsingHistory == value) ||
                (attribute == "timeOfDay" && dp.timeOfDay == value)) {
                left.push_back(dp);
            }
            else {
                right.push_back(dp);
            }
        }
        return { left, right };
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
        vector<DataPoint> bestLeft, bestRight;

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
                auto [left, right] = splitData(data, attr, value);
                double gini = (static_cast<double>(left.size()) / data.size()) * calculateGini(left) +
                    (static_cast<double>(right.size()) / data.size()) * calculateGini(right);

                if (gini < bestGini) {
                    bestGini = gini;
                    bestAttribute = attr;
                    bestValue = value;
                    bestLeft = left;
                    bestRight = right;
                }
            }
        }

        if (bestLeft.empty() || bestRight.empty()) {
            TreeNode* leaf = new TreeNode();
            leaf->prediction = count_if(data.begin(), data.end(), [](const DataPoint& dp) { return dp.click == 1; }) >= data.size() / 2 ? 1 : 0;
            return leaf;
        }

        TreeNode* root = new TreeNode();
        root->splitAttribute = bestAttribute;
        root->splitValue = bestValue;
        root->left = buildDecisionTree(bestLeft, attributes);
        root->right = buildDecisionTree(bestRight, attributes);

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
        }
    }

    int RandomForest::predict(const DataPoint& point) {
        vector<int> predictions;
        for (const auto& tree : trees) {
            predictions.push_back(predictTree(tree, point));
        }

        int ones = count(predictions.begin(), predictions.end(), 1);
        return (ones > predictions.size() / 2) ? 1 : 0;
    }

} // namespace std
