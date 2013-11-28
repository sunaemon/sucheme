#include "parser.hpp"
#include <sstream>
#include <iostream>
#include <memory>
#include "exceptions.hpp"
#include <stdio.h>
#include "gc.hpp"
#include "gc_objects.hpp"

namespace sucheme {
    using std::dynamic_pointer_cast;
    using std::static_pointer_cast;
    using std::cerr;
    using std::endl;

    std::tuple<int,int> parse_int(const std::string &s, int p) {
        int64_t ret=0;

        while(digit(s[p])) {
            ret = ret*10 + (s[p++] - '0');
        }
        return std::make_tuple(ret, p);
    }

    inline parse_result make_parse_result(GCObject *val, int pos)
    {
        return {val, pos};
    }

    parse_result PExpr(const std::string &s, int32_t p)
    {
        while(white_space(s[p])) p++;

        if(s[p] == '#') {
            p++;
            if(s[p] == 't') {
                p++;
                if(delimiter(s[p]))
                    return make_parse_result(alloc<Bool>(true), p);
                else
                    throw unsupported_grammer();
            } else if(s[p] == 'f') {
                p++;
                if(delimiter(s[p]))
                    return make_parse_result(alloc<Bool>(false), p);
                else 
                    throw unsupported_grammer();
            } else
                throw unsupported_grammer();
        }else if(s[p] == '-' || s[p] == '+') {
            if(delimiter(s[p+1])) {
                if(s[p] == '+') {
                    return make_parse_result(alloc<Symbol>("+"), p+1);
                }
                else if(s[p] == '-')
                    return make_parse_result(alloc<Symbol>("-"), p+1);
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
                return make_parse_result(alloc<Number>(sig?ret:-ret), p);
            else
                throw std::exception();
        }
    
        if(digit(s[p])) {
            int64_t ret=0;
            std::tie(ret,p) = parse_int(s,p);
            if(delimiter(s[p]))
                return make_parse_result(alloc<Number>(ret), p);
            else
                throw std::exception();
        }

        if(s[p] == '(') {
            p++;
            if(s[p] == ')')
                return make_parse_result(alloc<Empty>(), p+1);

            auto list = alloc<Pair>();
            Pair *next(list);

            while(white_space(s[p])) p++;

            auto ret = PExpr(s, p);
            auto a = ret.val;
        
            list->car = a;
            p = ret.pos;

            while(white_space(s[p])) p++;

            while(s[p] != ')') {
                while(white_space(s[p])) p++;

                auto ret = PExpr(s, p);

                next->cdr = alloc<Pair>();

                next = (Pair*)(next->cdr);

                next->car  = ret.val;

                p = ret.pos;

                while(white_space(s[p])) p++;
            }
            next->cdr = alloc<Empty>();
        
            return make_parse_result(list, p+1);
        }

        if(initial(s[p])) { // <initial> <subsequent>*
            int start = p;
            while(subsequent(s[++p])) {}
            if(delimiter(s[p]))
                return make_parse_result(alloc<Symbol>(string(&(s[start]), uint32_t(p - start))), p);
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

    GCObject *parse(const string &s) {
        auto ret = PExpr(s,0);
        if(! (ret.pos > 0 && s.length() == (unsigned int)ret.pos)) {
            cerr << "input:" << s.length() << endl;
            cerr << "parsed:" << ret.pos << endl;
        }
        return ret.val;
    }
}
