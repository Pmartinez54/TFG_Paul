#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../includes/MPL3115A2.h"
#include "../includes/Barometro_irakurketa.h"


void Irakurketa_Barometrikoa(int fitx, struct balioak_barometro * emaitzak)    
{   
    emaitzak->presioa = MPL3115A2_ReadPressure(fitx);
    emaitzak->temperatura= MPL3115A2_ReadTemperature(fitx);
}

//Irkurketak egiten ahal izateko beharrezko idazketak egite ditu kontrol sentsorearen kontrol erregistroetan
void Sentsorea_Hasieratu(int fitx)
{
    MPL3115A2_Hasieratu_Polling(fitx);
    MPL3115A2_BarometerMode(fitx);
    sleep(1);    
}

//Sentsorearen helbidea finkatzen du eta hau hasieratua ez egotekotan hasieratzen du.
int Senstorea_Aatzituu(int fitx,unsigned char helb)
{
    // i2c-1 motatako dispositiboen busaren helbidea
	char filename[] = "/dev/i2c-1"; 
    if(fitx==0)	
        fitx = open(filename, O_RDWR);
  	if (fitx < 0) {
    		printf("%s bus-a irekitzean arazoa, errzenb = %i \n",filename,fitx);
    		return -1;
  	}

  	//printf("%s Bus-a zuzen ireki da, %i\n",filename,fitx);
	//Busaren barruan bilatu sentsorearen helbidea eta hau slave bezela finkatu.
  	if (ioctl(fitx, I2C_SLAVE, helb) < 0) {
    		printf("Slave Helidea finkatzean arazoa \n");
    		return -1 ;
  	}

    //printf("Senstore barometrikoa zuzen atzitu da \n");
    //Konprobatzen du ea sentsorea irakurketa moduan dagoen edo geditua dagoen.
    if(!MPL3115A2_GetMode(fitx))
        Sentsorea_Hasieratu(fitx);
    
    return fitx;    

}

int Senstorea_Aatzitu(unsigned char helb)
{    
    int fitx;
	char filename[] = "/dev/i2c-1"; // i2c-1 motatako dispositiboen busen helbideak
    fitx = open(filename, O_RDWR);
  	if (fitx < 0) {
  		printf("%s bus-a irekitzean arazoa, errzenb = %i \n",filename,fitx);
    		return -1;
  	}
    
  	//printf("%s Bus-a zuzen ireki da, %i\n",filename,fitx);
	  // Busaren barruan bilatu sentsorearen helbidea eta hau slave bezela finkatu.
  	if (ioctl(fitx, I2C_SLAVE, helb) < 0) {
    	printf("Slave Helidea finkatzean arazoa \n");
    		return -1 ;
  	}

  	//printf("Senstore barometrikoa zuzen atzitu da \n");
    //Konprobatzen du ea sentsorea irakurketa moduan dagoen edo geditua dagoen.
    if(!MPL3115A2_GetMode(fitx))
    {
        Sentsorea_Hasieratu(fitx);
    }
    
    return fitx;    

}

/*int Konprobatu_flaga(int fitx)
{
    return MPL3115A2_konprobatu_flaga(fitx);
}*/



