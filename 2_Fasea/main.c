#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
void Emaitzak_bidali(int socket,int sentskop, unsigned char *Helbideak,int *SentsoreenMota,struct balioak_barometro * emaitzak, struct Komunikazio_info * kom_info);
void komunikazio_aldagaiak_irakurri(struct Komunikazio_info *kom_info);
void KopiatuBukaeraGabe(char *dest, const char *src);




//Main hau pentsatua dago denbora guztian exekutatzen egoteko beraz simple programatu dut
//Iterazio bakoitzean sentsore bakoitzaren irakurketa ba egin eta bidaltzeko.
void  main()
{
    unsigned char *Helbideak;
    int *SentsoreenMota;
    int sentsore_kop;

    struct Komunikazio_info kom_info;

    //Sentsoreen helbideak irakurtzen ditu fitxategi batetik
    Helbideak_irakurri(&Helbideak,&SentsoreenMota ,&sentsore_kop);

    komunikazio_aldagaiak_irakurri(&kom_info);

    struct balioak_barometro *emaitzak= calloc (sentsore_kop ,sizeof (struct balioak_barometro));

    int socket=1;
    //Zerbitzarira_Konektatu(&kom_info);

    while(1)
    {
        Irakurri_Sentsore_Guztiak(sentsore_kop,Helbideak,SentsoreenMota,emaitzak);
        Emaitzak_bidali(socket,sentsore_kop,Helbideak,SentsoreenMota,emaitzak,&kom_info);
    }

    close(socket);
    free(emaitzak);
    free(Helbideak);
    free(SentsoreenMota);
    
}

void komunikazio_aldagaiak_irakurri(struct Komunikazio_info *kom_info)
{
    
    //Era seguruan gordetzeko enviroment variable batean gorde dut komunikazio aldagaien fitxategia
    const char * fitx_izena = getenv("KOM_FITX");
    //const char *fitx_izena = "./data.txt";
    FILE *file = fopen(fitx_izena, "r");

    char buffer[1024];
    int kont = 0;

    int mempointer=0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
	    if(buffer[0] != '#' &&  buffer[0] != ' ' )
        {
            switch (kont)
            {
            case 0:
                kom_info->Server_ip = malloc(strlen(buffer)+1);
                KopiatuBukaeraGabe(kom_info->Server_ip,buffer);
                break;
            case 1:
                kom_info->Server_port = atoi(buffer);
                break;
            case 2:
                kom_info->org = malloc(strlen(buffer)*sizeof(char));
                KopiatuBukaeraGabe(kom_info->org,buffer);
                break;
            case 3:
                kom_info->bucket = malloc(strlen(buffer)*sizeof(char));
                KopiatuBukaeraGabe(kom_info->bucket,buffer);
                break;
            }
	        kont++;
	    }
    }
    fclose(file);

}

void Helbideak_irakurri( unsigned char **Helbideak,int **SentsoreenMota ,int *sentsore_kop){

    const char *fitx_izena = "/etc/Sentsoreen_irakurketa/Sentsore_helbideak.txt";

    FILE *file = fopen(fitx_izena, "r");
    if (file == NULL) {
        perror("Arazoa fitxategia irekitzen");
        exit(1);
    }

    unsigned char buff[1024];

    //Bigarren filan beti egongo da konektatutako sentsoreenkopurua
    fgets(buff, sizeof(buff), file);
    fgets(buff, sizeof(buff), file);
    *sentsore_kop =  (int) buff[0] -48 ;

    if(*sentsore_kop < 1){
	    perror("Ez daude sentsorerik konektatuak\n");
	    exit(1);
    }

    //Konektatutako sentsore guztien helbideak.
    *Helbideak = malloc(*sentsore_kop * sizeof(unsigned char) );

    //Ea sentsoreak tenperaturazkoak ala barometrikoak diren gordetzen duen array-a.
    *SentsoreenMota = malloc(*sentsore_kop * sizeof(int) );

    //irakurritako lineen kopurua
    int kont  = 0;
    while (fgets(buff, sizeof(buff), file) != NULL) {
	    if(buff[0] != '#')
        {
            *(*SentsoreenMota+kont) = buff[0] - 48;
            //printf("SentsoreMota %i\n",SentsoreenMota[kont]);
	        *(*Helbideak + kont) =  (int) strtol(buff+2,NULL,0);
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
    emaitzak->tenperatura =Irakurketa_Termometroa(fitx);
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
void Emaitzak_bidali(int socket,int sentskop, unsigned char *Helbideak,int *SentsoreenMota,struct balioak_barometro * emaitzak, struct Komunikazio_info * kom_info)
{
   
    int maxPayloadSize=1024;
    char *payload = calloc(maxPayloadSize,sizeof(char));
      //    char payload[2048]= "";



    for(int i=0;i<sentskop;i++)
    {
        /*Influxd Mezuaen formatoan beharrezkoa da jartzea mezu mota:tenperatura ala Barometrikoa(tenperatura eta presioa)
        Location(Bidaltzen duen sentsorearen identifikadorea( kasu honetan Sentsoreen montaketa ordenaren araberazkoa da)
        eta bukatzeko balioak datuaren izenarekin
        */
        if(SentsoreenMota[i]==TEMP)
            snprintf(payload + strlen(payload),maxPayloadSize-strlen(payload), "Tenperatura,location=Senstore_%i temp=%.2f\n", i, (emaitzak+i)->tenperatura);
        else
            snprintf(payload + strlen(payload), maxPayloadSize-strlen(payload), "Barometrikoa,location=Senstore_%i temp=%.2f,pres=%.2f\n", i, (emaitzak+i)->tenperatura, (emaitzak+i)->presioa);
    }

        Irakurketa_Bidali(socket,payload,kom_info);
        
    free(payload);
}

void KopiatuBukaeraGabe(char *dest, const char *src) {
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dest++ = *src;  
        }
        src++;  
    }
    *dest = '\0';  
}


