// DataImputer.cpp
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <numeric>

struct DataPoint {
    int age;
    std::string gender;
    std::string deviceType;
    std::string adPosition;
    std::string browsingHistory;
    std::string timeOfDay;
    int click; // 1 for click, 0 for no click
};

class DataImputer {
public:
    void impute(std::vector<DataPoint>& dataset) {
        imputeNumerical(dataset, "age");
        imputeCategorical(dataset, "gender");
        imputeCategorical(dataset, "deviceType");
        imputeCategorical(dataset, "adPosition");
        imputeCategorical(dataset, "browsingHistory");
        imputeCategorical(dataset, "timeOfDay");
    }

private:
    void imputeNumerical(std::vector<DataPoint>& dataset, const std::string& attribute) {
        // Calculate mean for the numerical attribute
        double sum = 0;
        int count = 0;
        for (const auto& point : dataset) {
            if (attribute == "age" && point.age != -1) {
                sum += point.age;
                count++;
            }
        }
        double mean = count > 0 ? sum / count : 0;

        // Replace missing values with the mean
        for (auto& point : dataset) {
            if (attribute == "age" && point.age == -1) {
                point.age = static_cast<int>(mean);
            }
        }
    }

    void imputeCategorical(std::vector<DataPoint>& dataset, const std::string& attribute) {
        // Calculate mode for the categorical attribute
        std::unordered_map<std::string, int> frequency;
        for (const auto& point : dataset) {
            std::string value;
            if (attribute == "gender") value = point.gender;
            else if (attribute == "deviceType") value = point.deviceType;
            else if (attribute == "adPosition") value = point.adPosition;
            else if (attribute == "browsingHistory") value = point.browsingHistory;
            else if (attribute == "timeOfDay") value = point.timeOfDay;

            if (!value.empty()) {
                frequency[value]++;
            }
        }

        // Find the most frequent value (mode)
        std::string mode = "";
        int maxCount = 0;
        for (const auto& pair : frequency) {
            const std::string& key = pair.first;
            int count = pair.second;

            if (count > maxCount) {
                maxCount = count;
                mode = key;
            }
        }

        // Replace missing values with the mode
        for (auto& point : dataset) {
            if (attribute == "gender" && point.gender.empty()) point.gender = mode;
            else if (attribute == "deviceType" && point.deviceType.empty()) point.deviceType = mode;
            else if (attribute == "adPosition" && point.adPosition.empty()) point.adPosition = mode;
            else if (attribute == "browsingHistory" && point.browsingHistory.empty()) point.browsingHistory = mode;
            else if (attribute == "timeOfDay" && point.timeOfDay.empty()) point.timeOfDay = mode;
        }
    }
};

// Example usage
int main() {
    std::vector<DataPoint> dataset = {
        {25, "Male", "Desktop", "Top", "Shopping", "Morning", 1},
        {-1, "Female", "Mobile", "", "News", "Evening", 0},
        {30, "", "Tablet", "Side", "Entertainment", "", 1},
        {35, "Male", "", "Bottom", "", "Night", 0}
    };

    DataImputer imputer;
    imputer.impute(dataset);

    for (const auto& point : dataset) {
        std::cout << "Age: " << point.age
            << ", Gender: " << point.gender
            << ", Device Type: " << point.deviceType
            << ", Ad Position: " << point.adPosition
            << ", Browsing History: " << point.browsingHistory
            << ", Time of Day: " << point.timeOfDay
            << ", Click: " << point.click
            << "\n";
    }

    return 0;
}
