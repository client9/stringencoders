
SOURCES = \
	src/modp_b2.c src/modp_b2.h \
	src/modp_b16.c src/modp_b16.h modp_b16_data.h \
	src/modp_b64.c src/modp_b64.h modp_b64_data.h \
	src/modp_b64w.c src/modp_b64w.h modp_b64w_data.h \
	src/modp_b85.h src/modp_b85.c modp_b85_data.h \
	src/modp_burl.h src/modp_burl.c modp_burl_data.h \
	src/modp_bjavascript.h src/modp_bjavascript.c \
	src/modp_numtoa.h src/modp_numtoa.c \
	src/modp_ascii.h src/modp_ascii.c modp_ascii_data.h

lib_LTLIBRARIES = libmodpbase64.la
libmodpbase64_la_SOURCES = ${SOURCES}

libmodpbase64_la_DEPENDENCIES = \
	modp_b2_data.h modp_b2_gen \
	modp_b16_data.h modp_b16_gen \
	modp_b64_data.h modp_b64w_data.h modp_b64_gen \
	modp_b85_data.h modp_b85_gen \
	modp_burl_data.h modp_burl_gen \
	modp_bjavascript_data.h modp_bjavascript_gen \
	modp_ascii_data.h modp_ascii_gen

include_HEADERS = \
	src/modp_b16.h src/modp_b64.h src/modp_b64w.h \
	src/modp_b85.h src/modp_burl.h src/modp_bjavascript.h \
	src/modp_numtoa.h src/modp_ascii.h src/modp_b2.h

CLEANFILES = *.log \
	*~ src/*~ test/*~ \
	*.tmp src/*.tmp test/*.tmp \
	doxy/html/* \
	*.gcda *.gcno *.gcov stringencoders.info \
	modp_b16_data.h modp_b64_data.h modp_b85_data.h modp_burl_data.h modp_bjavascript_data.h \
	modp_b64w_data.h src/modp_b64w.c test/modp_b64w_test.c \
	modp_ascii_data.h modp_b2_data.h

clean:
	rm -rf autom4te.cache
	rm -rf lcov-html

src/modp_b2.c: src/modp_b2.h modp_b2_data.h

src/modp_b16.c: src/modp_b16.h modp_b16_data.h

src/modp_b64.c: src/modp_b64.h modp_b64_data.h

src/modp_b64w.c: src/modp_b64.c src/modp_b64w.h modp_b64w_data.h
	perl -p -i -e 's/b64/b64w/g' < src/modp_b64.c > src/modp_b64w.c

src/modp_b85.c: src/modp_b85.h modp_b85_data.h

src/modp_burl.c: src/modp_burl.h modp_burl_data.h

src/modp_bjavascript.c: src/modp_bjavascript.h modp_bjavascript_data.h

src/modp_ascii.c: src/modp_ascii.h modp_ascii_data.h

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

modp_b85_data.h: modp_b85_gen
	./modp_b85_gen > modp_b85_data.h

modp_burl_data.h: modp_burl_gen
	./modp_burl_gen > modp_burl_data.h 

modp_bjavascript_data.h: modp_bjavascript_gen
	./modp_bjavascript_gen > modp_bjavascript_data.h 

noinst_PROGRAMS = \
	modp_b2_gen modp_b16_gen modp_b64_gen modp_b85_gen modp_burl_gen modp_ascii_gen \
	modp_b2_test \
	modp_b16_test modp_b64_test modp_b64w_test \
	modp_b85_test modp_burl_test \
	modp_bjavascript_gen modp_bjavascript_test \
	modp_numtoa_test modp_ascii_test \
	cxx_test \
	speedtest speedtest_numtoa speedtest_ascii

modp_b2_gen_SOURCES = src/arraytoc.c src/modp_b2_gen.c
modp_b16_gen_SOURCES = src/arraytoc.c src/modp_b16_gen.c
modp_b64_gen_SOURCES = src/arraytoc.c src/modp_b64_gen.c
modp_b85_gen_SOURCES = src/arraytoc.c src/modp_b85_gen.c
modp_burl_gen_SOURCES = src/arraytoc.c src/modp_burl_gen.c
modp_ascii_gen_SOURCES = src/arraytoc.c src/modp_ascii_gen.c
modp_bjavascript_gen_SOURCES = src/arraytoc.c src/modp_bjavascript_gen.c

speedtest_SOURCES = test/apr_base64.h test/apr_base64.c test/speedtest.c
speedtest_LDADD = libmodpbase64.la

speedtest_numtoa_SOURCES = test/speedtest_numtoa.c
speedtest_numtoa_LDADD = libmodpbase64.la

test/speedtest_ascii.c: modp_ascii_data.h
speedtest_ascii_SOURCES = test/speedtest_ascii.c 
speedtest_ascii_LDADD = libmodpbase64.la

modp_b2_test_SOURCES = test/modp_b2_test.c
modp_b2_test_LDADD = libmodpbase64.la

modp_b16_test_SOURCES = test/modp_b16_test.c
modp_b16_test_LDADD = libmodpbase64.la

modp_b64_test_SOURCES = test/modp_b64_test.c
modp_b64_test_LDADD = libmodpbase64.la

test/modp_b64w_test.c: test/modp_b64_test.c
	perl -p -i -e 's/b64/b64w/g' < test/modp_b64_test.c > test/modp_b64w_test.c

modp_b64w_test_SOURCES = test/modp_b64w_test.c
modp_b64w_test_LDADD = libmodpbase64.la

modp_b85_test_SOURCES = test/modp_b85_test.c
modp_b85_test_LDADD = libmodpbase64.la

modp_burl_test_SOURCES = test/modp_burl_test.c
modp_burl_test_LDADD = libmodpbase64.la

modp_bjavascript_test_SOURCES = test/modp_bjavascript_test.c
modp_bjavascript_test_LDADD = libmodpbase64.la

modp_numtoa_test_SOURCES = test/modp_numtoa_test.c
modp_numtoa_test_LDADD = libmodpbase64.la

modp_ascii_test_SOURCES = test/modp_ascii_test.c
modp_ascii_test_LDADD = libmodpbase64.la

cxx_test_SOURCES = test/cxx_test.cc
cxx_test_LDADD = libmodpbase64.la

.PHONY: test unittest perftest doxy lcov-html help
help:
	@echo ""
	@echo "make -- make everything"
	@echo "make install -- install everything"
	@echo "make test -- run unit and performance tests"
	@echo "make unittest -- run only unit tests"
	@echo "make perftest -- run only performance tests"
	@echo "make doxy -- generate doxygen output"
	@echo "make lcov-html -- make test + generate lcov html output"
	@echo ""

lcov-html:
	lcov -b . --directory . --zerocounters
	@make unittest
	lcov -b . --directory . --capture --output-file stringencoders.info
	lcov -b . --directory . --remove stringencoders.info '*arraytoc.c' -o stringencoders.info
	lcov -b . --directory . --remove stringencoders.info '*test*' -o stringencoders.info
	lcov -b . --directory . --remove stringencoders.info '*_gen.c' -o stringencoders.info
	lcov -b . --directory . --remove stringencoders.info '/usr/include*' -o stringencoders.info
	rm -rf lcov-html
	mkdir lcov-html
	genhtml -o lcov-html stringencoders.info

test: unittest speedtest

unittest: modp_b16_test modp_b64_test modp_b85_test modp_burl_test modp_bjavascript_test modp_numtoa_test modp_ascii_test cxx_test
	@find . -name '*da' | xargs rm -f
	@./modp_b16_test
	@./modp_b64_test
	@./modp_b64w_test
	@./modp_b85_test
	@./modp_b2_test
	@./modp_burl_test
	@./modp_bjavascript_test
	@./modp_numtoa_test
	@./modp_ascii_test
	@./cxx_test

perftest: speedtest speedtest_numtoa speedtest_ascii
	./speedtest
	./speedtest_numtoa
	./speedtest_ascii

.PHONY: doxy

doxy: ${SOURCES} doxy/header.html doxy/footer.html Doxyfile
	doxygen Doxyfile

