#include <pthread.h>
#include <signal.h>
#include "task_struct.h"
#include "cabinet_symbols.h"


unsigned long doctorId[DOCTOR_NUM];

Task taskQueue[256];
int taskCount = 0;

int activePatients;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;




int searchDoctorId(unsigned long id){
    for(int i = 0 ; i < DOCTOR_NUM ; i++)
        if(doctorId[i] == id)
            return i + 1;
}

void executeTask(Task* task) {
    
    struct timespec waitEnd;
    timespec_get(&waitEnd,TIME_UTC);

    double time_spent = (waitEnd.tv_sec - task->waitBegin.tv_sec)
                        + (waitEnd.tv_nsec - task->waitBegin.tv_nsec)/ 1000000000.0;

    // Sleep for the duration of consultation
    sleep(task->consultationTime);

    printf("Im patient %d, Consulted by doctor %d, I've waited %d, Consultation took %d !\n",
            task->patientId, 
            searchDoctorId(pthread_self()),
            (int)time_spent,
            task->consultationTime);

    }

// Take task from queue
void submitTask(Task task) {

    pthread_mutex_lock(&mutexQueue);
    
    taskQueue[taskCount] = task;
    taskCount++;

    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);

}

// Put the threads to work
void* startThread(void* args) {

    while (1 && activePatients) {
        Task task;

        pthread_mutex_lock(&mutexQueue);
        while (taskCount == 0) {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        task = taskQueue[0];
        
        for (int i = 0; i < taskCount - 1; i++) 
            taskQueue[i] = taskQueue[i + 1];
        
        taskCount--;
        
        pthread_mutex_unlock(&mutexQueue);
        executeTask(&task);
        activePatients--;
    }
    exit(EXIT_SUCCESS);
}