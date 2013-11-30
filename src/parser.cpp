#include "parser.hpp"
#include "exceptions.hpp"
#include <stdio.h>
#include "gc.hpp"
#include "gc_objects.hpp"
#include <string.h>

namespace sucheme {
    parse_int_result parse_int(const char *s, int p) {
        int64_t ret=0;

        while(digit(s[p])) {
            ret = ret*10 + (s[p++] - '0');
        }
        return {ret, p};
    }

    inline parse_result make_parse_result(GCPtr val, int pos)
    {
        return {val, pos};
    }

    parse_result PExpr(const char *s, int32_t p)
    {
        while(white_space(s[p])) p++;

        if(s[p] == '#') {
            p++;
            if(s[p] == 't') {
                p++;
                if(delimiter(s[p]))
                    return make_parse_result(ucast(alloc<Bool>(true)), p);
                else
                    throw unsupported_grammer();
            } else if(s[p] == 'f') {
                p++;
                if(delimiter(s[p]))
                    return make_parse_result(ucast(alloc<Bool>(false)), p);
                else 
                    throw unsupported_grammer();
            } else
                throw unsupported_grammer();
        }else if(s[p] == '-' || s[p] == '+') {
            if(delimiter(s[p+1])) {
                if(s[p] == '+') {
                    return make_parse_result(ucast(alloc<Symbol>("+")), p+1);
                }
                else if(s[p] == '-')
                    return make_parse_result(ucast(alloc<Symbol>("-")), p+1);
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

            parse_int_result res = parse_int(s,p);
            ret = res.val;
            p = res.pos;
            if(delimiter(s[p]))
                return make_parse_result(ucast(alloc<Number>(sig?ret:-ret)), p);
            else
                throw std::exception();
        }
    
        if(digit(s[p])) {
            int64_t ret=0;
            parse_int_result res = parse_int(s,p);
            ret = res.val;
            p = res.pos;
            if(delimiter(s[p]))
                return make_parse_result(ucast(alloc<Number>(ret)), p);
            else
                throw std::exception();
        }

        if(s[p] == '(') {
            p++;
            if(s[p] == ')')
                return make_parse_result(ucast(alloc<Empty>()), p+1);

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

                next->cdr = ucast(alloc<Pair>());

                next = (Pair*)(next->cdr);

                next->car  = ret.val;

                p = ret.pos;

                while(white_space(s[p])) p++;
            }
            next->cdr = ucast(alloc<Empty>());
        
            return make_parse_result(ucast(list), p+1);
        }

        if(initial(s[p])) { // <initial> <subsequent>*
            int start = p;
            while(subsequent(s[++p])) {}
            if(delimiter(s[p]))
                return make_parse_result(ucast(alloc<Symbol>(string(&(s[start]), uint32_t(p - start)))), p);
            else
                throw std::exception();
        }

        if(s[p])
            sprintf(ex_buf, "unsupported_grammer:cannot understand %c at %d in %s", s[p], p, s);
        else
            sprintf(ex_buf, "unsupported_grammer:unexpected eof at %d in %s", p, s);
        throw unsupported_grammer(ex_buf);
    }

    GCPtr parse(const char *s, unsigned int length) {
        parse_result ret = PExpr(s,0);
        if(! (ret.pos > 0 && length == (unsigned int)ret.pos)) {
            fprintf(stderr, "input:%d\n parsed:%d", length, ret.pos);
        }
        return ret.val;
    }

    GCPtr parse(const char *s) {
        return parse(s, strlen(s));
    }
}
