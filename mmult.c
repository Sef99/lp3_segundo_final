#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
 
struct thread_data{
    int* valores;
    int thread_num;
};

int cont_threads = 1;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *mult(void* arg){
    
    struct thread_data *valores = (struct thread_data *) arg;
    int sum = 0, i = 0;
    
    pthread_mutex_lock(&mutex); 
    while(1){
        if(cont_threads != valores->thread_num){
            pthread_cond_wait(&cond, &mutex);
        }else{
            break;
        }
    }

    printf("Hilo %d: ", valores->thread_num);
    int c1 = (valores->valores[0]);
    for (i = 1; i <= c1; i++){
        int a = (valores->valores[i]);
        int b = (valores->valores[i+c1]);
        sum += a*b;
        printf("%d x %d", a, b);
        if(i != c1){
            printf(" + ");
        }else{
            printf(" = ");
        }
    }
    int *resultado = (int*)malloc(sizeof(int));
    *resultado = sum;
    printf("%d\n", sum);

    cont_threads++;
    free(arg);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
    pthread_exit(resultado);
}

int main(int argc, char *argv[]){
    int fila1, columna1, fila2, columna2, i, j, k;
    fila1 = atoi(argv[1]);
    columna1 = atoi(argv[2]);
    fila2 = atoi(argv[3]);
    columna2 = atoi(argv[4]);

    int matriz1[fila1][columna1];
    int matriz2[fila2][columna2];
    int matriz3[fila1][columna2];
    
    for (i = 0; i < fila1; i++){
        for (j = 0; j < columna1; j++){
            matriz1[i][j] = rand() % 25;
        }
    }

    for (i = 0; i < fila2; i++){
        for (j = 0; j < columna2; j++){
            matriz2[i][j] = rand() % 25;
        }
    }
         
    printf("Matriz 1: \n");
    for (i = 0; i < fila1; i++){
        for(j = 0; j < columna1; j++){
            printf("%d ",matriz1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Matriz 2: \n");
    for (i = 0; i < fila2; i++){
        for(j = 0; j < columna2; j++){
            printf("%d ",matriz2[i][j]);
        }
        printf("\n");   
    }
    printf("\n");
    
    int num_threads = fila1*columna2;
      
    pthread_t *threads;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    threads = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
     
    int contador = 0;
    int* valores = NULL;
    
    for (i = 0; i < fila1; i++){
        for (j = 0; j < columna2; j++){
            struct thread_data *args = malloc(sizeof(struct thread_data));
            args->valores = NULL;
            args->valores = (int *)malloc((fila1*columna1)*sizeof(int));
            args->valores[0] = columna1;
     
            for (k = 0; k < columna1; k++){
                args->valores[k+1] = matriz1[i][k];
            }
     
            for (k = 0; k < fila2; k++){
                args->valores[k+columna1+1] = matriz2[k][j];
            }
            
            args->thread_num = contador + 1;
            if(pthread_create(&threads[contador], NULL, mult, (void *)args) != 0){
                perror("No se pudo crear el thread");
            }
            contador++;
        }
    }
    

    int aux1 = 0;
    int aux2 = 0;
    for (i = 0; i < num_threads; i++){
        void *resultado;
        if(pthread_join(threads[i], &resultado) != 0){
            perror("No se pudo unir el thread");
        }
        int *aux_resultado = (int *)resultado;
        matriz3[aux1][aux2] = *aux_resultado;
        aux2++;
        if ((i + 1) % columna2 == 0){
          aux1++;
          aux2 = 0;
        }
        free(aux_resultado);
    }
    
    printf("\n");
    printf("Matriz Resultante: \n");
    for (i = 0; i < fila1; i++){
        for(j = 0; j < columna2; j++)
            printf("%d ",matriz3[i][j]);
        printf("\n");
    }

    free(threads);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}