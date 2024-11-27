#include <fcntl.h>
#include <stdio.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "mpl3115.h"


/*// Select control register(0x26)
        // Active mode, OSR = 128, altimeter mode(0xB9)
        char config[2] = {0};
        config[0] = 0x26;
        config[1] = 0xB9;
        write(file, config, 2);
        // Select data configuration register(0x13)
        // Data ready event enabled for altitude, pressure, temperature(0x07)
        config[0] = 0x13;
        config[1] = 0x07;
        write(file, config, 2);
        // Select control register(0x26)
        // Active mode, OSR = 128, altimeter mode(0xB9)
        config[0] = 0x26;
        config[1] = 0xB9;
        write(file, config, 2);
*/

int main()
{

	int fitx;
	char filename[] = "/dev/i2c-1"; // i2c-1 motatako dispositiboentzat driver-en helbideak
	fitx = open(filename, O_RDWR);
  	if (fitx < 0) {
    		printf("%s bus-a irekitzean arazoa, errzenb = %i \n",filename,fitx);
    		exit(1);
  	}
  	printf("%s Bus-a zuzen ireki da, %i\n",filename,fitx);
	  // Busaren barruan bilatu sentsorearen helbidea eta hau slave bezela finkatu.
  	if (ioctl(fitx, I2C_SLAVE, MPL3115A2_ADDRESS) < 0) {
    		printf("Slave Helidea finkatzean arazoa \n");
    		exit(1);
  	}
	// Select control register(0x26)
        // Active mode, OSR = 128, altimeter mode(0xB9)

  	printf("Slave helbidea zuzen finkatua \n");

	printf("MPL3115A2 Altimeter/Barometer/Temperature sensor demo \n");

	MPL3115A2_Hasieratu_Polling(fitx); //Initialize the sensor

	unsigned char id = MPL3115A2_Id(fitx); //Verify chip id
	printf("Chip ID: 0x%02X . \r\n", id);
	
	
	float min = MPL3115A2_GetMinimumTemperature(fitx);
	printf("temp minimoa =%0.2f",min);
	//while(1)
//    sleep(1);
	//{
while(1){
        char reg[1] = {0x00};
        write(fitx, reg, 1);
        char data[6] = {0};
        if(read(fitx, data, 6) != 6)
        {
                printf("Error : Input/Output error \n");
                exit(1);
        }

        // Convert the data
        printf("data =%c",data[0]);
        int tHeight = ((data[1] * 65536) + (data[2] * 256 + (data[3] & 0xF0)) / 16);
        int temp = ((data[4] * 256) + (data[5] & 0xF0)) / 16;
        float altitude = tHeight / 16.0;
        float cTemp = (temp / 16.0);
        float fTemp = cTemp * 1.8 + 32;

        // Select control register(0x26)
        // Active mode, OSR = 128, barometer mode(0x39)
        char config[2];
        config[0] = 0x26;
        config[1] = 0x39;
        write(fitx, config, 2);
        sleep(1);

        // Read 4 bytes of data from register(0x00)
        // status, pres msb1, pres msb, pres lsb
        reg[0] = 0x00;
        write(fitx, reg, 1);
        read(fitx, data, 4);

        // Convert the data to 20-bits
        int pres = ((data[1] * 65536) + (data[2] * 256 + (data[3] & 0xF0))) / 16;
        float pressure = (pres / 4.0) / 1000.0;

        // Output data to screen
        printf("Pressure : %.2f kPa \n", pressure);
        printf("Altitude : %.2f m \n", altitude);
        printf("Temperature in Celsius : %.2f C \n", cTemp);
        printf("Temperature in Fahrenheit : %.2f F \n", fTemp);

}
//	    float temp = MPL3115A2_ReadTemperature(fitx);//Take a temperature reading
  //          printf("Temperature 1 : %0.2f degree Celsius.\r\n", temp);
            
            
     //       temp = MPL3115A2_ReadTemperature(fitx);//Take a new temperature reading
       //     printf("Temperature 2 : %0.2f degree Celsius.\r\n", temp);
	    //unsigned char va = MPL3115A2_ReadByte(fitx,OFF_T);
          //  printf("Reg: %x. \r\n", va);        
	return 0;
}

