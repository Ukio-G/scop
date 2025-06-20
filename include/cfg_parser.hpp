#ifndef CFG_PARSER_HPP
#define CFG_PARSER_HPP

#include <fstream>
#include <map>
#include <ranges>
#include <string>
#include <variant>

class config {

public:
  using section_type = std::map<
      std::string,
      std::variant<std::string, long, double, bool>>;
  class section : public section_type {};
  std::map<std::string, section> sections;

  config(const std::string &filepath) {
    std::ifstream file(filepath);
    std::string line;

    std::string active_section;

    auto trim_ws = [] (std::string& str) {
      auto not_space = [](unsigned char c) { return !std::isspace(c); };
      str.erase(std::ranges::find_if(str | std::views::reverse, not_space).base(), str.end());
      str.erase(str.begin(), std::ranges::find_if(str, not_space));
    };

    while (std::getline(file, line)) {
      trim_ws(line);
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

  section &operator[](const std::string &section_name) {
    auto it = sections.find(section_name);
    if (it != sections.end()) {
      return sections[section_name];
    }
    throw std::invalid_argument("No such section");
  }

private:
  section_type::value_type::second_type to_variant(const std::string &str) {
    if (str == "false" || str == "true") { // type bool
      return str == "true";
    }

    if (str[0] == '"') { // type string
      return str.substr(1, str.length() - 2);
    }

    if (str.find('.') != std::string::npos) { // type double
      return std::atof(str.c_str());
    }

    return std::atol(str.c_str()); // type int
  }
};

template <typename T>
T &operator<<(T &t, std::variant<std::string, long, double, bool> &v) {
  t = std::get<T>(v);
  return t;
}

#endif // CFG_PARSER_HPP
