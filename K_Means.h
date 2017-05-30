//
// Created by Feliks Scholze on 30.05.17.
//

#include "DEFINE.h"

#ifndef PROGRAMM_K_MEANS_H
#define PROGRAMM_K_MEANS_H


class K_Means {
public:
    K_Means(float centrum1, float centrum2, unsigned int maxNumberOfIterations = 10,
            const std::vector<float> &vector = {});

    float getThreshold();
private:
    float centrum1;
    float centrum2;
    unsigned int maxNumberOfIterations;
    std::vector<float> vector;
    std::vector<float> vectorLeft;
    std::vector<float> vectorRight;
    void run();

    float average(const std::vector<float> vec);
};


#endif //PROGRAMM_K_MEANS_H
