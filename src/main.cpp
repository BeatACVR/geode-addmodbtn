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

		// please merge geode pr #1844 so i dont have to do this fucking bullshit to get installbtn. this could be a one-liner.
		CCLayer* layer = m_fields->m_settingsPopup->getChildByType<CCLayer*>(0);
			if (!layer) {
				fail("trying to find cclayer.");
				return;
			}
		CCLayerColor* cclayercolor = layer->getChildByType<CCLayerColor*>(0);
			if (!cclayercolor) {
				fail("trying to find cclayercolor.");
				return;
			}
		geode::ScrollLayer* scrolllayer = cclayercolor->getChildByType<geode::ScrollLayer*>(0);
			if (!scrolllayer) {
				fail("trying to find scrolllayer.");
				return;
			}
		geode::GenericContentLayer* genericcontentlayer = scrolllayer->getChildByType<geode::GenericContentLayer*>(0);
			if (!genericcontentlayer) {
				fail("trying to find genericcontentlayer.");
				return;
			}
		CCNode* setting = alpha::utils::cocos::getChildByClassName(genericcontentlayer, "CopyButtonSettingNode", 0).value_or(nullptr);
			if (!setting) {
				fail("trying to find CopyButtonSettingNode.");
				return;
			}
		CCMenu* ccmenu = setting->getChildByType<CCMenu*>(1);
			if (!ccmenu) {
				fail("trying to find ccmenu.");
				return;
			}
		CCMenuItemSpriteExtra* installbtn = ccmenu->getChildByType<CCMenuItemSpriteExtra*>(2);
		if (!installbtn) {
			fail("trying to find install button.");
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