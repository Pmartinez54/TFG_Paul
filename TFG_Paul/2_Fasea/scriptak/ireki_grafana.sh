#!/bin/bash
if  [ $(pidof firefox  | wc -l) -eq 0 ]  
then
export DISPLAY=:0
firefox --kiosk "http://127.0.0.1:3000/playlists/play/eebxrg8f9z37kf?kiosk"  --start-fullscreen 
fi

