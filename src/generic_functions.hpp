#pragma once
#include "exceptions.hpp"
#include <sstream>

namespace sucheme {
    template <class T, class ...Args>
    inline std::unique_ptr<T> make_unique(Args &&...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T0,typename T1> std::shared_ptr<T0> dcast(const std::shared_ptr<T1> &a)
    {
        auto ret = std::dynamic_pointer_cast<T0>(a);
        if(ret)
            return std::move(ret);
        else {
            std::stringstream ost;
            ost <<  "bad_cast: tried to convert " << typeid(*a.get()).name() << " to " << typeid(T0).name();
            throw bad_lisp_cast(ost.str());
        }
    }
}

