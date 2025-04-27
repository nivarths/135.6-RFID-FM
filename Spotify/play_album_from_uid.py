import sys
import json
from play_album import play_album

def load_mappings():
    with open("/home/pi/Projects/Digital Record Player/Mappings/mapping.json", "r") as f:
        return json.load(f)

def main():
    if len(sys.argv) < 2:
        print("No UID provided. ")
        return
    
    uid = sys.argv[1].lower()
    uid_to_album = load_mappings()

    album_uri = uid_to_album.get(uid)

    if not album_uri:
        print(f"No album mapped for UID: {uid}")
        return
    
    print(f"Found album for UID {uid}: {album_uri}")
    play_album(album_uri)

if __name__ == "__main__":
    main()

