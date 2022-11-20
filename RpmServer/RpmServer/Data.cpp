#include <fstream>
#include "Data.h"
#include "Splitter.h"

inline bool isWhiteSpace (char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

inline void trimAll (std::string& str) {
    while (!str.empty () && isWhiteSpace (str.front ())) str.erase (str.begin ());
    while (!str.empty () && isWhiteSpace (str.back ())) str.erase (str.end () - 1);
}

Data::Data (const std::string filePath) {
    std::ifstream input (filePath);
    input >> *this;
}

std::string& Data::operator [] (const std::string key) {
    auto pos = values.find (key);
    if (pos == values.end ()) {
        pos = values.emplace (std::pair<std::string, std::string> (key, "")).first;
    }
    return pos->second;
}

std::istream& operator>> (std::istream &source, Data& self) {
    std::string input;
    while (source.good ()) {
        char ch;
        source.get (ch);
        input += ch;
    }
    self.parse (input);
    return source;
}

std::ostream& Data::operator<< (std::ostream &dest) {
    dest << serialize ();
    return dest;
}

void Data::parse (const std::string& source) {
    Splitter lines (source, '\n');
    values.clear ();
    for (auto& line: lines) {
        if (line.empty ()) continue;
        auto colonPos = line.find (':');
        if (colonPos == std::string::npos || colonPos == (line.length() - 1)) continue;
        std::string key = line.substr (0, colonPos);
        std::string value = line.substr (colonPos + 1);
        trimAll (key);
        trimAll (value);
        values.emplace (std::pair<std::string, std::string> (key, value));
    }
}

std::string Data::serialize () const {
    std::string result;
    for (auto kvp: values) {
        result += kvp.first;
        result += ": ";
        result += kvp.second;
        result += '\n';
    }
    return result;
}
