#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long duration) { 
    
    // calculate numeric values
    int hours = duration / 3600;
    duration -= hours * 3600;
    int minutes = duration / 60;
    duration -= minutes * 60;
    int seconds =  duration;

    // convert to string
    string hh = std::to_string(hours);
    string mm = std::to_string(minutes);
    string ss = std::to_string(seconds);

    // pad with zeros if necessary
    hh = (hh.length() == 1) ? "0" + hh : hh;
    mm = (mm.length() == 1) ? "0" + mm : mm;
    ss = (ss.length() == 1) ? "0" + ss : ss;

    return hh + ":" + mm + ":" + ss;
}