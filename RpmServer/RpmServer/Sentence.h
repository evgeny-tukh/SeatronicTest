#ifndef SENTENCE_H
#define SENTENCE_H

#include <vector>
#include <string>

class Sentence {
    public:
        Sentence ();
        Sentence (const std::string& source);

        bool isProprietary () const;
        const std::string talkerID () const;
        const std::string type () const;

        Sentence& operator = (const std::string& source);
        const std::string& operator [] (const size_t index) const;

        size_t size () { return fields.size (); }

        bool omitted (const size_t index) const { return fields [index].empty (); }

    private:
        bool proprietary;
        std::string signature;
        std::vector<std::string> fields;

        const std::string checkCrc (const std::string source);
        const std::string getFirstField () const;
};

#endif // SENTENCE_H
