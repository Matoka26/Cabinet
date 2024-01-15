#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "thread_pool.h"


void* createPatient(void* args){
    //Give a random time before creating a patient
        sleep(rand() % GEN_PATIENT_TIME);

        int nextId = *((int*)args);
        Task t = {
            // Give next id 
            .patientId = nextId,
            // Give random consultation time
            .consultationTime = rand() % WAIT_LIMIT 
        };
        timespec_get(&t.waitBegin, TIME_UTC);
        submitTask(t);
    }

int main(int arc, char * argv[]){

    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);
    

    pthread_t doctors[DOCTOR_NUM];
    pthread_t patients[PATIENT_NUM];
    int patientArgs[PATIENT_NUM];
    
    activePatients = PATIENT_NUM;

    // Create Doctors threads
    for (int i = 0; i < DOCTOR_NUM; i++) {
        if (pthread_create(&doctors[i], NULL, &startThread, NULL) != 0) 
            perror("Failed to create doctor thread");
        
        // Memorize the thread for id later
        doctorId[i] = doctors[i];
    }

    srand(time(NULL));

    // Create Patients
    for(int i = 1 ; i <= PATIENT_NUM ; i++){
        
        patientArgs[i-1] = i;
        
        if(pthread_create(&patients[i-1], NULL, createPatient, &patientArgs[i-1]))
            perror("Failed to create patient thread");

    }

    // Join Patients
    for(int i = 0 ; i < PATIENT_NUM ; i++)
        if (pthread_join(patients[i], NULL) != 0)
            perror("Failted to join patient thread");
    


    // Join Doctors
    for (int i = 0; i < DOCTOR_NUM; i++) 
        if (pthread_join(doctors[i], NULL) != 0)
            perror("Failed to join doctor thread");
        

    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);

    return 0;
}