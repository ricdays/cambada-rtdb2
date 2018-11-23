/*
 * testPutter.cpp
 *
 * Put an item on a fixed frequency;
 *
 *  Created on: Nov 23, 2018
 *      Author: Jan Feitsma (Falcons)
 */

#include <boost/lexical_cast.hpp>
#include <sys/time.h>
#include "../rtdb2/RtDB2.h"

double getTimeStamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    // magic offset ensures microseconds resolution
    return tp.tv_sec - 1388534400 + 1e-6 * tp.tv_usec;
}

#define T int

int main(int argc, char **argv)
{
    // setup
    std::string key = "EXAMPLE_ITEM";
    int agentId = 1;
    std::string storage = "/tmp/rtdb2_storage";
    T value;
    int pid = getpid();
    float frequency = 10;
    float jitter = 0.1;
    
    // arg parsing
    // first argument: frequency to run on, 0 means event-driven using new waitForPut
    // second argument: relative jitter on calc/sleep time
    if (argc > 1)
    {
        frequency = boost::lexical_cast<float>(argv[1]);
    }
    if (argc > 2)
    {
        jitter = boost::lexical_cast<float>(argv[2]);
    }
    
    // initialize RTDB
    auto rtdb = new RtDB2(agentId, storage);

    // create timestamp ZERO if not existing
    int r = 0, life = 0;
    double zero;
    r = rtdb->get("ZERO", &zero, life, agentId);
    if (r != 0)
    {
        zero = getTimeStamp();
        rtdb->put("ZERO", &zero);
    }
        
    // loop
    int counter = 0, sleepMicroSeconds = 0;
    while (true)
    {
        counter++;
        value = pid * 10000 + (counter % 1000);
        rtdb->put(key, &value);
        double tNow = getTimeStamp() - zero; // after PUT
        printf("%.6f %d WRITE %d\n", tNow, pid, value);
        fflush(stdout);
        sleepMicroSeconds = (1e6 / frequency) * (1.0 - (2.0 * rand() / RAND_MAX - 1.0) * jitter);
        usleep(sleepMicroSeconds);
    }

    return 0;
}

