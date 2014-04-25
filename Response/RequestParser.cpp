#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "RequestParser.h"

using namespace std;

RequestParser::RequestParser()
{

}

RequestParser::RequestParser(string s):request(s),parseError(0),validity(0)
{
	parse();
	check_validity(false);
}

bool RequestParser::error() const
{
	if(parseError || validity)
		return true;
	else
		return false;
}

void RequestParser::parse()
{
	string blank = " \t"; // remove tab or space
	string second_parse = "";
	istringstream buf(request);
	for(string token;getline(buf,token,'\r');)
	{
		second_parse += token;
	}
	istringstream buf2(second_parse);
	for(string token2;getline(buf2,token2,'\n');)
		lines.push_back(token2);
	lines.pop_back(); // delete last element :'\n'
	for(unsigned int i=0;i<lines.size();i++)
	{
		istringstream buf(lines[i]);
		if(i==0)
		{
			for(string token;getline(buf,token,' ');)
			{
				for(unsigned int k=0;k<blank.size();k++)
				{
					token.erase(remove(token.begin(),token.end(),blank[k]),token.end());
				}
				if (token != "")
					headers.push_back(token);
			}
		}
		else
		{
			for(string token;getline(buf,token,':');)
			{	
				for(unsigned int k=0;k<blank.size();k++)
				{
					token.erase(remove(token.begin(),token.end(),blank[k]),token.end());
				}
				if (token != "")
					headers.push_back(token);
			}
		}
	}
	return;
}


/* 
Check validity
Sample valid parse result:

0.0   GET
0.1   /hi.html
0.2   HTTP/1.1
1.0   Host
1.1   127.0.0.1
2.0   Connection
2.1   close
3.0   User-Agent
3.1   Mozilla/5.0
4.0   Accept-language
4.1   us

1. There should be 11 lines
2. 
*/
void RequestParser::check_validity(bool debug=false)
{
	if (11 != headers.size())
	{
		cerr << "* [fail]Invalid header size" << endl;
		validity = 1;
		return;
	}
	else
		if(debug) cout << "* [pass]Passed header size test" << endl;
	for(int i=0;i<headers.size();i++)
	{
		switch(i)
		{
			// check method
			case 0: 
			{
				if(0!=headers[0].compare("GET"))
				{
					cerr << headers[0] + " should be GET" << endl;
					validity = 1;
					return;
				}
				if(debug) cout << "* [pass]Passed method test" << endl;
				method = headers[0];
				break;
			}
			case 1:
			{
				// check if Request-URI starts with slash
				if(headers[1][0] != '/')
				{
					cerr << "* [fail]Request-URI should start with /. Client is doing something nasty!" << endl;
					validity = 1;
					return;
				}
				vector<string> file = split(headers[1],'.');
				if(file.size() != 2)
				{
					cerr << "* [fail]Invalid file name" << endl;
					validity = 1;
					return;
				}
				else
				{
					filetype = file[1];
					if (0 == filetype.compare("html")||
						0 == filetype.compare("txt"))
					{
						filegenre = "text";
					}
					else if (0 == filetype.compare("jpeg")||
						0 == filetype.compare("jpg") ||
						0 == filetype.compare("gif") )
					{
						filegenre = "img";
					}
					else
					{
						filegenre = "Unknown";
					}
				}
				if(debug) cout << "* [pass]Passed filename test" << endl;
				filename = headers[1];
				break;
			}
			case 2:
			{
				vector<string> httpversion = split(headers[2],'/');
				if(  httpversion.size() == 2 && 
					 0 == httpversion[0].compare("HTTP") &&
					 (0 == httpversion[1].compare("1.1") ||  0 == httpversion[1].compare("1.0"))
				  )
				{
					// do nothing

				}
				else
				{
					cerr << "* [fail]Invalid http version" << endl;
					validity = 1;
					return;
				}
				if(debug) 
					cout << "* [pass]Passed http version test" << endl;
				HTTPversion = headers[2];
				break;

			}
			case 3:
			{
				if (0 != headers[3].compare("Host"))
				{
					cerr << "* [fail]Invalid HOSTNAME" << endl;
					validity = 1;
					return;
				}
				else
					if(debug) cout << "* [pass]Passed HOSTNAME test" << endl;
				break;
			}
			case 4:
			{
				// i do not know what should check for this
				if(debug) cout << "* [pass]Passed host detail test" << endl;
				Host = headers[4];
				break;
			}
			case 5:
			{
				if (0 != headers[5].compare("Connection"))
				{
					cerr << "* [fail]Invalid Connection" << endl;
					validity = 1;
					return;
				}
				else
					if(debug) cout << "* [pass]Passed Connection test" << endl;
				break;

			}
			case 6:
			{
				if(debug) cout << "* [pass]Passed connection detail test" << endl;
				Connection = headers[6];
				break;
			}
			case 7:
			{
				if (0 != headers[7].compare("User-Agent"))
				{
					cerr << "* [fail]Invalid User-Agent" << endl;
					validity = 1;
					return;
				}
				else
					if(debug) cout << "* [pass]Passed User-Agent test" << endl;
				break;

			}
			case 8:
			{
				if(debug) cout << "* [pass]Passed User-Agent detail test" << endl;
				UserAgent = headers[8];
				break;
			}

			case 9:
			{
				if (0 != headers[i].compare("Accept-language"))
				{
					cerr << "* [fail]Invalid Accept-language" << endl;
					validity = 1;
					return;
				}
				else
					if(debug) cout << "* [pass]Passed Accept-language test" << endl;
				break;
			}

			case 10:
			{
				if(debug) cout << "* [pass]Passed Accept-language detail test" << endl;
				Aclang = headers[10];
				break;
			}

			default:
				break;
		}
	}

	if(debug) cout << "*** ALL TESTS PASSED ***" << endl;
	return;

}

string RequestParser::get_method()  const
{
	return method;
}
string RequestParser::get_filename() const
{
	return filename;
}
string RequestParser::get_HTTPversion() const
{
	return HTTPversion;
}

string RequestParser::get_Host() const
{ 
	return Host;
}
string RequestParser::get_Connection() const
{
	return Connection;
}
string RequestParser::get_UserAgent() const
{
	return UserAgent;
}
string RequestParser::get_Aclang() const 
{
	return Aclang;
}

string RequestParser::get_filetype() const
{
	return filetype;
}

string RequestParser::get_filegenre() const
{
	return filegenre;
}

string RequestParser::get_Connection_Type() const
{
	return Connection_Type;
}

vector<string> RequestParser::get_lines() const
{
	return lines;
}

vector<string> RequestParser::get_headers() const
{
	return headers;
}
// Print formatted
void RequestParser::print_lines() const
{	
	cout << "=== lines ===" << endl;
	for(unsigned int i=0;i<lines.size();i++)
	{
		ostringstream ss;
		ss<<i;
		cout << ss.str()+". "+ lines[i] << endl;
	}
	cout << "=== lines ===\n" << endl;
}

void RequestParser::print_headers() const
{
	cout << "\n=== headers ===" << endl;
	for(unsigned int i=0;i<headers.size();i++)
	{
		ostringstream ss;
		ss << i;
		cout << ss.str() + "   " +headers[i]<< endl;
	}
	cout << "=== headers ===\n" << endl;
}


vector<string> RequestParser::split(const string& s, const char& splitter){
	vector<string> ret;
	istringstream buf(s);
	for(string token;getline(buf,token,splitter);)
		ret.push_back(token);
	return ret;
}









