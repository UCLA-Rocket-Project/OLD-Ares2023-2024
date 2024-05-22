#ifndef STREAM_H
#define STREAM_H

#include <string>

namespace stream
{
    struct ostringstream
    {
        ostringstream() = default;
        ostringstream(std::string arg) : s(std::move(arg)) {}

        const std::string& str() const {
            return s;
        }

        // Arithmetic types: int, float, ...
        template <class T>
        auto operator<<(T arg) -> decltype(std::to_string(arg), *this) {
            s.append(std::to_string(arg));
            return *this;
        }

        // String types: std::string, const char*
        template <class T>
        auto operator<<(const T& arg) -> decltype(std::string().append(arg), *this) {
            s.append(arg);
            return *this;
        }

        // Single char: 'a', '\n', ...
        ostringstream& operator<<(char arg) {
            s.append(1, arg);
            return *this;
        }

    private:
        std::string s;
    };
}

#endif