//
// Created by Feliks Scholze
//

#include <iostream>
#include "Coordinator.h"

void run(std::string csvInputFile, std::string csvInputFileFalse) {
    auto coordinator = Coordinator(csvInputFile, csvInputFileFalse);
    coordinator.run();
    std::cout << "\n\n";
}

int main(int argc, const char *argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    std::string csvInputFileFalse{"/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/20.csv"};


    run("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/12.csv", csvInputFileFalse);
    run("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/13.csv", csvInputFileFalse);
    run("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/14.csv", csvInputFileFalse);
    run("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/15.csv", csvInputFileFalse);
    run("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/16.csv", csvInputFileFalse);
    run("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/17.csv", csvInputFileFalse);
    run("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/csvTableRoh/18.csv", csvInputFileFalse);

}





