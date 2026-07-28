#include <heat_seeker.hh>

using namespace HSE;

std::map<std::string, void*> HSE::File::asset_table;

void HSE::File::init() {
	std::cout << "Initializing PhysFS\n";
	PHYSFS_init(NULL);
	SetLoadFileDataCallback(read_bin_file_callback);
	SetLoadFileTextCallback(read_text_file_callback);
}

void HSE::File::mount(const std::string& directory, const std::string& mount_point) {
	PHYSFS_mount(directory.c_str(), mount_point.c_str(), 1);
}

FileData HSE::File::open(const std::string& filename) {
	// Open file
	PHYSFS_file* file = PHYSFS_openRead( filename.c_str() );
	size_t file_size = PHYSFS_fileLength(file);

	// Load data
	FileData file_data;
	file_data.resize(file_size);
	PHYSFS_readBytes(file, file_data.data(), file_size);
	PHYSFS_close(file);

	return file_data;
}

unsigned char* HSE::File::read_bin_file_callback(const char *fileName, int *dataSize) {
	PHYSFS_file* file = PHYSFS_openRead(fileName);
	*dataSize = PHYSFS_fileLength(file);

	unsigned char* data = new unsigned char[*dataSize];
	PHYSFS_readBytes(file, data, *dataSize);
	PHYSFS_close(file);

	return data;
}

char* HSE::File::read_text_file_callback(const char *filename) {
	PHYSFS_file* file = PHYSFS_openRead(filename);
	int size = PHYSFS_fileLength(file);

	char* data = new char[size];
	PHYSFS_readBytes(file, data, size);
	PHYSFS_close(file);

	return data;
}

template<> std::string* HSE::File::read_file(const std::string& filename) {
	auto data = File::open(filename);
	std::string* s = new std::string( data.begin(), data.end() );
	return s;
}

template<> HSE::ModelData* HSE::File::read_file(const std::string& filename) {
	auto md = new ModelData;
	md->model = LoadModel( filename.c_str() );
	md->animations = LoadModelAnimations(filename.c_str(), &md->animation_count);

	// Populate anim_names
	for (int i = 0; i < md->animation_count; i++) {
		md->anim_names[md->animations[i].name] = i;
	}

	return md;
}

template<> Texture* HSE::File::read_file(const std::string& filename) {
	auto tex = new Texture;
	*tex = LoadTexture( filename.c_str() );
	return tex;
}

template<> Sound* HSE::File::read_file(const std::string& filename) {
	auto sound = new Sound;
	*sound = LoadSound( filename.c_str() );
	return sound;
}

template<> void HSE::File::unload(std::string* data) {
	delete data;
}

template<> void HSE::File::unload(ModelData* data) {
	UnloadModel(data->model);
	if (data->animation_count > 0)
		UnloadModelAnimations(data->animations, data->animation_count);
}

template<> void HSE::File::unload(Texture* data) {
	UnloadTexture(*data);
}

template<> void HSE::File::unload(Sound* data) {
	UnloadSound(*data);
}
