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
        float vecElement = vec[i];
        dtwPuffer.append(std::to_string(vecElement));
        dtwPuffer.append(";");
    }
    dtwPuffer.pop_back();

    std::cout << dtwPuffer;
}

// Add false packets from enemy in our validation set
std::vector<int> fillValidationFileWithRandomFakePackets(const std::vector<CsiPacket> &resultFalseStation,
                                            std::vector<CsiPacket> &resultValidation) {
    int i = 5;
    int fakePaketsInARow = 4;
    std::vector<int> vectorWithPosOfFalse;

    while (i < resultValidation.size() - fakePaketsInARow) {
        if (rand() % 3 == 2) { // rand 0, 1 or 2 => ca 15% fake
            int random = rand() % (resultFalseStation.size() - fakePaketsInARow);
            int frameNumber = resultValidation.at(i).frame_no;
            resultValidation.at(i).csi_values = resultFalseStation.at(random).csi_values;
            vectorWithPosOfFalse.push_back(frameNumber);
            resultValidation.at(i + 1).csi_values = resultFalseStation.at(random + 1).csi_values;
            vectorWithPosOfFalse.push_back(frameNumber + 1);
            resultValidation.at(i + 2).csi_values = resultFalseStation.at(random + 2).csi_values;
            vectorWithPosOfFalse.push_back(frameNumber + 2);
            resultValidation.at(i + 3).csi_values = resultFalseStation.at(random + 3).csi_values;
            vectorWithPosOfFalse.push_back(frameNumber + 3);
            /*  if (rand() % 2 == 1) {
                  resultValidation.at(i + 3) = resultFalseStation.at(random +3);
                  if (rand() % 2 == 1) {
                      //resultValidation.at(i + 4) = resultFalseStation.at(random + 4);
                  }
              }*/
            i += 24;
        }
        i++;
    }
    return vectorWithPosOfFalse;
}

std::vector<CsiPacket> devideDataIntoTraningAndValidation(std::vector<CsiPacket> &trainingsData, float percents) {
    /*
     * percents: 0-1.0 => e.g.: percents = 0.6 => training: 60%, validation: 40%
     */
    int startValue = int(trainingsData.size() * percents);
    std::vector<CsiPacket> resultValidation;
    while (trainingsData.size() > startValue) {
        resultValidation.insert(resultValidation.begin(), (CsiPacket &&) trainingsData.back());
        trainingsData.pop_back();
    }
    return resultValidation;
}

void printDTW(std::string fileName, std::vector<CsiPacket> resultDataSet) {

    DTW dtwDataSet{resultDataSet};
    dtwDataSet.calculateCsiVectorToDtwVector();
    std::cout << fileName << " = np.array('";
    printVector(dtwDataSet.getDtwVector());
    printf("'.split(';'), dtype=np.float)\n");

}

float getThresholdOfTrainingSet(const std::vector<CsiPacket> &resultTraining) {
    DTW dtwTraining{resultTraining};
    dtwTraining.calculateCsiVectorToDtwVector();

    auto vectorTraining = dtwTraining.getDtwVector();
    if (vectorTraining.size() > 0) {
        float minElement = *(std::min_element(vectorTraining.begin(), vectorTraining.end()));
        float maxElement = *(std::max_element(vectorTraining.begin(), vectorTraining.end()));
        K_Means kmeans = K_Means{minElement, maxElement, MAX_NUMBER_OF_KMEANS_ITERATIONS, vectorTraining};
        return kmeans.getThreshold();
        //printf("+++ threshold (min: %f, max: %f): %f", minElement, maxElement, kmeans.getThreshold());

        auto falsePackets = dtwTraining.getFalsePacketsWithLimit(kmeans.getThreshold());
        printf("\nTrainings Set: von %i sind %d fehlerhaft", vectorTraining.size(), falsePackets.size());

        //c.savePacketStackToCsv("/Users/feliksscholze/Google Drive/Bachelorarbeit/Programm/Files/finishd.csv");
    }
    return 0;
}

void printFPErrorRate(const std::vector<int> &maybe, const std::vector<int> &definitly, int TN, int total) {
    int TP = 0;
    int FN = 0;
    int FP = 0;
    int i = 0;
    int j = 0;
    std::vector<int> _TP;
    std::vector<int> _FN;
    std::vector<int> _FP;
    while (i < maybe.size()) {
        if (j >= definitly.size()) {
            //FP+=(maybe.size()-i);
            while (i < maybe.size()) {
                FP++;
                _FP.push_back(maybe.at(i));
                i++;
            }


            break;
        }

        if (maybe.at(i) == definitly.at(j)) {
            TP++;
            _TP.push_back(maybe.at(i));
            i++;
            j++;
        } else {
            if (maybe.at(i) > definitly.at(j)) {
                FN++;
                _FN.push_back(definitly.at(j));
                j++;
            } else {
                if (maybe.at(i) < definitly.at(j)) {
                    FP++;
                    _FP.push_back(maybe.at(i));
                    i++;
                }
            }
        }
    }
    while (j < definitly.size()) {
        FP++;
        _FP.push_back(definitly.at(j));
        j++;
    }
    printf("\nTP: %d: ", TP);
    //printVector(_TP);
    printf("\nFP: %d: ", FP);
    //printVector(_FP);
    printf("\nFN: %d: ", FN);
    //printVector(_FN);
    printf("\nTN: %d: ", TN - FP);

    printf("\nRichtig Rate von %f Prozent!!! (total: %f)", double(TP + TN) / total), total;
    printf("\nPrecision %f Prozent!!!", double(TP) / (TP + FP));
    printf("\nRecall von %f Prozent!!!", double(TP) / (TP + FN));

}

void Coordinator::run() {
    /*
     * Convert file to result objects
     */
    Converter file{pathFile, 1};
    file.loadFromConvertedCSVFile(); // converts csv File
    Converter fileFalse{pathWrongPositionFile, 1};
    fileFalse.loadFromConvertedCSVFile(); // converts csv File
    auto resultTraining = file.getPacketsAsVector();
    auto resultValidation = devideDataIntoTraningAndValidation(resultTraining, 0.40);
    auto resultFalseStation = fileFalse.getPacketsAsVector();

    // get Filename
    std::string fileName = pathFile.substr(pathFile.size() - 6);
    fileName.erase(fileName.length() - 4);
    // printDTW("H"+fileName, resultValidation);


    /* for (auto item: resultValidation) {
         std::cout<<"\n["<<item.frame_no<<"]: ";
         for (int i = 0; i < 30; ++i) {
             std::cout<<item.csi_values.at(i)<<", ";
         }
     }*/
    // add to Validation File false Packets
    auto vectorWithPosOfFalsePackets = fillValidationFileWithRandomFakePackets(resultFalseStation, resultValidation);


    float threshold = getThresholdOfTrainingSet(resultTraining);
    printf("threshold: %f", threshold);

    DTW dtwValidation{resultValidation};
    dtwValidation.calculateCsiVectorToDtwVector();

    auto falsePackets = dtwValidation.getFalsePacketsWithLimit(threshold);
    printf("\nValidation Set: von %i sind %d fehlerhaft und %d wurden erkannt: ", dtwValidation.getDtwVector().size(),
           vectorWithPosOfFalsePackets.size(),
           falsePackets.size());

    printFPErrorRate(falsePackets, vectorWithPosOfFalsePackets,
                     dtwValidation.getDtwVector().size() - vectorWithPosOfFalsePackets.size(),
                     dtwValidation.getDtwVector().size());

}


