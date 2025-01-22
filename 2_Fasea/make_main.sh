#!/bin/bash
gcc -o main main.c source/Termometro_irakurketa.c source/Barometro_irakurketa.c source/MPL3115A2.c source/Komunikazioa_DB.c -li2c -lm

