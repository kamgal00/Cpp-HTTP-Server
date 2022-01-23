#include "cache.h"
#include <fstream>
#include <sstream>

using namespace std;

string *Cache::load_file(string path)
{
	if (loaded_files.find(path) == loaded_files.end())
	{
		std::ifstream t(path);
		if (!t.good())
			return nullptr;
		std::stringstream buffer;
		buffer << t.rdbuf();
		loaded_files[path] = buffer.str();
	}
	return &loaded_files[path];
}