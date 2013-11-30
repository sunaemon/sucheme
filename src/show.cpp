#include "parser.hpp"
#include "environment.hpp"
#include "gc.hpp"
#include "gc_objects.hpp"
#include <string.h>

namespace sucheme{

/*    char *memory_location(void *ptr)
    {
        bool in_active_buf = 0 <= rpos_active_mem(ptr) && rpos_active_mem(ptr) <= memsize;
        bool in_inactive_buf = 0 <= rpos_inactive_mem(ptr) && rpos_inactive_mem(ptr) <= memsize;

        char *ret = new char[256];

        if(!ptr)
            strcpy(ret, "nullptr");
        if(in_inactive_buf)
            printf("inactive: %d", rpos_inactive_mem(ptr));
        else if(in_active_buf)
            printf("active: %d", rpos_active_mem(ptr));
        else
            strcpy(ret, "out of gc control");
        return ret;
        }*/
    
/*    string showptr(const GCPtr val)
    {
        if(dcast_const<Empty>(val))
            return "Empty\n";
        if(auto b =dcast_const<Bool>(val)) {
            stringstream ost;
            ost << "Bool" << endl;
            ost << " value: " << (b->value ? "#t" : "#f") << endl;
            return ost.str();
        }
        if(auto num =dcast_const<Number>(val)) {
            stringstream ost;
            ost << "Number" << endl;
            ost << " value: " << num->integer << endl;
            return ost.str();
        }
        if(auto symbol =dcast_const<Symbol>(val)) {
            stringstream ost;
            ost << "Symbol" << endl;
            ost << " name : " << extern_symbol(symbol->id) << endl;
            return ost.str();
        }
        if(auto p =dcast_const<Pair>(val)) {
            stringstream ost;
            ost << "Pair" << endl;
            ost << " car  : " << memory_location(p->car) << endl;
            ost << " cdr  : " << memory_location(p->cdr) << endl;
            return ost.str();
        }
        if(auto proc =dcast_const<Procedure>(val)) {
            stringstream ost;
            ost << "Procedure" << endl;
            ost << " addr : 0x" << std::hex << (unsigned long)proc->func << endl;
            return ost.str();
        }
        if(auto lambdaproc =dcast_const<LambdaProcedure>(val)) {
            stringstream ost;
            ost << "LambdaProcedure" << endl;
            ost << " body : " << memory_location(lambdaproc->body) << endl;
            ost << " env  : " << memory_location(lambdaproc->environment) << endl;
            return ost.str();
        }
        if(auto e =dcast_const<Environment>(val)) {
            stringstream ost;
            ost << "Environment" << endl;
            ost << " paren: " << memory_location(e->parent) << endl;
            ost << " e_map: " << memory_location(e->env_map) << endl;
            return ost.str();
        }
        if(auto a =dcast_const<EnvironmentMap>(val)) {
            stringstream ost;
            ost << "EnvironmentMap" << endl;
            ost << " id   : " << extern_symbol(a->id) << endl;
            ost << " val  : " << memory_location(a->val) << endl;
            ost << " l    : " << memory_location(a->l) << endl;
            ost << " g    : " << memory_location(a->g) << endl;
            return ost.str();
        }
        sprintf(ex_buf, "not_implemented: %d", val->tag);
        throw not_implemented(ex_buf);
        }*/

    char *show(const GCPtr val)
    {
        char *buf;
        buf = (char*)malloc(1024);
        
        buf[0] = 0;

        if(dcast_const<Empty>(val))
            sprintf(buf, "()");
        if(auto b =dcast_const<Bool>(val))
            sprintf(buf, b->value ? "#t" : "#f");
        if(auto num =dcast_const<Number>(val))
            sprintf(buf, "%d", num->integer);
        if(auto symbol =dcast_const<Symbol>(val))
            sprintf(buf, "%s", extern_symbol(symbol->id));
        if(auto p =dcast_const<Pair>(val)) {
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
                
                if(!dcast_const<Pair>(next->cdr))
                    break;

                next = (Pair*)(next->cdr);
            }

            strcat(buf, ")");
            
            if(!dcast_const<Empty>(next->cdr)) {
                free(buf);
                sprintf(ex_buf, "improper list");
                throw_jump();
            }
        }
        if(auto proc =dcast_const<Procedure>(val)) {
            sprintf(buf ,"<Procedure 0x%lx >", (unsigned long)proc->func);
        }
        if(auto lambdaproc =dcast_const<LambdaProcedure>(val)) {
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
        if(dcast_const<Environment>(val))
            sprintf(buf, "environment");
        if(dcast_const<EnvironmentMap>(val))
            sprintf(buf, "environmentmap");

        return buf;
    }
}
