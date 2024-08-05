#pragma once
#include <vector>
#include <string>

std::vector<std::string> split(std::string, std::string, int);
std::vector<std::string> split(std::string, std::string);

std::string get_xml_attribute(std::string content, std::string key);
