# config
MKDIR=mkdir -p
RM=rm -rf
CXX=g++
CPPFLAGS=-g -O2 -std=c++11

RBIN=rmatch
RDIR=src
RSRCS=main.cpp Util.cpp

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

.PHONY: clean
clean:
	rm -rf $(OUT)

include $(DEPEND)
