SHELL=/bin/bash
MKDIR=mkdir -p
RM=rm -rf
CP=cp
CXX=g++
CPPSTD=-g -std=c++0x -I./include
CPPFLAGS=$(CPPSTD) -O2

RBIN=rmatch
RDIR=rmatch
RSRCS=rmatch.cpp mallocate.cpp timer.cpp init.cpp algo_def.cpp

GBIN=genfib
GDIR=genfib
GSRCS=genfib.cpp

TBIN=test
TDIR=test
TSRCS=TestSuite.cpp main.cpp ChrochemoreTest.cpp SuffixArrayTest.cpp \
			TestGenerator.cpp TestSuite.cpp ZAlgorithmTest.cpp \
			gs_count_test.cpp kmp_match_test.cpp naive_match_test.cpp

OUT=out
BINOUT=$(OUT)/bin
DEPEND=$(OUT)/depend

RFULLSRCS=$(addprefix $(RDIR)/,$(RSRCS))
ROBJDIR=$(OUT)/$(RDIR)
ROBJS=$(addprefix $(ROBJDIR)/,$(subst .cpp,.o,$(RSRCS)))
RFULLBIN=$(BINOUT)/$(RBIN)
EROBJDIR=$(subst /,\/,$(ROBJDIR))
RMATCH=$(RFULLBIN)

GFULLSRCS=$(addprefix $(GDIR)/,$(GSRCS))
GOBJDIR=$(OUT)/$(GDIR)
GOBJS=$(addprefix $(GOBJDIR)/,$(subst .cpp,.o,$(GSRCS)))
GFULLBIN=$(BINOUT)/$(GBIN)
EGOBJDIR=$(subst /,\/,$(GOBJDIR))

TFULLSRCS=$(addprefix $(TDIR)/,$(TSRCS))
TOBJDIR=$(OUT)/$(TDIR)
TOBJS=$(addprefix $(TOBJDIR)/,$(subst .cpp,.o,$(TSRCS)))
TFULLBIN=$(BINOUT)/$(TBIN)
ETOBJDIR=$(subst /,\/,$(TOBJDIR))

FULLSRCS=$(RFULLSRCS) $(TFULLSRCS) $(GFULLSRCS)

.PHONY: all
all: $(RFULLBIN) $(TFULLBIN) $(GFULLBIN)

.PHONY: help
help:
	@echo 'usage: $(MAKE) target'
	@echo 'All intermediate and produced files are placed in directory ./out/'
	@echo ''
	@echo 'Targets:'
	@echo '  help       display this help'
	@echo '  all        build rmatch and compile and run tests (default target)'
	@echo '  rmatch     build command line utility'
	@echo '  test       compile and run tests'
	@echo '  clean      delete all produced files'

.PHONY: $(RBIN)
$(RBIN): $(RFULLBIN)
.PHONY: $(TBIN)
$(TBIN): $(TFULLBIN)
	$(TFULLBIN)
.PHONY: $(GBIN)
$(GBIN): $(GFULLBIN)

SIMPLETEST=simple_test.txt
SIMPLETESTSRC=$(TDIR)/$(SIMPLETEST)
SIMPLETESTDST=$(BINOUT)/$(SIMPLETEST)

$(SIMPLETESTDST): $(SIMPLETESTSRC) | $(BINOUT)
	$(CP) $< $@

$(RFULLBIN): $(ROBJS) | $(BINOUT)
	$(CXX) -o $@ $^ $(LDLIBS) -lboost_regex
$(TFULLBIN): $(TOBJS) | $(BINOUT) $(SIMPLETESTDST)
	$(CXX) -o $@ $^ $(LDLIBS)
$(GFULLBIN): $(GOBJS) | $(BINOUT)
	$(CXX) -o $@ $^ $(LDLIBS)

# disable optimization for this file
$(ROBJDIR)/mallocate.o: $(RDIR)/mallocate.cpp | $(ROBJDIR)
	$(CXX) -c $(CPPSTD) $(CFLAGS) -o $@ $<

$(OUT)/%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

FIXDEP=bash ./scripts/fix_depend.sh

$(DEPEND): $(FULLSRCS) | $(OUT)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -MM $(RFULLSRCS) | $(FIXDEP) "$(EROBJDIR)" > $@
	$(CXX) $(CPPFLAGS) $(CFLAGS) -MM $(TFULLSRCS) | $(FIXDEP) "$(ETOBJDIR)" >> $@
	$(CXX) $(CPPFLAGS) $(CFLAGS) -MM $(GFULLSRCS) | $(FIXDEP) "$(EGOBJDIR)" >> $@

# test data

TESTDATADIR=$(OUT)/testdata

TESTFIB=$(TESTDATADIR)/fib
TESTDATA=influenza
TESTDATAURL=http://pizzachili.dcc.uchile.cl/repcorpus/real/
ZIPSUFFIX=.7z
CUTSUFFIX=.cut
PATTERNS=$(TESTDATADIR)/patterns
# 128 MiB
TESTSIZE=./scripts/testsize.sh
TESTEXP=$(shell $(TESTSIZE))
TESTN=$(shell echo "$$((2**$(TESTEXP)))")

PATTERNSIZE=./scripts/patternsize.sh
PATTERNEXP=$(shell $(PATTERNSIZE))

TESTDATAZIP=$(addsuffix $(ZIPSUFFIX),$(TESTDATA))
TESTDATACUT=$(addsuffix $(CUTSUFFIX),$(TESTDATA))
REMOTETESTDATA=$(addprefix $(TESTDATAURL)/,$(TESTDATAZIP))
LOCALTESTDATA=$(addprefix $(TESTDATADIR)/,$(TESTDATA))
LOCALTESTDATAZIP=$(addprefix $(TESTDATADIR)/,$(TESTDATAZIP))
LOCALTESTDATACUT=$(addprefix $(TESTDATADIR)/,$(TESTDATACUT))

.PHONY: testdata
testdata: $(LOCALTESTDATACUT) $(TESTFIB)

$(LOCALTESTDATAZIP): %: | $(TESTDATADIR)
	cd $(TESTDATADIR) && curl -O $(TESTDATAURL)$(notdir $@)

$(LOCALTESTDATA): %: %$(ZIPSUFFIX)
	7z x -o$(TESTDATADIR) $@$(ZIPSUFFIX)
	touch $@

$(LOCALTESTDATACUT): %$(CUTSUFFIX): % $(TESTSIZE)
	head -c $(TESTN) $< > $@

# generate fibonacci locally

$(TESTFIB): $(GFULLBIN) $(TESTSIZE) | $(TESTDATADIR)
	$< | head -c $(TESTN) > $@

.PHONY: patterns
patterns: $(PATTERNS)

POW=./scripts/pow.sh

$(PATTERNS): $(POW) $(PATTERNSIZE)
	$< $(PATTERNEXP) > $@

.PHONY: clean-testdata
clean-testdata:
	$(RM) $(TESTDATADIR)

.PHONY: clean-testdata-cut
clean-testdata-cut:
	$(RM) $(LOCALTESTDATACUT) $(TESTFIB)

.PHONY: clean-patterns
clean-patterns:
	$(RM) $(PATTERNS)

# performance test

PERFDIR=$(OUT)/perf
INFLUENZA=$(LOCALTESTDATACUT)
FIB=$(TESTFIB)

# Run tests silently with counting mode

.PHONY: perftest
perftest: perftest-n perftest-gs perftest-c perftest-z perftest-kmp

.PHONY: perftest-n perftest-gs perftest-c perftest-z perftest-kmp
perftest-n: perftest-fib-n perftest-inf-n
perftest-gs: perftest-fib-gs perftest-inf-gs
perftest-c: perftest-fib-c perftest-inf-c
perftest-z: perftest-fib-z perftest-inf-z
perftest-kmp: perftest-fib-kmp perftest-inf-kmp

PERFFIB=$(PERFDIR)/fib
PERFINF=$(PERFDIR)/influenza

PERFFIBN=$(PERFFIB).n
PERFFIBGS=$(PERFFIB).gs
PERFFIBC=$(PERFFIB).c
PERFFIBZ=$(PERFFIB).z
PERFFIBKMP=$(PERFFIB).kmp

PERFINFN=$(PERFINF).n
PERFINFGS=$(PERFINF).gs
PERFINFC=$(PERFINF).c
PERFINFZ=$(PERFINF).z
PERFINFKMP=$(PERFINF).kmp

.PHONY: perftest-fib-n
perftest-fib-n: $(PERFFIBN).1 $(PERFFIBN).2 $(PERFFIBN).3
.PHONY: perftest-inf-n
perftest-inf-n: $(PERFINFN).1 $(PERFINFN).2 $(PERFINFN).3

.PHONY: perftest-fib-gs
perftest-fib-gs: $(PERFFIBGS).1 $(PERFFIBGS).2 $(PERFFIBGS).3
.PHONY: perftest-inf-gs
perftest-inf-gs: $(PERFINFGS).1 $(PERFINFGS).2 $(PERFINFGS).3

.PHONY: perftest-fib-c
perftest-fib-c: $(PERFFIBC).1 $(PERFFIBC).2 $(PERFFIBC).3
.PHONY: perftest-inf-c
perftest-inf-c: $(PERFINFC).1 $(PERFINFC).2 $(PERFINFC).3

.PHONY: perftest-fib-z
perftest-fib-z: $(PERFFIBZ).1 $(PERFFIBZ).2 $(PERFFIBZ).3
.PHONY: perftest-inf-z
perftest-inf-z: $(PERFINFZ).1 $(PERFINFZ).2 $(PERFINFZ).3

.PHONY: perftest-fib-kmp
perftest-fib-kmp: $(PERFFIBKMP).1 $(PERFFIBKMP).2 $(PERFFIBKMP).3
.PHONY: perftest-inf-kmp
perftest-inf-kmp: $(PERFINFKMP).1 $(PERFINFKMP).2 $(PERFINFKMP).3

$(PERFFIBN).%: $(RMATCH) $(FIB) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(FIB) -ep $(PATTERNS) -cstm n | tee $@
$(PERFFIBGS).%: $(RMATCH) $(FIB) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(FIB) -ep $(PATTERNS) -cstm gs | tee $@
$(PERFFIBC).%: $(RMATCH) $(FIB) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(FIB) -ep $(PATTERNS) -cstm c | tee $@
$(PERFFIBZ).%: $(RMATCH) $(FIB) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(FIB) -ep $(PATTERNS) -cstm z | tee $@
$(PERFFIBKMP).%: $(RMATCH) $(FIB) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(FIB) -ep $(PATTERNS) -cstm kmp | tee $@

$(PERFINFN).%: $(RMATCH) $(INFLUENZA) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(INFLUENZA) -ep $(PATTERNS) -cstm n | tee $@
$(PERFINFGS).%: $(RMATCH) $(INFLUENZA) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(INFLUENZA) -ep $(PATTERNS) -cstm gs | tee $@
$(PERFINFC).%: $(RMATCH) $(INFLUENZA) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(INFLUENZA) -ep $(PATTERNS) -cstm c | tee $@
$(PERFINFZ).%: $(RMATCH) $(INFLUENZA) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(INFLUENZA) -ep $(PATTERNS) -cstm z | tee $@
$(PERFINFKMP).%: $(RMATCH) $(INFLUENZA) $(PATTERNS) | $(PERFDIR)
	$(RMATCH) -f $(INFLUENZA) -ep $(PATTERNS) -cstm kmp | tee $@

$(PERFDIR):
	$(MKDIR) $@

.PHONY: clean-perftest
clean-perftest:
	$(RM) $(PERFDIR)

# memtest

MEMTESTDIR=$(OUT)/memtest
MEMTESTPREFIX=massif.out.
MEMTESTOUT=$(addprefix $(MEMTESTDIR)/$(MEMTESTPREFIX),$(TESTDATA))

.PHONY: memtest
memtest: $(MEMTESTOUT)

MASSIFCMD=bash ./scripts/massif.sh

$(MEMTESTOUT): $(MEMTESTDIR)/$(MEMTESTPREFIX)%: $(RFULLBIN) $(TESTDATADIR)/% | \
		$(MEMTESTDIR)
	$(MASSIFCMD) $@ $(RFULLBIN) -m c -c 1000000 -s -f $< asdf qwer

.PHONY: clean-memtest
clean-memtest:
	$(RM) $(MEMTESTDIR)

$(OUT):
	$(MKDIR) $@
$(BINOUT):
	$(MKDIR) $@
$(ROBJDIR):
	$(MKDIR) $@
$(TOBJDIR):
	$(MKDIR) $@
$(GOBJDIR):
	$(MKDIR) $@
$(TESTDATADIR):
	$(MKDIR) $@
$(MEMTESTDIR):
	$(MKDIR) $@

.PHONY: clean
clean:
	$(RM) $(OUT)

ifeq ($(findstring help,$(MAKECMDGOALS)),)
include $(DEPEND)
endif
