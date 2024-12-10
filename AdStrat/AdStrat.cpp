#include <iostream>
#include "ImportedData.h"
#include "DataImputer.h"
#include "RandomForest.h"

using namespace std;

int main() {
    // Initialize the loader with the correct file path
    ImportedData loader("../ad_click_dataset.csv");
    DataImputer imputer;

    // Load data and display if successful
    if (loader.loadData()) {
        std::vector<DataPoint>& dataPoints = loader.getDataPoints();
        imputer.impute(dataPoints);
        cout << "Running";
        //loader.displayData();

        vector<string> attributes = {"gender", "deviceType", "adPosition", "browsingHistory", "timeOfDay"};

        RandomForest rf(10);
        rf.train(dataPoints, attributes);

        std::vector<DataPoint> predictPoints = {
            {0, "Non-Binary", "Mobile", "Bottom", "Shopping", "Morning", -1},
            {47, "Non-Binary", "Mobile", "Side", "Education", "Afternoon", -1},
            {21, "Female", "Desktop", "Top", "News", "Night", -1},
            {45, "Male", "Tablet", "Side", "Shopping", "Evening", -1},
            {18, "Non-Binary", "Desktop", "Bottom", "News", "Morning", -1},
            {64, "Female", "Mobile", "Top", "Entertainment", "Night", -1},
            {47, "Female", "Tablet", "Bottom", "Entertainment", "Evening", -1}
        };

        // Loop through the data points and predict the 'click' value
        for (const auto& point : predictPoints) {
            int prediction = rf.predict(point);
            std::cout << "Prediction: " << prediction << std::endl;
        }

    }
    else {
        cerr << "Data loading failed!" << endl;
    }

    return 0;
}
