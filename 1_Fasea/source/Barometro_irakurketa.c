#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../includes/MPL3115A2.h"
#include "../includes/Barometro_irakurketa.h"



//Emmaitzak aldagaian bueltatzen dira sentsoraren presio eta temperatura irakurketak
void Irakurketa_Barometrikoa(int fitx, struct balioak_barometro * Emaitzak)    
{   
    Emaitzak->presioa = MPL3115A2_ReadPressure(fitx);
    Emaitzak->temperatura= MPL3115A2_ReadTemperature(fitx);
}

/*
Irkurketak egiten ahal izateko beharrezko idazketak egite ditu sentsorearen kontrol erregistroetan.
Lehnenengo senstsorea martxan jartzen du eta ondoren presio irakurketa aukeratzen du altitude irakurketaren ordez.
Konfigurazioa aplikatzeko hurrengo irakurketa egin baino lehen segundu bat itxarotea beharrezkoa da.
*/
void Sentsorea_Hasieratu(int fitx)
{
    MPL3115A2_Hasieratu_Polling(fitx);
    MPL3115A2_BarometerMode(fitx);
    sleep(1);
}

//Senstorearen helbidea finkatzen du eta konprobatzen du ea hasieratua dagoen, ez badago, hasiertazen du.
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
    	perror("Slave Helidea finkatzean arazoa \n");
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

/*
Sentsorea atzitzeko kodearen bertsio ezberdin bat da, hau ez du itxi egiten sentsore bakoitza iraurri ondoren, denak irekita 
uzten ditu aldi berean baizik eraginkortasun probak egiteko bi bertsioak egin nituen, eta 2 sentsoreekin beste bertsio 
azkarragoa da, baino ondo egongo litzateke senstsore gehiago konektatzean konprobatzea ea horrela izaten jarraitzen duen.
*/
//Sentsorearen helbidea finkatzen du eta hau hasieratua ez egotekotan hasieratzen du.
int Senstorea_Aatzitu_irekita(int fitx,unsigned char helb)
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


