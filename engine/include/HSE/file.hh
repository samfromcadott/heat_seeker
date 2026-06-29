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

void init() {
	PHYSFS_init(NULL);
}

void mount(std::string directory, std::string mount_point = "/") {
	PHYSFS_mount(directory.c_str(), mount_point.c_str(), 1);
}

template<class T> T* read_file(const FileData& data);

}

template<typename T>
class Asset {
private:
	T* pointer = nullptr;
	size_t* uses = nullptr;
	std::string* filename = nullptr;

public:
	Asset() = default;

	Asset(const Asset<T>& other) {
		pointer = other.pointer;
		uses = other.uses;
		filename = other.filename;

		++(*uses);
	}

	Asset(std::string filename) {
		// Check if the file is already loaded
		if ( File::asset_table.contains(filename) ) {
			*this = *(Asset<T>*)(File::asset_table[filename]);
		}

		// Open file
		PHYSFS_file* file = PHYSFS_openRead( filename.c_str() );
		size_t file_size = PHYSFS_fileLength(file);

		// Load data
		FileData data;
		data.resize(file_size);
		PHYSFS_readBytes(file, data.data(), file_size);
		PHYSFS_close(file);

		// Read file
		pointer = File::read_file<T>(data);

		uses = new size_t;
		this->filename = new std::string;

		*uses = 1;
		*(this->filename) = filename;

		File::asset_table[filename] = this;
	}

	~Asset() {
		if (!pointer) return; // Don't do anything if the resource was never initialized

		--(*uses); // Decrement uses
		if (*uses > 0) return; // Return if resource still has users

		// Remove entry from resource_table
		File::asset_table.erase(*filename);

		// Delete pointers
		delete pointer;
		delete uses;
		delete filename;
	}

	Asset<T>& operator=(const Asset<T>& other) {
		pointer = other.pointer;
		uses = other.uses;
		filename = other.filename;

		++(*uses);

		return *this;
	}

	T operator*() {
		return *pointer;
	}

	T* operator->() const {
		return &*pointer;
	}

	explicit operator bool() const noexcept {
		return pointer != nullptr;
	}

	size_t use_count() const {
		return uses? *uses : 0;
	}

	std::string path() const {
		return filename? *filename : "Resource not initialized";
	}

	T* get() {
		return pointer;
	}

	bool unique() const {
		return *uses == 1;
	}
};

}
