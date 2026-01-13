#include <heat_seeker.hh>

void HSE::load_model(const ecs_function_ctx_t *ctx, int argc, const ecs_value_t *argv, ecs_value_t *result) {
	(void)ctx;
	(void)argc;

	std::string& filename = *(std::string*)argv[0].ptr;
	std::cout << "Loading model: " << filename.c_str() << "...\n";
	(*(HSE::Model*)result->ptr) = HSE::Model( filename.c_str() );
}
