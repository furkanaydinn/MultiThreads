#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

// Paylaşılan değişken ve kilitleme mekanizması
int sharedData = 0;
pthread_mutex_t lock;

// İş parçacığı fonksiyonu
void *threadFunc(void *threadID) {
    long tid = (long)threadID;
    int i;

    printf("İş parçacığı %ld başladı.\n", tid);

    // Paylaşılan değişkenin güncellenmesi
    for (i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        sharedData += 1;
        pthread_mutex_unlock(&lock);
    }

    printf("İş parçacığı %ld tamamlandı. Son değer: %d\n", tid, sharedData);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    // Mutex kilitleme mekanizmasının başlatılması
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex oluşturulamadı.\n");
        return 1;
    }

    // İş parçacıklarının oluşturulması ve başlatılması
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, threadFunc, (void *)i) != 0) {
            printf("İş parçacığı oluşturulamadı.\n");
            return 1;
        }
    }

    // İş parçacıklarının tamamlanmasının beklenmesi
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Mutex kilitleme mekanizmasının serbest bırakılması
    pthread_mutex_destroy(&lock);

    printf("Son değer: %d\n", sharedData);

    return 0;
}
