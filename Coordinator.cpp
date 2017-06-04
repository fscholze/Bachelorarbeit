//
// Created by Feliks Scholze
//

#include "Coordinator.h"
#include "Converter.h"
#include "DTW.h"
#include "K_Means.h"

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

void Coordinator::run() {
    Converter file{pathFile, 1};
    file.loadFromConvertedCSVFile(); // converts csv File
    auto resultBig = file.getPacketsAsVector();
    std::vector<CsiPacket> resultSmall;

    {
        int startValue = int(resultBig.size() * 0.7);
        while (resultBig.size() > startValue) {
            resultSmall.insert(resultSmall.begin(), (CsiPacket &&) resultBig.back());
            resultBig.pop_back();
        }
    }

    /*
     * Load 30% and the file from a wrong station.
     */
    Converter fileFalse{pathWrongPositionFile, 1};
    fileFalse.loadFromConvertedCSVFile(); // converts csv File
    auto resultFalseStation = fileFalse.getPacketsAsVector();

    /*
     * Fill 30 Percent File with random Fake Packets
     */
    int i = 1;
    int fakePaketsInARow = 4;
    int counterWithFalse = 0;
    while (i < resultSmall.size() - fakePaketsInARow - 5) {
        if (rand() % 3 == 2) { // rand 0, 1 or 2 => ca 30% fake
            int random = rand() % (resultFalseStation.size() - fakePaketsInARow);
            resultSmall.at(i) = resultFalseStation.at(random);
            resultSmall.at(i + 1) = resultFalseStation.at(random + 1);
            if (rand() % 2 == 1) {
                resultSmall.at(i + 2) = resultFalseStation.at(random + 2);
                if (rand() % 2 == 1) {
                    resultSmall.at(i + 3) = resultFalseStation.at(random + 3);
                }
            }
            counterWithFalse++;
            i += 4;
        }
        i++;
    }


    DTW dtw70{resultBig};
    dtw70.calculateCsiVectorToDtwVector();

    float threshold = 0.0;

    auto vector70 = dtw70.getDtwVector();
    if (vector70.size() > 0) {
        float minElement = *(std::min_element(vector70.begin(), vector70.end()));
        float maxElement = *(std::max_element(vector70.begin(), vector70.end()));
        K_Means kmeans = K_Means{minElement, maxElement, 1000, vector70};
        threshold = kmeans.getThreshold();
        printf("\n+++ threshold (min: %f, max: %f): %f", minElement, maxElement, kmeans.getThreshold());

        auto falsePackets = dtw70.getFalsePacketsWithLimit(kmeans.getThreshold());
        printf("\n70 Prozent: von %i sind %d fehlerhaft", vector70.size(), falsePackets.size());


        //c.savePacketStackToCsv("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/finishd.csv");

    }
    DTW dtw30{resultSmall};
    dtw30.calculateCsiVectorToDtwVector();
    auto falsePackets = dtw30.getFalsePacketsWithLimit(threshold);
    auto vector30 = dtw30.getDtwVector();
    printf("\n30 Prozent: von %i sind %d fehlerhaft und %d wurden erkannt :D", vector30.size(), counterWithFalse,
           falsePackets.size());
    printVector(falsePackets);
}
