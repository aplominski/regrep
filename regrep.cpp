#include "regrep.h"
#include <sstream>
#include <deque>
#include <vector>
#include <iostream>
#include <numeric>
#include <regex>
RegrepOptions parse_arguments(int argc, char* argv[]) {
    RegrepOptions opts;
    if (argc < 2) {
        opts.valid = false;
        return opts;
    }

    std::string pattern_input = argv[1];
    if (pattern_input.find("&&") != std::string::npos) {
        opts.use_and = true;
        std::stringstream ss(pattern_input);
        std::string token;
        while (std::getline(ss, token, '&')) {
            if (!token.empty() && token != "&") {
                opts.patterns.push_back(token);
            }
        }
    } else if (pattern_input.find("||") != std::string::npos) {
        opts.use_or = true;
        std::stringstream ss(pattern_input);
        std::string token;
        while (std::getline(ss, token, '|')) {
            if (!token.empty() && token != "|") {
                opts.patterns.push_back(token);
            }
        }
    } else {
        opts.patterns.push_back(pattern_input);
    }

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--invert") {
            opts.invert = true;
        } else if (arg == "--highlight") {
            opts.highlight = true;
        } else if (arg == "--replace" && i + 1 < argc) {
            opts.do_replace = true;
            opts.replacement = argv[++i];
        } else if (arg == "--count") {
            opts.count_only = true;
        } else if (arg == "--before" && i + 1 < argc) {
            opts.before = std::stoi(argv[++i]);
        } else if (arg == "--after" && i + 1 < argc) {
            opts.after = std::stoi(argv[++i]);
        } else if (arg == "--benchmark") {
            opts.benchmark = true;
        } else if (arg == "--showline") {
            opts.showline = true;
        }else {
            opts.valid = false;
            return opts;
        }
    }

    return opts;
}

void process_input(std::istream& in, std::ostream& out, const RegrepOptions& opts) {
    std::vector<std::regex> compiled_patterns;
    std::regex combined_pattern;

    if (opts.use_and) {
        for (const auto& pattern : opts.patterns) {
            compiled_patterns.emplace_back(pattern, std::regex::ECMAScript);
        }
    } else {
        std::string combined_pattern_str = std::accumulate(
            opts.patterns.begin(), opts.patterns.end(), std::string{},
            [](const std::string& a, const std::string& b) {
                return a.empty() ? b : a + "|" + b;
            });
        combined_pattern = std::regex(combined_pattern_str, std::regex::ECMAScript);
    }

    std::string line;
    std::deque<std::string> before_buffer;
    int after_remaining = 0;
    int match_count = 0;
    int line_count = 0;

    while (std::getline(in, line)) {
        ++line_count;
        bool match = false;

        if (opts.use_and) {
            match = std::all_of(compiled_patterns.begin(), compiled_patterns.end(),
                                [&](const std::regex& rx) {
                                    return std::regex_search(line, rx);
                                });
        } else {
            match = std::regex_search(line, combined_pattern);
        }

        if (opts.invert) match = !match;

        if (match) {
            ++match_count;

            if (!opts.count_only) {
                for (const auto& prev : before_buffer) {
                    out << prev << '\n';
                }
                before_buffer.clear();

                std::string output_line = line;

                if (opts.do_replace) {
                    if (opts.use_and) {
                        for (const auto& rx : compiled_patterns) {
                            output_line = std::regex_replace(output_line, rx, opts.replacement);
                        }
                    } else {
                        output_line = std::regex_replace(output_line, combined_pattern, opts.replacement);
                    }
                } else if (opts.highlight) {
                    if (opts.use_and) {
                        for (const auto& rx : compiled_patterns) {
                            output_line = std::regex_replace(output_line, rx, "\033[1;31m$&\033[0m");
                        }
                    } else {
                        output_line = std::regex_replace(output_line, combined_pattern, "\033[1;31m$&\033[0m");
                    }
                }

                if (opts.showline) {
                    out << "[" << line_count << "]: ";
                }
                out << output_line << '\n';

                after_remaining = opts.after;
            }
        } else if (after_remaining > 0) {
            out << line << '\n';
            --after_remaining;
        } else if (opts.before > 0) {
            if ((int)before_buffer.size() >= opts.before) {
                before_buffer.pop_front();
            }
            before_buffer.push_back(line);
        }
    }

    if (opts.count_only) {
        out << match_count << "\n";
    }
}
