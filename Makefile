CC=$(CXX)
CFLAGS=-Wall -Werror -Wextra -pedantic -pedantic-errors -std=c++11 -g3 -O0

# Modified to build against the Poco library installed with/by hyrax-dependencies.
# The env var $prefix must be set. Note my hacks for OSX 10.14.x and brew-installed
# openssl, which is not symlinked into /usr/local. jhrg 11/23/19

# MY_CFLAGS=-I/usr/local/include -I/usr/include/openssl
MY_CFLAGS=-I/usr/local/opt/openssl/include
LDFLAGS=-L/usr/local/opt/openssl/lib

LIBS=$(LDFLAGS) -lcrypto

# -lPocoNet

all: awsv4

awsv4: awsv4.o main.o url_parser.o url_parser.h awsv4.h
	$(CXX) -o awsv4 main.o awsv4.o url_parser.o $(LIBS)

lib: awsv4.o
	$(CXX) awsv4.o url_parser.o -shared -o libawsv4.so

awsv4.o: awsv4.cc awsv4.h
	$(CXX) -fPIC $(CFLAGS) $(MY_CFLAGS) -c awsv4.cc

main.o: main.cc awsv4.h url_parser.h
	$(CXX) $(CFLAGS) $(MY_CFLAGS) -c main.cc

clean:
	rm -f *.o awsv4
