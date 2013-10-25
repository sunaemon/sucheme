#include "parser.hpp"
#include <sstream>
#include <iostream>

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

std::tuple<unique_ptr<LispVal>, int> PExpr(const std::string &s, int32_t p)
{
    //std::cerr << "PExpr called:" << s << " " <<  s[p] << " " << p << std::endl;

    while(whitespace(s[p])) p++;

    if(s[p] == '-' || s[p] == '+'){
        if(delimiter(s[p+1])) {
            if(s[p] == '+')
                return std::make_tuple(new Symbol("+"), p+1);
            else if(s[p] == '-')
                return std::make_tuple(new Symbol("-"), p+1);
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
            return std::make_tuple(new Number(sig?ret:-ret), p);
        else
            throw std::exception();  
    }
    
    if(digit(s[p])) {
        int64_t ret=0;
        std::tie(ret,p) = parse_int(s,p);
        if(delimiter(s[p]))
            return std::make_tuple(new Number(ret), p);
        else
            throw std::exception();
    }

    if(s[p] == '(') {
        p++;
        std::list<unique_ptr<LispVal>> value;
        while(s[p] != ')') {
            while(whitespace(s[p])) p++;p

            auto ret = PExpr(s, p);
            p = std::get<1>(ret);
            value.push_back(std::move(std::get<0>(ret)));

            while(whitespace(s[p])) p++;
        }
        return std::make_tuple(List(value), p+1);
    }

    if(initial(s[p])) { // <initial> <subsequent>*
        int start = p;
        while(subsequent(s[++p])) {}
        if(delimiter(s[p]))
            return std::make_tuple(new Symbol(std::string(&(s[start]), uint32_t(p - start))), p);
        else
            throw std::exception();
    }
    std::cerr << s[p] << " " << p << std::endl;

    throw std::exception();    
}

}
