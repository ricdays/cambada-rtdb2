/*
 * testGetter.cpp
 *
 * Monitor an item, report immediately new values.
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
    bool waitEnabled = false;
    char* tmp;
    tmp = getenv("WAIT_ENABLED");
    if (tmp != 0) 
    {
        if (strcmp(tmp, "1"))
        {
            waitEnabled = true;
        }
    }
    float frequency = 10;
    float jitter = 0.1;
    float dutyCycle = 0.3;
    
    // arg parsing
    // first argument: frequency to run on, 0 means event-driven using new waitForPut
    // second argument: relative jitter on calc/sleep time
    // third argument: duty cycle (should be smaller than 1): relative calculation time w.r.t. frequency
    if (argc > 1)
    {
        frequency = boost::lexical_cast<float>(argv[1]);
    }
    if (argc > 2)
    {
        jitter = boost::lexical_cast<float>(argv[2]);
    }
    if (argc > 3)
    {
        dutyCycle = boost::lexical_cast<float>(argv[3]);
    }
    
    // initialize RTDB
    auto rtdb = new RtDB2(agentId, storage);

    // get timestamp ZERO
    int r = 0, life = 0;
    double zero;
    r = rtdb->get("ZERO", &zero, life, agentId);

    // loop
    int sleepMicroSeconds = 0;
    while (true)
    {
        // note: there is a very small chance that there is a new value arrived between wait and get
        double tNow = getTimeStamp() - zero;
        r = rtdb->get(key, &value, life, agentId);
        if (r != 0) break;
        
        // report
        printf("%.6f %d READ %d %d\n", tNow, pid, value, life);
        fflush(stdout);
        
        // simulate a calculation
        sleepMicroSeconds = dutyCycle * (1e6 / frequency) * (1.0 - (2.0 * rand() / RAND_MAX - 1.0) * jitter);
        usleep(sleepMicroSeconds);
        
        // sleep for a random period?
        if (waitEnabled)
        {
            //rtdb->waitForPut(key, agentId);
        }
        else
        {
            // emulate regular asynchronous loop by sleeping for the remainder of the cycle
            sleepMicroSeconds = (1e6 / frequency) - sleepMicroSeconds;
            usleep(sleepMicroSeconds);
        }
    }

    return 0;
}

