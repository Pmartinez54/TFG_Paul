#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "MPL3115A2.h"




void Irakurketa_Barometrikoa(int fitx, float *emaitzak)    
{
    emaitzak[0] = MPL3115A2_ReadPressure(fitx);
    emaitzak[1] = MPL3115A2_ReadTemperature(fitx);
}

void Sentsorea_Hasieratu(int fitx)
{
    MPL3115A2_Hasieratu_Polling(fitx);
    MPL3115A2_BarometerMode(fitx);
    sleep(1);    
}

int Senstorea_Aatzitu()
{
	int fitx;
	char filename[] = "/dev/i2c-1"; // i2c-1 motatako dispositiboen busen helbideak
	fitx = open(filename, O_RDWR);
  	if (fitx < 0) {
    		printf("%s bus-a irekitzean arazoa, errzenb = %i \n",filename,fitx);
    		return -1;
  	}
  	printf("%s Bus-a zuzen ireki da, %i\n",filename,fitx);
	  // Busaren barruan bilatu sentsorearen helbidea eta hau slave bezela finkatu.
  	if (ioctl(fitx, I2C_SLAVE, MPL3115A2_ADDRESS) < 0) {
    		printf("Slave Helidea finkatzean arazoa \n");
    		return -1 ;
  	}

  	printf("Senstore barometrikoa zuzen atzitu da \n");
    if(!MPL3115A2_GetMode(fitx))
    {
        Sentsorea_Hasieratu(fitx);
    }
    
    return fitx;    

}




