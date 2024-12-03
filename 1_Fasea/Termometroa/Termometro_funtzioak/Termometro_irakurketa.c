#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Termometro_irakurketa.h"


float Irakurketa_Termometroa(int fitx)    
{
    int temp_bit = i2c_smbus_read_word_data(fitx, 0x00); // Irakurri tenperaturaren erregistroa.

    // Bitak pixu txikitik handira ordenatuak daudenez berordenatu.
    temp_bit = ((temp_bit & 0xFF) << 8) | (temp_bit >> 8);

    // Formatua egokitu.
    temp_bit >>= 7; 
    float temperature = temp_bit * 0.5; 
    return temperature;
}

int Senstorea_Aatzitu_Term()
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
        if (ioctl(fitx, I2C_SLAVE, HELB) < 0) {
                printf("Slave Helidea finkatzean arazoa \n");
                return -1 ;
        }

        printf("Tenperatura sentsorea zuzen atzitu da \n");
    
    return fitx;    

}


