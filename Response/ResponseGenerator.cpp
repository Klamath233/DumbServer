#include "ResponseGenerator.h"
#include "RequestParser.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string>
#include <stdio.h>


// http status code initialized to 400
ResponseGenerator::ResponseGenerator(string s):file_exist(0), 
file_readable(0), HTTP_status_code(400),status_and_header(""),filesize(0)
{
	rp = RequestParser(s);
	//rp.print_headers();
	determine_status(false);
	gen_base();
	//cout << status_and_header << endl;

	//
}


string ResponseGenerator::int_to_string(int num) const
{
	ostringstream ss;
	ss << num;
	return ss.str();
}



void ResponseGenerator::determine_status(bool debug=false)
{
	HTTP_status_code = 400; // assume request has syntax error
	/* if parse error or request format is not valid
	   set code to 404 and return */
	if (rp.error())
	{
		if(debug)cerr << "* [fail]Bad request!" << endl;
		return;
	}
	HTTP_status_code = 404; // file not found now
	string filename = "."+rp.get_filename();
	const char* fn_ptr = filename.c_str();

	if( access(fn_ptr,F_OK ) != -1 )
	{
		if(debug) 
			cout << "* [pass]file does exist" << endl;
		file_exist = 1;
		HTTP_status_code = 403; // file forbidden now
	}
	else
	{
		if(debug)cerr << "* [fail]File does NOT exist!" << endl;
		return;
	}
	if (access(fn_ptr,R_OK) != -1)
	{
		if(debug) 
			cout << "* [pass]file is readable" << endl;
		file_readable = 1;
		HTTP_status_code = 200; // file can be served now

	}
	else
	{
		if(debug)cerr << "* [fail]File is NOT readable!" << endl;
		return;
	}
	// Get GMT for modified time
	struct stat buffer;
	struct tm *file_mod_time;
	stat(fn_ptr,&buffer);
	filesize = buffer.st_size;
	file_mod_time = gmtime(&(buffer.st_mtime));
	modified_time = asctime(file_mod_time);
	
   
}

bool ResponseGenerator::allow_data_transfer() const
{
	return (1==file_exist)&&(1==file_readable)&&(200==HTTP_status_code);
}

/* 
	Reference: 
		[1] http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
		[2] Computer Network, A Top-Down Approache. Page 107, figure 2.9

*/
void ResponseGenerator::gen_base()
{
	status_and_header = "";
	// add version
	status_and_header += rp.get_HTTPversion()+" ";
	// add status code
	status_and_header += int_to_string(HTTP_status_code) + " ";
	// add phrase
	// code could be 400, 404, 403, 200
	if(200==HTTP_status_code)
	{
		status_and_header += "OK";
	}
	else if(400==HTTP_status_code)
	{
		status_and_header += "Bad Request";
	}
	else if(403==HTTP_status_code)
	{
		status_and_header += "Forbidden";
	}
	else
	{
		status_and_header += "Not Found";
	}
	status_and_header += "\r\n";
	// add connection type
	status_and_header += "Connection: "+rp.get_Connection_Type()+"\r\n";
	// add time when response were being created
	// first generate GMT for curren time
    time_t rawtime;
  	struct tm * timeinfo;
  	time ( &rawtime );
  	timeinfo = gmtime (&rawtime);
  	current_time = asctime (timeinfo);
	string ct(current_time);
	status_and_header += "Date: " + ct.substr(0,ct.size()-1) + " GMT\r\n";
	// add server type
	status_and_header += "Server: DumbServer/2.3.3 (GNU/Linux)\r\n";
	// add file modified time
	if(200 == HTTP_status_code)
	{
		string mt(modified_time);
		status_and_header += "Last-Modified: " + mt.substr(0,mt.size()-1) + " GMT\r\n";
	}
	// add content length
	status_and_header += "Content-Length: "+int_to_string((int)filesize)+"\r\n";
	// add file genre and type
	status_and_header += "Content-Type: "+ rp.get_filegenre()+"/"+rp.get_filetype()+"\r\n";
	// add blank line
	status_and_header += "\r\n";
	// add actual data
	if(!allow_data_transfer())
		return;
	string fn = "./"+rp.get_filename();
	ifstream rf(fn.c_str()/*,ios::binary*/);
	stringstream buffer;
	buffer << rf.rdbuf();
	//cout << buffer.str() << endl;
	status_and_header += buffer.str();
}

string ResponseGenerator::get_status_and_header() const
{
	return status_and_header;
}