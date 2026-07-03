#pragma once

#include <string>
#include <vector>
#include <map>
#include <physfs.h>

namespace HSE {

typedef std::vector<unsigned char> FileData;
template<typename T> class Asset;

namespace File {

extern std::map<std::string, void*> asset_table;

void init();
void mount(const std::string& directory, const std::string& mount_point = "/");
FileData open(const std::string& filename);
unsigned char* read_bin_file_callback(const char *fileName, int *dataSize);

template<class T> T* read_file(const FileData& data);
template<> std::string* read_file(const FileData& data);
template<> ModelData* read_file(const FileData& data);

}

template<typename T>
class Asset {
private:
	struct Data {
		T* pointer = nullptr;
		size_t uses = 0;
		std::string filename = "";
	};
	Data* data = nullptr;

public:
	Asset() = default;

	Asset(const Asset<T>& other) {
		data = other.data;
		++(data->uses);
	}

	Asset(const std::string& filename) {
		// Check if the file is already loaded
		if ( File::asset_table.contains(filename) ) {
			data = (Data*)(File::asset_table[filename]);
			++(data->uses);
			return;
		}

		// Read file
		data = new Data;
		auto file_data = File::open(filename);
		data->pointer = File::read_file<T>(file_data);

		data->uses = 1;
		data->filename = filename;

		File::asset_table[filename] = data;
		std::cout << "Loaded " << data->filename << '\n';
	}

	~Asset() {
		if (!data->pointer) return; // Don't do anything if the asset was never initialized

		--(data->uses); // Decrement uses
		if (data->uses > 0) return; // Return if asset still has users

		// Delete pointers
		std::cout << "Unloaded " << data->filename << '\n';
		File::asset_table.erase(data->filename);
		delete data->pointer;
		delete data;
	}

	Asset<T>& operator=(const Asset<T>& other) {
		data = other.data;
		++(data->uses);

		return *this;
	}

	T operator*() {
		return *(data->pointer);
	}

	T* operator->() const {
		return &(data->pointer);
	}

	explicit operator bool() const noexcept {
		return data != nullptr;
	}

	size_t use_count() const {
		return data? data->uses : 0;
	}

	std::string path() const {
		return data? data->filename : "Resource not initialized";
	}

	T* get() {
		return data->pointer;
	}

	bool unique() const {
		return data->uses == 1;
	}
};

}
