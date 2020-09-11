#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // Creacion del socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Se intenta conectar con el puerto de la direccion ip establecida
    int connectionStatus = connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    // Verificar estado de conexion
    if (connectionStatus < 0) {
        printf("Error al intentar conectar con el servidor\n");
        return 0;
    }

    // Recibir respuesta del servidor
    char serverResponse[256];
    recv(clientSocket, &serverResponse, sizeof(serverResponse), 0);
    printf("Respuesta: %s\n", serverResponse);

    // Se cierra la conexion con el servidor
    close(clientSocket);

    return 0;

}