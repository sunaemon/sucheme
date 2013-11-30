#pragma once
#include "exceptions.hpp"
#include "generic_functions.hpp"

namespace sucheme {
    using std::string;

    inline bool white_space(wchar_t c) {
        return c==' ' || c == '\n';
    }

    inline bool delimiter(wchar_t c) {
        return white_space(c) || c=='(' || c==')' || c=='"' || c==';' || c==0;
    }

    inline bool digit(wchar_t c) {
        return '0' <= c && c <= '9';
    }

    inline bool str_in(const wchar_t *str, wchar_t c)
    {
        while(*str)
            if(*str++==c)
                return true;
        return false;
    }

    inline bool letter(wchar_t c)
    {
        return ('a' <= c && c<='z') || ('A' <= c && c<='Z');
    }

    inline bool initial(wchar_t c) {
        const wchar_t *special_initial = L"!$%&*/:<=>?^_;~";
        return str_in(special_initial, c) || letter(c);
    }

    inline bool subsequent(wchar_t c) {
        const wchar_t *special_subsequent = L"+-.@";
        return initial(c) || digit(c) || str_in(special_subsequent, c);
    }

    struct parse_result
    {
        GCPtr val;
        int pos;
    };
    
    struct parse_int_result
    {
        int64_t val;
        int pos;
    };

    parse_int_result parse_int(const char *s, int p);
    parse_result PExpr(const char *s, int32_t p = 0);
    GCPtr parse(const char *s, unsigned int length);
    GCPtr parse(const char *s);
}
