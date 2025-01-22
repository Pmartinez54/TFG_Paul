#define SERVER_IP "192.168.7.1"  // IP Del servidor influxd
#define SERVER_PORT 2083          // Default HTTP port for InfluxDB v2
#define ORG "ehu"              // Replace with your InfluxDB organization
#define BUCKET "ehu"        // Replace with your InfluxDB bucket
#define TOKEN "ZS9-oE4yU2rOdkiMTs8Jt9_CT5L5BHoI-0RuhEf4nLh5uteNRQ4oSzXGRGIkCwwi1cqQ1PmjpejyjXiW3xtDuw==" // Replace with your InfluxDB API token

//#include <Barometro_irakurketa.h>

void  mandar_lectura(int server_Socket,char *payload);
int conectar_Servidor();

