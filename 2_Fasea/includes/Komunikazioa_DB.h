#define SERVER_IP "192.168.7.1"  // InfluxD datu basearen ip-helbidea.
#define SERVER_PORT 2083          // InfluxD datu basearen portu zenbakia.
#define ORG "ehu"              // InfluxD datu basearen organizazioa.
#define BUCKET "ehu"        // InfluxD datu basearen buzoia.
#define TOKEN "lbrjv0oGzRa1HWeB2U7NdqDnWVqOz7gm4-DmTqjQAvzzve8MfZiuuBJqtLwo0Kimm5qactllM-MUSxtzNvtbNw==" //InfluxD datu basearen baimen token-a.


int  Zerbitzarira_Konektatu();
void Irakurketa_Bidali(int server_Socket,char *payload);
void Mezua_Jaso(int socket);

