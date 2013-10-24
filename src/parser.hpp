#pragma once

#include "named_variant/named_variant.hpp"
#include <vector>
#include <tuple>
#include <list>

namespace nv = named_variants;

struct BoolData {};
struct StringData {};
struct CharData {};
struct VectorData {};
struct PairData {};
struct SymbolData {};
struct PortData {};
struct ProcedureData {};
struct NumberData {};
struct ListData {};

// data value = BoolData bool
//            | StringData String
//           ....

struct port{};

struct number {
    int integer;
    number(int i) : integer(i) {}
};

using LispVal = nv::make_recursive_named_variant<
    NumberData, number,
    ListData, std::list<nv::recursive_named_variant_>,
    SymbolData, std::string
    >::type;
//    BoolData, bool,
//    StringData, std::string,
//    CharData, wchar_t,
//    PairData, std::pair<nv::recursive_named_variant_,nv::recursive_named_variant_>,
//    DottedList, std::pair<std::list<nv::recursive_named_variant_>,nv::recursive_named_variant_>

//    VectorData, std::vector<nv::recursive_named_variant_>,
//    PortData, port,
//    ProcedureData, procedure,


std::tuple<LispVal, int> PExpr(const std::string &s, int32_t p = 0);

std::string show(const LispVal &val);

LispVal eval(const LispVal &val);
