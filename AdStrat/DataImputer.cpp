#include "DataImputer.h"
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include "global.h"

void DataImputer::impute(std::vector<DataPoint>& dataset) {
    // Impute missing values for numerical and categorical attributes
    imputeNumerical(dataset, "age");
    imputeCategorical(dataset, "gender");
    imputeCategorical(dataset, "deviceType");
    imputeCategorical(dataset, "adPosition");
    imputeCategorical(dataset, "browsingHistory");
    imputeCategorical(dataset, "timeOfDay");
}

void DataImputer::imputeNumerical(std::vector<DataPoint>& dataset, const std::string& attribute) {
    double sum = 0;
    int count = 0;

    // Compute sum and count for non-missing values
    for (const auto& point : dataset) {
        if (attribute == "age" && point.age != -1) { // Assuming -1 represents missing data for age
            sum += point.age;
            count++;
        }
    }

    // Calculate the mean, or set it to 0 if there are no valid entries
    double mean = count > 0 ? sum / count : 0;

    // Impute missing values with the mean
    for (auto& point : dataset) {
        if (attribute == "age" && point.age == -1) { // Impute missing age with mean
            point.age = static_cast<int>(mean);
        }
    }
}

void DataImputer::imputeCategorical(std::vector<DataPoint>& dataset, const std::string& attribute) {
    std::unordered_map<std::string, int> frequency;

    // Count frequency of each category for the attribute
    for (const auto& point : dataset) {
        std::string value;

        if (attribute == "gender") value = point.gender;
        else if (attribute == "deviceType") value = point.deviceType;
        else if (attribute == "adPosition") value = point.adPosition;
        else if (attribute == "browsingHistory") value = point.browsingHistory;
        else if (attribute == "timeOfDay") value = point.timeOfDay;

        // If the value is not missing (empty), increment its frequency
        if (!value.empty()) {
            frequency[value]++;
        }
    }

    // Find the most frequent value (mode)
    std::string mode = "";
    int maxCount = 0;
    for (const auto& pair : frequency) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            mode = pair.first;
        }
    }

    // Impute missing values with the mode (most frequent value)
    for (auto& point : dataset) {
        if (attribute == "gender" && point.gender.empty()) point.gender = mode;
        else if (attribute == "deviceType" && point.deviceType.empty()) point.deviceType = mode;
        else if (attribute == "adPosition" && point.adPosition.empty()) point.adPosition = mode;
        else if (attribute == "browsingHistory" && point.browsingHistory.empty()) point.browsingHistory = mode;
        else if (attribute == "timeOfDay" && point.timeOfDay.empty()) point.timeOfDay = mode;
    }
}
