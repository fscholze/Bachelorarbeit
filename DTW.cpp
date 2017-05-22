//
// Created by Feliks Scholze
//

#include "DTW.h"
#include <iostream>
#include <memory>
#include <cmath>
#include "DEFINE.h"
#include <algorithm>

/*
void DTW::printMatrix() {
    for (int i=0; i<SUBCARRIER; i++) {
        for (int j=0; j<SUBCARRIER; j++) {
            std::cout<<matrix[i][j]<<"\t";
        }
        std::cout<<std::endl;
    }
}
*/

inline void sortVector(std::vector<float> &vec) {
    std::sort(vec.begin(), vec.end());
}

std::vector<float> DTW::getDtwVectorsXPercents(float percents) {
    std::vector<float> _dtwVector = dtwVector;
    //sortVector(_dtwVector);
    if (percents < 1) {
        int elementsCount = dtwVector.size() * percents;
        std::vector<float> _dtwVectorPart(_dtwVector.begin(), _dtwVector.begin() + elementsCount);
        return _dtwVectorPart;
    }

    return dtwVector;
    //return matrix[SUBCARRIER-1][SUBCARRIER-1];
}

float DTW::getSum() {
    float _sum = 0;
    for (auto item: dtwVector) {
        _sum += item;
    }
    return _sum;
}

unsigned int DTW::getEntries() {
    return dtwVector.size();
}


inline float DTW::getDistance(float value1, float value2) {
    // normal
    // return std::abs(value1-value2);

    // euklid Norm
    return std::sqrt((value1 - value2) * (value1 - value2));
}


float DTW::calcDtwFromTwoArrays(CsiDataArray &array1, CsiDataArray &array2) { // get DTW of 2 arrays
    std::array<CsiDataArray, SUBCARRIER> matrix;

    // Set Matrix to a high number
    {
        CsiDataArray arr;
        arr.fill(MAX_INF);
        matrix.fill(arr);
    }

    // calculate start value/ distance
    matrix.at(0).at(0) = getDistance(array1.at(0), array2.at(0));

    // build the first row and column
    for (int i = 1; i < SUBCARRIER; i++) {
        matrix.at(i).at(0) = matrix.at(i - 1).at(0) + getDistance(array1.at(i), array2.at(0));
    }

    for (int i = 1; i < SUBCARRIER; i++) {
        matrix.at(0).at(i) = matrix.at(0).at(i - 1) + getDistance(array1.at(0), array2.at(i));
    }

    // Calculate distance for all entries
    // with DTW Algorithm
    int i = 1;
    int j = 1;
    for (i = 1; i < SUBCARRIER; i++) {
        for (j = 1; j < SUBCARRIER; j++) {
            float smallestNumber = matrix.at(i).at(j - 1);
            if (matrix.at(i - 1).at(j) < smallestNumber) smallestNumber = matrix.at(i - 1).at(j);
            if (matrix.at(i - 1).at(j - 1) < smallestNumber) smallestNumber = matrix.at(i - 1).at(j - 1);
            matrix.at(i).at(j) = smallestNumber + getDistance(array1.at(i), array2.at(j));
        }
    }

    return matrix.at(i - 1).at(j - 1);
}


void DTW::calculateCsiVectorToDtwVector() {

    for (unsigned int i = 1; i < vectorWithCsiValues.size() - 2; i++) {
        float dtwResult = calcDtwFromTwoArrays(vectorWithCsiValues.at(i - 1).csi_values,
                                               vectorWithCsiValues.at(i).csi_values);
        //dtwVector.insert(dtwVector.begin(), dtwResult);
/*        float value1 = MAX_INF;
        if (dtwVector.size() > 0) {
            value1 = dtwVector.at(dtwVector.size()-1);
        }

        int line = 0;
        if (dtwResult > 2*value1) {
            dtwResult = calcDtwFromTwoArrays(vectorWithCsiValues.at(i-1).csi_values, vectorWithCsiValues.at(i+1).csi_values);
            line = 1;
            if (dtwResult > 2*value1) {
                dtwResult = calcDtwFromTwoArrays(vectorWithCsiValues.at(i-1).csi_values, vectorWithCsiValues.at(i+2).csi_values);
                line = 2;
                if (dtwResult > 2*value1) {
                    dtwResult = 999;
                    line = 3;
                    vectorWithCsiValues.at(i).csi_values = vectorWithCsiValues.at(i-1).csi_values;
                }
            }
        }
        if (dtwResult != 999) {

        }*/
        dtwVector.push_back(dtwResult);
    }

}

std::string DTW::printVectorWithDtwValuesXPercents(float percents) {

    auto _dtwVector = getDtwVectorsXPercents(percents);

    std::string dtwPuffer{};

    for (int i = 0; i < _dtwVector.size(); i++) {
        double dtw_0 = _dtwVector.at(i);
        dtwPuffer.append(std::to_string(dtw_0));
        dtwPuffer.append("\n");
    }
    dtwPuffer.pop_back();

    //std::cout<<sum/data.size()<<std::endl;

    std::cout << dtwPuffer << std::endl;
    return dtwPuffer;
    //return "";
}

std::vector<int> DTW::getFalsePacketsWithLimit(float limit) {
    std::vector<int> vectorWithFalsePacketsIDs;
    for (int i = 1; i < dtwVector.size() - 1; i++) {
        if (dtwVector.at(i) > limit) {
            auto current = dtwVector.at(i);
            auto last = dtwVector.at(i - 1);
            auto next = dtwVector.at(i + 1);
            // if (fabs((last-next)) > limit) {
            ++i;
            vectorWithFalsePacketsIDs.push_back(i + 1);
            //}
        }
    }
    return vectorWithFalsePacketsIDs;
}