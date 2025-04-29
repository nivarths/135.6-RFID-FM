import spotipy
import os
import json
import threading
import socket
from spotipy.oauth2 import SpotifyOAuth
from dotenv import load_dotenv

load_dotenv() 

## set up Spotify credentials
client_id = os.getenv("spotipy_client_id")
client_secret = os.getenv("spotipy_client_secret")
redirect_URI = os.getenv("spotipy_redirect_uri")
scope = "user-read-playback-state user-modify-playback-state user-read-currently-playing"

## object handles logging into Spotify using credentials
sp_oauth = SpotifyOAuth(
        client_id = client_id,
        client_secret = client_secret,
        redirect_uri = redirect_URI,
        scope = scope,
        cache_path = ".cache",
        open_browser = False
)

## make requests to Spotify servers, proves identity and goal  
token_info = sp_oauth.get_access_token(as_dict=True)

## Spotify client object, sp, used to connect with Spotify to make API commands
sp = spotipy.Spotify(auth=token_info['access_token'])

## JSON contents are converted to Python dictionary, tells the server what album to play 
with open("/home/pi/Projects/Digital Record Player/Mappings/mapping.json", "r") as file:
    uid_to_album = json.load(file)


## SERVER SETUP

## local host, ip address the server will listen on
HOST = "127.0.0.1" 

## arbitrary number, specific "area" for client/server comms
PORT = 5050 

## passing parameters address family internet (IP) and TCP
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

 ## connects server to the host IP and the port 5050
server.bind((HOST, PORT))

## enables server to accept connections (max 5). Connection in this case is the C++ RFID reader
server.listen(5)

print(f"[SERVER] Listening on {HOST}:{PORT}...")

## conn is the connection object (represents communication between client and server) addr is the IP and the port
def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} has been connected") 
    while True:
        try:
            ## 1024 bytes are recovered, decoded into UTF-8 text, creates clean uid tag
            uid = conn.recv(1024).decode().strip() 
            if not uid:
                break
            print(f"[UID RECEIVED] {uid}")
            ## converts to lowercase for consistency. If UID exists, returns album
            album_uri = uid_to_album.get(uid.lower())

            if album_uri:
                ## looks for active devices, and plays music on first device identified
                spotify_devices = sp.devices()
                if spotify_devices['devices']:
                    active_device = spotify_devices['devices'][0]
                    active_device_id = active_device['id']
                    print(f"Playing album on device: {active_device['name']}")
                    sp.start_playback(device_id = active_device_id, context_uri = album_uri)
                else:
                    print("[ERROR] No active Spotify devices found.")
            else:
                print(f"[ERROR] No album mapped for UID: {uid}")
        ## catch and print any errors like token expiration/socket closing
        except Exception as e:
            print(f"[ERROR] {e}")
            break
    ## close socket connection after exiting loop for proper memory cleanup
    conn.close()

def main():
    ## keep accepting clients until force exit (ctrl + c)
    while True:
        conn, addr = server.accept()
        ## new thread to handle client, server can accept multiple clients
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.active_count() - 1}")

main()


