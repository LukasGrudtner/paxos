g++ -o acceptor src/acceptor.cpp src/client_socket.cpp src/connection.cpp src/paxos_component.cpp src/server_socket.cpp src/service.cpp -W -ansi -pedantic -Wno-unused-parameter -std=c++11 -lboost_system -lboost_filesystem -lboost_serialization -lpthread