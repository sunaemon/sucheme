#pragma once
#include <cppunit/extensions/HelperMacros.h>
#include <string>
#include <memory>
#include "../src/parser.hpp"
#include "../src/environment.hpp"
#include "../src/functions.hpp"

using namespace sucheme;
using std::string;
using std::shared_ptr;

class parser_test : public CPPUNIT_NS::TestCase
{
    CPPUNIT_TEST_SUITE( parser_test );
    CPPUNIT_TEST( test_number_parser );
    CPPUNIT_TEST( test_list_parser );
    CPPUNIT_TEST_SUITE_END();
public:
    parser_test();
    virtual ~parser_test();
    
    void test_number_parser(int i);
    void test_number_parser();
    void test_list_parser();
    void test_parse(const string &s, const string &t);
    void test_parse(const string &s);
    void test_parse();
    void test_eval(shared_ptr<LispVal> a, shared_ptr<LispVal> b);
    void test_plus();


    parser_test( const parser_test &copy ) = delete;
    void operator =( const parser_test &copy ) = delete;
};
