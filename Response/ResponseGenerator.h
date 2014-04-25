#ifndef _RESPONSE_GENERATOR_
#define _RESPONSE_GENERATOR_

#include "RequestParser.h"
#include <string>
#include <iostream>

class ResponseGenerator
{
public:
	ResponseGenerator(string s);
	string int_to_string(int num) const;

	bool allow_data_transfer() const;

	void determine_status(bool debug);
	void gen_base();

	string get_status_and_header() const;
	
	

private:
	RequestParser rp;
	int file_exist;
	int file_readable;
	
	int HTTP_status_code;

	char* current_time;
	char* modified_time;
	off_t filesize;

	string status_and_header;


	


};

#endif