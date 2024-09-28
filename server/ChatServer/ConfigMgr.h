#pragma once
#include "const.h"

struct SectionInfo {
	SectionInfo(){}
	~SectionInfo() {
		_section_data.clear();
	}
	SectionInfo(const SectionInfo& src) {
		_section_data = src._section_data;

	}
	SectionInfo& operator=(const SectionInfo& src) {
		if (&src == this)
			return *this;
		this->_section_data = src._section_data;
	}

	std::map<std::string, std::string> _section_data;
	std::string operator[](const std::string& key) {
		if (_section_data.find(key) == _section_data.end())
			return "";
		return _section_data[key];
	}
};

class ConfigMgr
{
public:
	~ConfigMgr() {
		_config_data.clear();
	}
	ConfigMgr(const ConfigMgr& src) {
		_config_data = src._config_data;
	}

	static ConfigMgr& Inst() {
		static ConfigMgr cfg;
		return cfg;
	}

	ConfigMgr& operator=(const ConfigMgr& src) {
		if (&src == this)
			return *this;
		this->_config_data = src._config_data;
	}

	SectionInfo operator[](const std::string& section) {
		if (_config_data.find(section) == _config_data.end()) {
			return SectionInfo();
		}
		return _config_data[section];
	}
private:
	ConfigMgr();
	std::map <std::string, SectionInfo> _config_data;
};

