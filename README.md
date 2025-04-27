# Digital Record Player

A Raspberry Pi project that uses an RFID scanner to play Spotify albums.

## Features
- RFID scanning with PN532
- Spotify API integration via Spotipy
- Local communication between C++ and Python
- Real-time album playback

## Tech Stack
- C++ (RFID Scanneing)
- Python (Spotify API)
- Raspberry Pi
- PN532 NFC RFID Scanner

## Skills
- C++ programming for RFID hardware control
- Python programming for Spotify API integration
- Raspberry Pi hardware setup and GPIO for communication (SPI with PN532 NFC RFID scanner)
- Soldering and wiring electronics (RFID scanner connections)

## Improvements/Additions
- Allow switching between multiple Spotify accounts
- Improve communication between C++ (RFID) and Python (Spotify) with real-time socket messages instead of launching scripts
- Log file with timestamps of every scan and album played
- Custom RFID case/enclosure
- Album cover sleeves for RFID tags to feel even more like "vinyls"

![IMG_2897](https://github.com/user-attachments/assets/dd38e200-1755-4341-aaeb-89feb5bbff93)
