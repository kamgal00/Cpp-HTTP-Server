#include "HTTP_parser.h"

using namespace std;

int ParsedRequest::parse_request(const char *begin)
{
	lines = string(begin);
	if (get_request_line())
		return -1;
	if (get_header_fields())
		return -1;
	data = lines;
	return 0;
}

int ParsedRequest::get_request_line()
{
	load_line();
	get_word(command);
	get_word(path);
	get_word(protocol);
	if (current_line.size())
		return -1;
	return 0;
}

int ParsedRequest::get_header_fields()
{
	header_fields.clear();
	while (true)
	{
		load_line();
		if (!current_line.length())
			break;
		int n = current_line.find(':');
		if (n == string::npos)
			return -1;
		header_fields[current_line.substr(0, n)] = current_line.substr(n + 2);
	}
	return 0;
}

int ParsedRequest::load_line()
{
	size_t n = lines.find('\r');
	if (n == string::npos)
	{
		current_line = lines;
		lines = "";
	}
	else
	{
		current_line = lines.substr(0, n);
		lines.erase(0, n + 2);
	}
	return 0;
}

void ParsedRequest::get_word(string &a)
{
	int n = current_line.find(' ');
	if (n == string::npos)
	{
		a = current_line;
		current_line = "";
	}
	else
	{
		a = current_line.substr(0, n);
		current_line.erase(0, n + 1);
	}
}