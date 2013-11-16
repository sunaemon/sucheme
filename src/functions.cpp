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
 
   shared_ptr<LispVal> add(const vector<LispVal*> &arg) {
        int ret=0;
        for(auto &i : arg) 
            ret += dcast<Number>(&i)->integer;

        return make_shared<Number>(ret);
    }

   shared_ptr<LispVal> mul(const vector<LispVal*> &arg) {
        int ret=1;
        for(auto &i : arg) 
            ret *= dcast<Number>(*i)->integer;

        return make_shared<Number>(ret);
    }

   shared_ptr<LispVal> sub(const vector<shared_ptr<LispVal> > &arg) {
        if(arg.size() <= 1)
            throw invalid_aplication("invalid_aplication:sub");

        int ret=dcast<Number>(arg[0])->integer;

        for(auto i = arg.begin()+1; i<arg.end(); i++) 
            ret -= dcast<Number>(*i)->integer;

        return make_shared<Number>(ret);
    }

   shared_ptr<LispVal> eq(const vector<shared_ptr<LispVal> > &arg) {
       if(arg.size() != 2)
            throw invalid_aplication("invalid_aplication:eq" + arg.size());

        return make_shared<Bool>(dcast<Number>(arg[0])->integer == dcast<Number>(arg[1])->integer);
    }

   shared_ptr<LispVal> lt(const vector<shared_ptr<LispVal> > &arg) {
       if(arg.size() != 2)
           throw invalid_aplication("invalid_aplication:lt");

       return make_shared<Bool>(dcast<Number>(arg[0])->integer < dcast<Number>(arg[1])->integer);
    }

   shared_ptr<LispVal> print(const vector<shared_ptr<LispVal> > &arg) {
        for(auto &i : arg) 
            cout << show(i);
        cout << endl;
        return make_shared<Empty>();
    }

   shared_ptr<LispVal> car(const vector<shared_ptr<LispVal> > &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:car" + arg.size());
       return dcast<Pair>(arg[0])->car;
    }

   shared_ptr<LispVal> cdr(const vector<shared_ptr<LispVal> > &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:car" + arg.size());
       return dcast<Pair>(arg[0])->cdr;
    }

   shared_ptr<LispVal> cons(const vector<shared_ptr<LispVal> > &arg) {
       if(arg.size() != 2)
            throw invalid_aplication("invalid_aplication:cons" + arg.size());
       return cons(arg[0], arg[1]);
    }

   shared_ptr<LispVal> null_is(const vector<shared_ptr<LispVal> > &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:null?" + arg.size());
       return make_shared<Bool>((bool)dynamic_pointer_cast<Empty>(arg[0]));
    }
}
