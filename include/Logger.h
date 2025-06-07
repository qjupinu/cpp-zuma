#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <iostream>

template<typename T>
class Logger {
private:
    std::vector<T> entries;

public:
    void log(const T& value) {
        entries.push_back(value);
    }

    void print() const {
        for (const auto& e : entries) {
            std::cout << "[LOG] " << e << "\n";
        }
    }

    size_t size() const {
        return entries.size();
    }

    void clear() {
        entries.clear();
    }
};

Logger<std::string>& getChainLogger();

#endif
