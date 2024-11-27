#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Barometroak bueltatzen duen temperatura balioa gordetzen duen fitxategiaren helbidea.
static const char TEMP_FITX[]="/sys/class/i2c-dev/i2c-1/device/1-0060/iio\:device0/in_temp_raw";
//Barometroren  temperatura balioa gradu zentigradutan itzultzeko beharrezko biderkatzailea.
static const float temp_eskala = 0.062500;
//Barometroak bueltatzen duen presioaren balioa gordetzen duen fitxategiaren helbidea.
static const char PRESIO_FITX[]="/sys/class/i2c-dev/i2c-1/device/1-0060/iio\:device0/in_presssure_raw";
//Barometroren  temperatura balioa gradu zentigradutan
static const float presio_eskala = 0.000250;


float Temp_irakurketa();
void probak();




void main(){

	probak();
}

void probak()
{
	int i=0;
	printf("xd\n");
	float denb = 0.00;
	while(i<1000){
		clock_t t= clock();
		Temp_irakurketa();	
		denb=denb + ((clock()-t)/CLOCKS_PER_SEC);
		i++;
	}	
	denb = denb / 1000.0;
	printf("Batazbesteko exekuzio denbora = %f",denb );

}

float Temp_irakurketa(){
	
	float tenperatura=0.0;
	char buf[10];
	FILE *fitx = fopen(TEMP_FITX,"r");
//	if(fitx== NULL)	
	fgets(buf,sizeof(buf),fitx);
	tenperatura=atof(buf) * temp_eskala;
//        printf("Temp = %2.6f \n", tenperatura);
	fclose(fitx);
	return tenperatura;
}



