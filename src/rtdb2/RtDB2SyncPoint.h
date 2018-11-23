#ifndef CAMBADA_RTDB2SYNCPOINT_H
#define CAMBADA_RTDB2SYNCPOINT_H

#include <sys/sem.h>

struct RtDB2SyncPoint {
	//TODO: int process_id; // PID of the "subscriber"
	int sem_ID; // semaphore ID
};

#endif //CAMBADA_RTDB2SYNCPOINT_H
