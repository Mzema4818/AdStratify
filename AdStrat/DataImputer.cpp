// DataImputer.cpp
//This is justa sample code for a basic mean imputation
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<vector<double>> data = {
        {1, 2, 3},
        {4, NAN, 6},
        {7, 8, 9}
    };

    // Calculate mean for each column
    vector<double> columnMeans(data[0].size(), 0);
    int numValidValues = 0;

    for (const auto& row : data) {
        for (int i = 0; i < row.size(); ++i) {
            if (!isnan(row[i])) {
                columnMeans[i] += row[i];
                ++numValidValues;
            }
        }
    }

    for (auto& mean : columnMeans) {
        mean /= numValidValues;
    }

    // Impute missing values with mean
    for (auto& row : data) {
        for (int i = 0; i < row.size(); ++i) {
            if (isnan(row[i])) {
                row[i] = columnMeans[i];
            }
        }
    }

    // Print the imputed data
    for (const auto& row : data) {
        for (const auto& value : row) {
            cout << value << " ";
        }
        cout << endl;
    }

    return 0;
}