//
// Created by Feliks Scholze
//

#include <iostream>
#include <memory>
#include <array>
#include "DTW.h"
#include <vector>
#include <string>

#include "DEFINE.h"
#include "Converter.h"


void printVector(std::vector<int> vec) {
    std::string dtwPuffer{};

    for (int i = 0; i < vec.size(); i++) {
        int vecElement = vec[i];
        dtwPuffer.append(std::to_string(vecElement));
        dtwPuffer.append(";");
    }
    dtwPuffer.pop_back();


    std::cout << dtwPuffer << std::endl;
}


int main(int argc, const char *argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    std::string csvInputFile_log11("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/log_11.csv");

    Converter c11_1{csvInputFile_log11, 1};


    c11_1.startConverting(60);
    auto result = c11_1.getPacketsAsVector();
    //c11_1.savePacketStackToCsv("20.csv");
    DTW dtw{result};
    dtw.calculateCsiVectorToDtwVector();
    //dtw.sort();
    //dtw.printVectorWithDtwValuesXPercents(1);

    double res = (double) dtw.getSum() / (double) dtw.getEntries();
    res *= 2;
    printf("+++%d - %f - %f+++", dtw.getEntries(), dtw.getSum(), res);

    auto bla = dtw.getFalsePacketsWithLimit(res);
    printf("\n%d", bla.size());


    //c.savePacketStackToCsv("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/finishd.csv");



    return 0;
}



