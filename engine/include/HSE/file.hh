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

inline void init() {
	PHYSFS_init(NULL);
}

inline void mount(std::string directory, std::string mount_point = "/") {
	PHYSFS_mount(directory.c_str(), mount_point.c_str(), 1);
}

template<class T> T* read_file(const FileData& data);

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

	Asset(std::string filename) {
		// Check if the file is already loaded
		if ( File::asset_table.contains(filename) ) {
			data = (Data*)(File::asset_table[filename]);
			++(data->uses);
			return;
		}

		// Open file
		PHYSFS_file* file = PHYSFS_openRead( filename.c_str() );
		size_t file_size = PHYSFS_fileLength(file);

		// Load data
		FileData file_data;
		file_data.resize(file_size);
		PHYSFS_readBytes(file, file_data.data(), file_size);
		PHYSFS_close(file);

		// Read file
		data = new Data;
		data->pointer = File::read_file<T>(file_data);

		data->uses = 1;
		data->filename = filename;

		File::asset_table[filename] = data;
	}

	~Asset() {
		if (!data->pointer) return; // Don't do anything if the asset was never initialized

		--(data->uses); // Decrement uses
		if (data->uses > 0) return; // Return if asset still has users

		// Remove entry from asset_table
		File::asset_table.erase(data->filename);

		// Delete pointers
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
