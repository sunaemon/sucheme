#include "parser.h"
#include "environment.h"
#include "gc.h"
#include "gc_objects.h"
#include <string.h>

char *show(const GCPtr val)
{
    const Symbol *s;
    const Number *n;
    const Bool *b;
    const Procedure *proc;
    const LambdaProcedure *lambdaproc;
    const Pair *p;

    char *buf;
    buf = (char*)malloc(1024);
        
    buf[0] = 0;

    if(dcast_const_Empty(val))
        sprintf(buf, "()");
    if((b =dcast_const_Bool(val)))
        sprintf(buf, b->value ? "#t" : "#f");
    if((n =dcast_const_Number(val)))
        sprintf(buf, "%d", n->integer);
    if((s =dcast_const_Symbol(val)))
        sprintf(buf, "%s", extern_symbol(s->id));
    if((p = dcast_const_Pair(val))) {
        const Pair *next=p;

        strcat(buf, "(");
            
        bool flag = false;

        for(;;){
            if(flag)
                strcat(buf, " ");
            else
                flag = true;

            char *str = show(next->car);
            strcat(buf, str);
            free(str);
                
            if(!dcast_const_Pair(next->cdr))
                break;

            next = (Pair*)(next->cdr);
        }

        strcat(buf, ")");
            
        if(!dcast_const_Empty(next->cdr)) {
            free(buf);
            sprintf(ex_buf, "improper list");
            throw_jump();
        }
    }
    if((proc =dcast_const_Procedure(val))) {
        sprintf(buf ,"<Procedure 0x%lx >", (unsigned long)proc->func);
    }
    if((lambdaproc =dcast_const_LambdaProcedure(val))) {
        strcat(buf, "<Lambda Procedure (lambda (");
        for(int i=0; i< lambdaproc->argc; i++) {
            strcat(buf, extern_symbol(lambdaproc->argv[i]));
            strcat(buf, " ");
        }
            
        strcat(buf, ") ");
        char *str = show(ucast(lambdaproc->body));
        strcat(buf, str);
        free(str);
        strcat(buf,")>");
    }
    if(dcast_const_Environment(val))
        sprintf(buf, "environment");
    if(dcast_const_EnvironmentMap(val))
        sprintf(buf, "environmentmap");

    return buf;
}
