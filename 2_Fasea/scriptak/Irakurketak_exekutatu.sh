#!/bin/bash
##Konprobatzen du ea Senstsoreen irakurketa errutina exekutatzen ari den.. /root/.profile
if  [ $(pidof Sentsoreen_irakurketa | wc -l) -eq 0 ]  
then
. /root/.profile
##Exekutatzen du senstore irakurketaren errutina.
/etc/Sentsoreen_irakurketa/Sentsoreen_irakurketa
fi

 
