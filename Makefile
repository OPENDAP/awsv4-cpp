CC=$(CXX)
CFLAGS=-Wall -Werror -Wextra -pedantic -pedantic-errors -std=c++11 -g3 -O0

# Modified to build against the Poco library installed with/by hyrax-dependencies.
# The env var $prefix must be set. Note my hacks for OSX 10.14.x and brew-installed
# openssl, which is not symlinked into /usr/local. jhrg 11/23/19

# MY_CFLAGS=-I/usr/local/include -I/usr/include/openssl
MY_CFLAGS=-I$$prefix/deps/include -I/usr/local/Cellar/openssl@1.1/1.1.1d/include
LDFLAGS=-L$$prefix/deps/lib -L/usr/local/Cellar/openssl@1.1/1.1.1d/lib

LIBS=$(LDFLAGS) -lPocoFoundation -lcrypto

# -lPocoNet

all: awsv4

awsv4: awsv4.o main.o url.o url.h awsv4.h
	$(CXX) -o awsv4 main.o awsv4.o url.o $(LIBS)

lib: awsv4.o
	$(CXX) awsv4.o url.o -shared -o libawsv4.so

awsv4.o: awsv4.cc awsv4.h
	$(CXX) -fPIC $(CFLAGS) $(MY_CFLAGS) -c awsv4.cpp

main.o: main.cc awsv4.h url.h
	$(CXX) $(CFLAGS) $(MY_CFLAGS) -c main.cpp

clean:
	rm -f *.o awsv4 tokenizer
