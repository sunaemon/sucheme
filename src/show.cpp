#include "parser.hpp"
#include <sstream>
#include <iostream>

namespace sucheme{

    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;

    string show(const LispVal *val)
    {
        if(auto empty = dynamic_cast<const Empty*>(val))
            return "()";
        if(auto b = dynamic_cast<const Bool*>(val))
            return b->value ? "#t" : "#f";
        if(auto num = dynamic_cast<Number*>(val))
            return to_string(num->integer);
        if(auto symbol = dynamic_pointer_cast<Symbol>(val))
            return symbol->name;
        if(auto p = dynamic_pointer_cast<Pair>(val)) {
            vector<string> buf;
            
            Pair *next=p.get();
            
            for(;;){
                buf.push_back(show(next->car));
                
                if(typeid(*next->cdr.get()) != typeid(Pair))
                    break;
                
                next = static_cast<Pair*>(next->cdr.get());
            }
            
            if(typeid(*next->cdr.get()) == typeid(Empty)) {
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
        if(auto proc = dynamic_pointer_cast<Procedure>(val)) {
            stringstream ost;
            ost << "<Procedure 0x" << std::hex << (unsigned long)proc->func << ">";
            return ost.str();
        }
        if(auto lambdaproc = dynamic_pointer_cast<LambdaProcedure>(val)) {
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
