#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

int createDirectory(char* filepath);
char* concat(const char *s1, const char *s2);
char* processDirectory();
char* long2str(long number);
char* createDataFolders();

int main() {
    char* namePDir = processDirectory();

    char* dataFolder = createDataFolders();
    printf("Resultado: %s\n", dataFolder);

    return 0;
}

char* long2str(long number) {
    int n = snprintf(NULL, 0, "%lu", number);
    char* buf = malloc(n+1);
    snprintf(buf, n+1, "%lu", number);
    return buf;
}

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1); // Largo de s1
    const size_t len2 = strlen(s2); // Largo de s2
    char *result = malloc(len1 + len2 + 1); // +1 para el null-terminator
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}

/**
 * Funcion para crear un directorio
 * filepath: ruta con la direccion y nombre de la carpeta
 * return: 0 en caso exitoso, -1 si hubo un error
*/ 
int createDirectory(char* filepath) {
    return mkdir(filepath, 0777);
}

/**
 * Funcion para crear los directorios de procesamiento de imagenes
 * return: path a la carpeta correspondiente al contenedor
*/  
char* processDirectory() {
    createDirectory("psot1-dprocessing");
    char *sCounter = NULL;
    char *name = NULL;
    int created = -1; 
    int counter = 0;

    // Se determina el numero de contenedor correspondiente
    while (created != 0){
        counter++;
        sCounter = long2str(counter);
        name = concat("psot1-dprocessing/container", sCounter);
        created = createDirectory(name);
    }
    return name;
}

/**
 * Funcion para crear los directorios de almacenamiento de imagenes procesadas
 * return: path a la carpeta correspondiente al contenedor
*/  
char* createDataFolders() {
    createDirectory("psot1-dstorage");
    char *sCounter = NULL;
    char *name = NULL;
    int created = -1; 
    int counter = 0;

    // Se determina el numero de contenedor correspondiente
    while (created != 0){
        counter++;
        sCounter = long2str(counter);
        name = concat("psot1-dstorage/container", sCounter);
        created = createDirectory(name);
    }
    // Creacion de los nombres de las carpetas
    char *redFolder = concat(name,"/R");
    char *greenFolder = concat(name,"/G");
    char *blueFolder = concat(name,"/B");
    char *notTrustedFolder = concat(name,"/Not trusted");

    // Creacion de directorios
    createDirectory(redFolder);
    createDirectory(greenFolder);
    createDirectory(blueFolder);
    createDirectory(notTrustedFolder);
    return name;
}