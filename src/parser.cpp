#include "parser.hpp"

PResult PNumber(const std::string &, int p)
{
//    if(s[s.p] == '#') {
//        p++;
//        throw 0;
//    } 
    return std::make_tuple(nv::init<NumberData>(number(0)), p);
}
