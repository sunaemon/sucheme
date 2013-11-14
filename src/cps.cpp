#include <sstream>
#include "parser.hpp"
#include "environment.hpp"
#include <map>
#include <vector>
#include <iostream>
#include "exceptions.hpp"
#include "generic_functions.hpp"
#include "list.hpp"
#include <cassert>
#include <random>

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::unique_ptr;
    using std::dynamic_pointer_cast;
    using std::endl;
    using std::cerr;
    using std::shared_ptr;

    std::random_device rd;
    std::mt19937 mt(rd());

    bool is_simple(const shared_ptr<LispVal> &program, const Environment &e) {
        if(auto empty = dynamic_pointer_cast<Empty>(program))
            return true;
        if(auto num = dynamic_pointer_cast<Number>(program))
            return true;
        if(auto b = dynamic_pointer_cast<Bool>(program))
            return true;
        if(auto symbol = dynamic_pointer_cast<Symbol>(program))
            return have(&e, symbol->name);
        if(auto p = dynamic_pointer_cast<Pair>(program))
            return is_simple(p->car, e) && is_simple(p->cdr, e);
        throw not_implemented();
    }

    shared_ptr<LispVal> cps(const shared_ptr<LispVal> &program, const shared_ptr<LispVal> &cont, const Environment &e)
    {
        //cerr << "cps(\n" << program->show() << "\n" << cont->show() << "\n)" << endl << endl;
        shared_ptr<LispVal> ret;

        if(auto num = dynamic_pointer_cast<Number>(program))
            ret = make_list(cont, num);
        if(auto b = dynamic_pointer_cast<Bool>(program))
            ret = make_list(cont, b);
        if(auto symbol = dynamic_pointer_cast<Symbol>(program))
            ret = make_list(cont, symbol);
        if(auto p = dynamic_pointer_cast<Pair>(program)) {
            auto a = ListToVector(p);
            if(auto s = dynamic_pointer_cast<Symbol>(a[0])){
                if(s->name == "lambda") {
                    char name[] = {char('a' + (mt() % 26)), char('a' + (mt() % 26)), char('a' + (mt() % 26)), 0};
                    string r = name;

                    auto new_formals = cons(make_symbol("%"+r), a[1]);
                    auto new_body = cps(a[2], make_symbol("%"+r), e);
                    auto new_lambda = make_list(make_symbol("lambda"),
                                                new_formals,
                                                new_body);
                    ret = make_list(cont, new_lambda);
                } else if(s->name == "cond") {
                    shared_ptr<LispVal> new_cond = make_shared<Empty>();

                    for(int i=a.size()-1; i>0; i--) {
                        auto old_case = ListToVector(dcast<Pair>(a[i]));

                        auto new_case = make_list(old_case[0], cps(old_case[1], cont, e));
                        
                        new_cond = cons(new_case, new_cond);
                    }
                    new_cond = cons(make_symbol("cond"), new_cond);

                    ret = new_cond;
                } else if(s->name == "letrec") {
                    assert(a.size() == 3);

                    shared_ptr<LispVal> new_letrec = make_list(cps(a[2], cont, e));

                    for(int i=a.size()-1; i>2; i--)
                        new_letrec = cons(cps(a[i], cont, e), new_letrec);
                    
                    auto old_binding_specs = ListToVector(dcast<Pair>(a[1]));

                    shared_ptr<LispVal> new_binding_specs = nil();

                    for(int i=old_binding_specs.size()-1; i>=0; i--) {
                        auto old_binding_spec = ListToVector(dcast<Pair>(old_binding_specs[i]));

                        assert(old_binding_spec.size() == 2);

                        auto new_binding_spec = make_list(old_binding_spec[0],
                                                          cps(old_binding_spec[1], cont, e));
                                                          
                        new_binding_specs = cons(new_binding_spec, new_binding_specs);
                    }

                    new_letrec = cons(new_binding_specs, new_letrec);
                    new_letrec = cons(make_symbol("letrec"), new_letrec);

                    ret = new_letrec;
                } else {
                    char name[] = {char('a' + (mt() % 26)), char('a' + (mt() % 26)), char('a' + (mt() % 26)), 0};
                    string r = name;

                    auto s = is_simple(a[0], e);

                    shared_ptr<LispVal> new_cont = nil();

                    for(int i=a.size()-1; i>0; i--)
                        new_cont = cons(make_symbol("%" + r + to_string(i)), new_cont);

                    if(s)
                        new_cont = make_list(cont, cons(a[0], new_cont));
                    else {
                        new_cont = cons(cont, new_cont);
                        new_cont = cons(make_symbol("%"+r+"0"), new_cont);
                    }
                    
                    shared_ptr<LispVal> new_apply = nil();

                    if(s)
                        for(int i=a.size()-1; i>0; i--)
                            new_cont = cps(a[i], make_list(make_symbol("lambda"),
                                                           make_list(make_symbol("%" + r+ to_string(i))),
                                                           new_cont), e);
                    else
                        for(int i=a.size()-1; i>=0; i--)
                            new_cont = cps(a[i], make_list(make_symbol("lambda"),
                                                           make_list(make_symbol("%" + r + to_string(i))),
                                                           new_cont), e);
                    ret = new_cont;
                }
                
            }
        }
        //cerr << "cps(\n" << program->show() << ",\n" << cont->show() << "\n)=\n" << ret->show() << endl << endl;
        return ret;
        throw not_implemented();
    }
}

