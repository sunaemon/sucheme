#include "parser.h"
#include "exceptions.h"
#include <stdio.h>
#include "gc.h"
#include "gc_objects.h"
#include <string.h>
#include <stdint.h>
#include "list.h"

parse_int_result parse_int(const char *s, int p) {
    int64_t ret=0;

    while(digit(s[p])) {
        ret = ret*10 + (s[p++] - '0');
    }
    parse_int_result res = {ret, p};
    return res;
}

inline parse_result make_parse_result(GCPtr val, int pos)
{
    parse_result res = {val,pos};
    return res;
}

parse_result PExpr(const char *s, int32_t p)
{
    while(white_space(s[p])) p++;

    if(s[p] == '#') {
        p++;
        if(s[p] == 't') {
            p++;
            if(delimiter(s[p]))
                return make_parse_result(ucast(alloc_Bool(true)), p);
            else {
                sprintf(ex_buf, "unsupported_grammer");
                longjmp(ex_jbuf,0);
            }
        } else if(s[p] == 'f') {
            p++;
            if(delimiter(s[p]))
                return make_parse_result(ucast(alloc_Bool(false)), p);
            else {
                sprintf(ex_buf, "unsupported_grammer");
                longjmp(ex_jbuf,0);
            }
        } else {
            sprintf(ex_buf, "unsupported_grammer");
            longjmp(ex_jbuf,0);
        }
    }else if(s[p] == '-' || s[p] == '+') {
        if(delimiter(s[p+1])) {
            if(s[p] == '+') {
                return make_parse_result(ucast(make_symbol("+")), p+1);
            }
            else if(s[p] == '-')
                return make_parse_result(ucast(make_symbol("-")), p+1);
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
            return make_parse_result(ucast(alloc_Number(sig?ret:-ret)), p);
        else {
            sprintf(ex_buf, "unsupported_grammer");
            longjmp(ex_jbuf,0);
        }
    }
    
    if(digit(s[p])) {
        int64_t ret=0;
        parse_int_result res = parse_int(s,p);
        ret = res.val;
        p = res.pos;
        if(delimiter(s[p]))
            return make_parse_result(ucast(alloc_Number(ret)), p);
        else {
            sprintf(ex_buf, "unsupported_grammer");
            longjmp(ex_jbuf,0);
        }
    }

    if(s[p] == '(') {
        p++;
        if(s[p] == ')')
            return make_parse_result(ucast(alloc_Empty()), p+1);

        Pair *list = alloc_Pair(NULL, NULL);
        Pair *next=list;

        while(white_space(s[p])) p++;

        parse_result ret = PExpr(s, p);
        GCPtr a = ret.val;
        
        list->car = a;
        p = ret.pos;

        while(white_space(s[p])) p++;

        while(s[p] != ')') {
            while(white_space(s[p])) p++;

            parse_result ret = PExpr(s, p);

            next->cdr = ucast(alloc_Pair(NULL, NULL));

            next = (Pair*)(next->cdr);

            next->car  = ret.val;

            p = ret.pos;

            while(white_space(s[p])) p++;
        }
        next->cdr = ucast(alloc_Empty());
        
        return make_parse_result(ucast(list), p+1);
    }

    if(initial(s[p])) { // <initial> <subsequent>*
        int start = p;
        while(subsequent(s[++p])) {}
        if(delimiter(s[p])) {
            char buf[256];
            memcpy(buf, s+start, p-start);
            buf[p-start] = 0;
            return make_parse_result(ucast(make_symbol(buf)), p);
        } else {
            sprintf(ex_buf, "unsupported_grammer");
            longjmp(ex_jbuf,0);
        }
    }

    if(s[p])
        sprintf(ex_buf, "unsupported_grammer:cannot understand %c at %d in %s", s[p], p, s);
    else {
        sprintf(ex_buf, "unsupported_grammer:unexpected eof at %d in %s", p, s);
        longjmp(ex_jbuf,0);
    }
}

GCPtr parse_len(const char *s, unsigned int length) {
    parse_result ret = PExpr(s,0);
    if(! (ret.pos > 0 && length == (unsigned int)ret.pos)) {
        fprintf(stderr, "input:%d\n parsed:%d", length, ret.pos);
    }
    return ret.val;
}

GCPtr parse(const char *s) {
    return parse_len(s, strlen(s));
}
