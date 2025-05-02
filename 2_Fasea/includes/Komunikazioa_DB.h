
struct Komunikazio_info
{
    char *Server_ip;
    int   Server_port;
    char *org;
    char *bucket;
};

int Zerbitzarira_Konektatu(struct Komunikazio_info * ServerInf);
void Irakurketa_Bidali(int server_Socket,char *payload,struct Komunikazio_info * kom_info);
void Mezua_Jaso(int socket);

