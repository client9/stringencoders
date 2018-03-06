
include_HEADERS = \
	extern_c_begin.h extern_c_end.h \
	modp_b16.h modp_b64.h modp_b64w.h modp_b64r.h \
	modp_b85.h modp_burl.h modp_bjavascript.h \
	modp_numtoa.h modp_ascii.h modp_b2.h \
	modp_qsiter.h modp_xml.h modp_html.h modp_json.h

lib_LTLIBRARIES = libmodpbase64.la
libmodpbase64_la_SOURCES = \
	modp_b2.c modp_b2.h modp_b2_data.h \
	modp_b16.c modp_b16.h modp_b16_data.h \
	modp_b64.c modp_b64.h modp_b64_data.h \
	modp_b64w.c modp_b64w.h modp_b64w_data.h \
	modp_b64r.c modp_b64r.h modp_b64r_data.h \
	modp_b85.h modp_b85.c modp_b85_data.h \
	modp_burl.h modp_burl.c modp_burl_data.h \
	modp_bjavascript.h modp_bjavascript.c modp_bjavascript_data.h \
	modp_numtoa.h modp_numtoa.c \
	modp_qsiter.h modp_qsiter.c \
	modp_xml.h modp_xml.c \
	modp_ascii.h modp_ascii.c modp_ascii_data.h \
	modp_utf8.h modp_utf8.c \
	modp_html.h modp_html.c \
	modp_json.h modp_json.c 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = stringencoders.pc

#libmodpbase64_la_DEPENDENCIES = \
#	modp_b2_data.h modp_b2_gen \
#	modp_b16_data.h modp_b16_gen \
#	modp_b64_data.h modp_b64w_data.h modp_b64r_data.h modp_b64_gen \
#	modp_b85_data.h modp_b85_gen \
#	modp_ascii_data.h modp_ascii_gen

CLEANFILES = *.log \
	*~ src/*~ test/*~ \
	*.tmp src/*.tmp test/*.tmp \
	doxy/html/* \
	doxy/*.tmp \
	m4 \
	*.loT \
	*.ll *.ll.out \
	*.gcda *.gcno *.gcov stringencoders.info \
	modp_b16_data.h modp_b64_data.h modp_b85_data.h \
	modp_b64w_data.h modp_b64w.c test/modp_b64w_test.c \
	modp_b64r_data.h modp_b64r.c test/modp_b64r_test.c \
	modp_ascii_data.h modp_b2_data.h \
	modp_xml_test modp_qsiter_test modp_html_test modp_json_test \
	cxx_test

modp_b2.c: modp_b2.h modp_b2_data.h

modp_b16.c: modp_b16.h modp_b16_data.h

modp_b64.c: modp_b64.h modp_b64_data.h

modp_b64r.c: modp_b64.c modp_b64r.h modp_b64r_data.h
	perl -p -i -e 's/b64/b64r/g' < modp_b64.c > modp_b64r.c

modp_b64w.c: modp_b64.c modp_b64w.h modp_b64w_data.h
	perl -p -i -e 's/b64/b64w/g' < modp_b64.c > modp_b64w.c

modp_b85.c: modp_b85.h modp_b85_data.h

modp_burl.c: modp_burl.h modp_burl_data.h

modp_bjavascript.c: modp_bjavascript.h modp_bjavascript_data.h

modp_json.c: modp_json.h modp_json_data.h

modp_ascii.c: modp_ascii.h modp_ascii_data.h

modp_qsiter.c: modp_qsiter.h

modp_xml.c: modp_xml.h

modp_b2_data.h: modp_b2_gen
	./modp_b2_gen > modp_b2_data.h

modp_b16_data.h: modp_b16_gen
	./modp_b16_gen > modp_b16_data.h

modp_b64_data.h: modp_b64_gen
	./modp_b64_gen > modp_b64_data.h

modp_ascii_data.h: modp_ascii_gen
	./modp_ascii_gen > modp_ascii_data.h

#
# Recall B64WCHARS by default is "-_."
#
modp_b64w_data.h: modp_b64_gen
	./modp_b64_gen $(B64WCHARS) > modp_b64w_data.h

#
# RFC 4648 alphabet is -_=
# http://www.ietf.org/rfc/rfc4648.txt
modp_b64r_data.h: modp_b64_gen
	./modp_b64_gen "-_=" > modp_b64r_data.h

modp_b85_data.h: modp_b85_gen
	./modp_b85_gen > modp_b85_data.h

modp_burl_data.h: modp_burl_gen
	./modp_burl_gen > modp_burl_data.h

modp_bjavascript_data.h: modp_bjavascript_gen
	./modp_bjavascript_gen > modp_bjavascript_data.h

modp_json_data.h: modp_json_gen.py
	./modp_json_gen.py

noinst_PROGRAMS = \
	modp_b2_gen modp_b16_gen modp_b64_gen modp_b85_gen \
	modp_burl_gen modp_ascii_gen modp_bjavascript_gen

modp_b2_gen_SOURCES = arraytoc.c modp_b2_gen.c

modp_b16_gen_SOURCES = arraytoc.c modp_b16_gen.c

modp_b64_gen_SOURCES = arraytoc.c modp_b64_gen.c

modp_b85_gen_SOURCES = arraytoc.c modp_b85_gen.c

modp_burl_gen_SOURCES = arraytoc.c modp_burl_gen.c

modp_ascii_gen_SOURCES = arraytoc.c modp_ascii_gen.c

modp_bjavascript_gen_SOURCES = arraytoc.c modp_bjavascript_gen.c



