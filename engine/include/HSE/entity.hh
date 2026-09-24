#pragma once

namespace HSE {

typedef flecs::entity Entity;

template <typename T>
T& get(Entity& entity) {
	return entity.get_mut<T>();
}

template <typename T>
const T& get(const Entity& entity) {
	return entity.get<T>();
}

template <typename T>
T* get_if(Entity& entity) {
	if ( entity.has<T>() )
		return &entity.get_mut<T>();
	else
		return nullptr;
}

template <typename T>
const T* get_if(const Entity& entity) {
	return entity.try_get<T>();
}

template <typename T>
bool has(const Entity& entity) {
	return entity.has<T>();
}

template <typename T>
T& add(Entity& entity) {
	return entity.add<T>();
}

}
