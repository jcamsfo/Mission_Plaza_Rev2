#ifndef MEASURE_H
#define MEASURE_H
#pragma once

using namespace std::chrono;


class Prog_Durations {
  public:
    high_resolution_clock::time_point Time_Stamp, Time_Stamp_Current ;
    void Start_Delay_Timer(void);
    double End_Delay_Timer(void);
    double time_delay;
    double time_delay_accum_temp;
    double time_delay_accum;
    double time_delay_max;
    double time_delay_min;
    double time_delay_avg;
    long   loop_Count;
    long   accum_count;
};

#endif /* MEASURE_H */

