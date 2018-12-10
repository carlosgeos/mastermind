CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)
CXX=mpic++
TARGET=mastermind

.PHONY: clean $(TARGET)

all: clean $(TARGET)

mastermind: main.cpp
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $<

transfer:
	scp ./* hydra:mastermind

run:
	mpirun $(TARGET)

submit:
	qsub $(TARGET).pbs

clean:
	-@rm $(TARGET)
