#ifndef RFID_UTILS_H
#define RFID_UTILS_H

#include <nfc/nfc.h>
#include <string>

class rfidReader
{
private:
    nfc_device* pnd; // pointer to NFC device
    nfc_context* context;
public:
    rfidReader();
    ~rfidReader();
    bool initialize();
    std::string readTag();
};

#endif