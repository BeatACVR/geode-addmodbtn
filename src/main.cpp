#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>
#include <alphalaneous.alphas_geode_utils/include/Utils.hpp>

using namespace geode::prelude;

class $nodeModify(AddBtnModsLayer, ModsLayer) {
	// modify code #stolen from alphalaneous and adapted
	void modify() {
		bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");
		if (CCMenu* actionsMenu = typeinfo_cast<CCMenu*>(getChildByID("actions-menu"))) {

			CCSprite* addspr = CircleButtonSprite::createWithSpriteFrameName(
				"geode.loader/file-add.png", .85f,
				(geodeTheme ? CircleBaseColor::DarkPurple : CircleBaseColor::Green)
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
		takeAdvantageOfThatFuckingInstallButtonThatIHateSoFuckingMuch();
	}

	// everything below here is entirely written by me :3
	// and its so atrociously bad that i should not be bragging about it.
	struct Fields {
		geode::Popup* m_settingsPopup;
	};
	
	void takeAdvantageOfThatFuckingInstallButtonThatIHateSoFuckingMuch() {
		// get geode's mod settings popup then click the install mod from file button, finally kill the popup.
		Mod* geodeLoader = Loader::get()->getInstalledMod("geode.loader");
		m_fields->m_settingsPopup = geode::openSettingsPopup(geodeLoader, false);
		m_fields->m_settingsPopup->setVisible(false); // not necessary on my machine but i fear low end devices could need it.

		CCNode* installNode = m_fields->m_settingsPopup->getChildByIDRecursive("install-from-file-button");
		if (!installNode) {
			fail("trying to find install button.");
			return;
		}
		
		CCMenuItemSpriteExtra* installbtn = typeinfo_cast<CCMenuItemSpriteExtra*>(installNode);
		if (!installbtn) {
			fail("trying to cast install button.");
			return;
		} else {
			installbtn->activate();
			m_fields->m_settingsPopup->removeFromParentAndCleanup(true);
		}
	}

	void fail(std::string reason) {
		if (m_fields->m_settingsPopup) {
			m_fields->m_settingsPopup->removeFromParentAndCleanup(true);
		}
		FLAlertLayer::create("Install Failed", fmt::format("Failed while {}", reason), "OK")->show();
	}
};