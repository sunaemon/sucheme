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
        for(auto &i : arg) {
            auto a = dynamic_pointer_cast<Number>(i);

            if(nullptr == a)
                throw std::exception();

            ret += a->integer;
        }
        return make_shared<Number>(ret);
    }
}
