//
// Created by Feliks Scholze
//

#ifndef PROGRAMM_COORDINATOR_H
#define PROGRAMM_COORDINATOR_H

#include <array>
#include "DEFINE.h"

class Coordinator {
public:
    Coordinator(std::string pathFile, std::string pathWrongPositionFile)
            : pathFile(pathFile), pathWrongPositionFile(pathWrongPositionFile) {};

    void run();

private:
    std::string pathFile;
    std::string pathWrongPositionFile;

};


#endif //PROGRAMM_COORDINATOR_H
