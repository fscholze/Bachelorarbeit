//
// Created by Feliks Scholze
//

#include "K_Means.h"

inline void sortVector(std::vector<float> &vec) {
    std::sort(vec.begin(), vec.end());
}

K_Means::K_Means(unsigned int centrum1, unsigned int centrum2, unsigned int maxNumberOfIterations, const std::vector<float> &vector) : maxNumberOfIterations(maxNumberOfIterations), vector(vector) {
    if (centrum1 < centrum2) {
        this->centrum1 = centrum1;
        this->centrum2 = centrum2;
    } else {
        this->centrum1 = centrum2;
        this->centrum2 = centrum1;
    }
    sortVector(this->vector);

}

void run() {

}








