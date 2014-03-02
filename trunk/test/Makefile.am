
STRINGENCODERS_INCLUDE = -I$(top_srcdir)/src
STRINGENCODERS_LTLIB = $(top_srcdir)/src/libmodpbase64.la

check_PROGRAMS = \
	modp_ascii_test \
	modp_b2_test \
	modp_b16_test \
	modp_b64_test \
	modp_b64r_test \
	modp_b64w_test \
	modp_b85_test \
	modp_burl_test \
	modp_bjavascript_test \
	modp_numtoa_test \
	modp_utf8_test \
	modp_xml_test \
	modp_html_test \
	modp_json_test \
	modp_qsiter_test \
	cxx_test

TESTS = $(check_PROGRAMS)

noinst_PROGRAMS = \
	speedtest \
	speedtest_numtoa \
	speedtest_msg

speedtest_SOURCES = apr_base64.h apr_base64.c speedtest.c
speedtest_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
speedtest_LDADD = $(STRINGENCODERS_LTLIB)

speedtest_numtoa_SOURCES = speedtest_numtoa.c
speedtest_numtoa_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
speedtest_numtoa_LDADD = $(STRINGENCODERS_LTLIB)

speedtest_msg_SOURCES = speedtest_msg.c
speedtest_msg_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
speedtest_msg_LDADD = $(STRINGENCODERS_LTLIB)

modp_b2_test_SOURCES = modp_b2_test.c
modp_b2_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_b2_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_b16_test_SOURCES = modp_b16_test.c
modp_b16_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_b16_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_b64_test_SOURCES = modp_b64_test.c
modp_b64_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_b64_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_b64w_test.c: modp_b64_test.c
	perl -p -i -e 's/b64/b64w/g' < modp_b64_test.c > modp_b64w_test.c

modp_b64w_test_SOURCES = modp_b64w_test.c
modp_b64w_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_b64w_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_b64r_test.c: modp_b64_test.c
	perl -p -i -e 's/b64/b64r/g' < modp_b64_test.c > modp_b64r_test.c

modp_b64r_test_SOURCES = modp_b64r_test.c
modp_b64r_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_b64r_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_b85_test_SOURCES = modp_b85_test.c
modp_b85_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_b85_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_burl_test_SOURCES = modp_burl_test.c
modp_burl_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_burl_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_bjavascript_test_SOURCES = modp_bjavascript_test.c
modp_bjavascript_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_bjavascript_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_ascii_test_SOURCES = modp_ascii_test.c
modp_ascii_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_ascii_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_xml_test_SOURCES = modp_xml_test.c
modp_xml_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_xml_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_numtoa_test_SOURCES = modp_numtoa_test.c
modp_numtoa_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_numtoa_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_html_test_SOURCES = modp_html_test.c
modp_html_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_html_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_qsiter_test_SOURCES = modp_qsiter_test.c
modp_qsiter_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_qsiter_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_utf8_test_SOURCES = modp_utf8_test.c
modp_utf8_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_utf8_test_LDADD = $(STRINGENCODERS_LTLIB)

modp_json_test_SOURCES = modp_json_test.c
modp_json_test_CPPFLAGS = $(STRINGENCODERS_INCLUDE)
modp_json_test_LDADD = $(STRINGENCODERS_LTLIB)

cxx_test_SOURCES = cxx_test.cc
cxx_test_CPPFLAGS =$(STRINGENCODERS_INCLUDE)
cxx_test_LDADD = $(STRINGENCODERS_LTLIB)

clean-local:
	rm -f *~
