OBJ = CGOL
SRCS = $(wildcard src/*.cpp) 
OBJS = ${SRCS:.cpp=.o} 

HEADERDIR = src/headers

CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -w -I$(HEADERDIR)
SDL = -lSDL2main -lSDL2 #-lSDL_image -lSDL_ttf

all: $(OBJS) 
	$(CXX) $(OBJS) $(SDL) -o $(OBJ)

%.o: %.cpp
	$(CXX) -c $^ $(CXXFLAGS) -o $@


clean:
	@echo ---- Cleaning ----
	find . -name "*.o" -type f -delete 
