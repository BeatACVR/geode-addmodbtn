#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Button.hpp>
#include <alphalaneous.alphas_geode_utils/ObjectModify.hpp>
#include <alphalaneous.alphas_geode_utils/Utils.hpp>

using namespace geode::prelude;

class $nodeModify(AddBtnModsLayer, ModsLayer) {
	// modify code #stolen from alphalaneous and adapted
	void modify() {
		std::string geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<std::string>("used-theme");
		if (CCMenu* actionsMenu = typeinfo_cast<CCMenu*>(getChildByID("actions-menu"))) {
			bool useDarkTheme = true;
			if (geodeTheme == "Geometry Dash") useDarkTheme = false;

			CCSprite* addspr = CircleButtonSprite::createWithSpriteFrameName(
				"geode.loader/file-add.png", .85f,
				(useDarkTheme ? CircleBaseColor::DarkPurple : CircleBaseColor::Green)
			);
			addspr->setScale(0.8f);

			CCMenuItemSpriteExtra* addModBtn = CCMenuItemSpriteExtra::create(
				addspr,
				this,
				menu_selector(AddBtnModsLayer::onAddModBtn)
			);
			addModBtn->setID("add-mod-button");
			actionsMenu->addChild(addModBtn);
			actionsMenu->updateLayout();
		}
	}

	void onAddModBtn(CCObject*) {
		// popup #stolen from geode!!!!
		if (!Loader::get()->getInstalledMod("geode.loader")->setSavedValue("shown-manual-install-info", true)) {
			return FLAlertLayer::create(
				nullptr,
				"Manually Installing Mods",
				"You can <cg>manually install mods</c> by selecting their <cd>.geode</c> files. "
				"Do note that manually installed mods <co>are not verified to be safe and stable</c>!\n"
				"<cr>Proceed at your own risk!</c>",
				"OK", nullptr,
				350
			)->show();
		}
		pressInstallModButton();
	}
	
	void pressInstallModButton() {
		Mod* geodeLoader = Loader::get()->getInstalledMod("geode.loader");
		if (!geodeLoader) {
			FLAlertLayer::create("Install Failed", "geode.loader is not installed?", "What")->show();
			return;
		}
		ButtonSettingPressedEventV3(geodeLoader, "action-buttons").send("install-from-file");
	}
};