#pragma once
#include "exceptions.hpp"
#include <sstream>
#include "show.hpp"
#define dump(x) (std::cerr << #x" : " << x << std::endl)

namespace sucheme {
    
    template <class T, class ...Args>
    inline std::unique_ptr<T> make_unique(Args &&...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T0,typename T1> T0* dcast(T1 *a)
    {
        auto ret = dynamic_cast<T0*>(a);
        if(ret)
            return std::move(ret);
        else {
            std::stringstream ost;
            ost <<  "bad_cast: tried to convert " << typeid(*a.get()).name() << " to " << typeid(T0).name() << "\n";
            ost << "value:" << show(a);
            throw bad_lisp_cast(ost.str());
        }
    }
}
