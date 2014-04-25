#ifndef PARSE_REQUEST_H
#define PARSE_REQUEST_H

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

using namespace std;



class RequestParser
{
	public:
		RequestParser(string s);
		void parse();
		void check_validity(bool debug);


		string get_filename();
		string get_HTTPversion();
		string get_Host();
		string get_Connection();
		string get_UserAgent();
		string get_Aclang();

		string get_filetype();
		string get_Connection_Type();

		vector<string> get_lines();
		vector<string> get_headers();
		void print_lines();
		void print_headers();


		vector<string> split(const string& s, const char& splitter);

	private:
		int parseError;
		int validity;

		/* Basic data structure */
		vector<string> lines;
		vector<string> headers;
		string request;

		/* Basic headers */
		string method; // GET
		string filename; // /hi.html
		string HTTPversion; // HTTP/1.1
		string Host; // 127.0.0.1
		string Connection; // Close
		string UserAgent; // Mozilla/5.0
		string Aclang; // us

		/* Additional info */
		string filetype;
		string Connection_Type;
};

#endif