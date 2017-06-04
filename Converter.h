//
// Created by Feliks Scholze
//

#include "DEFINE.h"

#ifndef PROGRAMM_CONVERTER_H
#define PROGRAMM_CONVERTER_H

/*
 * Converter clas:
 * is converting data (pcket number, tx, rx, csiData) to vector.
 * Elements from Vector are CsiPacket (Definition in DEFINE.h)
 * -> returns a vector with CsiPackets.
 */

class Converter {
public:
    Converter(std::string path, unsigned short antenna) : path(path), antenna(antenna) {};

    std::vector<CsiPacket> getPacketsAsVector();

    bool startConverting();

    bool loadFromConvertedCSVFile();

    bool savePacketStackToCsv(std::string path);

private:
    std::string path; // Path to file
    unsigned short antenna; // recievers 1st oder 2nd antenna
    std::vector<CsiPacket> packets; // vector with all packets, where each single has SUBCARRIER entries
    std::vector<CsiPacket>
    convertCsvToCsiPacketStack(); // convert csi packets from file
    std::vector<CsiPacket> loadConvertedCSVToCsiPacketStack();

    bool savePacketStackToCsv(std::vector<CsiPacket> packetStack, std::string fileName); // save packets to file
};


#endif //PROGRAMM_CONVERTER_H
