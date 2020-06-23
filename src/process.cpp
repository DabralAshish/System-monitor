#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using namespace LinuxParser;

// Return this process's ID
int Process::Pid() { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
  
  long Hertz = sysconf(_SC_CLK_TCK);
  
  //Use system up time and not process time
  long uptime = LinuxParser::UpTime();
  
  std::vector<long> process_data = LinuxParser::CpuUtilization(pid);
  
  long total_time = 0;
  
  //For total time, take sum of all but the last (starttime) value.
  for(int j = 0; j < process_data.size()-1; j++){
  	total_time += process_data[j];
  }
  
  //uptime - (starttime / Hertz);
  float seconds = uptime - (process_data[4] / Hertz);
  
  float cpu_usage = (( total_time / Hertz)) / seconds;
  
  return cpu_usage; 
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }