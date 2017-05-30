//
// Created by Feliks Scholze
//

#ifndef PROGRAMM_DTWMATRIX_H
#define PROGRAMM_DTWMATRIX_H


#include <stdio.h>
#include <array>
#include "DEFINE.h"

/*
 * input: vector with csiPackets as entries
 * DTW job: calculate for every 2 vector entries the DTW and save it in an vector
 * output: vector with DTW values
 */
class DTW {
private:
    std::vector<float> dtwVector;
    std::vector<CsiPacket> vectorWithCsiValues;

    inline float getDistance(float value1, float value2);

    float calcDtwFromTwoArrays(CsiDataArray &array1, CsiDataArray &array2);

    void sort();

public:
    DTW(std::vector<CsiPacket> vector) : vectorWithCsiValues(vector) {};

    void calculateCsiVectorToDtwVector();

    std::vector<float> getDtwVectorsXPercents(float percents);

    std::string printVectorWithDtwValuesXPercents(float percents);

    float getSum();

    unsigned int getEntries();

    std::vector<int> getFalsePacketsWithLimit(float limit);
};


#endif //PROGRAMM_DTWMATRIX_H
