#ifndef PARSING_UTILS_H
#define PARSING_UTILS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

vector<string> split_on_colon(string line);
vector<string> split_on_space(string line);

vector<vector<string>> file_to_tokenized_lines(string absolutePath,
                                               bool colon = false);

#endif