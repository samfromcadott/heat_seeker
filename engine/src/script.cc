#include <heat_seeker.hh>

void HSE::load_model(const ecs_function_ctx_t *ctx, int argc, const ecs_value_t *argv, ecs_value_t *result) {
	std::cout << "Calling load_model...\n";
	(void)ctx;
	(void)argc;

	std::string& filename = *(std::string*)argv[0].ptr;
	if (model_files.count(filename) == 0) model_files[filename].model = LoadModel( filename.c_str() );
	(*(HSE::Model*)result->ptr).data = &model_files[filename];
}
