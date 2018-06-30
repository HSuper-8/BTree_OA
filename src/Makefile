IDIR = ../include
CXX = g++ -std=c++11
CXXFLAGS = -I$(IDIR) -g

ODIR = ../obj
LDIR = ../lib

_DEPS = indice.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = indice.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.gcno *~ core $(INCDIR)/*~ 
