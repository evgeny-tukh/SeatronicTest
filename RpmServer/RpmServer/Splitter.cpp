#include "Splitter.h"

Splitter::Splitter () {}

Splitter::Splitter (const std::string& source, char separator) {
    split (source, separator);
}

Splitter& Splitter::operator = (const std::string& source) {
    split (source);
    return *this;
}

const std::string& Splitter::operator [] (const size_t index) const {
    if (index >= fields.size ()) throw (std::exception ());
    return fields [index];
}

void Splitter::split (const std::string& source, char separator) {
    fields.clear ();
    fields.emplace_back ();
    for (char ch: source) {
        if (ch == separator) {
            fields.emplace_back ();
        } else {
            fields.back ().push_back (ch);
        }
    }
}
