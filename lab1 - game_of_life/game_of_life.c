//Douglas Diniz Landim. RA 76681
#define _XOPEN_SOURCE 600 // X/Open 6, incorporating POSIX 2004
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#define DEAD 0
#define ALIVE 1
#define NUM_THREADS 8
#define SRAND_VALUE 1985
#define GENERATIONS 2000
#define dim 2048

struct thread{
	int inicio;
	int fim;
	int index;
	pthread_t thr;
};

pthread_barrier_t barWait;

int **grid, **new_grid, count[NUM_THREADS];

int linha_superior(int **grid, int *count, int i, int j){
	if(grid[dim-1][j] == ALIVE)
		count++; //acima
	if(grid[i+1][j] == ALIVE)
		count++; //abaixo

	//diagonal esquerda
	if(j == 0){
		//vizinhos da esquerda
		if(grid[dim-1][dim-1] == ALIVE)
			count++;
		if(grid[i][dim-1] == ALIVE)
			count++;
		if(grid[i+1][dim-1] == ALIVE)
			count++;

		//vizinhos da direita
		if(grid[dim-1][j+1] == ALIVE)
			count++;
		if(grid[i][j+1] == ALIVE)
			count++;
		if(grid[i+1][j+1] == ALIVE)
			count++;

		return count;
	}

	//diagonal direita
	if(j == dim - 1){
		//vizinhos da esquerda
		if(grid[dim-1][j-1] == ALIVE)
			count++;
		if(grid[i][j-1] == ALIVE)
			count++;
		if(grid[i+1][j-1] == ALIVE)
			count++;
		//vizinhos da direita
		if(grid[dim-1][0] == ALIVE)
			count++;
		if(grid[i][0] == ALIVE)
			count++;
		if(grid[i+1][0] == ALIVE)
			count++;

		return count;
	}

	//fora dos cantos
	else {
		//vizinhos da esquerda
		if(grid[dim-1][j-1] == ALIVE)
			count++;
		if(grid[i][j-1] == ALIVE)
			count++;
		if(grid[i+1][j-1] == ALIVE)
			count++;
		//vizinhos da direita
		if(grid[dim-1][j+1] == ALIVE)
			count++;
		if(grid[i][j+1] == ALIVE)
			count++;
		if(grid[i+1][j+1] == ALIVE)
			count++;

		return count;
	}
}

int linha_inferior(int **grid, int *count, int i, int j){
	if(grid[i-1][j] == ALIVE)
		count++; //acima
	if(grid[0][j] == ALIVE)
		count++; //abaixo

	//diagonal esquerda
	if(j == 0){
			//vizinhos direita
			if(grid[i-1][dim-1] == ALIVE)
				count++;
			if(grid[i][dim-1] == ALIVE)
				count++;
			if(grid[0][dim-1] == ALIVE)
				count++;
			//vizinhos esquerda
			if(grid[i-1][j+1] == ALIVE)
			 count++;
			if(grid[i][j+1] == ALIVE)
			 count++;
			if(grid[0][j+1] == ALIVE)
			 count++;
			return count;
		}

	//diagonal direita
	if(j == dim - 1){
			//vizinhos esquerda
			if(grid[i-1][j-1] == ALIVE)
			 count++;
			if(grid[i][j-1] == ALIVE)
			 count++;
			if(grid[0][j-1] == ALIVE)
			 count++;
			//vizinhos direita
			if(grid[i-1][0] == ALIVE)
			 count++;
			if(grid[i][0] == ALIVE)
			 count++;
			if(grid[0][0] == ALIVE)
			 count++;
			return count;
		}

	//fora dos cantos
	else{
			//esquerda
			if(grid[i-1][j-1] == ALIVE) count++;
			if(grid[i][j-1] == ALIVE) count++;
			if(grid[0][j-1] == ALIVE) count++;
			//direita
			if(grid[i-1][j+1] == ALIVE) count++;
			if(grid[i][j+1] == ALIVE) count++;
			if(grid[0][j+1] == ALIVE) count++;
			return count;
		}
}

int coluna_esquerda(int **grid, int *count, int i, int j){
	//esquerda
	if(grid[i-1][dim-1] == ALIVE)
	 count++;
	if(grid[i][dim-1] == ALIVE)
	 count++;
	if(grid[i+1][dim-1] == ALIVE)
	 count++;
	//direita
	if(grid[i-1][j+1] == ALIVE)
	 count++;
	if(grid[i][j+1] == ALIVE)
	 count++;
	if(grid[i+1][j+1] == ALIVE)
	 count++;
	return count;
}

int coluna_direita(int **grid, int *count, int i, int j) {
	//esquerda
	if(grid[i-1][j-1] == ALIVE)
	 count++;
	if(grid[i][j-1] == ALIVE)
	 count++;
	if(grid[i+1][j-1] == ALIVE)
	 count++;
	//direita
	if(grid[i-1][0] == ALIVE)
	 count++;
	if(grid[i][0] == ALIVE)
	 count++;
	if(grid[i+1][0] == ALIVE)
	 count++;
	return count;
}

int getNeighbors(int** grid, int i, int j){
	int count = 0;
	//VERIFICANDO AS BORDAS
  if(i == 0)
		return linha_superior(grid,&count,i,j);
	if(i == dim - 1)
		return linha_inferior(grid,&count,i,j);
	//Vizinhos acima e abaixo
	if(grid[i-1][j] == ALIVE)
		count++; //acima
  if(grid[i+1][j] == ALIVE)
		count++; //abaixo
	if(j == 0)
		return coluna_esquerda(grid,&count,i,j);
	if(j == dim-1)
		return ultima_coluna(grid,&count,i,j);

	//FORA DAS BORDAS
	//vizinhos esquerda
		if(grid[i-1][j-1] == ALIVE)
		 count++;
		if(grid[i][j-1] == ALIVE)
		 count++;
		if(grid[i+1][j-1] == ALIVE)
		 count++;
	//vizinhos direita
		if(grid[i-1][j+1] == ALIVE)
		 count++;
		if(grid[i][j+1] == ALIVE)
		 count++;
		if(grid[i+1][j+1] == ALIVE)
		 count++;
		return count;
}

void *new_generation(void *args){
	struct thread *t = (struct thread *)args;
	for (int i = t->inicio; i < t->fim; i++){
		for (int j = 0; j < dim; j++)
			if(grid[i][j] == ALIVE) count[t->index]++;
		pthread_barrier_wait(&barWait);
	}
	int count_gen;
	for (count_gen= 0; count_gen < GENERATIONS; count_gen++){
		for (int i = t->inicio; i < t->fim; i++){
			for (int j = 0; j < dim; j++){
				if(grid[i][j] == ALIVE){
					if(getNeighbors(grid, i, j) < 2){
						new_grid[i][j] = DEAD;
						count[t->index]--;
					}else if(getNeighbors(grid, i, j) >= 4){
						new_grid[i][j] = DEAD;
						count[t->index]--;
					}else if(getNeighbors(grid, i, j) == 2 || getNeighbors(grid, i, j) == 3){
						new_grid[i][j] = ALIVE;
					}
				}else{
					if(getNeighbors(grid, i, j) == 3){
						new_grid[i][j] = ALIVE;
						count[t->index]++;
					}
				}
			}
		}
	}
	pthread_barrier_wait(&barWait);
	for (int i = t->inicio; i < t->fim; i++){
		for (int j = 0; j < dim; j++){
			grid[i][j] = new_grid[i][j];
		}
	}
	pthread_barrier_wait(&barWait);
}

void aloca_grid(int **matrix, int size){
	int i = 0;
	matrix = malloc(size * dimof(int*));
    for(i = 0; i < size; i++) grid[i] = malloc(dim * dimof(int));
}

int main(int argc, char *argv[]){
	clock_t start, end;
  	struct timeval inicio, inicioLaco, final2, finalLaco;
  	int tmili, tempoLaco;
  	start = clock();
  	gettimeofday(&inicio, NULL);
	int  i, total = 0;
	grid = malloc(dim * dimof(int*));
    for(i = 0; i < dim; i++) grid[i] = malloc(dim * dimof(int));
	aloca_grid(grid, dim);
	aloca_grid(new_grid, dim);
	for ( i = 0; i < NUM_THREADS; i++){
		count[i] = 0;
	}
	srand(SRAND_VALUE);
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j< dim; j++) {
            grid[i][j] = rand() % 2;
        }
    }

	pthread_barrier_init(&barWait, NULL, NUM_THREADS);
	struct thread threadArray[NUM_THREADS];
	threadArray[0].inicio = 0;
	threadArray[0].fim = dim/NUM_THREADS;
	threadArray[0].index = 0;

	for (i = 1; i < NUM_THREADS; i++) {
		threadArray[i].inicio = threadArray[i-1].fim;
		threadArray[i].fim = threadArray[i].inicio + (dim/NUM_THREADS);
		threadArray[i].index = i;
	}

	gettimeofday(&inicioLaco, NULL);
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_create(&threadArray[i].thr, NULL, new_generation, &threadArray[i]);
	}
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threadArray[i].thr, NULL);
	}
	pthread_barrier_destroy(&barWait);

	for (i = 0; i < NUM_THREADS; i++){
		total += count[i];
	}

	printf("%d\n", total);

	gettimeofday(&finalLaco, NULL);
	tempoLaco = (int) (1000 * (finalLaco.tv_sec - inicioLaco.tv_sec) + (finalLaco.tv_usec - inicioLaco.tv_usec) / 1000);
	printf("tempo decorrido LACO: %d milisegundos\n", tempoLaco);

	gettimeofday(&final2, NULL);
	tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);
  	printf("tempo decorrido: %d milisegundos\n", tmili);
  return 0;
}
