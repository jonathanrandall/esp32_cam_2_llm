#ifndef ALL_DEF_H
#define ALL_DEF_H
QueueHandle_t audioPlayQueue = NULL;

SemaphoreHandle_t Semaphore_Controls = NULL;
SemaphoreHandle_t Semaphore_wait = NULL;

bool waiting_resp = false;

#endif
