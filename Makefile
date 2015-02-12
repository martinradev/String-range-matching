MKDIR=mkdir -p
RM=rm -rf
CP=cp
CXX=g++
CPPSTD=-g -std=c++0x -I./include
CPPFLAGS=$(CPPSTD) -O2

RBIN=rmatch
RDIR=rmatch
RSRCS=rmatch.cpp mallocate.cpp timer.cpp

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

TFULLSRCS=$(addprefix $(TDIR)/,$(TSRCS))
TOBJDIR=$(OUT)/$(TDIR)
TOBJS=$(addprefix $(TOBJDIR)/,$(subst .cpp,.o,$(TSRCS)))
TFULLBIN=$(BINOUT)/$(TBIN)
ETOBJDIR=$(subst /,\/,$(TOBJDIR))

FULLSRCS=$(RFULLSRCS) $(TFULLSRCS)
DEPENDDIR=$(dir $(DEPEND))

.PHONY: all
all: $(RFULLBIN) $(TFULLBIN)

.PHONY: $(RBIN)
$(RBIN): $(RFULLBIN)
.PHONY: $(TBIN)
$(TBIN): $(TFULLBIN)
	$(TFULLBIN)

SIMPLETEST=simple_test.txt
SIMPLETESTSRC=$(TDIR)/$(SIMPLETEST)
SIMPLETESTDST=$(BINOUT)/$(SIMPLETEST)

$(SIMPLETESTDST): $(SIMPLETESTSRC) | $(BINOUT)
	$(CP) $< $@

$(BINOUT)/$(RBIN): $(ROBJS) | $(BINOUT)
	$(CXX) -o $@ $^ $(LDLIBS)
$(BINOUT)/$(TBIN): $(TOBJS) | $(BINOUT) $(SIMPLETESTDST)
	$(CXX) -o $@ $^ $(LDLIBS)

# disable optimization for this file
$(ROBJDIR)/mallocate.o: $(RDIR)/mallocate.cpp | $(ROBJDIR)
	$(CXX) -c $(CPPSTD) $(CFLAGS) -o $@ $<

$(OUT)/%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

FIXDEP=bash ./scripts/fix_depend.sh

$(DEPEND): $(FULLSRCS) | $(DEPENDDIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -MM $(RFULLSRCS) | $(FIXDEP) "$(EROBJDIR)" > $@
	$(CXX) $(CPPFLAGS) $(CFLAGS) -MM $(TFULLSRCS) | $(FIXDEP) "$(ETOBJDIR)" >> $@

# test data

TESTDATADIR=$(OUT)/testcases

TESTDATA=dblp.xml.00001.1 dblp.xml.00001.2
TESTDATAURL=http://pizzachili.dcc.uchile.cl/repcorpus/pseudo-real/
ZIPSUFFIX=.7z

TESTDATAZIP=$(addsuffix $(ZIPSUFFIX),$(TESTDATA))
REMOTETESTDATA=$(addprefix $(TESTDATAURL)/,$(TESTDATAZIP))
LOCALTESTDATA=$(addprefix $(TESTDATADIR)/,$(TESTDATA))
LOCALTESTDATAZIP=$(addprefix $(TESTDATADIR)/,$(TESTDATAZIP))

.PHONY: testdata
testdata: $(LOCALTESTDATA)

$(LOCALTESTDATAZIP): %: | $(TESTDATADIR)
	cd $(TESTDATADIR) && curl -O $(TESTDATAURL)$(notdir $@)

$(LOCALTESTDATA): %: %$(ZIPSUFFIX)
	7z x -o$(TESTDATADIR) $@$(ZIPSUFFIX)
	touch $@

.PHONY: clean-testdata
clean-testdata:
	$(RM) $(TESTDATADIR)

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
$(DEPENDDIR):
	$(MKDIR) $@
$(ROBJDIR):
	$(MKDIR) $@
$(TOBJDIR):
	$(MKDIR) $@
$(TESTDATADIR):
	$(MKDIR) $@
$(MEMTESTDIR):
	$(MKDIR) $@

.PHONY: clean
clean:
	$(RM) $(OUT)

include $(DEPEND)
