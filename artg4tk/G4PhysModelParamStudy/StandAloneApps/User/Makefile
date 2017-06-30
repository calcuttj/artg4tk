CXX = g++
CXXFLAGS = `$(ROOTSYS)/bin/root-config --cflags --glibs`
LDLIBS = `$(ROOTSYS)/bin/root-config --libs`

# --> NOT in Root6 LDLIBS += -lCintex -lReflex
LDLIBS += $(MRB_BUILDDIR)/artg4tk/lib/libartg4tk_G4PhysModelParamStudy_AnalysisProducts_dict.so
LDLIBS += $(MRB_BUILDDIR)/artg4tk/lib/libartg4tk_G4PhysModelParamStudy_AnalysisProducts.so

BINARIES = PlotBertiniVariants WeightParams CreateCovMtx test
TARGETS = PlotBertiniVariants.o WeightParams.o CreateCovMtx.o test.o 

#--- if using 'make all' ---#
all : $(TARGETS)

#--- if making individual targets ---#

PlotBertiniVariants.o : PlotBertiniVariants.cxx
	$(CXX) $(CXXFLAGS) -I$(MRB_SOURCE)/artg4tk -o $*.o -c $*.cxx #compile
	$(CXX) -o $* $*.o $(LDLIBS) $(CXXFLAGS)       #link

WeightParams.o : WeightParams.cxx
	$(CXX) $(CXXFLAGS) -I$(MRB_SOURCE)/artg4tk -o $*.o -c $*.cxx #compile
	$(CXX) -o $* $*.o $(LDLIBS) $(CXXFLAGS)       #link

CreateCovMtx.o : CreateCovMtx.cxx
	$(CXX) $(CXXFLAGS) -I$(MRB_SOURCE)/artg4tk -o $*.o -c $*.cxx #compile
	$(CXX) -o $* $*.o $(LDLIBS) $(CXXFLAGS)       #link

test.o : test.cxx
	$(CXX) $(CXXFLAGS)  -o $*.o -c $*.cxx #compile
	$(CXX) -o $* $*.o $(LDLIBS) $(CXXFLAGS)       #link

clean:
	rm -f $(BINARIES) $(TARGETS)
