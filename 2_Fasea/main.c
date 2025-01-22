#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include "includes/Barometro_irakurketa.h"
#include "includes/Termometro_irakurketa.h"
#include "includes/Komunikazioa_DB.h"



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

void Temp_Irakurri(unsigned char helb,struct balioak_barometro * emaitza);
void Baro_Irakurri(unsigned char helb,struct balioak_barometro * emaitzak);
void Irakurri_Sentsore_Guztiak(struct balioak_barometro * emaitzak);
void Emaitzak_erakutsi(struct balioak_barometro * emaitzak);
void Emaitzak_bidali(int socket,struct balioak_barometro * emaitzak);



void  main()
{
    struct balioak_barometro *emaitzak= calloc (SENTSOREKOP ,sizeof (struct balioak_barometro));
    int socket= conectar_Servidor();
    while(1)
    {
        Irakurri_Sentsore_Guztiak(emaitzak);
       // Emaitzak_erakutsi(emaitzak);
        Emaitzak_bidali(socket,emaitzak);
        sleep(1);
    }
        close(socket);

}

//Barometroaren balioak irakurtzen ditu emaitzak array-ean gordez.
void Baro_Irakurri(unsigned char helb,struct balioak_barometro * emaitzak)
{
    int fitx = Senstorea_Aatzitu(helb);
    if(fitx<0)
        exit(1);
    //printf("fitx = %i\n",fitx);

    Irakurketa_Barometrikoa(fitx,emaitzak);
    
    close(fitx);

}

//Termometroaren balioa irakurtzen du emaitzak array-ean gordez.
void Temp_Irakurri(unsigned char helb,struct balioak_barometro * emaitzak)
{
    int fitx;
    fitx= Senstorea_Aatzitu_Term(helb);

    if(fitx<0)
        exit(1);
   // printf("fitx = %i",fitx);
    emaitzak->temperatura =Irakurketa_Termometroa(fitx);
    close(fitx);

}

//Konektatutako sentsoreen arabera irakurketak egin etagordetzen ditu enaitzetan
void Irakurri_Sentsore_Guztiak(struct balioak_barometro * emaitzak)
{
    for(int i =0; i<SENTSOREKOP; i++ )
    {
        if(SentsoreenMota[i]==TEMP)
        {
           // printf("sents numb %i\n", i);
            Temp_Irakurri(Helbideak[i],emaitzak+i);
         //   printf("\n");
        }
        else if(SentsoreenMota[i]==BAR)
        {
           // printf("sents numb %i\n", i);
            //printf("helb 0x%02X\n", Helbideak[i]);
            Baro_Irakurri(Helbideak[i],emaitzak+i);
           // printf("\n");
        }
        else
        {
            printf("%i -garren setsoreren izaera definitu gabe dago\n",i);
        }
    }   
}

//irakukrritako datuak emaitzen datubasera bidaltzen ditu http mezu baten bidez.
void Emaitzak_bidali(int socket,struct balioak_barometro * emaitzak)
{
    char payload[2048]="";
 // Prepare the payload (line protocol format)
    for(int i=0;i<SENTSOREKOP;i++)
    {
        if(SentsoreenMota[i]==TEMP)
            snprintf(payload + strlen(payload), sizeof(payload), "Temperatura,location=Senstore_%i temp=%.2f\n", i, (emaitzak+i)->temperatura);
        else
            snprintf(payload + strlen(payload), sizeof(payload), "Barometrikoa,location=Senstore_%i temp=%.2f,pres=%.2f\n", i, (emaitzak+i)->temperatura, (emaitzak+i)->presioa);
    }
    mandar_lectura(socket,payload);


}


