#include <string>
#include <iostream>
#include "ResponseGenerator.h"

using namespace std;


/* For HANXI:
	use ResponseGenerator to create status and header info,
	  compile and try it by yourself and see the output
	  $ g++ *.cpp *.h
	  $ a.out 2>/dev/null
	try change the file you request and see different output
*/
int main()
{
	string req = "GET  /main.cspp   HTTP/1.1\r\n Host: 127.0.0.1\r\n Connection: close  \r\n User-Agent: Mozilla/5.0 \r\n  Accept-language:  us\r\n\r\n";
	ResponseGenerator r(req);
	cout << r.get_status_and_header() << endl;
	return 0;
}
