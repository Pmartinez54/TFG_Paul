#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "../../includes/Barometro_irakurketa.h"
#include "../../includes/Termometro_irakurketa.h"

int Temp_Irakurri(int fitx,unsigned char helb);
int Baro_Irakurri(int fitx,unsigned char helb);


void  main()
{
    int fitx1;
    int fitx2;
    struct timespec begin, end; // Structures to store the start and end times for timing
    long double total_time = 0.0; // Accumulate total time
    fitx1 = Temp_Irakurri(0,0x48);
    fitx1 = Baro_Irakurri(fitx1,0x60);
    int N= pow(10,4);
    for (int i=0; i<N; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &begin);
        Temp_Irakurri(fitx1,0x48);
        Baro_Irakurri(fitx1,0x60);
        clock_gettime(CLOCK_MONOTONIC, &end); // End time for encryption
        long double start_time = begin.tv_sec * pow(10,6)  + begin.tv_nsec*pow(10,-3);
        long double end_time = end.tv_sec * pow(10,6) + end.tv_nsec*pow(10,-3);
        long double elapsed_time = end_time - start_time;
                total_time += elapsed_time; // Add to total

//        sleep(1);
    }
    long double average_time = total_time;
    average_time = total_time/(float)N;
    printf("Average Execution Time: %.3Lf nanoseconds\n", average_time);
}


int Baro_Irakurri(int fitx,unsigned char helb)
{
    fitx = Senstorea_Aatzitu_irekita(fitx,helb);
    if(fitx<0)
        exit(1);
//    printf("fitx = %i\n",fitx);    

    float Emaitzak[2]; 
    Irakurketa_Barometrikoa(fitx,Emaitzak);
  //  printf("Presioa: %0.2f Paskal\n",Emaitzak[0]);       
   // printf("Tenperatura: %0.2f Celcius\n",Emaitzak[1]);        
//    close(fitx);
    return fitx;
}

int Temp_Irakurri(int fitx,unsigned char helb)
{
    fitx = Senstorea_Aatzitu_irekita_Term(fitx, helb);
   // printf("fitx = %i",fitx);
    if(fitx<0)
        exit(1);

    float Emaitzak[2];
    float emaitza = Irakurketa_Termometroa(fitx);
   // printf("Tenperatura: %0.2f Celcius\n",emaitza);
    //close(fitx);
    return fitx;
}
