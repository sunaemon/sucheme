#pragma once
#include <tuple>
#include <string>
#include <memory>
#include "exceptions.hpp"
#include "generic_functions.hpp"
#include "lispval.hpp"

namespace sucheme {
    using std::shared_ptr;
    using std::string;

    //bool whitespace(wchar_t);
    bool delimiter(wchar_t c);
    bool digit(wchar_t c);
    bool str_in(const wchar_t *str, wchar_t c);
    bool letter(wchar_t c);
    bool initial(wchar_t c);
    bool subsequent(wchar_t c);

    struct parse_result
    {
        shared_ptr<LispVal> val;
        int pos;
    };

    std::tuple<int,int> parse_int(const std::string &s, int p);
    parse_result PExpr(const string &s, int32_t p = 0);
    shared_ptr<LispVal> parse(const string &s);
}
