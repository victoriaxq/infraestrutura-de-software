#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int num_threads;

//Struct para receber cor dos pixels
typedef struct {
    int r;
    int g;
    int b;
} Pixel;

//Struct para receber infos da imagem
typedef struct {
    int largura;
    int altura;
    int max_value;
    Pixel* pixels;
} Image;


Image* image;

//Funcao para converter para tons de cinza
Pixel conversor_cinza(Pixel pixel) {
    int valor_cinza = (int)(pixel.r * 0.30 + pixel.g * 0.59 + pixel.b * 0.11);
    Pixel pixel_cinza = {valor_cinza, valor_cinza, valor_cinza};
    return pixel_cinza;
}

//funcao para threads
void* funcao(void *id) {
    
 int thread_id = *((int*)id);   

 image->pixels[thread_id]  = conversor_cinza(image->pixels[thread_id]);

    pthread_exit(NULL);
}

//funcao para ler do arquivo
Image* ler_imagem(char* filename) {
    FILE* file = fopen(filename, "r+b");
    if (file == NULL) {
        printf("Erro ao abrir arquivo.\n");
        exit(1);
    }

    char magic_number[3];
    fscanf(file, "%s", magic_number);
    if (magic_number[0] != 'P' || magic_number[1] != '3') {//verificar se o arquivo válido
        printf("Formato de arquivo inválido.\n");
        exit(1);
    }

    int largura, altura, max_value;
    fscanf(file, "%d %d", &largura, &altura);
    fscanf(file, "%d", &max_value);

    Pixel* pixels = (Pixel*)malloc(sizeof(Pixel) * largura * altura);

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            int r, g, b;
            fscanf(file, "%d %d %d", &r, &g, &b);
            Pixel pixel = {r, g, b};
            pixels[i * largura + j] = pixel;
        }
    }

    fclose(file);

    Image* image = (Image*)malloc(sizeof(Image));
    image->largura = largura;
    image->altura = altura;
    image->max_value = max_value;
    image->pixels = pixels;

    return image;
}

//Funcao para ler do arquivo
void escrita_imagem(char* filename, Image* image) {
    FILE* file = fopen(filename, "w+b");
    if (file == NULL) {
        printf("Erro ao abrir arquivo.\n");
        exit(1);
    }

    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", image->largura, image->altura);
    fprintf(file, "%d\n", image->max_value);

    for (int i = 0; i < image->altura; i++) {
        for (int j = 0; j < image->largura; j++) {
            int index = i * image->largura + j;
            fprintf(file, "%d %d %d ", image->pixels[index].r, image->pixels[index].g, image->pixels[index].b);
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

int main() 
{

char *input_filename = "rgb.ppm";//nome do arquivo para ler as infos
char *output_filename = "cinza.ppm";//nome do arquivo para escrever


image = ler_imagem(input_filename);

num_threads = image->altura * image->largura; //numero de pixels

pthread_t threads[num_threads];
int *id[num_threads];


for (int i = 0; i < num_threads; i++) {
    id[i] = (int *)malloc(sizeof(int));
    *id[i] = i;

    int aux = pthread_create(&threads[i], NULL, funcao, (void *)id[i]);
}

for (int i = 0; i < num_threads; i++) {

    pthread_join(threads[i], NULL);
    
}

escrita_imagem(output_filename, image);

free(image->pixels);
free(image);

return 0;
}
