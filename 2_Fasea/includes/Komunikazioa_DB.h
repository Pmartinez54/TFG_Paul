#define SERVER_IP "192.168.7.1"  // InfluxD datu basearen ip-helbidea.
#define SERVER_PORT 2083          // InfluxD datu basearen portu zenbakia.
#define ORG "ehu"              // InfluxD datu basearen organizazioa.
#define BUCKET "ehu"        // InfluxD datu basearen buzoia.
#define TOKEN "ZS9-oE4yU2rOdkiMTs8Jt9_CT5L5BHoI-0RuhEf4nLh5uteNRQ4oSzXGRGIkCwwi1cqQ1PmjpejyjXiW3xtDuw==" //InfluxD datu basearen baimen token-a.


int  Zerbitzarira_Konektatu();
void Irakurketa_Bidali(int server_Socket,char *payload);
void Mezua_Jaso(int socket);

