// ImportedData.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct DataPoint {
    int age;
    string gender;
    string deviceType;
    string adPosition;
    string browsingHistory;
    string timeOfDay;
    int click; // 1 for click, 0 for no click
};

// Function definition
//void importCSVdata(const char* filename) {

class ImportedData {
private:
    vector<DataPoint> dataPoints; // Stores all DataPoint objects
    string fileName;
    
public:
    ImportedData(const string& file) : fileName(file) {}

    // Method to load data from the file
    bool loadData() {
        ifstream inputFile(fileName);
        if (!inputFile.is_open()) {
            cerr << "Failed to open file: " << fileName << endl;
            return false;
        }
        
        string line;
        //vector<DataPoint> dataPoints;
        
        //if (!inputFile) {
        //cerr << "Could not open the file!" << endl;
        //return 1;
        //}
        
        while (getline(inputFile, line)) {  // Read each line
            stringstream ss(line);  // Create a stringstream to parse the line
            string token;
            DataPoint dp;
            
            getline(ss, token, ','); // id
            getline(ss, token, ','); // full_name
            
            // Extract fields for DataPoint
            getline(ss, token, ','); dp.age = stoi((token));
            getline(ss, dp.gender, ','); dp.gender = (dp.gender);
            getline(ss, dp.deviceType, ','); dp.deviceType = (dp.deviceType);
            getline(ss, dp.adPosition, ','); dp.adPosition = (dp.adPosition);
            getline(ss, dp.browsingHistory, ','); dp.browsingHistory = (dp.browsingHistory);
            getline(ss, dp.timeOfDay, ','); dp.timeOfDay = (dp.timeOfDay);
            getline(ss, token, ','); dp.click = stoi((token));
            
            dataPoints.push_back(dp);
        }
        
        inputFile.close();  // Close the file
        return true;
    }
    
    void displayData() const {
    // Display the loaded data for verification
        for (const auto& dp : dataPoints) {
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
    
    // Getter for data points (if needed elsewhere)
    const vector<DataPoint>& getDataPoints() const {
        return dataPoints;
    }
};

// run the code, put this in main program later
int main() {
    // Initialize the class with the file name
    ImportedData loader("ad_click_dataset.csv");

    // Load the data
    if (loader.loadData()) {
        // Display the loaded data
        loader.displayData();
    }

    return 0;
}
