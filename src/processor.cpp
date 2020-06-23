
#include <iostream>
#include "processor.h"


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  float agg_utilization = 0.0;
  
  //These explicit assignments are only for clarity
  //Typically, I will use current_utilization/prev_utilization directly
  //in calculations
  
  float user = current_utilization[0];
  float nice = current_utilization[1];
  float system = current_utilization[2];
  float idle = current_utilization[3];
  float iowait = current_utilization[4];
  float irq = current_utilization[5];
  float softirq = current_utilization[6];
  float steal = current_utilization[7];
  float guest = current_utilization[8];
  float guest_nice = current_utilization[9];
  
  float usertime = user - guest;
  float nicetime = nice - guest_nice;
  float idlealltime = idle + iowait;
  float systemalltime = system + irq + softirq;
  float virtalltime = guest + guest_nice;
  float nonidle = user + nice + system + irq + softirq + steal;
  float totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;
  
  user = prev_utilization[0];
  nice = prev_utilization[1];
  system = prev_utilization[2];
  idle = prev_utilization[3];
  iowait = prev_utilization[4];
  irq = prev_utilization[5];
  softirq = prev_utilization[6];
  steal = prev_utilization[7];
  guest = prev_utilization[8];
  guest_nice = prev_utilization[9];
  
  float prev_usertime = user - guest;
  float prev_nicetime = nice - guest_nice;
  float prev_idlealltime = idle + iowait;
  float prev_systemalltime = system + irq + softirq;
  float prev_virtalltime = guest + guest_nice;
  float prev_nonidle = user + nice + system + irq + softirq + steal;
  float prev_totaltime = prev_usertime + prev_nicetime + prev_systemalltime + prev_idlealltime + steal + prev_virtalltime;
  
  float ftotal = totaltime - prev_totaltime;
  float fidle = idlealltime - prev_idlealltime;
  
  agg_utilization = (ftotal - fidle)/ftotal;
  
  prev_utilization = current_utilization;
  
  return agg_utilization; 
}