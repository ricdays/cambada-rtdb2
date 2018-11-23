#ifndef CAMBADA_RTDB2SYNCPOINT_H
#define CAMBADA_RTDB2SYNCPOINT_H

#include <semaphore.h>

struct RtDB2SyncPoint {
	//TODO: int process_id; // PID of the "subscriber"
	sem_t semaphore; // semaphore ID
};

#endif //CAMBADA_RTDB2SYNCPOINT_H
