#ifndef CFG_PARSER_HPP
#define CFG_PARSER_HPP

#include <fstream>
#include <map>
#include <string>
#include <variant>

class config {

public:
    using section_type = std::map<std::string, std::variant<std::string, long, double >>;
    class section : public section_type {    };
    std::map<std::string, section> sections;

    config(const std::string &filepath) {
        std::ifstream file(filepath);
        std::string line;

        std::string active_section;
        while (std::getline(file, line)) {
            if (line[0] == '[' && line[line.length() - 1] == ']') {
                active_section = line.substr(1, line.length() - 2);
                continue;
            }

            auto eq_pos = line.find('=');
            if (eq_pos == std::string::npos) {
                continue;
            }

            auto name = line.substr(0, eq_pos);
            auto value = line.substr(eq_pos + 1);
            sections[active_section][name] = to_variant(value);
        }
    }

    section& operator[](const std::string& section_name) {
        auto it = sections.find(section_name);
        if (it != sections.end()) {
            return sections[section_name];
        }
        throw std::invalid_argument("No such section");
    }

private:
    std::variant<std::string, long, double > to_variant(const std::string& str) {
        if(str[0] == '"') { // type string
            return str.substr(1, str.length() - 2);
        }

        if (str.find('.') != std::string::npos) { // type double
            return std::atof(str.c_str());
        }

        return std::atol(str.c_str()); // type int
    }
};

template<typename T>
T& operator<<(T&t,  std::variant<std::string, long, double >&v) {
    t = std::get<T>(v);
    return t;
}

#endif //CFG_PARSER_HPP
