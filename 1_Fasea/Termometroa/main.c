#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "./Termometro_funtzioak/Termometro_irakurketa.h"

void  main()
{
    int fitx;
    fitx= Senstorea_Aatzitu_Term();
    
    if(fitx<0)
        exit(1);
    
    float Emaitzak[2]; 
    while(1)
    {
        float emaitza = Irakurketa_Termometroa(fitx);
        printf("Tenperatura: %0.2f Celcius\n",emaitza);        
        sleep(1);
    }
}

