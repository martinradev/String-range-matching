# config
MKDIR=mkdir -p
RM=rm -rf
CXX=g++
CPPFLAGS=-std=c++11

RBIN=rmatch
RDIR=src
RSRCS=rmatch.cpp mallocate.cpp

TBIN=test
TDIR=tests
TSRCS=TestSuite.cpp main.cpp

OUT=out
BINOUT=$(OUT)/bin
DEPEND=$(OUT)/depend

#

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

$(BINOUT)/$(RBIN): $(ROBJS) | $(BINOUT)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)
$(BINOUT)/$(TBIN): $(TOBJS) | $(BINOUT)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OUT)/%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(DEPEND): $(FULLSRCS) | $(DEPENDDIR)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -MM $(RFULLSRCS) | \
		sed -e 's/^\([^ ]\)/$(EROBJDIR)\/\1/g' | \
		sed -e 's/\([^\]\)$$/\1 | $(EROBJDIR)/g' > $@
	$(CXX) $(CFLAGS) $(CPPFLAGS) -MM $(TFULLSRCS) | \
		sed -e 's/^\([^ ]\)/$(ETOBJDIR)\/\1/g' | \
		sed -e 's/\([^\]\)$$/\1 | $(ETOBJDIR)/g' >> $@

# test data

TESTDATADIR=testcases

TESTDATA=dblp.xml.00001.1 dblp.xml.00001.2
# dna.001.1
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

# memtest

MEMTESTDIR=$(OUT)/memtest

.PHONY: memtest
memtest: $(RFULLBIN) $(LOCALTESTDATA) | $(MEMTESTDIR)
	valgrind --tool=massif --massif-out-file=$(MEMTESTDIR)/massif.out \
		--ignore-fn=mallocate --stacks=yes \
		$(RFULLBIN) -s -f $(TESTDATADIR)/dblp.xml.00001.1 asdf qwer

#valgrind --tool=massif --stacks=yes --massif-out-file=massif.out ./out/bin/rmatch -m count -f ./testcases/dblp.xml.00001.1.10000 asdf qwer

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


.PHONY: clean-testdata
clean-testdata:
	$(RM) $(TESTDATADIR)

.PHONY: clean
clean:
	$(RM) $(OUT)

include $(DEPEND)
