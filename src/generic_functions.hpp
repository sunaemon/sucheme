#pragma once
#include "exceptions.hpp"
#include <sstream>
#include "show.hpp"
#define dump(x) (std::cerr << #x" : " << x << std::endl)

namespace sucheme {
    
    template<typename T0,typename T1> inline T0* dcast_ex(T1 *a)
    {
        auto ret = dynamic_cast<T0*>(a);
        if(ret)
            return std::move(ret);
        else {
            std::stringstream ost;
            ost <<  "bad_cast: tried to convert " << typeid(*a).name() << " to " << typeid(T0).name() << "\n";
            ost << "value:" << show(a);
            throw bad_lisp_cast(ost.str());
        }
    }
}
