#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "ncurses_display.h"
#include "system.h"

int main(int argc, char* argv[]) {
  
  // Take an integer number as an argument to limit
  // no of processes displayed.
  // In case of multiple arguments, an exception is thrown.
  // The default value for noproc is 10 processes.
  // If noproc is larger than actual no. of processes, the
  //processes are capped at max no. from the system class.
  int noproc;
  
  if(argc == 1){
  	noproc = 10;
  }
  else{
    if(argc == 2){
        std::istringstream linestream(argv[1]);
        if( linestream >> noproc){
          if(noproc == 0){ noproc = 10; }
        }
        else{
            noproc = 10;
        }
    }
    else{
       throw std::invalid_argument("Only one integer argument allowed. \n");
    }
    
  }
  
    System system;
    NCursesDisplay::Display(system, noproc);
 
}