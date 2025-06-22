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

void Temp_Irakurri(unsigned char helbidea);
void Baro_Irakurri(unsigned char helbidea);

void  main()
{
    struct timespec begin, end; // Structures to store the start and end times for timing
    long double total_time = 0.0; // Accumulate total time
    int N= pow(10,4);
    for (int i=0; i<N; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &begin);
        Temp_Irakurri(0x48);
        Baro_Irakurri(0x60);
        clock_gettime(CLOCK_MONOTONIC, &end); // End time for encryption
        long double start_time = begin.tv_sec * pow(10,6)  + begin.tv_nsec*pow(10,-3);
        long double end_time = end.tv_sec * pow(10,6) + end.tv_nsec*pow(10,-3);
        long double elapsed_time = end_time - start_time;
                total_time += elapsed_time; // Add to total
    }
    long double average_time = total_time;
    average_time = total_time/N;
    printf("Average Execution Time: %.3Lf nanoseconds\n", average_time);
}


void Baro_Irakurri(unsigned char helbidea)
{
    int fitx= Senstorea_Aatzitu(helbidea);
    if(fitx<0)
        exit(1);
//    printf("fitx = %i\n",fitx);    
    float Emaitzak[2]; 

    Irakurketa_Barometrikoa(fitx,Emaitzak);
  ///  printf("Presioa: %0.2f Paskal\n",Emaitzak[0]);       
  //  printf("Tenperatura: %0.2f Celcius\n",Emaitzak[1]);        
    close(fitx);
    //return fitx;
}

void Temp_Irakurri(unsigned char helbidea)
{
    int fitx;
    fitx= Senstorea_Aatzitu_Term(helbidea);

    if(fitx<0)
        exit(1);
    //printf("fitx = %i",fitx);
    float Emaitzak[2];
    
    float emaitza = Irakurketa_Termometroa(fitx);
    //printf("Tenperatura: %0.2f Celcius\n",emaitza);
    close(fitx);
}
