#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using namespace LinuxParser;
using namespace Format;

// Return the system's CPU
Processor& System::Cpu() {

  auto cpu_data = LinuxParser::CpuUtilization();
  
  for(int i=0; i < cpu_data.size(); i++){
    cpu_.current_utilization[i] = stof(cpu_data[i]) ;
  }
  
  return cpu_; 
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  
  vector<Process> processes;
  
  auto pids = LinuxParser::Pids();
  
  for(int i = 0; i < pids.size(); i++){
  	Process process;
    process.pid = pids[i];
    process.uid = LinuxParser::Uid(pids[i]);
    process.username = LinuxParser::User(pids[i]);
    process.uptime = LinuxParser::UpTime(pids[i]);
    process.ram = LinuxParser::Ram(pids[i]);
    processes_.push_back(process);
  }
  
  return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime();}