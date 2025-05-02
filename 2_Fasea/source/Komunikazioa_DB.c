#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../includes/Komunikazioa_DB.h"
#include "../includes/Barometro_irakurketa.h"

void Mezua_Jaso(int socket);

//InfulxDB zerbitzarira konektatzen da socketa-ren zenbakia bueltatuz.
int Zerbitzarira_Konektatu(struct Komunikazio_info * ServerInf)
{
    int sockfd;
    struct sockaddr_in server_addr;
    

    // Sortu TCP Socket-a.
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Errorea Socket-a sortzen");
        exit(1);
    }

    // Zerbitzariaren helbidea konfiguratu.
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ServerInf->Server_port);
    if (inet_pton(AF_INET, ServerInf->Server_ip, &server_addr.sin_addr) <= 0) {
	printf("%s %i\n", ServerInf->Server_ip,ServerInf->Server_port);
        perror("Zerbitzariaren helbidea ez da zuzena");
        close(sockfd);
        exit(1);
    }


    // Zerbitzarira konektatu.
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errorea konektatzean");
        exit(1);
    }

    return sockfd;
}

void Irakurketa_Bidali(int server_Socket,char *payload,struct Komunikazio_info * ServerInf ) {
    
    char *request;
    int maxMessageSize = 1028;

    request = malloc(sizeof (char)*maxMessageSize);



    // Prestatu HTTP Mezua.
    snprintf(request, sizeof(char)*maxMessageSize,
             "POST /api/v2/write?org=%s&bucket=%s&precision=s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Authorization: Token %s\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %d\r\n"
             "\r\n"
             "%s",
             ServerInf->org, ServerInf->bucket, ServerInf->Server_ip,getenv("Token_Api"), strlen(payload), payload);
    
    printf("%s\n",request);

    // Datuak bidali zerbitzarira HTTP mezu batean.
    if (send(server_Socket, request, strlen(request), 0) < 0) {
        perror("Send failed");
        close(server_Socket);
        exit(EXIT_FAILURE);
    }

    Mezua_Jaso(server_Socket);
    free(request); 

}

//Mezu hau balio du bidalketa bat egin ondoren jaso den erantzuna jaso eta printeatzeko
void Mezua_Jaso(int socket){
    
    char response[1024];

    usleep(100);//Itxaroten da zihurtatzeko denbora eman duela zerbitzariaren erantzuna jasotzeko.
    recv(socket, response, sizeof(response) - 1, 0);
    printf("Server Response:\n%s\n", response);
   
}
