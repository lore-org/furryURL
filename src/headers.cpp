#include <cctype>
#include <furryurl/headers.h>

#include <optional>

using namespace furryurl;

void Headers::append(std::string name, std::string value) {
    Headers::_formatKey(name);

    if (_headers.contains(name)) _headers[name] += ", " + value;
    else _headers[name] = value;
}

void Headers::remove(std::string name) {
    Headers::_formatKey(name);

    if (_headers.contains(name)) _headers.erase(name);
}

std::unordered_map<std::string, std::string> Headers::headers() {
    return _headers;
}

std::optional<std::string> Headers::get(std::string name) {
    Headers::_formatKey(name);
    
    if (_headers.contains(name)) return _headers[name];
    else return { };
}

void Headers::set(std::string name, std::string value) {
    Headers::_formatKey(name);
    
    _headers[name] = value;
}

bool Headers::has(std::string name) {
    Headers::_formatKey(name);
    
    return _headers.contains(name);
}

void Headers::_formatKey(std::string& key) {
    char lastChar = 0;
    for (char& c : key) {
        if (std::isalpha(c)) {
            if (lastChar == 0 || !std::isalpha(lastChar))
                c = std::toupper(c);
            else
                c = std::tolower(c);
        }

        lastChar = c;
    }
}