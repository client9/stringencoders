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

using namespace modp;

void test_b16()
{
    string orig("this is a test");
    string s(orig);
    b16_encode(s);
    b16_decode(s);
    if (s != orig) {
        cerr << "Expected " << orig << ", recieved " << s << "\n";
    }

    // test bad input
    s = "1";
    b16_decode(s);
    if (!s.empty()) {
        cerr << "Expected decode to be empty\n";
    }
}

void test_b64()
{
    string orig("this is a test");
    string s(orig);
    b64_encode(s);
    if (s == orig) {
        cerr << "something wrong\n";
    }

    b64_decode(s);
    if (s != orig) {
        cerr << "Expected " << orig << ", recieved " << s << "\n";
    }

    // test bad decode length
    s = "a";
    b64_decode(s);
    if (!s.empty()) {
        cerr << "Expected decode output to be empty\n";
    }

}

void test_b85()
{
    // must be multiple of 4
    string orig("this is a test!!");
    string s(orig);
    b85_encode(s);
    if (s == orig) {
        cerr << "something wrong\n";
    }
    b85_decode(s);
    if (s != orig) {
        cerr << "Expected " << orig << ", recieved " << s << "\n";
    }

    // test non-multiple for decode
    string badstr;

    badstr = "abcd";
    b85_decode(badstr);
    if (!badstr.empty()) {
        cerr << "Expected decode output to be empty\n";
    }

    badstr = "abcdef";
    b85_encode(badstr);
    if (!badstr.empty()) {
        cerr << "Expected encode output to be empty\n";
    }
}


void test_url()
{
    string orig("this is a test");
    string s(orig);
    url_encode(s);
    if (s != "this%20is%20a%20test") {
        cerr << "Expected " << orig << ", recieved " << s << "\n";
    }

    url_decode(s);
    if (s != orig) {
        cerr << "Expected " << orig << ", recieved " << s << "\n";
    }

    s = "bad\n";
    url_decode(s);
    if (s.empty()) {
        cerr << "Expected empty decode\n";
    }

}

void test_javascript()
{
    string orig("this \"is\' a test\n");
    string expected("this \\\"is\\' a test\\n");
    javascript_encode(orig);
    if (orig != expected) {
        cerr << "Expected: " << expected << "Received: " << orig << "\n";
    }
}

int main()
{
    test_b16();
    test_b64();
    test_b85();
    test_url();
    test_javascript();

    return 0;
}
