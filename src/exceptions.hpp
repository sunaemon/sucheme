#pragma once
#include <exception>
#include <string>

namespace sucheme{
    using std::exception;

#define EXEPT(T) \
    class T : public exception {\
    std::string str;\
    public:\
        T(const std::string str = #T) : str(str) {}\
        const char *what() const noexcept override {return str.c_str(); }\
    };

    EXEPT(not_implemented);
    EXEPT(improper_list);
    EXEPT(bad_lisp_cast);
    EXEPT(invaild_aplication);
    EXEPT(malformed_lambda);
    EXEPT(malformed_define);
#undef EXEPT
}
