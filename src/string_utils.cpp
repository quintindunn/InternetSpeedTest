#include <iostream>
#include <sstream>
#include <vector>


using std::string;
using std::vector;


vector<string> split(string content, string delimiter, int count) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	int i = 0;
	while ((pos_end = content.find(delimiter, pos_start)) != string::npos && (i < count || count == -1)) {
		token = content.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
		i++;
	}

	res.push_back(content.substr(pos_start));
	return res;
}

vector<string> split(string content, string delimiter) {
	return split(content, delimiter, -1);
}


string get_xml_attribute(string content, string key) {
	content = split(content, key + (string) "=\"", 1).at(1);
	content = split(content, "\"", 1).at(0);
	return content;
}