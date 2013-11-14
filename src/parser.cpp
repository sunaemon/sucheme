#include "parser.hpp"
#include <sstream>
#include <iostream>
#include <memory>
#include "exceptions.hpp"
#include <stdio.h>

namespace sucheme {
    using std::dynamic_pointer_cast;
    using std::static_pointer_cast;
    using std::cerr;
    using std::endl;

    bool whitespace(wchar_t c) {
        return c==' ' || c == '\n';
    }

    bool delimiter(wchar_t c) {
        return whitespace(c) || c=='(' || c==')' || c=='"' || c==';' || c==0;
    }

    bool digit(wchar_t c) {
        return '0' <= c && c <= '9';
    }

    bool str_in(const wchar_t *str, wchar_t c)
    {
        while(*str)
            if(*str++==c)
                return true;
        return false;
    }

    bool letter(wchar_t c)
    {
        return ('a' <= c && c<='z') || ('A' <= c && c<='Z');
    }

    bool initial(wchar_t c) {
        const wchar_t *special_initial = L"!$%&*/:<=>?^_;~";
        return str_in(special_initial, c) || letter(c);
    }

     bool subsequent(wchar_t c) {
        const wchar_t *special_subsequent = L"+-.@";
        return initial(c) || digit(c) || str_in(special_subsequent, c);
    }

    std::tuple<int,int> parse_int(const std::string &s, int p) {
        int64_t ret=0;

        while(digit(s[p])) {
            ret = ret*10 + (s[p++] - '0');
        }
        return std::make_tuple(ret, p);
    }

    std::tuple<std::shared_ptr<LispVal>, int> PExpr(const std::string &s, int32_t p)
    {
        while(whitespace(s[p])) p++;

        if(s[p] == '#') {
            p++;
            if(s[p] == 't') {
                p++;
                if(delimiter(s[p]))
                    return make_tuple(make_shared<Bool>(true), p);
                else
                    throw 1;
            } else if(s[p] == 'f') {
                p++;
                if(delimiter(s[p]))
                    return make_tuple(make_shared<Bool>(false), p);
                else
                    throw "a";
            } else
                throw exception();
        }else if(s[p] == '-' || s[p] == '+') {
            if(delimiter(s[p+1])) {
                if(s[p] == '+') {
                    return make_tuple(make_shared<Symbol>("+"), p+1);
                }
                else if(s[p] == '-')
                    return make_tuple(make_shared<Symbol>("-"), p+1);
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
                return make_tuple(make_shared<Number>(sig?ret:-ret), p);
            else
                throw std::exception();
        }
    
        if(digit(s[p])) {
            int64_t ret=0;
            std::tie(ret,p) = parse_int(s,p);
            if(delimiter(s[p]))
                return make_tuple(make_shared<Number>(ret), p);
            else
                throw std::exception();
        }

        if(s[p] == '(') {
            p++;
            if(s[p] == ')')
                return std::make_tuple(make_shared<Empty>(), p+1);

            auto list = make_shared<Pair>();
            shared_ptr<Pair> next(list);

            while(whitespace(s[p])) p++;

            auto ret = PExpr(s, p);
            auto a = move(std::get<0>(ret));
        
//        std::cerr << (bool)a << " " << a.use_count() << " " << a.get() << std::endl;

            list->car = a;
            p = std::get<1>(ret);

            while(whitespace(s[p])) p++;

            while(s[p] != ')') {
                while(whitespace(s[p])) p++;

                auto ret = PExpr(s, p);

                next->cdr = make_shared<Pair>();

                next = static_pointer_cast<Pair>(next->cdr);

                next->car  = move(std::get<0>(ret));

                p = std::get<1>(ret);

                while(whitespace(s[p])) p++;
            }
            next->cdr = make_shared<Empty>();
        
            return std::make_tuple(move(list), p+1);
        }

        if(initial(s[p])) { // <initial> <subsequent>*
            int start = p;
            while(subsequent(s[++p])) {}
            if(delimiter(s[p]))
                return make_tuple(make_shared<Symbol>(string(&(s[start]), uint32_t(p - start))), p);
            else
                throw std::exception();
        }

        {
            char buf[256];
            if(s[p])
                sprintf(buf, "unsupported_grammer:cannot understand %c at %d in %s", s[p], p, s.c_str());
            else
                sprintf(buf, "unsupported_grammer:unexpected eof at %d in %s", p, s.c_str());
            throw unsupported_grammer(buf);
        }
    }

    shared_ptr<LispVal> parse(const string &s) {
        auto ret = PExpr(s);
        if(! (get<1>(ret) > 0 && s.length() == (unsigned int)get<1>(ret))) {
            cerr << "input:" << s.length() << endl;
            cerr << "parsed:" << get<1>(ret) << endl;
        }
        return move(get<0>(ret));
    }
}
