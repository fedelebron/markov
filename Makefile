CXXFLAGS = -std=c++11

SRCS = Markov.cpp main.cpp
OBJS = Markov.o main.o

Markov: $(OBJS)
	$(CXX) $(CXXFLAGS) -o Markov $(OBJS)

depend: .depend

.depend: $(SRCS)
	rm -f .depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(OBJS) Markov

include .depend
