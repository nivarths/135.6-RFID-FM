#include "rfid_utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

rfidReader::rfidReader()
{
    pnd = nullptr; // pointer to nfc device
    context = nullptr;
    nfc_init(&context);
}

rfidReader::~rfidReader()
{
    if(pnd)
    {
        nfc_close(pnd);
    }

    if(context)
    {
        nfc_exit(context);
    }
}

bool rfidReader::initialize()
{
    if(context == nullptr)
    {
        nfc_init(&context);
    }
    
    pnd = nfc_open(context, NULL);

    if(!pnd)
    {
        cerr << "Error opening the NFC device. " << endl;
        return false;
    }

    if(nfc_initiator_init(pnd) < 0)
    {
        cerr << "Error initializing NFC device as initiator." << endl;
        return false;
    }

    cout << "NFC reader initialized: " << nfc_device_get_name(pnd) << endl;
    return true;
}

string rfidReader::readTag()
{
    nfc_target nt;
    const nfc_modulation nmMifare = 
    {
        .nmt = NMT_ISO14443A,
        .nbr = NBR_106,
    };

    int res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt);

    if(res > 0)
    {
        ostringstream uidStream;
        for(size_t i = 0; i < nt.nti.nai.szUidLen; ++i)
        {
            uidStream << hex << setw(2) << setfill('0') << static_cast<int>(nt.nti.nai.abtUid[i]);
        }
        return uidStream.str();
    }
    else if(res == 0)
    {
        return "";
    }
    else
    {
        return "";
    }
    
}