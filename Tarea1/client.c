#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main() {
    // Creacion del socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int connectionStatus = connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    // Verificar estado de conexion
    if (connectionStatus < 0) {
        printf("Error al intentar conectar con el servidor");
        return 0;
    }

    // Recibir respuesta del servidor
    char serverResponse[256];
    recv(clientSocket, &serverResponse, sizeof(serverResponse), 0);
    printf("Respuesta recibida: %s\n", serverResponse);

    // Se cierra la conexion con el servidor
    close(clientSocket);

    return 0;

}