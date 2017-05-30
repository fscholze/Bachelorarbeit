//
// Created by Feliks Scholze
//

#include <iostream>
#include <memory>
#include <array>
#include <vector>
#include <string>

#ifndef PROGRAMM_DEFINE_H
#define PROGRAMM_DEFINE_H

const unsigned short MAX_INF = 999;
const unsigned short SUBCARRIER = 30;
const float FALSE_LIMIT = 55;

typedef std::array<float, SUBCARRIER> CsiDataArray;

struct CsiPacket {
    unsigned int frame_no = 0;
    unsigned short tx = 0;
    unsigned short rx = 0;
    CsiDataArray csi_values;
};

#endif //PROGRAMM_DEFINE_H
