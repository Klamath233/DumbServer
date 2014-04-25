#ifndef _PARSE_REQUEST_H_
#define _PARSE_REQUEST_H_

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
		RequestParser();
		RequestParser(string s);

		bool error() const;
		void parse();
		void check_validity(bool debug);

		string get_method() const;
		string get_filename() const;
		string get_HTTPversion()const;
		string get_Host()const;
		string get_Connection()const;
		string get_UserAgent()const;
		string get_Aclang()const;

		string get_filetype()const;
		string get_filegenre() const;
		string get_Connection_Type()const;

		vector<string> get_lines()const;
		vector<string> get_headers()const;
		void print_lines() const;
		void print_headers() const;


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
		string filegenre;
		string Connection_Type;
};

#endif