#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char serverMessage[256] = "Conexion recibida!";


    // Creacion del descriptor del socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // 
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    // Se asigna el puerto al socket
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    // Escucha de conexiones entrantes
    listen(serverSocket, 5);

    // Parametros NULL: estan relacionados a querer conocer la ip del cliente
    struct sockaddr_in clientAddr;
    unsigned int sin_size = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &sin_size);
    char *ip = inet_ntoa(clientAddr.sin_addr);
    printf("Direccion IP del cliente: %s\n", ip);

    // Se envian datos al cliente
    send(clientSocket, serverMessage, sizeof(serverMessage), 0);

    // Cerrar la conexion
    close(serverSocket);

    return 0;
}