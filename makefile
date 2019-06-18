PROJECT_NAME=paxos

# .cpp files
CXX_SRC=$(wildcard ./src/*.cpp)

# .h files
H_SRC=$(wildcard ./include/*.h)

# object files
OBJ=$(subst .cpp,.o,$(subst src,obj,$(CXX_SRC)))

# compiler and linker
CXX=g++

# flags for compiler
CXX_FLAGS=-W                    \
          -Wall                 \
          -ansi                 \
          -pedantic             \
		  -Wno-unused-parameter \
          -std=c++11            \
          -lboost_system        \
          -lboost_filesystem    \
          -lboost_serialization \
          -lpthread


# command used at clean target
RM = rm -rf

# compilation and linking
all: make_folders $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ)
	@ echo "Building target using G++ compiler: $<"
	@ $(CXX) $^ -o $@ $(CXX_FLAGS) 
	@ echo "Finished building binary: $@"
	@ echo " "


run:
	@ ./$(PROJECT_NAME)

./obj/%.o: ./src/%.cpp ./include/%.h
	@ echo "Building target using G++ compiler: $<"
	@ $(CXX) $< $(CXX_FLAGS) -c -o $@
	@ echo " "

./obj/main.o: ./src/main.cpp $(H_SRC)
	@ echo "Building target using G++ compiler: $<"
	@ $(CXX) $< $(CXX_FLAGS) -c -o $@
	@ echo " "

clean:
	@ $(RM) ./obj/* ./obj *~

debug:
	@ gdb ./$(PROJECT_NAME)

make_folders:
	@ mkdir -p obj

.PHONY: all run clean debug