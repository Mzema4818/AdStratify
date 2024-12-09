// ImportedData.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Function definition
void displayCSVData(const char* filename) {
    ifstream file(filename);  // Open the CSV file
    string line;

    if (!file) {
        cerr << "Could not open the file!" << endl;
        return;
    }

    while (getline(file, line)) {  // Read each line
        stringstream ss(line);  // Create a stringstream to parse the line
        string value;

        while (getline(ss, value, ',')) {  // Split by comma
            cout << value << " ";  // Display the value
        }
        cout << endl;  // Newline after each row
    }

    file.close();  // Close the file
}
