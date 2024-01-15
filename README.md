# Cabinet
**Task:**  
![Screenshot from 2024-01-16 00-39-31](https://github.com/Matoka26/Cabinet/assets/106425405/2f07ea0d-71ec-4297-800a-ae5a83aa1bdb)

* The doctors are represented by a [thread_pool](https://code-vault.net/lesson/j62v2novkv:1609958966824)
* The patients are created in different threads after a random delay and are submited to the thread pool taskQueue to be executed
* Each patient has an id and a creation time, used to calculate the time it took to be reach a free doctor (thread from thread pool)
* Each patient stays at a cabinet for a random time to keep the doctor occupied
