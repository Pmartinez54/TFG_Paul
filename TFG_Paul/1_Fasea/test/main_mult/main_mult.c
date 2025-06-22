#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../../includes/Barometro_irakurketa.h"
#include "../../includes/Termometro_irakurketa.h"

#define MULT_HELB 0x70 //I2c bus multiplexorearen lehnengo helbidea
#define SENTSOREKOP 2 // Sentsore kopurua konektatuk guztira
#define HELBIDE_BAR 0x60//Busera zuzenan konektatutako sentsore barometrikoaren helbidea.
#define HELBIDE_TERM 0x48 //Busera zuzenan konektatutako Termometroaren helbidea
#define BAR 1
#define TEMP 0


//Konektatutako sentsore guztien helbideak, lehennengo zuzenean konektatuak eta gero multiplexorera konektatutakoak;(multiplexoreko helbideak 0x70-tik 0x77-ra dijoazte)
static const unsigned char Helbideak[10] = {HELBIDE_TERM,HELBIDE_BAR,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77};

//Hauek instalazioaren araberazkoak izango dira definitu daitezke lehnago eta gero instalazioan errespetatu zein helbidetan jarri sentsore mota bakarra ala
//hau bete senstsorekm konektatu ondoren. Lehenengo biak definitu ditut momentuz, zuzenan konektatuta daudelako.
static const int SentsoreenMota[10] ={TEMP,BAR,-1,-1,-1,-1,-1,-1,-1,-1,};


void Temp_Irakurri(unsigned char helb);
void Baro_Irakurri(unsigned char helb);
void Irakurri_Sentsore_Guztiak();

void  main()
{
    while(1)
    {
        Irakurri_Sentsore_Guztiak();
        sleep(3);
    }
}


void Baro_Irakurri(unsigned char helb)
{
    int fitx = Senstorea_Aatzitu(helb);
    if(fitx<0)
        exit(1);
    printf("fitx = %i\n",fitx);
    float Emaitzak[2];

    Irakurketa_Barometrikoa(fitx,Emaitzak);
    printf("Presioa: %0.2f Paskal\n",Emaitzak[0]);
    printf("Tenperatura: %0.2f Celcius\n",Emaitzak[1]);
    close(fitx);
  //  return fitx;
}
void Temp_Irakurri(unsigned char helb)
{
    int fitx;
    fitx= Senstorea_Aatzitu_Term(helb);

    if(fitx<0)
        exit(1);
    printf("fitx = %i",fitx);
    float Emaitzak[2];

    float emaitza = Irakurketa_Termometroa(fitx);
    printf("Tenperatura: %0.2f Celcius\n",emaitza);
    close(fitx);
}


void Irakurri_Sentsore_Guztiak()
{
    for(int i =0; i<SENTSOREKOP; i++ )
    {
        if(SentsoreenMota[i]==TEMP)
        {
            printf("sents numb %i\n", i);
            Temp_Irakurri(Helbideak[i]);
            printf("\n");
        }
        else if(SentsoreenMota[i]==BAR)
        {
            printf("sents numb %i\n", i);
            printf("helb 0x%02X\n", Helbideak[i]);
            Baro_Irakurri(Helbideak[i]);
            printf("\n");
        }
        else
        {
            printf("%i -garren setsoreren izaera definitu gabe dago\n",i);
        }
    }   

}