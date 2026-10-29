#include <heat_seeker.hh>

void HSE::play_sound_spatial(flecs::world world, Sound sound, const HSE::vec3& pos, float max_dist) {
	// Calculate direction vector and distance between listener and sound source
	Vector3 direction = Vector3Subtract(glm_to_raylib(pos), camera.position);
	float distance = Vector3Length(direction);

	// Apply logarithmic distance attenuation and clamp between 0-1
	float attenuation = 1.0f/(1.0f + (distance/max_dist));
	attenuation = Clamp(attenuation, 0.0f, 1.0f);

	// Calculate normalized vectors for spatial positioning
	Vector3 normalizedDirection = Vector3Normalize(direction);
	Vector3 forward = Vector3Normalize(camera.target - camera.position);
	Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

	// Reduce volume for sounds behind the listener
	float dotProduct = Vector3DotProduct(forward, normalizedDirection);
	if (dotProduct < 0.0f) attenuation *= (1.0f + dotProduct*0.5f);

	// Set stereo panning based on sound position relative to listener
	float pan = Vector3DotProduct(normalizedDirection, right);

	// Apply final sound properties
	SetSoundVolume(sound, attenuation);
	SetSoundPan(sound, pan);
	PlaySound(sound);
}
