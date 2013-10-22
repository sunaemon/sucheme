#include <readline/readline.h>
#include <readline/history.h>
#include <boost/variant.hpp>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
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

using Result = std::tuple<LispVal, int>;

Result PNumber(const std::string &s, int p = 0)
{
//    if(s[s.p] == '#') {
//        p++;
//        throw 0;
//    } 
    return std::make_tuple(nv::init<NumberData>(number(0)), p);
}

int main(int, char**)
{
    char *command;

    using_history();
    read_history(".my_history");
    while(command = readline(">> ")) {
        add_history(command);

        printf("%s\n", command);

        free(command);
    }
    write_history(".my_history");
    fflush(stdout);
}

