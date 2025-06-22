#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../..//includes/MPL3115A2.h"
#include "../../includes/Barometro_irakurketa.h"

void Irakurketa_Bar(unsigned char helb);

void  main()
{
    Irakurketa_Bar(0x60); 
}


void Irakurketa_Bar(unsigned char helb)
{
    int fitx=0;
    fitx= Senstorea_Aatzitu(helb);
    
    if(fitx<0)
        exit(1);
    
    float Emaitzak[2]; 
    while(1)
    {
        Irakurketa_Barometrikoa(fitx,Emaitzak);
        printf("Presioa: %0.2f Paskal\n",Emaitzak[0]);       
        printf("Tenperatura: %0.2f Celcius\n",Emaitzak[1]);        
        sleep(1);
    }
}

