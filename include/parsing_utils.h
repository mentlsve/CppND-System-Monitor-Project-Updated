#ifndef PARSING_UTILS_H
#define PARSING_UTILS_H

#include <vector>
#include <string>

using std::vector;
using std::string;

vector<string> tokenize(string line);

vector<vector<string>> file_to_tokenized_lines(string absolutePath);

#endif