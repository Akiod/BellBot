#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdarg.h>


void BELLOUT(std::string query);
void BELLOUTN(std::string query);

std::vector<std::string> split( const std::string &delimiter, const std::string &str);
bool is_number(const std::string& s);

#endif
