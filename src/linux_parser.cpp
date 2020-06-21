#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::atol;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem(){
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() { 
  float memory_utilization = 0.0;
  
  //MemInfo parameters
  string mem_params[] = {"MemTotal:", "MemFree:","MemAvailable:", "Buffers:", "Cached:", 
  "Sreclaimable:", "Shmem:", "SwapTotal:", "SwapFree:"};
  
  //mem_values holds some of the memory values written to mem info file
  //These values are connected to mem_params by index
  double mem_values[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  
  //File read parameters
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  
  //Check if the stream is open, if not, the default value of 0.0 is returned
  if(stream.is_open()){
    //Loop until the end of the filestream
    while(getline(stream, line)){
       std::istringstream linestream(line);
       
       //Variables to hold just the first and second words of each line
       // Example : "MemTotal: 777777777 kB" ; only "MemTotal:" and "77777777" are stored
       std::string mem_keyword;
   	   linestream >> mem_keyword;
      	
       //Find the mem_keyword just read from file in the mem_params array
       //If it is found, find its index and update the corresponding value into mem_values
       auto itr = std::find(std::begin(mem_params), std::end(mem_params), mem_keyword);
      
       if( itr != std::end(mem_params)){
          int i = std::distance(mem_params, itr);
          float tmp = 0.0;
          linestream >> tmp;
          mem_values[i] = tmp;
       }

    }
    
    memory_utilization = (mem_values[0] - mem_values[1]) * 100 / (mem_values[0] * 1024.0);
  }
  
  return memory_utilization; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream fstream(kProcDirectory + kUptimeFilename );
  long uptime = 0.0;
  long idletime = 0.0;
  std::string line;
  
  if( fstream.is_open()){
    getline(fstream, line);
    std::istringstream linestream(line);
    
    linestream >> uptime >> idletime ;
  }
  
  return uptime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {return 0;}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream fstream(kProcDirectory + kStatFilename );
  
  std::string cpu;
  std::string user;
  std::string nice;
  std::string system;
  std::string idle;
  std::string iowait;
  std::string irq;
  std::string softirq;
  std::string steal;
  std::string guest;
  std::string guest_nice;
  
  std::vector<string> cpu_data;
  std::string line;
  
  if(fstream.is_open()){
    std::getline(fstream, line);
  	std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  
  cpu_data.push_back(cpu);
  cpu_data.push_back(user);
  cpu_data.push_back(nice);
  cpu_data.push_back(system);
  cpu_data.push_back(idle);
  cpu_data.push_back(iowait);
  cpu_data.push_back(irq);
  cpu_data.push_back(softirq);
  cpu_data.push_back(steal);
  cpu_data.push_back(guest);
  cpu_data.push_back(guest_nice);
  
  return cpu_data; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return Pids().size(); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  vector<int> pids = Pids();
  std::string line;
  std::string keyword;
  std::string state;
  int running = 0;
  
  for(int i=0; i < pids.size(); i++){
      //Read a specific pid status file
      std::ifstream process_file(kProcDirectory + std::to_string(pids[i]) + "/status");
      if(process_file.is_open()){
        while(std::getline(process_file, line)){
          std::istringstream linestream(line);
          linestream >> keyword;
          if(keyword == "State:"){
            linestream >> state;
            if(state == "R"){
              //check only for processes with state "R"
              running += 1;
            }
          }
        }
      }
  }

  return running;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }