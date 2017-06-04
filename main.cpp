//
// Created by Feliks Scholze
//

#include <iostream>
#include "Coordinator.h"


int main(int argc, const char *argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    std::string csvInputFileFalse{"/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/20.csv"};

    std::string csvInputFile_1{"/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/12.csv"};
    auto coordinator = Coordinator(csvInputFile_1, csvInputFileFalse);
    coordinator.run();

    std::string csvInputFile_2{"/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/13.csv"};

    auto coordinator2 = Coordinator(csvInputFile_2, csvInputFileFalse);
    coordinator2.run();

    std::string csvInputFile_3{"/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/14.csv"};

    auto coordinator3 = Coordinator(csvInputFile_3, csvInputFileFalse);
    coordinator3.run();

}



