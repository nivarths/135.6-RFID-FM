import os
import spotipy
import webbrowser
from dotenv import load_dotenv
from spotipy import Spotify
from spotipy.oauth2 import SpotifyOAuth
from urllib.parse import urlparse, parse_qs

load_dotenv()

client_id = os.getenv("spotipy_client_id")
client_secret = os.getenv("spotipy_client_secret")
## redirect_URI = 'http://127.0.0.1:8888/callback'
redirect_URI = os.getenv("spotipy_redirect_uri")
scope = "user-read-playback-state user-modify-playback-state user-read-currently-playing"

def create_spotify_client():
    if not client_id or not client_secret:
        raise EnvironmentError("Missing Spotify client credentials. Check the .env file.")
    
    sp_oauth = SpotifyOAuth(
        client_id = client_id,
        client_secret = client_secret,
        redirect_uri = redirect_URI,
        scope = scope,
        cache_path = ".cache",
        open_browser = False
    )

    token_info = sp_oauth.get_access_token(as_dict=True)

    if not token_info:
        auth_url = sp_oauth.get_authorize_url()
        print("\n===================================")
        print("Please open this URL in your browser:")
        print(auth_url)
        print("After approving, copy and paste the FULL redirect URL here.")
        print("===================================\n")

        # Manually get redirect URL from user
        response_url = input("Paste the full redirect URL here: ").strip()

        # Extract the authorization code from the response
        code = sp_oauth.parse_response_code(response_url)
        token_info = sp_oauth.get_access_token(code=code, as_dict=True)

    access_token = token_info["access_token"]
    sp = Spotify(auth=access_token)
    return sp


