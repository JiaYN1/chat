#include "ConfigMgr.h"

std::string ConfigMgr::GetValue(const std::string& section, const std::string& key)
{
	if (_config_data.find(section) == _config_data.end()) {
		return "";
	}

	return _config_data[section].GetValue(key);
}

ConfigMgr::ConfigMgr()
{
	boost::filesystem::path current_path = boost::filesystem::current_path();
	boost::filesystem::path config_path = current_path / "config.ini";
	std::cout << "Config Path: " << config_path << std::endl;

	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);

	// ±éÀúËùÓÐsection
	for (const auto& section_pair : pt) {
		const std::string& section_name = section_pair.first;
		const boost::property_tree::ptree& section_tree = section_pair.second;
		
		std::map<std::string, std::string> config_data;
		for (const auto& config : section_tree) {
			const std::string& key = config.first;
			const std::string& value = config.second.get_value<std::string>();
			config_data[key] = value;
		}
		SectionInfo sec;
		sec._section_data = config_data;
		_config_data[section_name] = sec;
	}

	for (const auto& section_entry : _config_data) {
		const std::string& section_name = section_entry.first;
		SectionInfo section_config = section_entry.second;
		std::cout << "[" << section_name << "]" << std::endl;
		for (const auto& key_value_pair : section_config._section_data)
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
	}
}
