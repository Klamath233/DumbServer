VPATH = Response:Connection

DumbServer: Run.o ConnectionHandler.o ResponseGenerator.o RequestParser.o
	g++ -o $@ Run.o ConnectionHandler.o ResponseGenerator.o RequestParser.o

Run.o: Run.cpp
	g++ -c $<

ConnectionHandler.o: ConnectionHandler.cpp
	g++ -c $<

ResponseGenerator.o: ResponseGenerator.cpp
	g++ -c $<

RequestParser.o: RequestParser.cpp
	g++ -c $<

.PHONY:
clean:
	rm -rf *.o *.gch DumbServer
