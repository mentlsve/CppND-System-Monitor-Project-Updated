#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>
#include <iostream>

using namespace std;

vector<string> tokenize(string line){
  std::istringstream linestream(line);
  vector<string> results(std::istream_iterator<std::string>{linestream}, std::istream_iterator<std::string>());
  return results;
}

template<typename T> void printVector(vector<T> elems){   
  for (int i = 0; i < elems.size(); i++) {
    cout << "Element at index " << i << ": " << elems.at(i) << std::endl;
  }
}

vector<vector<string>> file_to_tokenized_lines(string absolutePath) {
  vector<vector<string>> lines;
  std::ifstream stream(absolutePath);
  for(string line; std::getline(stream, line);) {
    lines.push_back(tokenize(line));
  }
  return lines;
}



int main(){
  auto lines = file_to_tokenized_lines("/proc/meminfo");
  cout << lines.at(0).at(0) << lines.at(0).at(1);
  cout << lines.at(1).at(0) << lines.at(1).at(1);
}