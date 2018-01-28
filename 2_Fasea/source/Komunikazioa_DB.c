#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <../includes/Komunikazioa_DB.h>



int conectar_Servidor()
{
    int sockfd;
    struct sockaddr_in server_addr;
    

    // Create a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");


        exit(EXIT_FAILURE);
    }
    return sockfd;

}

void  mandar_lectura(int server_Socket,int senstsore_zenb, float temperatura, float presioa) {
    char request[1024], payload[256];


    // Prepare the payload (line protocol format)
    if(presioa==0.0)
        snprintf(payload, sizeof(payload), "Temperatura,location=Senstore_%i temp=%.2f\n", senstsore_zenb, temperatura);
    else
        snprintf(payload, sizeof(payload), "Barometrikoa,location=Senstore_%i temp=%.2f,pres=%.2f\n", senstsore_zenb, temperatura, presioa);

    // Prepare the HTTP POST request
    snprintf(request, sizeof(request),
             "POST /api/v2/write?org=%s&bucket=%s&precision=s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Authorization: Token %s\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %d\r\n"
             "\r\n"
             "%s",
             ORG, BUCKET, SERVER_IP,TOKEN, strlen(payload), payload);
    
    printf("%s\n",request);
    // Send the request
    if (send(server_Socket, request, strlen(request), 0) < 0) {
        perror("Send failed");
        close(server_Socket);
        exit(EXIT_FAILURE);
    }

    printf("Data sent: %s", payload);

    char response[1024];
    usleep(100);

    recv(server_Socket, response, sizeof(response) - 1, 0);
    printf("Server Response:\n%s\n", response);
    // Close the socket
    close(server_Socket);
}

int main() {
    // Simulated sensor data
    float sensor_value = 23.5;
    int socket = conectar_Servidor();
    // Send data to InfluxDB
    mandar_lectura(socket,1, sensor_value,3.0);

    

    return 0;
}
