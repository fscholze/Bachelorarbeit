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

    std::cout << dtwPuffer;
}

void printVector(std::vector<float> vec) {
    std::string dtwPuffer{};

    for (int i = 0; i < vec.size(); i++) {
        int vecElement = vec[i];
        dtwPuffer.append(std::to_string(vecElement));
        dtwPuffer.append(";");
    }
    dtwPuffer.pop_back();

    std::cout << dtwPuffer;
}

int fillValidationFileWithRandomFakePackets(std::vector<CsiPacket> &resultFalseStation,
                                            std::vector<CsiPacket> &resultValidation) {
    int i = 1;
    int fakePaketsInARow = 4;
    int counterWithFalse = 0;
    while (i < resultValidation.size() - fakePaketsInARow - 5) {
        if (rand() % 3 == 2) { // rand 0, 1 or 2 => ca 15% fake
            int random = rand() % (resultFalseStation.size() - fakePaketsInARow);
            resultValidation.at(i) = resultFalseStation.at(random);
            resultValidation.at(i + 1) = resultFalseStation.at(random + 1);
            if (rand() % 2 == 1) {
                resultValidation.at(i + 2) = resultFalseStation.at(random + 2);
                if (rand() % 2 == 1) {
                    resultValidation.at(i + 3) = resultFalseStation.at(random + 3);
                }
            }
            counterWithFalse++;
            i += 4;
        }
        i++;
    }
    return counterWithFalse;
}

void Coordinator::run() {
    Converter file{pathFile, 1};
    file.loadFromConvertedCSVFile(); // converts csv File
    auto resultTraining = file.getPacketsAsVector();
    std::vector<CsiPacket> resultValidation;

    {
        int startValue = int(resultTraining.size() * 0.7);
        while (resultTraining.size() > startValue) {
            resultValidation.insert(resultValidation.begin(), (CsiPacket &&) resultTraining.back());
            resultTraining.pop_back();
        }
    }

    /*
     * Load Validation Set and the file from a wrong station.
     */
    Converter fileFalse{pathWrongPositionFile, 1};
    fileFalse.loadFromConvertedCSVFile(); // converts csv File
    auto resultFalseStation = fileFalse.getPacketsAsVector();

    /*
     * Fill Validation File with random Fake Packets
     */
    int counterWithFalse = fillValidationFileWithRandomFakePackets(resultFalseStation, resultValidation);


    DTW dtwTraining{resultTraining};
    dtwTraining.calculateCsiVectorToDtwVector();

    float threshold = 0.0;

    auto vectorTraining = dtwTraining.getDtwVector();
    if (vectorTraining.size() > 0) {
        std::cout << "DTW_Values: \nH = np.array('";
        printVector(vectorTraining);
        printf("'.split(';'), dtype=np.float)\n");
        float minElement = *(std::min_element(vectorTraining.begin(), vectorTraining.end()));
        float maxElement = *(std::max_element(vectorTraining.begin(), vectorTraining.end()));
        K_Means kmeans = K_Means{minElement, maxElement, MAX_NUMBER_OF_KMEANS_ITERATIONS, vectorTraining};
        threshold = kmeans.getThreshold();
        printf("+++ threshold (min: %f, max: %f): %f", minElement, maxElement, kmeans.getThreshold());

        auto falsePackets = dtwTraining.getFalsePacketsWithLimit(kmeans.getThreshold());
        printf("\nTrainings Set: von %i sind %d fehlerhaft", vectorTraining.size(), falsePackets.size());


        //c.savePacketStackToCsv("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/finishd.csv");

    }
    DTW dtwValidation{resultValidation};
    dtwValidation.calculateCsiVectorToDtwVector();
    auto falsePackets = dtwValidation.getFalsePacketsWithLimit(threshold);
    auto vectorValidation = dtwValidation.getDtwVector();
    printf("\nValidation Set: von %i sind %d fehlerhaft und %d wurden erkannt: ", vectorValidation.size(),
           counterWithFalse,
           falsePackets.size());
    printVector(falsePackets);
}
