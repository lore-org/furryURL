#pragma once

#include <string>
#include <unordered_map>
#include <optional>

namespace furryurl {

struct Headers {
public:
    void append(std::string name, std::string value);
    void remove(std::string name);
    std::unordered_map<std::string, std::string> headers();
    std::optional<std::string> get(std::string name);
    void set(std::string name, std::string value);
    bool has(std::string name);

    auto begin() { return _headers.begin(); };
    auto end() { return _headers.end(); };

protected:
    std::unordered_map<std::string, std::string> _headers;

    static void _formatKey(std::string& key);
};

}