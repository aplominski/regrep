#ifndef REGREP_HPP
#define REGREP_HPP

#include <string>
#include <vector>

struct RegrepOptions {
    std::vector<std::string> patterns;
    std::string replacement;
    bool invert = false;
    bool highlight = false;
    bool do_replace = false;
    bool count_only = false;
    int before = 0;
    int after = 0;
    bool valid = true;
    bool benchmark = false;
    bool use_and = false;
    bool use_or = false;
    bool showline = false;
};

RegrepOptions parse_arguments(int argc, char* argv[]);
void process_input(std::istream& in, std::ostream& out, const RegrepOptions& opts);

#endif // REGREP_HPP