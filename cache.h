#ifndef H_CACHE
#define H_CACHE

#include <unordered_map>

class Cache
{
	std::unordered_map<std::string, std::string> loaded_files;

public:
	std::string *load_file(std::string path);
};

#endif