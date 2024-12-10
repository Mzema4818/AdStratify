#include "ImportedData.h"
#include "global.h"

using namespace std;

// Constructor
ImportedData::ImportedData(const string& file) : fileName(file) {}

// Loads data from the CSV file
bool ImportedData::loadData() {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cerr << "Failed to open file: " << fileName << endl;
        return false;
    }

    string line;
    bool firstLine = true;  // To skip the header if present
    while (getline(inputFile, line)) {
        if (firstLine) {
            firstLine = false;  // Skip the first line if it contains headers
            continue;
        }

        stringstream ss(line);
        string token;
        DataPoint dp;

        // Skip irrelevant columns (ID and Full Name)
        getline(ss, token, ',');  // Skip ID
        getline(ss, token, ',');  // Skip Full Name

        // Extract relevant fields, checking for empty columns
        getline(ss, token, ',');
        dp.age = token.empty() ? 0 : stoi(token);  // If empty, set default value

        getline(ss, dp.gender, ',');
        if (dp.gender.empty()) dp.gender = "";  // Leave empty if no data

        getline(ss, dp.deviceType, ',');
        if (dp.deviceType.empty()) dp.deviceType = "";  // Leave empty if no data

        getline(ss, dp.adPosition, ',');
        if (dp.adPosition.empty()) dp.adPosition = "";  // Leave empty if no data

        getline(ss, dp.browsingHistory, ',');
        if (dp.browsingHistory.empty()) dp.browsingHistory = "";  // Leave empty if no data

        getline(ss, dp.timeOfDay, ',');
        if (dp.timeOfDay.empty()) dp.timeOfDay = "";  // Leave empty if no data

        getline(ss, token, ',');
        dp.click = token.empty() ? 0 : stoi(token);  // If empty, set default value

        // Debugging: Print each data point as it's read
        /*cout << "Loaded DataPoint: Age=" << dp.age
            << ", Gender=" << dp.gender
            << ", DeviceType=" << dp.deviceType
            << ", AdPosition=" << dp.adPosition
            << ", BrowsingHistory=" << dp.browsingHistory
            << ", TimeOfDay=" << dp.timeOfDay
            << ", Click=" << dp.click << endl;*/

        // Add to the dataPoints vector
        dataPoints.push_back(dp);
    }

    inputFile.close();  // Close file after reading
    return true;
}

void ImportedData::displayData() {
    for (auto& dp : dataPoints) {  // Removed 'const'
        cout << "Age: " << dp.age
            << ", Gender: " << dp.gender
            << ", DeviceType: " << dp.deviceType
            << ", AdPosition: " << dp.adPosition
            << ", BrowsingHistory: " << dp.browsingHistory
            << ", TimeOfDay: " << dp.timeOfDay
            << ", Click: " << dp.click
            << endl;
    }
}

// Returns the vector of data points (removed const reference)
vector<DataPoint>& ImportedData::getDataPoints() {
    return dataPoints;
}
