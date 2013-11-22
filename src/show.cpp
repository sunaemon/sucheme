#include "parser.hpp"
#include <sstream>
#include <iostream>
#include "environment.hpp"
#include "gc.hpp"

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::endl;

    inline string memory_location(void *ptr)
    {
        auto in_active_buf = 0 <= rpos_active_mem(ptr) && rpos_active_mem(ptr) <= memsize;
        auto in_inactive_buf = 0 <= rpos_inactive_mem(ptr) && rpos_inactive_mem(ptr) <= memsize;

        if(!ptr)
            return "nullptr";
        if(in_inactive_buf)
            return "inactive: " + to_string(rpos_inactive_mem(ptr));
        else if(in_active_buf)
            return "active  : " + to_string(rpos_active_mem(ptr));
        else
            return "out of gc control";
    }
    
    string showptr(const GCObject *val)
    {
        if(dcast<const Empty>(val))
            return "Empty\n";
        if(auto b = dcast<const Bool>(val)) {
            stringstream ost;
            ost << "Bool" << endl;
            ost << " value: " << (b->value ? "#t" : "#f") << endl;
            return ost.str();
        }
        if(auto num = dcast<const Number>(val)) {
            stringstream ost;
            ost << "Number" << endl;
            ost << " value: " << num->integer << endl;
            return ost.str();
        }
        if(auto symbol = dcast<const Symbol>(val)) {
            stringstream ost;
            ost << "Symbol" << endl;
            ost << " name : " << extern_symbol(symbol->id) << endl;
            return ost.str();
        }

        if(auto p = dcast<const Pair>(val)) {
            stringstream ost;
            ost << "Pair" << endl;
            ost << " car  : " << memory_location(p->car) << endl;
            ost << " cdr  : " << memory_location(p->cdr) << endl;
            return ost.str();
        }
        if(auto proc = dcast<const Procedure>(val)) {
            stringstream ost;
            ost << "Procedure" << endl;
            ost << " addr : 0x" << std::hex << (unsigned long)proc->func << endl;
            return ost.str();
        }
        if(auto lambdaproc = dcast<const LambdaProcedure>(val)) {
            stringstream ost;
            ost << "LambdaProcedure" << endl;
            ost << " body : " << memory_location(lambdaproc->body) << endl;
            ost << " env  : " << memory_location(lambdaproc->environment) << endl;
            return ost.str();
        }
        if(auto e = dcast<const Environment>(val)) {
            stringstream ost;
            ost << "Environment" << endl;
            ost << " paren: " << memory_location(e->parent) << endl;
            ost << " e_map: " << memory_location(e->env_map) << endl;
            return ost.str();
        }
        if(auto a = dcast<const EnvironmentMap>(val)) {
            stringstream ost;
            ost << "EnvironmentMap" << endl;
            ost << " name : " << extern_symbol(a->name) << endl;
            ost << " val  : " << memory_location(a->val) << endl;
            ost << " l    : " << memory_location(a->l) << endl;
            ost << " g    : " << memory_location(a->g) << endl;
            return ost.str();
        }
        throw not_implemented(string("not_implemented ") + typeid(*val).name());
    }

    string show(const GCObject *val)
    {
        if(dcast<const Empty>(val))
            return "()";
        if(auto b = dcast<const Bool>(val))
            return b->value ? "#t" : "#f";
        if(auto num = dcast<const Number>(val))
            return to_string(num->integer);
        if(auto symbol = dcast<const Symbol>(val))
            return extern_symbol(symbol->id);
        if(auto p = dcast<const Pair>(val)) {
            vector<string> buf;
            
            const Pair *next=p;
            
            for(;;){
                buf.push_back(show(next->car));
                
                if(typeid(*next->cdr) != typeid(Pair))
                    break;
                
                next = static_cast<Pair*>(next->cdr);
            }
            
            if(typeid(*next->cdr) == typeid(Empty)) {
                stringstream ost;
                ost << "(";
                bool flag = false;
                for(auto &s : buf) {
                    if(flag)
                        ost << " ";
                    else
                        flag = true;
                    ost << s;
                }
                ost << ")";
                return ost.str();
            } else {
                throw std::exception();
            }
        }
        if(auto proc = dcast<const Procedure>(val)) {
            stringstream ost;
            ost << "<Procedure 0x" << std::hex << (unsigned long)proc->func << ">";
            return ost.str();
        }
        if(auto lambdaproc = dcast<const LambdaProcedure>(val)) {
            stringstream ost;
            ost << "<Lambda Procedure (lambda (";
            for(auto &n : lambdaproc->formals) {
                ost << n << " ";
            }
            ost << ") ";
            ost << show(lambdaproc->body);
            ost << ")>";
            return ost.str();
        }
        throw not_implemented();
    }
}
