from spotify_auth import create_spotify_client

def play_album(album_uri: str):
    sp = create_spotify_client()

    spotify_devices = sp.devices()

    if not spotify_devices['devices']:
        print("No active Spotify devices are available. Please connect a phone or pc and open Spotify.")
        return
    
    active_device = spotify_devices['devices'][0]
    active_device_id = active_device['id']

    print(f"Playing album on device: {active_device['name']}")

    sp.start_playback(device_id = active_device_id, context_uri = album_uri)

