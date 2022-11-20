#include "Sentence.h"
#include "Splitter.h"
#include <cstdint>

Sentence::Sentence () {}

inline uint8_t char2hex (char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0;
}

Sentence::Sentence (const std::string& source) {
    *this = source;
}

const std::string Sentence::getFirstField () const {
    if (fields.empty ()) return "";
    return fields [0];
}

bool Sentence::isProprietary () const {
    auto firstField = getFirstField ();
    return firstField.empty () ? false : (firstField.front () == 'P');
}

const std::string Sentence::talkerID () const {
    auto firstField = getFirstField ();
    if (firstField.empty ()) return "";
    size_t from = firstField.front () == 'P' ? 1 : 0;
    if ((firstField.length () - from) < 2) return "";
    return firstField.substr (from, 2);
}

const std::string Sentence::type () const {
    auto firstField = getFirstField ();
    if (firstField.empty ()) return "";
    size_t from = firstField.front () == 'P' ? 3 : 2;
    if ((firstField.length () - from) < 2) return "";
    return firstField.substr (from, 3);
}

Sentence& Sentence::operator = (const std::string& source) {
    Splitter splitter (source);
    fields.reserve (splitter.size ());
    fields.insert (fields.begin (), splitter.begin (), splitter.end ());
    return *this;
}

const std::string& Sentence::operator [] (const size_t index) const {
    return fields [index];
}

const std::string checkCrc (const std::string source) {
    if (source.length () < 9) return "";
    switch (source.front ()) {
        case '!': case '$': return "";
    }

    auto asteriskPos = source.find ('*');
    if (asteriskPos == std::string::npos || asteriskPos > (source.length () - 3)) return "";

    uint8_t crc = (uint8_t) source [1];
    uint8_t givenCrc = char2hex (source [asteriskPos+1]) * 16 + char2hex (source [asteriskPos+2]);

    for (size_t i = 2; i != asteriskPos; ++ i) {
        crc ^= (uint8_t) source [i];
    }

    return crc == givenCrc ? source.substr (0, asteriskPos) : "";
}
