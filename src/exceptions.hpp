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

    EXEPT(unbouded_variable);
    EXEPT(not_implemented);
    EXEPT(improper_list);
    EXEPT(bad_lisp_cast);
    EXEPT(invalid_aplication);
    EXEPT(malformed_lambda);
    EXEPT(malformed_define);
    EXEPT(malformed_cond);
    EXEPT(malformed_letrec);
    EXEPT(malformed_set);
    EXEPT(malformed_quote);
    EXEPT(unsupported_grammer);
#undef EXEPT
}
