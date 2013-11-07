#include "functions.hpp"

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::unique_ptr;
    using std::dynamic_pointer_cast;
    using std::make_shared;
 
   shared_ptr<LispVal> add(const vector<shared_ptr<LispVal> > &arg) {
        int ret=0;
        for(auto &i : arg) 
            ret += dcast<Number>(i)->integer;

        return make_shared<Number>(ret);
    }

   shared_ptr<LispVal> mul(const vector<shared_ptr<LispVal> > &arg) {
        int ret=1;
        for(auto &i : arg) 
            ret *= dcast<Number>(i)->integer;

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

}
