//
// Created by Feliks Scholze
//

#include "K_Means.h"


inline void sortVector(std::vector<float> &vec) {
    std::sort(vec.begin(), vec.end());
}

K_Means::K_Means(float centrum1, float centrum2, unsigned int maxNumberOfIterations, const std::vector<float> &vector)
        : maxNumberOfIterations(maxNumberOfIterations), vector(vector) {
    if (centrum1 < centrum2) {
        centrum1 = centrum1;
        centrum2 = centrum2;
    } else {
        centrum1 = centrum2;
        centrum2 = centrum1;
    }
    sortVector(this->vector);

    run();
/*
        std::cout<<"vectorLeft:\n";
        for (auto &item : vectorLeft) {
            std::cout<<item<<", ";
        }
        std::cout<<"vectorRight:\n";
        for (auto &item : vectorRight) {
            std::cout<<item<<", ";
        }
*/
}

void K_Means::run() {
    int counter = 0;
    float oldCentrum1 = -1;
    float oldCentrum2 = -1;

    while (counter < maxNumberOfIterations) {
        if (centrum1 == oldCentrum1 || centrum2 == oldCentrum2) {
            return;
        } else {
            oldCentrum1 = centrum1;
            oldCentrum2 = centrum2;
            vectorLeft.clear();
            vectorRight.clear();
        }
        float threshold = (centrum1 + centrum2) / 2.0;
        unsigned int i = 0;
        while (i < vector.size()) {
            if (vector.at(i) > threshold) {
                for (int j = i; j < vector.size(); j++) {
                    float element = vector.at(j);
                    vectorRight.push_back(element);
                }
                break;
            } else {
                float element = vector.at(i);
                vectorLeft.push_back(element);
                i++;
            }
        }
        centrum1 = average(vectorLeft);
        centrum2 = average(vectorRight);
    }
}

float K_Means::average(const std::vector<float> vec) {
    float sum = 0;
    for (auto &item : vec) {
        sum += item;
    }
    if (vec.size() > 0) {
        return sum / vec.size();
    }
    return 0;
}

float K_Means::getThreshold() {
    if (vectorRight.size() > 0) {
        return vectorRight.at(0);
    }
    return 0;
}









