#include "rfid_utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>


using namespace std;
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5050

int main()
{
    // SOCKET SETUP

    // new network communication endpoint to communicate with python code
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        cerr << "[ERROR] Failed to create socket" << endl;
        return 1;
    }

    // stuct to hold connection info (IP + port)
    sockaddr_in server_addr;

    // clean struct before use
    memset(&server_addr, 0, sizeof(server_addr));

    // setting sin_family field to AF_INET (IP4 address) so linux knows
    server_addr.sin_family = AF_INET;

    // setting port number to server address and convert port number from host byte order to network byte order
    server_addr.sin_port = htons(SERVER_PORT);

    // converts ip address to binary network format so linux can read it, so connection can happen
    if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        cerr << "[ERROR] Invalid Address/Address not supported." << endl;
        return 1;
    }

    // connects socket to spotify
    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "[ERROR] Connection Failed." << endl;
        return 1;
    }

    cout << "[Connected] Connected to server at " << SERVER_IP << ":" << SERVER_PORT << endl;

    // RFID READING SETUP
    rfidReader reader;
    if(!reader.initialize())
    {
        cerr << "RFID reader failed to initialize. " << endl;
        close(sock);
        return 1;
    }

    cout << "Waiting for RFID tag... " << endl;

    string lastUID = "";
    auto lastScanTime = chrono::steady_clock::now();

    // MAIN SCANNING LOOP
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

            // sending UID over socket

            // in python code .strip() will work if there is newline escape
            currentUID += "\n";

            if(send(sock, currentUID.c_str(), currentUID.length(), 0) == -1)
            {
                cerr << "[ERROR] Failed to send UID over socket." << endl;
            }
            else
            {
                cout << "[SENT] sent UID over socket to server." << endl;
            }

            // cooldown before tapping card
            this_thread::sleep_for(std::chrono::seconds(3)); 
            cout << "Waiting for RFID tag... " << endl;
        }

        // polling delay
        this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // cleanup
    close(sock);
    return 0;
}


// BELOW ARE THE UIDs AND THEIR CORRESPONDING ALBUMS
// 84d772c0 - BRAT
// 12384705 - BLONDE