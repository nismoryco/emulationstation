#include "MetaData.h"

#include "utils/FileSystemUtil.h"
#include "Log.h"
#include <pugixml/src/pugixml.hpp>

MetaDataDecl gameDecls[] = {
	// key,         type,                   default
	{"name",        MD_STRING,              ""},
	{"sortname",    MD_STRING,              ""},
	{"desc",        MD_MULTILINE_STRING,    ""},
	{"image",       MD_PATH,                ""},
	{"video",       MD_PATH     ,           ""},
	{"marquee",     MD_PATH,                ""},
	{"thumbnail",   MD_PATH,                ""},
	{"rating",      MD_RATING,              "0.000000"},
	{"releasedate", MD_DATE,                "not-a-date-time"},
	{"developer",   MD_STRING,              "unknown"},
	{"publisher",   MD_STRING,              "unknown"},
	{"genre",       MD_STRING,              "unknown"},
	{"players",     MD_INT,                 "1"},
	{"favorite",    MD_BOOL,                "false"},
	{"hidden",      MD_BOOL,                "false"},
	{"kidgame",     MD_BOOL,                "false"},
	{"playcount",   MD_INT,                 "0"},
	{"lastplayed",  MD_TIME,                "0"}
};
const std::vector<MetaDataDecl> gameMDD(gameDecls, gameDecls + sizeof(gameDecls) / sizeof(gameDecls[0]));

MetaDataDecl folderDecls[] = {
	{"name",        MD_STRING,              ""},
	{"sortname",    MD_STRING,              ""},
	{"desc",        MD_MULTILINE_STRING,    ""},
	{"image",       MD_PATH,                ""},
	{"thumbnail",   MD_PATH,                ""},
	{"video",       MD_PATH,                ""},
	{"marquee",     MD_PATH,                ""},
	{"rating",      MD_RATING,              "0.000000"},
	{"releasedate", MD_DATE,                "not-a-date-time"},
	{"developer",   MD_STRING,              "unknown"},
	{"publisher",   MD_STRING,              "unknown"},
	{"genre",       MD_STRING,              "unknown"},
	{"players",     MD_INT,                 "1"}
};
const std::vector<MetaDataDecl> folderMDD(folderDecls, folderDecls + sizeof(folderDecls) / sizeof(folderDecls[0]));

const std::vector<MetaDataDecl>& getMDDByType(MetaDataListType type)
{
	switch(type)
	{
	case GAME_METADATA:
		return gameMDD;
	case FOLDER_METADATA:
		return folderMDD;
	}

	LOG(LogError) << "Invalid MDD type";
	return gameMDD;
}



MetaDataList::MetaDataList(MetaDataListType type)
	: mType(type), mWasChanged(false)
{
	const std::vector<MetaDataDecl>& mdd = getMDD();
	for(auto iter = mdd.cbegin(); iter != mdd.cend(); iter++)
		set(iter->key, iter->defaultValue);
}


MetaDataList MetaDataList::createFromXML(MetaDataListType type, pugi::xml_node& node, const std::string& relativeTo)
{
	MetaDataList mdl(type);

	const std::vector<MetaDataDecl>& mdd = mdl.getMDD();

	for(auto iter = mdd.cbegin(); iter != mdd.cend(); iter++)
	{
		pugi::xml_node md = node.child(iter->key.c_str());
		if(md)
		{
			// if it's a path, resolve relative paths
			std::string value = md.text().get();
			if (iter->type == MD_PATH)
			{
				value = Utils::FileSystem::resolveRelativePath(value, relativeTo, true, true);
			}
			mdl.set(iter->key, value);
		}else{
			mdl.set(iter->key, iter->defaultValue);
		}
	}

	return mdl;
}

void MetaDataList::appendToXML(pugi::xml_node& parent, bool ignoreDefaults, const std::string& relativeTo) const
{
	const std::vector<MetaDataDecl>& mdd = getMDD();

	for(auto mddIter = mdd.cbegin(); mddIter != mdd.cend(); mddIter++)
	{
		auto mapIter = mMap.find(mddIter->key);
		if(mapIter != mMap.cend())
		{
			// we have this value!
			// if it's just the default (and we ignore defaults), don't write it
			if(ignoreDefaults && mapIter->second == mddIter->defaultValue)
				continue;

			// try and make paths relative if we can
			std::string value = mapIter->second;
			if (mddIter->type == MD_PATH)
				value = Utils::FileSystem::createRelativePath(value, relativeTo, true, true);

			parent.append_child(mapIter->first.c_str()).text().set(value.c_str());
		}
	}
}

void MetaDataList::set(const std::string& key, const std::string& value)
{
	mMap[key] = value;
	mWasChanged = true;
}

const std::string& MetaDataList::get(const std::string& key) const
{
	return mMap.at(key);
}

int MetaDataList::getInt(const std::string& key) const
{
	return atoi(get(key).c_str());
}

float MetaDataList::getFloat(const std::string& key) const
{
	return (float)atof(get(key).c_str());
}

bool MetaDataList::wasChanged() const
{
	return mWasChanged;
}

void MetaDataList::resetChangedFlag()
{
	mWasChanged = false;
}
