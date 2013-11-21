#include "functions.hpp"
#include <iostream>
#include "show.hpp"
#include "list.hpp"

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::unique_ptr;
    using std::dynamic_pointer_cast;
    using std::make_shared;
    using std::cout;
    using std::endl;
 
    LispVal* add(const vector<LispVal*> &arg) {
        int ret=0;
        for(auto &i : arg) 
            ret += dcast_ex<Number>(i)->integer;

        return alloc<Number>(ret);
    }

   LispVal* mul(const vector<LispVal*> &arg) {
        int ret=1;
        for(auto &i : arg) 
            ret *= dcast_ex<Number>(i)->integer;

        return alloc<Number>(ret);
    }

   LispVal* sub(const vector<LispVal*> &arg) {
        if(arg.size() <= 1)
            throw invalid_aplication("invalid_aplication:sub");

        int ret=dcast_ex<Number>(arg[0])->integer;

        for(auto i = arg.begin()+1; i<arg.end(); i++) 
            ret -= dcast_ex<Number>(*i)->integer;

        return alloc<Number>(ret);
    }

   LispVal* eq(const vector<LispVal*> &arg) {
       if(arg.size() != 2)
            throw invalid_aplication("invalid_aplication:eq" + arg.size());

        return alloc<Bool>(dcast_ex<Number>(arg[0])->integer == dcast_ex<Number>(arg[1])->integer);
    }

   LispVal* lt(const vector<LispVal*> &arg) {
       if(arg.size() != 2)
           throw invalid_aplication("invalid_aplication:lt");

       return alloc<Bool>(dcast_ex<Number>(arg[0])->integer < dcast_ex<Number>(arg[1])->integer);
    }

   LispVal* print(const vector<LispVal*> &arg) {
        for(auto &i : arg) 
            cout << show(i);
        cout << endl;
        return alloc<Empty>();
    }

   LispVal* car(const vector<LispVal*> &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:car" + arg.size());
       return dcast_ex<Pair>(arg[0])->car;
    }

   LispVal* cdr(const vector<LispVal*> &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:car" + arg.size());
       return dcast_ex<Pair>(arg[0])->cdr;
    }

   LispVal* cons(const vector<LispVal*> &arg) {
       if(arg.size() != 2)
            throw invalid_aplication("invalid_aplication:cons" + arg.size());
       return cons(arg[0], arg[1]);
    }

   LispVal* null_is(const vector<LispVal* > &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:null?" + arg.size());
       return alloc<Bool>((bool)dynamic_cast<Empty*>(arg[0]));
    }
}
