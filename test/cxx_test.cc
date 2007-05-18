/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <string>
using std::string;

#include <iostream>
using std::cerr;

#include "modp_b16.h"
#include "modp_b64.h"
#include "modp_b85.h"
#include "modp_burl.h"
#include "modp_bjavascript.h"
#include "modp_ascii.h"

using namespace modp;

#define WHERE(A) A << "[" << __FILE__ << ":" << __LINE__ << "] "

void test_b16()
{
    string orig("this is a test");
    string s(orig);
    b16_encode(s);
    b16_decode(s);
    if (s != orig) {
        WHERE(cerr) << "Expected " << orig << ", recieved " << s << "\n";
        exit(1);
    }

    // test bad input
    s = "1";
    b16_decode(s);
    if (!s.empty()) {
        WHERE(cerr) << "Expected decode to be empty\n";
        exit(1);
    }
}

void test_b64()
{
    string orig("this is a test");
    string s(orig);
    b64_encode(s);
    if (s == orig) {
        WHERE(cerr) << "something wrong\n";
        exit(1);
    }

    b64_decode(s);
    if (s != orig) {
        WHERE(cerr) << "Expected " << orig << ", recieved " << s << "\n";
        exit(1);
    }

    // test bad decode length
    s = "a";
    b64_decode(s);
    if (!s.empty()) {
        WHERE(cerr) << "Expected decode output to be empty\n";
        exit(1);
    }

}

void test_b85()
{
    // must be multiple of 4
    string orig("this is a test!!");
    string s(orig);
    b85_encode(s);
    if (s == orig) {
        WHERE(cerr) << "something wrong\n";
        exit(1);
    }
    b85_decode(s);
    if (s != orig) {
        WHERE(cerr) << "Expected " << orig << ", recieved " << s << "\n";
        exit(1);
    }

    // test non-multiple for decode
    string badstr;

    badstr = "abcd";
    b85_decode(badstr);
    if (!badstr.empty()) {
        WHERE(cerr) << "Expected decode output to be empty\n";
        exit(1);
    }

    badstr = "abcdef";
    b85_encode(badstr);
    if (!badstr.empty()) {
        WHERE(cerr) << "Expected encode output to be empty\n";
        exit(1);
    }
}


void test_url()
{
    string orig("this is a test");
    string expected("this+is+a+test");
    string s(orig);
    url_encode(s);
    if (s != expected) {
        WHERE(cerr) << "Expected " << expected << ", recieved " << s << "\n";
        exit(1);
    }

    url_decode(s);
    if (s != orig) {
        WHERE(cerr) << "Expected " << orig << ", recieved " << s << "\n";
        exit(1);
    }

    s = "bad\n";
    url_decode(s);
    if (s.empty()) {
        WHERE(cerr) << "Expected empty decode\n";
        exit(1);
    }

}

void test_javascript()
{
    string orig("this \"is\' a test\n");
    string expected("this \\\"is\\' a test\\n");
    javascript_encode(orig);
    if (orig != expected) {
        WHERE(cerr) << "Expected: " << expected << "Received: " << orig << "\n";
        exit(1);
    }
}

void test_ascii_copy()
{
    string orig;

    orig = "abcd123";
    toupper(orig);
    if (orig != "ABCD123") {
        WHERE(cerr) << "to upper copy failed: " << orig << " (size=" << orig.size() << ")\n";
        exit(1);
    }

    orig = "ABCD123";
    tolower(orig);
    if (orig != "abcd123") {
        WHERE(cerr) << "to lower copy failed: " << orig << " (size=" << orig.size() << ")\n";
        exit(1);
    }
}

int main()
{
    test_b16();
    test_b64();
    test_b85();
    test_url();
    test_javascript();
    test_ascii_copy();

    return 0;
}
