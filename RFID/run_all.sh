#!/bin/bash

echo "[BOOT] Killing previous sessions (if any)..."
pkill -f spotify_server.py
pkill -f rfid_reader
sleep 1

echo "[BOOT] Launching Spotify server and RFID reader..."

# Navigate to Spotify folder and run server in background
cd ../Spotify
source .venv/bin/activate
echo "[STARTING] spotify_server.py..."
python3 spotify_server.py 2>/dev/null &

# Wait 2 seconds to give the server time to start
sleep 2

# Navigate back to RFID folder and run the reader
cd ../RFID
echo "[STARTING] rfid_reader..."
./rfid_reader 2> >(grep -v 'Unable to wait for SPI data')
