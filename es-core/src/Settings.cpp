#include "Settings.h"

#include "utils/FileSystemUtil.h"
#include "Log.h"
#include "platform.h"
#include <pugixml/src/pugixml.hpp>
#include <algorithm>
#include <vector>

Settings* Settings::sInstance = NULL;

// these values are NOT saved to es_settings.xml
// since they're set through command-line arguments, and not the in-program settings menu
std::vector<const char*> settings_dont_save {
	{ "Debug" },
	{ "DebugGrid" },
	{ "DebugText" },
	{ "DebugImage" },
	{ "ForceKiosk" },
	{ "IgnoreGamelist" },
	{ "HideConsole" },
	{ "ShowExit" },
	{ "ConfirmQuit" },
	{ "SplashScreen" },
	{ "SplashScreenProgress" },
	{ "VSync" },
	{ "FullscreenBorderless" },
	{ "Windowed" },
	{ "WindowWidth" },
	{ "WindowHeight" },
	{ "ScreenWidth" },
	{ "ScreenHeight" },
	{ "ScreenOffsetX" },
	{ "ScreenOffsetY" },
	{ "ScreenRotate" }
};

Settings::Settings()
{
	setDefaults();
	loadFile();
}

Settings* Settings::getInstance()
{
	if(sInstance == NULL)
		sInstance = new Settings();

	return sInstance;
}

void Settings::setDefaults()
{
	mBoolMap.clear();
	mIntMap.clear();

	mBoolMap["BackgroundJoystickInput"] = false;
	mBoolMap["ParseGamelistOnly"] = false;
	mBoolMap["ShowHiddenFiles"] = false;
	mBoolMap["DrawFramerate"] = false;
	mBoolMap["ShowExit"] = true;
	mBoolMap["ConfirmQuit"] = true;
	mBoolMap["FullscreenBorderless"] = false;
	mBoolMap["Windowed"] = false;
	mBoolMap["SplashScreen"] = true;
	mBoolMap["SplashScreenProgress"] = true;
	mStringMap["StartupSystem"] = "";

	mBoolMap["VSync"] = true;

	mBoolMap["EnableSounds"] = true;
	mBoolMap["ShowHelpPrompts"] = true;
	mBoolMap["DoublePressRemovesFromFavs"] = false;
	mBoolMap["IgnoreGamelist"] = false;
	mBoolMap["HideConsole"] = true;
	mBoolMap["QuickSystemSelect"] = true;
	mBoolMap["MoveCarousel"] = true;

	mBoolMap["ThreadedLoading"] = false;

	mBoolMap["Debug"] = false;
	mBoolMap["DebugGrid"] = false;
	mBoolMap["DebugText"] = false;
	mBoolMap["DebugImage"] = false;

	mIntMap["ScreenSaverTime"] = 5 * Settings::ONE_MINUTE_IN_MS;

	#ifdef _RPI_
		mIntMap["MaxVRAM"] = 80;
	#else
		mIntMap["MaxVRAM"] = 100;
	#endif

	mStringMap["TransitionStyle"] = "fade";
	mStringMap["ThemeSet"] = "";
	mStringMap["ScreenSaverBehavior"] = "dim";
	mStringMap["GamelistViewStyle"] = "automatic";
	mStringMap["SaveGamelistsMode"] = "on exit";

	mBoolMap["ScreenSaverControls"] = true;

	mStringMap["CollectionSystemsAuto"] = "";
	mStringMap["CollectionSystemsCustom"] = "";
	mBoolMap["CollectionShowSystemInfo"] = true;
	mBoolMap["SortAllSystems"] = false;
	mBoolMap["UseCustomCollectionsSystem"] = true;

	mBoolMap["LocalArt"] = false;

	// Audio out device for volume control
	#ifdef _RPI_
		mStringMap["AudioDevice"] = "HDMI";
	#else
		mStringMap["AudioDevice"] = "Master";
	#endif

	mStringMap["AudioCard"] = "default";
	mBoolMap["ForceDisableFilters"] = false;

	mIntMap["WindowWidth"]   = 0;
	mIntMap["WindowHeight"]  = 0;
	mIntMap["ScreenWidth"]   = 0;
	mIntMap["ScreenHeight"]  = 0;
	mIntMap["ScreenOffsetX"] = 0;
	mIntMap["ScreenOffsetY"] = 0;
	mIntMap["ScreenRotate"]  = 0;

	mBoolMap["UseFullscreenPaging"] = false;

	mBoolMap["IgnoreLeadingArticles"] = false;
	//No spaces! Order is important!
	//"The A Squad" given [a,an,the] will sort as "A Squad", but given [the,a,an] will sort as "Squad"
	mStringMap["LeadingArticles"] = "a,an,the";
}

template <typename K, typename V>
void saveMap(pugi::xml_document& doc, std::map<K, V>& map, const char* type)
{
	for(auto iter = map.cbegin(); iter != map.cend(); iter++)
	{
		// key is on the "don't save" list, so don't save it
		if(std::find(settings_dont_save.cbegin(), settings_dont_save.cend(), iter->first) != settings_dont_save.cend())
			continue;

		pugi::xml_node node = doc.append_child(type);
		node.append_attribute("name").set_value(iter->first.c_str());
		node.append_attribute("value").set_value(iter->second);
	}
}

void Settings::saveFile()
{
	LOG(LogDebug) << "Settings::saveFile() : Saving Settings to file.";
	const std::string path = Utils::FileSystem::getHomePath() + "/.emulationstation/es_settings.cfg";

	pugi::xml_document doc;

	saveMap<std::string, bool>(doc, mBoolMap, "bool");
	saveMap<std::string, int>(doc, mIntMap, "int");
	saveMap<std::string, float>(doc, mFloatMap, "float");

	//saveMap<std::string, std::string>(doc, mStringMap, "string");
	for(auto iter = mStringMap.cbegin(); iter != mStringMap.cend(); iter++)
	{
		pugi::xml_node node = doc.append_child("string");
		node.append_attribute("name").set_value(iter->first.c_str());
		node.append_attribute("value").set_value(iter->second.c_str());
	}

	doc.save_file(path.c_str());
}

void Settings::loadFile()
{
	const std::string path = Utils::FileSystem::getHomePath() + "/.emulationstation/es_settings.cfg";

	if(!Utils::FileSystem::exists(path))
		return;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());
	if(!result)
	{
		LOG(LogError) << "Could not parse Settings file!\n   " << result.description();
		return;
	}

	for(pugi::xml_node node = doc.child("bool"); node; node = node.next_sibling("bool"))
		setBool(node.attribute("name").as_string(), node.attribute("value").as_bool());
	for(pugi::xml_node node = doc.child("int"); node; node = node.next_sibling("int"))
		setInt(node.attribute("name").as_string(), node.attribute("value").as_int());
	for(pugi::xml_node node = doc.child("float"); node; node = node.next_sibling("float"))
		setFloat(node.attribute("name").as_string(), node.attribute("value").as_float());
	for(pugi::xml_node node = doc.child("string"); node; node = node.next_sibling("string"))
		setString(node.attribute("name").as_string(), node.attribute("value").as_string());
}

template<typename Map>
void Settings::renameSetting(Map& map, std::string&& oldName, std::string&& newName)
{
	typename Map::const_iterator it = map.find(oldName);
	if (it != map.end()) {
		map[newName] = it->second;
		map.erase(it);
	}
}

//Print a warning message if the setting we're trying to get doesn't already exist in the map, then return the value in the map.
#define SETTINGS_GETSET(type, mapName, getMethodName, setMethodName) type Settings::getMethodName(const std::string& name) \
{ \
	if(mapName.find(name) == mapName.cend()) \
	{ \
		LOG(LogError) << "Tried to use unset setting " << name << "!"; \
	} \
	return mapName[name]; \
} \
void Settings::setMethodName(const std::string& name, type value) \
{ \
	mapName[name] = value; \
}

SETTINGS_GETSET(bool, mBoolMap, getBool, setBool);
SETTINGS_GETSET(int, mIntMap, getInt, setInt);
SETTINGS_GETSET(float, mFloatMap, getFloat, setFloat);
SETTINGS_GETSET(const std::string&, mStringMap, getString, setString);
