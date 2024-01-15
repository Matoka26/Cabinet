typedef struct Task {
    int patientId;
    struct timespec waitBegin;
    int consultationTime;
} Task;
