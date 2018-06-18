/*
 * =====================================================================================
 *
 *       Filename:  cmdarg.cpp
 *
 *    Description:  command line arg parsing
 *
 *        Created:  18/06/18 17:46:22
 *       Compiler:  gcc
 *
 *         Author:  Jacob Garby (), thyself@jacobgarby.co.uk
 *
 * =====================================================================================
 */

#include <vector>
#include <string>
#include <algorithm>

#ifndef CMDARG_HPP
#define CMDARG_HPP

namespace cmdarg {
    bool exists(std::string arg, std::vector<std::string> args);
    bool has_value(std::string arg, std::vector<std::string> args);
    std::string get_value(std::string arg, std::vector<std::string> args);
}

#endif
