#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "./MPL3115A2_LIB/MPL3115A2.h"
#include "./Barometro_funtzioak/Barometro_irakurketa.h"

void  main()
{
    int fitx;
    fitx= Senstorea_Aatzitu(fitx);
    
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

