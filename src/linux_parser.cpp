#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include "linux_parser.h"
#include "parsing_utils.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  // documentation from http://man7.org/linux/man-pages/man5/proc.5.html
  // mem_available is not used https://superuser.com/questions/980820/what-is-the-difference-between-memfree-and-memavailable-in-proc-meminfo
  long int mem_total; // Total usable RAM (i.e., physical RAM minus a few reserved bits and the kernel binary code).
  long int mem_free; // The sum of LowFree+HighFree. (User space == high memory, Kernel space == low memory)
  long int buffers; // Relatively temporary storage for raw disk blocks that shouldn't get tremendously large (20MB or so).
  long int cached; // In-memory cache for files read from the disk (the page cache). Doesn't include SwapCached.

  vector<vector<string>> lines = file_to_tokenized_lines(kProcDirectory + kMeminfoFilename);

  const int identifier_idx = 0;
  const int value_idx = 1;

  const int mem_total_line_number = 0;
  assert(lines.at(mem_total_line_number).at(identifier_idx).find("MemTotal") == 0);
    mem_total = std::stoi(lines.at(mem_total_line_number).at(value_idx));

  const int mem_free_line_number = 1;
  assert(lines.at(mem_free_line_number).at(identifier_idx).find("MemFree") == 0);
    mem_free = std::stoi(lines.at(mem_free_line_number).at(value_idx));

  const int buffers_line_number = 3;
    assert(lines.at(buffers_line_number).at(identifier_idx).find("Buffers") == 0);
    buffers = std::stoi(lines.at(buffers_line_number).at(value_idx));

  const int cached_line_number = 4;
    assert(lines.at(cached_line_number).at(identifier_idx).find("Cached") == 0);
    cached = std::stoi(lines.at(cached_line_number).at(value_idx));

  long int total_used_memory = mem_total - mem_free + buffers + cached; // actually memory for chaches is missing here

  return static_cast<float>(total_used_memory) / mem_total;
}


// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

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


// utility functions for file parsing
vector<string> tokenize(string line){
  std::istringstream linestream(line);
  vector<string> results(std::istream_iterator<std::string>{linestream}, std::istream_iterator<std::string>());
  return results;
}

vector<vector<string>> file_to_tokenized_lines(string absolutePath) {
  vector<vector<string>> lines;
  std::ifstream stream(absolutePath);
  for(string line; std::getline(stream, line);) {
    lines.push_back(tokenize(line));
  }
  return lines;
}
