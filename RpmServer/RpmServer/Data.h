#ifndef DATA_H
#define DATA_H

#include <string>
#include <unordered_map>
#include <iostream>

class Data {
    public:
        Data () {}
        Data (const std::string filePath);

        std::string& operator [] (const std::string key);
        friend std::istream& operator>> (std::istream &, Data&);
        std::ostream &operator<< (std::ostream &dest);

        void parse (const std::string& source);
        std::string serialize () const;

    private:
        std::unordered_map<std::string, std::string> values;
};

#endif // DATA_H
