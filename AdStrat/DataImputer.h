#ifndef DATAIMPUTER_H
#define DATAIMPUTER_H

#include <vector>
#include <string>
#include "global.h"

class DataImputer {
public:
    // Function to impute missing data for all attributes
    void impute(std::vector<DataPoint>& dataset);

private:
    // Helper functions to impute specific types of attributes
    void imputeNumerical(std::vector<DataPoint>& dataset, const std::string& attribute);
    void imputeCategorical(std::vector<DataPoint>& dataset, const std::string& attribute);
};

#endif  // DATAIMPUTER_H
