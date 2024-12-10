// globals.h
#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>

using namespace std;

// Global struct definition
struct DataPoint {
    int age;
    string gender;
    string deviceType;
    string adPosition;
    string browsingHistory;
    string timeOfDay;
    int click; // 1 for click, 0 for no click
};

// Declare a global variable of type GlobalData
extern DataPoint dataPoint;

#endif // GLOBALS_H
