#!/bin/bash
sudo gcc -o main_Fase1 main.c source/Termometro_irakurketa.c source/Barometro_irakurketa.c source/MPL3115A2.c -li2c -lm
sudo chmod u+s main_Fase1 
