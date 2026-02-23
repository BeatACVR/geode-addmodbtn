#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>

using namespace geode::prelude;

// code #stolen from alphalaneous
class $nodeModify(AddBtnModsLayer, ModsLayer) {
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

		promptSelectionAndInstallMod();
	}

	void promptSelectionAndInstallMod() {
		async::spawn(file::pick(file::PickMode::OpenFile, file::FilePickOptions {
			.filters = { file::FilePickOptions::Filter {
				.description = "Geode Mods",
				.files = { "*.geode" },
			}}
		}), [](Result<std::optional<std::filesystem::path>> result) {
			if (result.isOk() && result.unwrap().has_value()) {
				std::filesystem::path modToInstall = result.unwrap().value();
				std::filesystem::path modDir = dirs::getModsDir() / modToInstall.filename();

				std::error_code error;
				std::filesystem::copy_file(
					modToInstall,
					modDir,
					std::filesystem::copy_options::overwrite_existing,
					error
				);

				if (error) {
					FLAlertLayer::create(
						"Install failed",
						fmt::format("Failed to install mod:\n{}", error.message()),
						"OK"
					)->show();
				}
			}
			else if (!result.isOk()) {
				FLAlertLayer::create(
					"Unable to Select File",
					result.unwrapErr(),
					"OK"
				)->show();
			}
		});
	}
};