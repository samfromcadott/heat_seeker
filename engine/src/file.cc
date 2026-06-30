#include <heat_seeker.hh>

using namespace HSE;

std::map<std::string, void*> HSE::File::asset_table;

template<> std::string* HSE::File::read_file(const FileData& data) {
	std::string* s = new std::string( data.begin(), data.end() );
	return s;
}
