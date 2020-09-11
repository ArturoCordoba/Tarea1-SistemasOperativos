#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main() {
    char serverMessage[256] = "Conexion recibida!";

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // 
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    //Segundo argumento: cantidad de clientes que se espera
    listen(serverSocket, 5);

    // Parametros NULL: estan relacionados a querer conocer la ip del cliente
    int clientSocket = accept(serverSocket, NULL, NULL);

    send(clientSocket, serverMessage, sizeof(serverMessage), 0);

    // Cerrar la conexion
    close(serverSocket);

    return 0;
}