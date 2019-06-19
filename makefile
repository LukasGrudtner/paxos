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

# $(PROJECT_NAME): $(OBJ)
# 	@ echo "Building target using G++ compiler: $<"
# 	@ $(CXX) $^ -o $@ $(CXX_FLAGS) 
# 	@ echo "Finished building binary: $@"
# 	@ echo " "

acceptor: clean make_folders
	@ echo "Building target using G++ compiler: acceptor"
	@ $(CXX) -o acceptor src/acceptor.cpp src/socket/client/client_socket.cpp src/socket/server/connection.cpp src/paxos_component.cpp src/socket/server/server_socket.cpp src/socket/server/service.cpp $(CXX_FLAGS)

proposer: clean make_folders
	@ echo "Building target using G++ compiler: proposer"
	@ $(CXX) -o proposer src/proposer.cpp src/socket/client/client_socket.cpp src/socket/server/connection.cpp src/paxos_component.cpp src/socket/server/server_socket.cpp src/socket/server/service.cpp $(CXX_FLAGS)

learner: clean make_folders
	@ echo "Building target using G++ compiler: learner"
	@ $(CXX) -o learner src/learner.cpp src/socket/client/client_socket.cpp src/socket/server/connection.cpp src/paxos_component.cpp src/socket/server/server_socket.cpp src/socket/server/service.cpp $(CXX_FLAGS)

run:
	@ ./$(PROJECT_NAME)

./obj/%.o: ./src/%.cpp ./include/%.h
	@ echo "Building target using G++ compiler: $<"
	@ $(CXX) $< $(CXX_FLAGS) -c -o $@
	@ echo " "

# ./obj/main.o: ./src/main.cpp $(H_SRC)
# 	@ echo "Building target using G++ compiler: $<"
# 	@ $(CXX) $< $(CXX_FLAGS) -c -o $@
# 	@ echo " "

clean:
	@ $(RM) ./obj/* ./obj *~

debug:
	@ gdb ./$(PROJECT_NAME)

make_folders:
	@ mkdir -p obj

.PHONY: all run clean debug