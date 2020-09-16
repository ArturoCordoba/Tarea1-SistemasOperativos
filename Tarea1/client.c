#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

const int  BUFFER_SIZE = 1023;
const char START_MSG[] = "START";
const char INCOMPLETE_MSG[] = "INCOMPLETE";
const char COMPLETE_MSG[] = "COMPLETE";
const char PROCESS_COMPLETE_MSG[] = "PROCESS_COMPLETE";
const char END_MSG[] = "END";
const char ERROR[] = "*$ERROR";

int sendFile(char* filepath, int socket);
char* concat(const char *s1, const char *s2);
char* long2str(long number);

int main() {
    // Creacion del socket
    int cSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configuracion de direccion y puerto del cliente
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081); // Puerto
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Direccion IP del servidor 

    // Se intenta conectar con el puerto de la direccion ip establecida
    int connectionStatus = connect(cSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    // Verificar estado de conexion
    if (connectionStatus == -1) {
        printf("Error al intentar conectar con el servidor\n");
        return -1;
    }

    unsigned char* buffer = (char*) malloc(sizeof(unsigned char)*BUFFER_SIZE);
    while (1) {
        memset(buffer, 0, sizeof(unsigned char)*BUFFER_SIZE);

        char filepath[100];
        printf("Ingrese la ruta: ");
        scanf("%s", filepath);

        // Envio del mensaje de inicio
        send(cSocket, START_MSG, BUFFER_SIZE, 0);

        // Envio del archivo
        //char filepath[] = "beach2.png";
        sendFile(filepath, cSocket);

        // Se espera por el mensaje de finalizacion
        int r =recv(cSocket, buffer, BUFFER_SIZE, 0);

        if (r == 0){ // Se perdio la conexion
            break;
        }
    }
    
    // Se cierra la conexion con el servidor
    close(cSocket);
    return 0;

}

/**
 * Funcion para enviar un archivo a un servidor
 * filepath: direccion del archivo que se desea enviar
 * socket: descriptor del socket servidor
 * return: 0 si todo salio bien, -1 en caso contrario
*/ 
int sendFile(char* filepath, int socket) {
    // Apertura del archivo en modo de lectura binaria
    FILE *pFile = fopen(filepath, "rb");
    if (!pFile) {
        printf("Error al abrir el archivo\n");
        send(socket, ERROR, sizeof(ERROR), 0);
        return -1;
    }

    // Obtener dimension del archivio (peso)
    fseek(pFile, 0, SEEK_END); // Mueve el cursor de lectura final del archivo
    long size = ftell(pFile); // Obtiene la posicion del cursor en el archivo
    rewind(pFile); // Reinicia el cursor de lectura

    // Mensaje al servidor indicando datos del archivo
    char *aux1 = concat(filepath, "*"); // Nombre del archivo
    char *aux2 = long2str(size);       // Extension del archivo
    char *fileData = concat(aux1,aux2);
    int info = send(socket, fileData, BUFFER_SIZE, 0);
    if (!info) {
        printf("Error al enviar datos del archivo");
        return -1;
    }
    
    // Envio del archivo al servidor
    while (1) {
        unsigned char* buffer = (char*) malloc(sizeof(unsigned char)*BUFFER_SIZE);
        memset(buffer, 0, sizeof(unsigned char)*BUFFER_SIZE);
        char* status = (char*) malloc(sizeof(char)*BUFFER_SIZE);
        memset(buffer, 0, sizeof(unsigned char)*BUFFER_SIZE);

        // Lectura del archivo
        int readBytes = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, pFile);

        //Envio del archivo
        if (send(socket, buffer, readBytes, 0) == -1){
            printf("Error al enviar chunk del archivo\n");
            return -1;
        }

        int completed = 0;
        // Se verifica el estado del envio del archivo
        while (!completed) {
            // Se obtiene el mensaje enviado por el servidor
            recv(socket, status, BUFFER_SIZE, 0);
            if (strcmp(status, COMPLETE_MSG) == 0) {
                completed = 1;
            } else {
                // Se envia el buffer de nuevo
                send(socket, buffer, readBytes, 0);
            }
        }
        // Limpieza de memoria
        free(buffer);
        free(status);

        // Condicion de parada
        if (readBytes != BUFFER_SIZE) {
            break;
        }
    }
    // Indica al servidor que ha finalizado el envio del archivo
    if (send(socket, END_MSG, sizeof(END_MSG), 0) == -1) {
        printf("Error al enviar mensaje de finalizacion");
    }
    
    fclose(pFile);
    return 0;
}

/**
 * Funcion para concatenar un string s2 al final de un string s1
 * s1: string que se desea concatenar
 * s2: string que se desea concatenar
 * return: string con la concatenacion de s1 y s2
*/ 
char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1); // Largo de s1
    const size_t len2 = strlen(s2); // Largo de s2
    char *result = malloc(len1 + len2 + 1); // +1 para el null-terminator
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}

/**
 * Funcion para obtener un string a partir de un numero
 * number: numero que se desea convertir a string
 * return: string que representa al numero
*/
char* long2str(long number) {
    int n = snprintf(NULL, 0, "%lu", number);
    char* buf = malloc(n+1);
    snprintf(buf, n+1, "%lu", number);
    return buf;
}