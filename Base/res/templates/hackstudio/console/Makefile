PROGRAM = "__PROJECT_NAME__"
OBJS = main.o

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

clean:
	rm $(OBJS) $(PROGRAM)

run:
	./$(PROGRAM)
