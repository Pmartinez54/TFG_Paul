#!/bin/bash
##Konprobatzen du ea Senstsoreen irakurketa errutina exekutatzen ari den.
if  [ $(pidof Sentsoreen_irakurketa | wc -l) -eq 0 ]  
then
##Exekutatzen du senstore irakurketaren errutina.
/etc/Sentsoreen_irakurketa/Sentsoreen_irakurketa
fi

 