

SOURCES = \
	src/modp_b16.c src/modp_b16.h modp_b16_data.h \
	src/modp_b64.c src/modp_b64.h modp_b64_data.h \
	src/modp_b64w.c src/modp_b64w.h modp_b64w_data.h \
	src/modp_b85.h src/modp_b85.c modp_b85_data.h \
	src/modp_burl.h src/modp_burl.c modp_burl_data.h \
	src/modp_bjavascript.h src/modp_bjavascript.c

lib_LTLIBRARIES = libmodpbase64.la
libmodpbase64_la_SOURCES = ${SOURCES}


libmodpbase64_la_DEPENDENCIES = \
	modp_b16_data.h modp_b16_gen \
	modp_b64_data.h modp_b64w_data.h modp_b64_gen \
	modp_b85_data.h modp_b85_gen \
	modp_burl_data.h modp_burl_gen \
	modp_bjavascript_data.h modp_bjavascript_gen

include_HEADERS = \
	src/modp_b16.h src/modp_b64.h src/modp_b64w.h \
	src/modp_b85.h src/modp_burl.h src/modp_bjavascript.h

doxy: ${SOURCES} doxy/header.html doxy/footer.html Doxyfile
	doxygen Doxyfile

CLEANFILES = *.log \
	*~ src/*~ test/*~ \
	*.tmp src/*.tmp test/*.tmp \
	doxy/html/* \
	modp_b16_data.h modp_b64_data.h modp_b85_data.h modp_burl_data.h modp_bjavascript_data.h \
	modp_b64w_data.h src/modp_b64w.c test/modp_b64w_test.c


src/modp_b16.c: src/modp_b16.h modp_b16_data.h

src/modp_b64.c: src/modp_b64.h modp_b64_data.h

src/modp_b64w.c: src/modp_b64.c src/modp_b64w.h modp_b64w_data.h
	perl -p -i -e 's/b64/b64w/g' < src/modp_b64.c > src/modp_b64w.c

src/modp_b85.c: src/modp_b85.h modp_b85_data.h

src/modp_burl.c: src/modp_burl.h modp_burl_data.h

src/modp_bjavascript.c: src/modp_bjavascript.h modp_bjavascript_data.h

modp_b16_data.h: modp_b16_gen
	./modp_b16_gen > modp_b16_data.h

modp_b64_data.h: modp_b64_gen
	./modp_b64_gen > modp_b64_data.h

modp_b64w_data.h: modp_b64_gen
	./modp_b64_gen "-_." > modp_b64w_data.h

modp_b85_data.h: modp_b85_gen
	./modp_b85_gen > modp_b85_data.h

modp_burl_data.h: modp_burl_gen
	./modp_burl_gen > modp_burl_data.h 

modp_bjavascript_data.h: modp_bjavascript_gen
	./modp_bjavascript_gen > modp_bjavascript_data.h 

noinst_PROGRAMS = \
	modp_b16_gen  modp_b64_gen  modp_b85_gen  modp_burl_gen \
	modp_b16_test modp_b64_test modp_b64w_test \
	modp_b85_test modp_burl_test \
	modp_bjavascript_gen modp_bjavascript_test \
	cxx_test \
	speedtest

modp_b16_gen_SOURCES = src/arraytoc.c src/modp_b16_gen.c
modp_b64_gen_SOURCES = src/arraytoc.c src/modp_b64_gen.c
modp_b85_gen_SOURCES = src/arraytoc.c src/modp_b85_gen.c
modp_burl_gen_SOURCES = src/arraytoc.c src/modp_burl_gen.c
modp_bjavascript_gen_SOURCES =  src/arraytoc.c src/modp_bjavascript_gen.c

speedtest_SOURCES = test/apr_base64.h test/apr_base64.c test/speedtest.c
speedtest_LDADD = libmodpbase64.la
speedtest_CFLAGS = $(CFLAGS) -Isrc

modp_b16_test_SOURCES = test/modp_b16_test.c cutest/CuTest.c
modp_b16_test_LDADD = libmodpbase64.la
modp_b16_test_CFLAGS = $(CFLAGS) -Isrc -Icutest

modp_b64_test_SOURCES = test/modp_b64_test.c cutest/CuTest.c
modp_b64_test_LDADD = libmodpbase64.la
modp_b64_test_CFLAGS = $(CFLAGS) -Isrc -Icutest

test/modp_b64w_test.c: test/modp_b64_test.c
	perl -p -i -e 's/b64/b64w/g' < test/modp_b64_test.c > test/modp_b64w_test.c

modp_b64w_test_SOURCES = test/modp_b64w_test.c cutest/CuTest.c
modp_b64w_test_LDADD = libmodpbase64.la
modp_b64w_test_CFLAGS = $(CFLAGS) -Isrc -Icutest

modp_b85_test_SOURCES = test/modp_b85_test.c cutest/CuTest.c
modp_b85_test_LDADD = libmodpbase64.la
modp_b85_test_CFLAGS = $(CFLAGS) -Isrc -Icutest

modp_burl_test_SOURCES = test/modp_burl_test.c cutest/CuTest.c
modp_burl_test_LDADD = libmodpbase64.la
modp_burl_test_CFLAGS = $(CFLAGS) -Isrc -Icutest

modp_bjavascript_test_SOURCES = test/modp_bjavascript_test.c cutest/CuTest.c
modp_bjavascript_test_LDADD = libmodpbase64.la
modp_bjavascript_test_CFLAGS = $(CFLAGS) -Isrc -Icutest

cxx_test_SOURCES = test/cxx_test.cc
cxx_test_LDADD = libmodpbase64.la
cxx_test_CXXFLAGS = $(CFLAGS) -Isrc

.PHONY: test doxy
test: modp_b16_test modp_b64_test modp_b85_test modp_burl_test modp_bjavascript_test speedtest
	./modp_b16_test
	./modp_b64_test
	./modp_b64w_test
	./modp_b85_test
	./modp_burl_test
	./modp_bjavascript_test
	./speedtest

