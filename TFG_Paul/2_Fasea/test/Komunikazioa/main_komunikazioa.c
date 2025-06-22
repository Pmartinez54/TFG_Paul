#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../includes/Komunikazioa_DB.h"

int main() 
{

    // Simulated sensor data
    float temp = 23.5;
    float bar = 1071.6;
    int socket = conectar_Servidor();
    // Send data to InfluxDB

    char payload[2048];

    snprintf(payload + strlen(payload), sizeof(payload), "Barometrikoa,location=Senstore temp=%.2f,pres=%.2f\n",temp, bar);
    
    mandar_lectura(socket,payload);

    

    return 0;
}
