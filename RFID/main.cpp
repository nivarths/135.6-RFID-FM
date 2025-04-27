#include "rfid_utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

int main()
{
    rfidReader reader;
    if(!reader.initialize())
    {
        cerr << "RFID reader failed to initialize. " << endl;
        return 1;
    }

    cout << "Waiting for RFID tag... " << endl;

    string lastUID = "";
    auto lastScanTime = chrono::steady_clock::now();

    while(true)
    {
        string currentUID = reader.readTag();
        auto now = chrono::steady_clock::now();
        auto durationSinceLastScan = chrono::duration_cast<chrono::seconds>(now - lastScanTime);

        if(!currentUID.empty() && (currentUID != lastUID || durationSinceLastScan.count() > 5))
        {
            cout << "Tag detected. UID: " << currentUID << endl;
            lastUID = currentUID;
            lastScanTime = now;

            string command = "bash -c 'source /home/pi/Projects/Digital\\ Record\\ Player/Spotify/.venv/bin/activate && python3 /home/pi/Projects/Digital\\ Record\\ Player/Spotify/play_album_from_uid.py " + currentUID + " &'";
            system(command.c_str());

            this_thread::sleep_for(std::chrono::seconds(3)); // cooldown before tapping card
            cout << "Waiting for RFID tag... " << endl;
        }

        this_thread::sleep_for(std::chrono::milliseconds(500)); // polling delay
    }

    return 0;
}


// BELOW ARE THE UIDs AND THEIR CORRESPONDING ALBUMS
// 84d772c0 - BRAT
// 12384705 - BLONDE