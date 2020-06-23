#include <string>

#include "format.h"

using std::string;
using std::to_string;

//Format a long time to Days:Hours:Minutes
string Format::ElapsedTime(long time) { 
  
  std::string uptime;
  
  long min_secs = 60; //seconds in a minute
  long hour_secs = 60 * min_secs; //seconds in an hour
  long day_secs = 24 * hour_secs; //seconds in a day
  
  long days = 0;
  long hours = 0;
  long minutes = 0;
  long seconds = 0;
  
  days = time/day_secs;
  
  hours = (time - days * day_secs) / hour_secs;
  if(hours < 0){hours = 0;}
  
  minutes = (time - days * day_secs - hours * hour_secs) / min_secs;
  if(minutes < 0){ minutes = 0;}
  
  seconds = (time - days * day_secs - hours * hour_secs - minutes * min_secs);
  
  //Get (at least) a two character formatted string for each value : hours, minutes, seconds
  //Example : 1 becomes 01 but 22 and 222 remain the same.
  uptime = Format::TimeLengthFormat(hours) + ":" + Format::TimeLengthFormat(minutes) + ":" + Format::TimeLengthFormat(seconds);
  
  return uptime; 
}

// Function to format any value to at least two characters
string Format::TimeLengthFormat(long value){
  std::string formatted_val;
  if(std::to_string(value).length() < 2){
     formatted_val = "0" + to_string(value);
  }
  else{
     formatted_val = std::to_string(value);
  }
  return formatted_val;
}