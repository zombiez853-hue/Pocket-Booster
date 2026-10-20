// Thins out in-level particle effects.
// If the build fails here, the createParticle signature in the bindings
// differs from this one: check geode/bindings/.../GJBaseGameLayer.hpp, fix
// the signature, or just delete this file to drop the feature.
#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify(PBBaseGameLayer, GJBaseGameLayer) {
	CCParticleSystemQuad* createParticle(int p0, char const* p1, int p2, tCCPositionType p3) {
		auto particle = GJBaseGameLayer::createParticle(p0, p1, p2, p3);

		auto keep = Mod::get()->getSettingValue<int64_t>("particle-percent");
		if (particle && keep < 100) {
			// Deterministic thinning: keep `keep` out of every 100 systems.
			static uint32_t counter = 0;
			if ((counter++ % 100) >= static_cast<uint32_t>(keep)) {
				particle->stopSystem();
				particle->setVisible(false);
				particle->unscheduleUpdate();
			}
		}
		return particle;
	}
};
