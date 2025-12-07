#include <heat_seeker.hh>

using namespace HSE;

BroadPhaseLayerInterface::BroadPhaseLayerInterface() {
	// Create a mapping table from object to broad phase layer
	mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
	mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
}

JPH::uint BroadPhaseLayerInterface::GetNumBroadPhaseLayers() const {
	return BroadPhaseLayers::NUM_LAYERS;
}

JPH::BroadPhaseLayer BroadPhaseLayerInterface::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const {
	JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
	return mObjectToBroadPhase[inLayer];
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)

const char* BroadPhaseLayerInterface::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const {
	switch ( (JPH::BroadPhaseLayer::Type)inLayer ) {
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING: return "NON_MOVING";
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING: return "MOVING";
		default: JPH_ASSERT(false); return "INVALID";
	}
}

#endif
