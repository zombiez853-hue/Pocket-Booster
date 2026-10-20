// Frees textures nobody is using shortly after you leave a level.
// Pure cocos2d + Geode API, no platform-specific code.
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
	// Cocos schedulers don't retain their target, so keep one object alive forever.
	class Janitor : public CCObject {
	public:
		static Janitor* get() {
			static Janitor* instance = new Janitor();
			return instance;
		}

		void queue() {
			auto scheduler = CCDirector::sharedDirector()->getScheduler();
			scheduler->unscheduleSelector(schedule_selector(Janitor::sweep), this);
			// interval 0, repeat 0 (run once), delay 2s so the old scene is gone first
			scheduler->scheduleSelector(schedule_selector(Janitor::sweep), this, 0.f, 0, 2.f, false);
		}

		void sweep(float) {
			CCTextureCache::sharedTextureCache()->removeUnusedTextures();
			log::debug("Pocket Boost: released unused textures");
		}
	};
}

class $modify(PBPlayLayer, PlayLayer) {
	void onQuit() {
		PlayLayer::onQuit();
		if (Mod::get()->getSettingValue<bool>("texture-cleanup")) {
			Janitor::get()->queue();
		}
	}
};
