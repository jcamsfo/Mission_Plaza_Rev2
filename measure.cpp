

#include <iostream>
#include <cstdio>
#include <time.h>
#include <ctime>
#include <chrono>
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include "ftd2xx.h"
#include <thread>

#include "measure.h"


using namespace std::chrono;
using namespace std;




void Prog_Durations::Start_Delay_Timer (void) {
    Time_Stamp = high_resolution_clock::now() ;
}

double Prog_Durations::End_Delay_Timer (void) {
    Time_Stamp_Current = high_resolution_clock::now() ;
    time_delay =  (duration_cast<microseconds>( Time_Stamp_Current - Time_Stamp ).count() ) / 1000.;
    loop_Count++;
    if(loop_Count >= 100)  // get past the program startup stuff
    {
        accum_count++;
        if(accum_count >= 100)
        {
            accum_count = 0;
            time_delay_avg = time_delay_accum_temp / 100. ;
            time_delay_accum_temp = time_delay;
        }
        else time_delay_accum_temp += time_delay;
    //    time_delay_avg = time_delay_accum / ((float) (loop_Count - 99)  );
        if(time_delay > time_delay_max)time_delay_max = time_delay;
        if(time_delay < time_delay_min)time_delay_min = time_delay;
        return time_delay;
    }
};


