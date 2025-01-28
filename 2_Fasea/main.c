#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include "includes/Barometro_irakurketa.h"
#include "includes/Termometro_irakurketa.h"
#include "includes/Komunikazioa_DB.h"

#define BAR 1
#define TEMP 0


void Helbideak_irakurri( unsigned char **Helbideak,int **SentsoreenMota ,int *sentsore_kop);
void Temp_Irakurri(unsigned char helb,struct balioak_barometro * emaitza);
void Baro_Irakurri(unsigned char helb,struct balioak_barometro * emaitzak);
void Irakurri_Sentsore_Guztiak(int sentskop, unsigned char *Helbideak,int *SentsoreenMota,struct balioak_barometro * emaitzak);
void Emaitzak_erakutsi(int sentskop, unsigned char *Helbideak,int *SentsoreenMota,struct balioak_barometro * emaitzak);
void Emaitzak_bidali(int socket,int sentskop, unsigned char *Helbideak,int *SentsoreenMota,struct balioak_barometro * emaitzak);


//Main hau pentsatua dago denbora guztian exekutatzen egoteko beraz simple programatu dut
//Iterazio bakoitzean sentsore bakoitzaren irakurketa ba egin eta bidaltzeko.
void  main()
{   
    unsigned char *Helbideak;
    int *SentsoreenMota;
    int sentsore_kop;
    
    //Sentsoreen helbideak irakurtzen ditu fitxategi batetik
    Helbideak_irakurri(&Helbideak,&SentsoreenMota ,&sentsore_kop);
    
    struct balioak_barometro *emaitzak= calloc (sentsore_kop ,sizeof (struct balioak_barometro));
    
    int socket= Zerbitzarira_Konektatu();
    
    while(1)
    {
        Irakurri_Sentsore_Guztiak(sentsore_kop,Helbideak,SentsoreenMota,emaitzak);
        Emaitzak_bidali(socket,sentsore_kop,Helbideak,SentsoreenMota,emaitzak);
        sleep(1);
    }
        close(socket);
        free(emaitzak);
        free(Helbideak);
        free(SentsoreenMota);
}

void Helbideak_irakurri( unsigned char **Helbideak,int **SentsoreenMota ,int *sentsore_kop){

    const char *fitx_izena = "/etc/Sentsoreen_irakurketa/Sentsore_helbideak.txt";

    FILE *file = fopen(fitx_izena, "r");
    if (file == NULL) {
        perror("Arazoa fitxategia irekitzen");
        exit(1);
    }
 
    unsigned char buffer[1024];
    
    //Bigarren filan beti egongo da konektatutako sentsoreenkopurua
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);
    *sentsore_kop =  (int) buffer[0] -48 ;

    if(*sentsore_kop < 1){
	    perror("Ez daude sentsorerik konektatuak\n");
	    exit(1);
    }

    //Konektatutako sentsore guztien helbideak.
    *Helbideak = malloc(*sentsore_kop * sizeof(unsigned char) );

    //Ea sentsoreak temperaturazkoak ala barometrikoak diren gordetzen duen array-a.
    *SentsoreenMota = malloc(*sentsore_kop * sizeof(int) );

    //irakurritako lineen kopurua 
    int kont  = 0; 
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
	    if(buffer[0] != '#')
        {
            *(*SentsoreenMota+kont) = buffer[0] - 48;	
            //printf("SentsoreMota %i\n",SentsoreenMota[kont]);
	        *(*Helbideak + kont) =  (int) strtol(buffer+2,NULL,0);
	        kont++;
	    } 
    }

    if(*sentsore_kop!=kont)
    {
        perror("Sentsorearen fitxategia gaizki idatzita dago");
	    exit (1);
    }

    fclose(file);
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

void Irakurri_Sentsore_Guztiak(int sentskop,unsigned char *Helbideak,int *SentsoreenMota,struct balioak_barometro * emaitzak)
{
    for(int i =0; i<sentskop; i++ )
    {
        if(SentsoreenMota[i]==TEMP)
        {
            //printf("sents numb %i\n", i);
            Temp_Irakurri(Helbideak[i],emaitzak+i);
           // printf("\n");
        }
        else if(SentsoreenMota[i]==BAR)
        {
            //printf("sents numb %i\n", i);
            Baro_Irakurri(Helbideak[i],emaitzak+i);
            //printf("\n");
        }
        else
        {
            printf("%i -garren setsoreren izaera definitu gabe dago\n",i);
        }
    }   
}

//irakukrritako datuak emaitzen datubasera bidaltzen ditu http mezu baten bidez.Sentsore guztien 
//irakurketak http mezu berdinean bidaltzen dira trafikoa ahal den bezain beste murrizteko.
void Emaitzak_bidali(int socket,int sentskop, unsigned char *Helbideak,int *SentsoreenMota,struct balioak_barometro * emaitzak)
{
    char payload[2048]="";
    for(int i=0;i<sentskop;i++)
    {
        /*Influxd Mezuaen formatoan beharrezkoa da jartzea mezu mota:Temperatura ala Barometrikoa(temperatura eta presioa)
        Location(Bidaltzen duen sentsorearen identifikadorea( kasu honetan Sentsoreen montaketa ordenaren araberazkoa da)
        eta bukatzeko balioak datuaren izenarekin
        */
        if(SentsoreenMota[i]==TEMP)
            snprintf(payload + strlen(payload), sizeof(payload), "Temperatura,location=Senstore_%i temp=%.2f\n", i, (emaitzak+i)->temperatura);
        else
            snprintf(payload + strlen(payload), sizeof(payload), "Barometrikoa,location=Senstore_%i temp=%.2f,pres=%.2f\n", i, (emaitzak+i)->temperatura, (emaitzak+i)->presioa);
    }
    Irakurketa_Bidali(socket,payload);


}


