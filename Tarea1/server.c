#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

const int  BUFFER_SIZE = 1023;
const char INCOMPLETE_MSG[] = "INCOMPLETE";
const char COMPLETE_MSG[] = "COMPLETE";
const char END_MSG[] = "END";

int receiveFile(int socket); 

int main() {
    // Creacion del descriptor del socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configuracion de direccion y puerto del servidor
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Puerto
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Direccion IP
    
    // Se asigna el puerto al socket
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    // Escucha de conexiones entrantes
    listen(serverSocket, 5);

    // Estructura para obtener la informacion del cliente
    struct sockaddr_in clientAddr;
    unsigned int sin_size = sizeof(clientAddr);

    // Se espera por una conexion con un cliente
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &sin_size);
    char *ipClient = inet_ntoa(clientAddr.sin_addr);
    //printf("Direccion IP del cliente: %s\n", ipClient);

    // Recepcion del archivo
    receiveFile(clientSocket);

    // Cerrar la conexion
    close(serverSocket);

    return 0;
}

/**
 * Funcion para recibir un archivo proveniente de un socket clietne
 * socket: descriptor del socket cliente
 * return: 0 si todo salio bien, -1 en caso contrario
*/ 
int receiveFile(int socket) {
    // Mensaje del cliente con el nombre y dimension del archivo
    unsigned char clientMessage[BUFFER_SIZE];
    int r = recv(socket, &clientMessage, BUFFER_SIZE, 0);
    char *filename = strtok(clientMessage, "*");
    char *fileSize = strtok(NULL, "*");

    // Apertura del archivo
    FILE *write_ptr;
    write_ptr = fopen(filename,"wb");

    // Calculo aproximado de iteraciones necesarias
    int iter = (atoi(fileSize)/BUFFER_SIZE);
    int iter_max = iter + 5; // Para asegurar la recepcion completa

    // Recepcion del archivo
    for (int i = 0; i < iter_max; i++) {
        // Creacion del buffer para leer el mensaje enviado por el socket cliente
        unsigned char* buffer = (char*) malloc(sizeof(unsigned char)*BUFFER_SIZE);
        memset(buffer, 0, sizeof(unsigned char)*BUFFER_SIZE);

        // Lectura del mensaje entrante
        int readBytes = recv(socket, buffer, BUFFER_SIZE, 0);

        // Se verifica si ha finalizado el envio del archivo
        if (strcmp(END_MSG, buffer) == 0) {
            printf("Envio completado!\n");
            break;
        }
        
        // Se verifica si se recibio el mensaje correctamente (bytes recibidos = BUFFER_SIZE)
        if(readBytes != BUFFER_SIZE && i < iter) {
            int completed = 0;
            while (!completed) {
                // Envio del mensaje indicando una recepcion incompleta
                send(socket, INCOMPLETE_MSG, BUFFER_SIZE, 0);

                // Se restablece la direccion de memoria del buffer
                memset(buffer, 0, sizeof(unsigned char)*BUFFER_SIZE);

                // Se recibe de nuevo el mensaje
                readBytes = recv(socket, buffer, BUFFER_SIZE, 0);

                // Se verifica si el nuevo mensaje se recibio correctamente
                if (readBytes == BUFFER_SIZE) {
                    completed = 1;
                    fwrite(buffer,readBytes,1,write_ptr);   
                    send(socket, COMPLETE_MSG, BUFFER_SIZE, 0);
                }
            }
        } else {
            // Se almacena la informacion recibida
            fwrite(buffer,readBytes,1,write_ptr);
            send(socket, COMPLETE_MSG, BUFFER_SIZE, 0);
        }

        // Limpieza de memoria
        free(buffer); 
    }   
    fclose(write_ptr);
    return 0;
}