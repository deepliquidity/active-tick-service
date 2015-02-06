CC=g++ -std=c++0x
INCLUDES+= -I include -I src/util/ -I src -I src/session/ -I src/streamer -I src/socketIO -I src/messages/request/
LIBRARIES+= -lActiveTickServerAPI -lpthread
LIBRARYPATH+= lib/

all: createDir Util Session Request Streamer SocketIO Messages Main
	$(CC) -o ActiveTickService build/Util.o build/Session.o build/CConfig.o build/Streamer.o build/Request.o build/ServerSocket.o build/requestMessage.o build/quoteMessage.o build/Main.o -L $(LIBRARYPATH) $(LIBRARIES) -Xlinker -rpath -Xlinker $(LIBRARYPATH)

createDir:
	mkdir -p build

Main:
	$(CC) -c -o build/Main.o bin/Main.C $(INCLUDES)
Util:
	$(CC) -c -o build/Util.o src/util/Util.C $(INCLUDES)
	$(CC) -c -o build/CConfig.o src/util/CConfig.C $(INCLUDES)

Session:
	$(CC) -c -o build/Session.o src/session/Session.C $(INCLUDES)

Streamer:
	$(CC) -c -o build/Streamer.o src/streamer/Streamer.C $(INCLUDES)

Request:
	$(CC) -c -o build/Request.o src/request/Request.C $(INCLUDES)

SocketIO:
	$(CC) -c -o build/ServerSocket.o src/socketIO/ServerSocket.C $(INCLUDES)

Messages:
	$(CC) -c -o build/requestMessage.o src/messages/request/Message.C $(INCLUDES)
	$(CC) -c -o build/quoteMessage.o src/messages/response/QuoteMessage.C $(INCLUDES)
clean:
	rm -fr build/* ActiveTickService build
