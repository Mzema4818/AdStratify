#ifndef IMPORTEDDATA_H
#define IMPORTEDDATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "global.h"

using namespace std;

class ImportedData {
private:
    string fileName;
    vector<DataPoint> dataPoints;

public:
    // Constructor
    ImportedData(const string& file);

    // Loads data from the CSV file
    bool loadData();

    // Displays all loaded data
    void displayData();

    // Returns the vector of data points
    vector<DataPoint>& getDataPoints();
};

#endif // IMPORTEDDATA_H
