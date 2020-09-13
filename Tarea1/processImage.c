#include <stdio.h>
#include <stdlib.h>
#include <png.h>

long maxOfTwo(long a, long b);
int getPredominantColor(long sumR, long sumG, long sumB);
int processImage(char* filepath);

int main() {
    char filename[] = "beach2.png";
    processImage(filename);

    return 0;
}

int processImage(char* filepath) {
    FILE *pFile = fopen(filepath, "rb");
    if(!pFile) {
        printf("Error al leer el archivo %s\n", filepath);
        return -1;
    }
    // Se leen los primeros 8 bits del archivo para
    // verificar que sea una imagen en formato png
    int len = 8; // Largo del buffer
    char header[len]; // Buffer
    fread(header, 1, len, pFile); // Lectura de los primeros 8 bits
    int is_png = !png_sig_cmp(header, 0, len); 
    if (!is_png) {
        printf("Archivo %s no es una imagen en formato png\n", filepath);
        fclose(pFile);
        return -1;
    }

    // Creacion de la estructura de lectura
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(setjmp(png_jmpbuf(png_ptr))) {
        printf("Error al obtener la informacion del archivo %s\n", filepath);
        fclose(pFile);
        return -1;
    }

    // Lectura de la informacion de la imagen
    png_init_io(png_ptr, pFile);
    png_set_sig_bytes(png_ptr, len); // Se indica que se han leido 8 bits
    png_read_info(png_ptr, info_ptr);
    int width = png_get_image_width(png_ptr, info_ptr);   // Ancho
    int height = png_get_image_height(png_ptr, info_ptr); // Largo
    int channels = png_get_channels(png_ptr, info_ptr);   // Canales
    int number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    // Lectura de los datos
    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("Error durante la lectura de los pixeles\n");
        fclose(pFile);
        return -1;
    }

    png_bytep *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (int i = 0; i < height; i++) {
        row_pointers[i] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    fclose(pFile);

    //printf("Width: %d - Height: %d - Channels: %d\n", width, height, channels);

    long sumR = 0, sumG = 0, sumB = 0; // Sumatoria de cada canal
    for (int y = 0; y < height; y++) {
        png_byte* row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_byte* pixel = &(row[x*3]);
            sumR += pixel[0];
            sumG += pixel[1];
            sumB += pixel[2];
        }
    }
    int predominant = getPredominantColor(sumR, sumG, sumB);
    printf("Color: %d\n", predominant);

    return 0;
}

/**
 * Funcion que calcula el maximo entre dos numeros
 * a: primer numero de la comparacion
 * b: segundo numero de la comparacion
 * return: el numero mas grande entre a y b
*/ 
long maxOfTwo(long a, long b) {
    return (a > b) ? a : b; 
}

/**
 * Funcion que calcula indica el canal predominante de una imagen
 * sumR: sumatoria de los valores del canal rojo
 * sumG: sumatoria de los valores del canal verde
 * sumB: sumatoria de los valores del canal azul
 * return: 1 canal rojo, 2 canal verde, 3 canal azul
*/
int getPredominantColor(long sumR, long sumG, long sumB) {    
    // Se calcula el numero maximo
    long biggest = maxOfTwo(maxOfTwo(sumR,sumG),sumB);
    // Se identifica cual canal tiene la sumatoria mas grande
    if (sumR == biggest) {
        return 1;
    } else if (sumG == biggest) {
        return 2;
    } else {
        return 3;
    }
}