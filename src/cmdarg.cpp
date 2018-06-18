/*
 * =====================================================================================
 *
 *       Filename:  cmdarg.hpp
 *
 *    Description:  command line argument parsing
 *
 *        Created:  18/06/18 17:45:54
 *       Compiler:  gcc
 *
 *         Author:  Jacob Garby (), thyself@jacobgarby.co.uk
 *
 * =====================================================================================
 */

#include "cmdarg.hpp"

namespace cmdarg {
    bool exists(std::string arg, std::vector<std::string> args) {
        return std::find(args.begin(), args.end(), arg) != args.end();
    }

    bool has_value(std::string arg, std::vector<std::string> args) {
        if (!exists(arg, args)) return false;
        if (std::find(args.begin(), args.end(), arg) >= args.end() - 1) return false;
        return true;
    }

    std::string get_value(std::string arg, std::vector<std::string> args) { 
        int pos = std::find(args.begin(), args.end(), arg) - args.begin();
        return args.at(pos+1);
    }
}
