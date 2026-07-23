#pragma once

#include <string>
#include <vector>
#include <map>
#include <physfs.h>

namespace HSE {

typedef std::vector<unsigned char> FileData;
template<typename T> class Asset;
struct ModelData;

namespace File {

extern std::map<std::string, void*> asset_table;

void init();
void mount(const std::string& directory, const std::string& mount_point = "/");
FileData open(const std::string& filename);
unsigned char* read_bin_file_callback(const char *fileName, int *dataSize);

template<class T> T* read_file(const std::string& filename);
template<> std::string* read_file(const std::string& filename);
template<> ModelData* read_file(const std::string& filename);
template<> Texture* read_file(const std::string& filename);
template<> Sound* read_file(const std::string& filename);

template<class T> void unload(T* data);
template<> void unload(std::string* data);
template<> void unload(ModelData* data);
template<> void unload(Texture* data);
template<> void unload(Sound* data);

// For serialization
template<class T> void assign_string(T* dest, const char* path) {
	*dest = T(path);
}

template<class T> int serialize(const flecs::serializer* s, const T* data) {
	const char *path = data->path().c_str();
	return s->value(flecs::String, &path);
}

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

	void unload() {
		--(data->uses); // Decrement usess
		if (data->uses > 0) return; // Return if asset still has users

		// Delete pointers
		std::cout << "Unloaded " << data->filename << '\n';
		if (data->pointer)
			File::unload(data->pointer);
		File::asset_table.erase(data->filename);
		delete data;
	}

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

		data = new Data;
		data->uses = 1;
		data->filename = filename;

		File::asset_table[filename] = data;
	}

	~Asset() {
		if (data) unload();
	}

	Asset<T>& operator=(const Asset<T>& other) {
		if (data) unload();

		data = other.data;
		++(data->uses);

		return *this;
	}

	T& operator*() {
		return *(data->pointer);
	}

	const T& operator*() const {
		return *(data->pointer);
	}

	T* operator->() const {
		return data->pointer;
	}

	explicit operator bool() const noexcept {
		return data != nullptr;
	}

	void load() {
		if (not data) return;
		if (data->pointer) return; // File already loaded

		// Read file
		data->pointer = File::read_file<T>(data->filename);
		std::cout << "Loaded " << data->filename << '\n';
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

	void make_new() {
		data->pointer = new T;
	}

	bool unique() const {
		return data->uses == 1;
	}
};

}
