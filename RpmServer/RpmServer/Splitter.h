#ifndef SPLITTER_H
#define SPLITTER_H

#endif // SPLITTER_H

#include <string>
#include <vector>

class Splitter {
    public:
        Splitter ();
        Splitter (const std::string& source, char separator = ',');

        Splitter& operator = (const std::string& source);
        const std::string& operator [] (const size_t index) const;

        void split (const std::string& source, char separator = ',');

        size_t size () const { return fields.size (); }
        std::vector<std::string>::const_iterator begin () const { return fields.cbegin (); }
        std::vector<std::string>::const_iterator end () const { return fields.cend (); }

    private:
        std::vector<std::string> fields;
};
