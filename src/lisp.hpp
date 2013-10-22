#pragma once

#include "named_variant/named_variant.hpp"
#include <vector>
#include <tuple>

namespace nv = named_variants;

struct BoolData {};
struct StringData {};
struct CharData {};
struct VectorData {};
struct PairData {};
struct SybmolData {};
struct PortData {};
struct ProcedureData {};
struct NumberData {};

// data value = BoolData bool
//            | StringData String
//           ....

struct port{};

struct number {
    int integer;
    number(int i) : integer(i) {}
};

using LispVal = nv::make_recursive_named_variant<
    NumberData, number
    >::type;
//    BoolData, bool,
//    StringData, std::string,
//    CharData, wchar_t,
//    SybmolData, std::string,
//    PairData, std::pair<nv::recursive_named_variant_,nv::recursive_named_variant_>,
//    ListData, std::list<nv::recursive_named_variant_>,
//    DottedList, std::pair<std::list<nv::recursive_named_variant_>,nv::recursive_named_variant_>

//    VectorData, std::vector<nv::recursive_named_variant_>,
//    PortData, port,
//    ProcedureData, procedure,
