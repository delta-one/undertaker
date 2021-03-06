# Add .d to Make's recognized suffixes.
SUFFIXES += .d

#We don't need to clean up when we're making these targets
NODEPS:=clean clean-check

CFLAGS = -Wall -Wextra -O2
CPPFLAGS = -I $(CURDIR)/../scripts/kconfig -I $(CURDIR)/../picosat
CXXFLAGS = $(CFLAGS) -std=gnu++11

# use g++ for linking, will automaticly use "-lstdc++ -lm" libraries
CC = g++

LDFLAGS =
BOOST_LIBS = -lboost_system -lboost_regex -lboost_filesystem -lboost_thread
LDLIBS = $(BOOST_LIBS) -lPuma

# LDCOV = -coverage
ifdef LDCOV
	LDLIBS += $(LDCOV)
endif

# DEBUG = -g3 -DDEBUG
# to compile with debug, compile like this:
# make DEBUG="-g3 -DDEBUG"
ifdef DEBUG
	CFLAGS += $(DEBUG)
endif

PARSEROBJ = KconfigWhitelist.o Logging.o Tools.o \
		BoolExpLexer.o BoolExpParser.o BoolExpSymbolSet.o BoolExpSimplifier.o \
		BoolExpGC.o bool.o CNFBuilder.o PicosatCNF.o \
		ConditionalBlock.o PumaConditionalBlock.o RsfReader.o ModelContainer.o \
		ConfigurationModel.o RsfConfigurationModel.o CnfConfigurationModel.o \
		BlockDefectAnalyzer.o CoverageAnalyzer.o SatChecker.o

SATYROBJ = KconfigWhitelist.o Logging.o Tools.o \
		BoolExpLexer.o BoolExpParser.o BoolExpSymbolSet.o BoolExpSimplifier.o \
		BoolExpGC.o bool.o CNFBuilder.o PicosatCNF.o \
		ExpressionTranslator.o SymbolTranslator.o SymbolTools.o SymbolParser.o \
		KconfigAssumptionMap.o

PROGS = undertaker predator rsf2cnf satyr
TESTPROGS = test-SatChecker test-ConditionalBlock test-ConfigurationModel \
            test-Bool test-CNFBuilder test-BoolExpSymbolSet test-PicosatCNF

DEPFILES:=$(patsubst %.o,%.d,$(PARSEROBJ) $(SATYROBJ)) undertaker.d satyr.d

all: $(PROGS) $(TESTPROGS)

undertaker: libparser.a ../picosat/libpicosat.a
rsf2cnf: libparser.a ../picosat/libpicosat.a
predator: predator.o PredatorVisitor.o
satyr: libsatyr.a zconf.tab.o ../picosat/libpicosat.a

../picosat/libpicosat.a:
	$(MAKE) -C .. picosat/libpicosat.a

../version.h:
	$(MAKE) -C .. version.h

undertaker.d: ../version.h

rsf2cnf.d: ../version.h

satyr.d: ../version.h

BoolExpLexer.d:

BoolExpParser.d:

BoolExpLexer.cpp: BoolExpParser.cpp BoolExpLexer.l
	flex -o $@ BoolExpLexer.l

BoolExpParser.cpp: BoolExpParser.y
	bison -o $@ --defines=BoolExpParser.h $<

stack.hh location.hh postion.hh: BoolExpParser.cpp

bool.o: bool.cpp bool.h BoolExpParser.o

libparser.a: $(DEPFILES) $(PARSEROBJ)
	ar r $@ $(PARSEROBJ)

libsatyr.a: $(DEPFILES) $(SATYROBJ)
	ar r $@ $(SATYROBJ)

zconf.tab.o:
	$(MAKE) -C $(CURDIR)/.. scripts/kconfig/dumpconf
	ln -sf ../scripts/kconfig/$@ $@

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    #Chances are, these files don't exist.  GMake will create them and
    #clean up automatically afterwards
    -include $(DEPFILES)
endif

#This is the rule for creating the dependency files
%.d: %.cpp
	 @$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MG $< > $@

clean: clean-check
	rm -rf *.o *.a *.gcda *.gcno *.d
	rm -rf location.hh stack.hh position.hh
	rm -rf BoolExpParser.cpp BoolExpParser.h BoolExpLexer.cpp
	rm -rf coverage-wl.cnf
	rm -rf $(PROGS) $(TESTPROGS)

test-%: test-%.cpp libparser.a ../picosat/libpicosat.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -g -O0 -o $@ $^ -lcheck -lrt -lpthread $(LDFLAGS) $(LDLIBS)


run-libcheck: $(TESTPROGS)
	@for t in $^; do echo "Executing test $$t"; ./$$t || exit 1; done

run-rsf2cnfcheck: rsf2cnf
	./rsf2cnf \
	    -m validation/coverage-wl.model \
	    -r validation/coverage-wl.rsf \
	    -W validation/coverage-wl.whitelist \
	    -B validation/coverage-wl.blacklist > coverage-wl.cnf
	@if ! diff -q coverage-wl.cnf validation/coverage-wl.cnf-ref; then \
	    diff -u validation/coverage-wl.cnf-ref coverage-wl.cnf; false; fi

# coverage analysis will create validation/sched.c.config*
run-coveragecheck: undertaker predator
	@$(MAKE) -C kconfig-dumps all
	./undertaker -j coverage -C simple_decision -m kconfig-dumps/models validation/sched.c
#	we expect between 5 and 30 solutions
	test `find validation -name 'sched.c.config*' | wc -l` -gt 5
	test `find validation -name 'sched.c.config*' | wc -l` -lt 30
	md5sum validation/sched.c.config* | awk '// {print $$1}' | sort | uniq -c \
		| awk '// { if ($$1 != 1) { print "duplicate solutions found" ; exit 1 } }' \
		|| md5sum validation/sched.c.config* | sort
	if grep -q '^CONFIG_IA64=y' validation/sched.c.config*; then echo "IA64 must not be enabled!"; false ; fi
	if grep -q '^CONFIG_CHOICE' validation/sched.c.config*; then echo "must not contain CONFIG_CHOICE*"; false ; fi

run-satyrcheck: satyr
	@cd validation-satyr && ./checkall.sh

check: $(PROGS)
	@$(MAKE) -s clean-check
	@$(MAKE) run-rsf2cnfcheck
	@$(MAKE) run-coveragecheck
	@$(MAKE) -C kconfig-dumps
	cd def-tests && env PATH=$(CURDIR):$(PATH) ./run-tests
	cd coverage-tests && env PATH=$(CURDIR):$(PATH) ./run-tests
	cd validation && env PATH=$(CURDIR):$(PATH) ./test-suite -t $$(getconf _NPROCESSORS_ONLN)
	@$(MAKE) -s run-libcheck
	@$(MAKE) -C validation-rsf2cnf check
	@$(MAKE) -s run-satyrcheck

clean-check:
	find coverage-tests validation/ \
	                 \( -name "*.c.output.expected" \
	                 -o -name "*.c.output.got" \
	                 -o -name "*.c.output.diff" \
	                 -o -name "*.c.error.expected" \
	                 -o -name "*.c.error.got" \
	                 -o -name "*.c.error.diff" \
	                 -o -name "*.c.config*" \
	                 -o -name "*.c.cppflags*" \
	                 -o -name "*.c.source*" \
	                 -o -name "*.dead" \
	                 -o -name "*.undead" \
	                 \) -exec rm {} \;

	rm -vf coverage-tests/coverage-cat.c.got
	@$(MAKE) -C validation-rsf2cnf clean

docs:
	doxygen

run-lcov:
	$(MAKE) -B DEBUG="-g -O0 -fprofile-arcs -ftest-coverage" LDCOV="-coverage -lgcov"
	rm -rf coverage-html ; mkdir coverage-html
	lcov --directory $(CURDIR) --zerocounters
	-$(MAKE) check
	lcov --directory $(CURDIR) --capture --output-file coverage-html/undertaker.info.in
	lcov -e coverage-html/undertaker.info.in "$(CURDIR)*" --output-file coverage-html/undertaker.info
	genhtml -o coverage-html coverage-html/undertaker.info

FORCE:
.PHONY: all clean clean-check FORCE run-% docs validation run-lcov run-coveragecheck
