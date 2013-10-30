#include "parser.hpp"
#include <sstream>
#include <iostream>
#include <assert.h>

namespace sucheme {

inline bool whitespace(wchar_t c) {
    return c==' ' || c == '\n' || c==0;
}

inline bool delimiter(wchar_t c) {
    return whitespace(c) || c=='(' || c==')' || c=='"' || c==';';
}

inline bool digit(wchar_t c) {
    return '0' <= c && c <= '9';
}

inline bool str_in(const wchar_t *str, wchar_t c)
{
    while(*++str)
        if(*str==c)
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

inline std::tuple<int,int> parse_int(const std::string &s, int p) {
    int64_t ret=0;

    while(digit(s[p])) {
        ret = ret*10 + (s[p++] - '0');
    }
    return std::make_tuple(ret, p);
}

std::tuple<std::unique_ptr<LispVal>, int> PExpr(const std::string &s, int32_t p)
{
    //std::cerr << "PExpr called:" << s << " " <<  s[p] << " " << p << std::endl;

    while(whitespace(s[p])) p++;

    if(s[p] == '-' || s[p] == '+'){
        if(delimiter(s[p+1])) {
            if(s[p] == '+')
                return std::make_tuple(std::unique_ptr<LispVal>(new Symbol("+")), p+1);
            else if(s[p] == '-')
                return std::make_tuple(std::unique_ptr<LispVal>(new Symbol("-")), p+1);
        }

        int64_t ret=0;
        bool sig=true;
    
        if(s[p] =='+') {
            p++;
            sig=true;
        } else if(s[p] =='-') {
            p++;
            sig=false;
        }

        std::tie(ret,p) = parse_int(s,p);
        if(delimiter(s[p]))
            return std::make_tuple(std::unique_ptr<LispVal>(new Number(sig?ret:-ret)), p);
        else
            throw std::exception();
    }
    
    if(digit(s[p])) {
        int64_t ret=0;
        std::tie(ret,p) = parse_int(s,p);
        if(delimiter(s[p]))
            return std::make_tuple(std::unique_ptr<LispVal>(new Number(ret)), p);
        else
            throw std::exception();
    }

    if(s[p] == '(') {
        p++;
        if(s[p] == ')')
            return std::make_tuple(std::unique_ptr<LispVal>(new Empty), p+1);

        Pair *next;

        std::unique_ptr<Pair> list(next = new Pair);

        while(whitespace(s[p])) p++;

        auto ret = PExpr(s, p);

        list->car = std::move(std::get<0>(ret));
        p = std::get<1>(ret);

        while(whitespace(s[p])) p++;

        while(s[p] != ')') {
            while(whitespace(s[p])) p++;

            auto ret = PExpr(s, p);

            Pair *tmp;

            next->cdr = std::unique_ptr<Pair>(tmp = new Pair);
            next = tmp;
            next->car  = std::move(std::get<0>(ret));

            p = std::get<1>(ret);

            while(whitespace(s[p])) p++;
        }
        next->cdr = std::unique_ptr<LispVal>(new Empty);
        
        return std::make_tuple(std::move(list), p+1);
    }

    if(initial(s[p])) { // <initial> <subsequent>*
        int start = p;
        while(subsequent(s[++p])) {}
        if(delimiter(s[p]))
            return std::make_tuple(std::unique_ptr<LispVal>(new Symbol(std::string(&(s[start]), uint32_t(p - start)))), p);
        else
            throw std::exception();
    }
    std::cerr << s[p] << " " << p << std::endl;

    throw std::exception();    
}

    std::unique_ptr<LispVal> parse(const std::string &s) {
        auto ret = PExpr(s);
        assert(s.length() == std::get<1>(ret));
        return move(std::get<0>(ret));
    }


}
