//
// Created by Feliks Scholze
//

#include "DTW.h"
#include <cmath>


std::vector<float> DTW::getDtwVector() {
    return dtwVector;
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
        dtwVector.push_back(dtwResult);
    }

}

std::string DTW::printVectorWithDtwValues() {

    auto _dtwVector = dtwVector;

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
    int i = 3;
    while (i < dtwVector.size() - 1) {
        if (dtwVector.at(i) > limit) {
            if (calcDtwFromTwoArrays(vectorWithCsiValues.at(i - 1).csi_values,
                                     vectorWithCsiValues.at(i + 1).csi_values) >
                limit) {
                if (calcDtwFromTwoArrays(vectorWithCsiValues.at(i - 1).csi_values,
                                         vectorWithCsiValues.at(i + 2).csi_values) >
                    limit) {
                    if (calcDtwFromTwoArrays(vectorWithCsiValues.at(i - 1).csi_values,
                                             vectorWithCsiValues.at(i + 3).csi_values) >
                        limit) {
                        int frameNo = vectorWithCsiValues.at(i).frame_no + 1;
                        vectorWithFalsePacketsIDs.push_back(frameNo);
                        vectorWithFalsePacketsIDs.push_back(frameNo + 1);
                        vectorWithFalsePacketsIDs.push_back(frameNo + 2);
                        vectorWithFalsePacketsIDs.push_back(frameNo + 3);

                        i++;
                    }

                }

            }
        }
        i++;
    }


    return vectorWithFalsePacketsIDs;
}
