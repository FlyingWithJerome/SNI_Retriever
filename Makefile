CPP_OPT = -std=c++11 -g
GTEST = -lgtest_main  -lgtest
OPTIMIZATION = -O3
BOOST_FLAG = -lboost_thread -lboost_system
POSIX_FLAG = -lpthread -lrt

GPP = g++
GPP_MODULE = $(GPP) -c

.PHONY: clean

tls_parser : tls_parser.cpp
	$(GPP_MODULE) tls_parser.cpp $(CPP_OPT) $(OPTIMIZATION)

tls_test : tls_parser_test.cpp
	$(GPP_MODULE) tls_parser_test.cpp $(CPP_OPT) $(OPTIMIZATION)

test : tls_test tls_parser
	$(GPP) tls_parser_test.o tls_parser.o $(CPP_OPT) $(OPTIMIZATION) $(GTEST) -o test_main && ./test_main

tcp_connection : tcp_connection.cpp
	$(GPP_MODULE) tcp_connection.cpp $(CPP_OPT) $(OPTIMIZATION)

tcp_server : tcp_server.cpp
	$(GPP_MODULE) tcp_server.cpp $(CPP_OPT) $(OPTIMIZATION)

server_main : server_main.cpp
	$(GPP_MODULE) server_main.cpp $(CPP_OPT) $(OPTIMIZATION)

keyboard_interruption : keyboard_interruption.cpp
	$(GPP_MODULE) keyboard_interruption.cpp $(CPP_OPT) $(OPTIMIZATION)

tls_server : tcp_connection tcp_server tls_parser server_main keyboard_interruption
	$(GPP) tcp_connection.o tcp_server.o tls_parser.o server_main.o keyboard_interruption.o $(BOOST_FLAG) $(POSIX_FLAG) -o tls_server

clean :
	rm -rf *.o test_main