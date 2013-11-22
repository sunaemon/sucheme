#include <sstream>
#include "parser.hpp"
#include "environment.hpp"
#include <map>
#include <vector>
#include <iostream>
#include "exceptions.hpp"
#include "generic_functions.hpp"
#include "list.hpp"

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::dynamic_pointer_cast;
    using std::endl;
    using std::cerr;
 
    vector<GCObject*> ListToVector(Pair *list) {
        vector<GCObject *> ret;
        ListForeach(list,[&ret](GCObject *v){ret.push_back(v);});
        return ret;
    }
}
