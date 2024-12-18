#!/bin/bash
gcc -o ../test_main_mult main_mult.c ../../source/Termometro_irakurketa.c ../../source/Barometro_irakurketa.c ../../source/MPL3115A2.c -li2c -lm

