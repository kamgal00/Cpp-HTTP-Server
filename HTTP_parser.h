#ifndef H_HTTP_PARSER
#define H_HTTP_PARSER

#include <unordered_map>

class ParsedRequest
{
	void prepare(const char *begin, const char *end);
	std::string lines, current_line;
	int load_line();
	void get_word(std::string &a);
	int get_request_line();
	int get_header_fields();

public:
	std::string command, path, protocol, data;
	std::unordered_map<std::string, std::string> header_fields;
	int parse_request(const char *begin);
};

#endif