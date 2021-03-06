//
// Created by Feliks Scholze
//

#include "Converter.h"
#include <sstream>
#include <fstream>

std::vector<CsiPacket> Converter::convertCsvToCsiPacketStack() {
    int frameNumber = 1;

    std::ifstream file(path);
    std::string currentLine;
    std::vector<std::string> _tempCurrentLineStack;
    CsiDataArray _tempCsiValueArray;

    std::vector<CsiPacket> pakets;

    // frameno;tx;rx;subcarrier;csi
    unsigned short counter = 0;
    while (std::getline(file, currentLine)) {
        std::string currentLineValue;
        std::stringstream sStream(currentLine);

        _tempCurrentLineStack.clear();


        while (std::getline(sStream, currentLineValue, ';')) {
            _tempCurrentLineStack.push_back(currentLineValue);
        }
        _tempCsiValueArray.at(counter) = atof(_tempCurrentLineStack.back().c_str());
        counter++;

        if (counter >= SUBCARRIER) { // csi array is full with 30 csi values
            counter = 0;
            _tempCurrentLineStack.pop_back(); // delete csi value row with csi value
            _tempCurrentLineStack.pop_back(); // delete subcarrier row
            CsiPacket currentPacket;
            currentPacket.csi_values = _tempCsiValueArray;
            currentPacket.rx = stoi(_tempCurrentLineStack.back());
            _tempCurrentLineStack.pop_back();
            currentPacket.tx = stoi(_tempCurrentLineStack.back());
            _tempCurrentLineStack.pop_back();
            currentPacket.frame_no = stoi(_tempCurrentLineStack.back());
            if (currentPacket.rx == antenna) pakets.push_back(currentPacket);
        }
    }
    if (pakets.size() > 0) {
        return pakets;
    }
    packets.clear();
    return packets;
}


std::vector<CsiPacket> Converter::loadConvertedCSVToCsiPacketStack() {

    std::ifstream file(path);
    std::string currentLine;
    std::vector<std::string> _tempCurrentLineStack;
    CsiDataArray _tempCsiValueArray;
    std::vector<CsiPacket> pakets;

    // frameno;tx;rx;subcarrier;csi
    while (std::getline(file, currentLine)) {
        std::string currentLineValue;
        std::stringstream sStream(currentLine);

        _tempCurrentLineStack.clear();


        while (std::getline(sStream, currentLineValue, ';')) {
            _tempCurrentLineStack.push_back(currentLineValue);
        }
        int counter = 29;
        while (_tempCurrentLineStack.size() > 3) {
            _tempCsiValueArray.at(counter) = atof(_tempCurrentLineStack.back().c_str());
            _tempCurrentLineStack.pop_back();
            counter--;
        }
        CsiPacket currentPacket;
        currentPacket.csi_values = _tempCsiValueArray;
        currentPacket.rx = stoi(_tempCurrentLineStack.back());
        _tempCurrentLineStack.pop_back();
        currentPacket.tx = stoi(_tempCurrentLineStack.back());
        _tempCurrentLineStack.pop_back();
        currentPacket.frame_no = stoi(_tempCurrentLineStack.back());
        pakets.push_back(currentPacket);
    }
    if (pakets.size() > 0) {
        return pakets;
    }
    packets.clear();
    return packets;
}


// Saving Packets in following scheme:
//'frame_no','tx','rx','1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28','29','30'
bool Converter::savePacketStackToCsv(std::vector<CsiPacket> packetStack, std::string fileName) {
    if (packetStack.empty()) return false;
    try {
        std::ofstream outFile;
        outFile.open(fileName);

        for (auto item : packetStack) {
            outFile << item.frame_no << ";" << item.tx << ";" << item.rx;
            for (auto csiItem  : item.csi_values) {
                outFile << ";" << csiItem;
            }
            outFile << "\n";
        }
        outFile << std::endl;
        outFile.close();
        return true;
    }
    catch (...) {
        std::cout << "Error with saving Packets to File" << std::endl;
        return false;
    }
}

std::vector<CsiPacket> Converter::getPacketsAsVector() {
    return packets;
}

bool Converter::startConverting() {
    packets = convertCsvToCsiPacketStack();
    return packets.size() > 0;
}

bool Converter::loadFromConvertedCSVFile() {
    packets = loadConvertedCSVToCsiPacketStack();
    return packets.size() > 0;
}

bool Converter::savePacketStackToCsv(std::string outputPath) {
    return savePacketStackToCsv(packets, outputPath);
}
