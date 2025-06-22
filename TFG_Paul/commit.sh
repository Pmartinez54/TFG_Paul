#!/bin/bash
cd ~/Documents/TFG_Paul
git add --all
echo "Sartu commit-aren mezua" 
read  mezua
git commit -m $mezua   
git push
