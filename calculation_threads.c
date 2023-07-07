#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000000

// Paylaşılan veri yapısı
typedef struct {
    int start;
    int end;
    int* array;
    long sum;
} ThreadData;

// İş parçacığı fonksiyonu
void *threadFunc(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    long threadSum = 0;

    for (int i = data->start; i < data->end; i++) {
        threadSum += data->array[i];
    }

    data->sum = threadSum;

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    int array[ARRAY_SIZE];

    // Diziye rastgele değerler atanması
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100;
    }

    // İş parçacıklarının oluşturulması ve başlatılması
    int chunkSize = ARRAY_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].start = i * chunkSize;
        threadData[i].end = (i + 1) * chunkSize;
        threadData[i].array = array;
        threadData[i].sum = 0;

        pthread_create(&threads[i], NULL, threadFunc, (void *)&threadData[i]);
    }

    // İş parçacıklarının tamamlanmasının beklenmesi
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Toplamın hesaplanması
    long totalSum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        totalSum += threadData[i].sum;
    }

    printf("Toplam: %ld\n", totalSum);

    return 0;
}
