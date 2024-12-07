#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <map>

// Define the structure for data points
struct DataPoint {
    int age;
    std::string gender;
    std::string deviceType;
    std::string adPosition;
    std::string browsingHistory;
    std::string timeOfDay;
    int click; // 1 for click, 0 for no click
};

// Define the structure for decision tree nodes
struct TreeNode {
    std::string splitAttribute;
    std::string splitValue;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    int prediction = -1; // -1 for non-leaf nodes, 0 or 1 for leaf nodes
};

// Function to calculate Gini Impurity for a split
double calculateGini(const std::vector<DataPoint>& data) {
    if (data.empty()) return 0.0;

    int total = data.size();
    int countClick = std::count_if(data.begin(), data.end(), [](const DataPoint& dp) { return dp.click == 1; });
    double pClick = static_cast<double>(countClick) / total;
    double pNoClick = 1.0 - pClick;

    return 1.0 - (pClick * pClick + pNoClick * pNoClick);
}

// Split data based on an attribute and value
std::pair<std::vector<DataPoint>, std::vector<DataPoint>> splitData(
    const std::vector<DataPoint>& data,
    const std::string& attribute,
    const std::string& value) {

    std::vector<DataPoint> left, right;
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
TreeNode* buildDecisionTree(std::vector<DataPoint>& data, const std::vector<std::string>& attributes) {
    if (data.empty()) return nullptr;

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
        int countClick = std::count_if(data.begin(), data.end(), [](const DataPoint& dp) { return dp.click == 1; });
        TreeNode* leaf = new TreeNode();
        leaf->prediction = (countClick >= data.size() / 2) ? 1 : 0;
        return leaf;
    }

    double bestGini = 1.0;
    std::string bestAttribute, bestValue;
    std::vector<DataPoint> bestLeft, bestRight;

    for (const auto& attr : attributes) {
        std::vector<std::string> uniqueValues;
        for (const auto& point : data) {
            if (attr == "gender") uniqueValues.push_back(point.gender);
            else if (attr == "deviceType") uniqueValues.push_back(point.deviceType);
            else if (attr == "adPosition") uniqueValues.push_back(point.adPosition);
            else if (attr == "browsingHistory") uniqueValues.push_back(point.browsingHistory);
            else if (attr == "timeOfDay") uniqueValues.push_back(point.timeOfDay);
        }
        std::sort(uniqueValues.begin(), uniqueValues.end());
        uniqueValues.erase(std::unique(uniqueValues.begin(), uniqueValues.end()), uniqueValues.end());

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
        leaf->prediction = std::count_if(data.begin(), data.end(), [](const DataPoint& dp) { return dp.click == 1; }) >= data.size() / 2 ? 1 : 0;
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
class RandomForest {
    int numTrees;
    std::vector<TreeNode*> trees;

public:
    RandomForest(int n) : numTrees(n) {}

    void train(const std::vector<DataPoint>& data, const std::vector<std::string>& attributes) {
        std::mt19937 rng(std::random_device{}());
        for (int i = 0; i < numTrees; ++i) {
            std::vector<DataPoint> sample;
            for (size_t j = 0; j < data.size(); ++j) {
                sample.push_back(data[rng() % data.size()]);
            }

            std::vector<std::string> selectedAttributes = attributes;
            std::shuffle(selectedAttributes.begin(), selectedAttributes.end(), rng);
            selectedAttributes.resize(3); // Choose a subset of attributes

            trees.push_back(buildDecisionTree(sample, selectedAttributes));
        }
    }

    int predict(const DataPoint& point) {
        std::vector<int> predictions;
        for (const auto& tree : trees) {
            predictions.push_back(predictTree(tree, point));
        }

        int ones = std::count(predictions.begin(), predictions.end(), 1);
        return (ones > predictions.size() / 2) ? 1 : 0;
    }
};
