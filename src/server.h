#pragma once
#include <vector>
#include <string>

#include "string_utils.h"

using std::string;

class Server {
private:
	string url;
	float lat;
	float lon;
	string name;
	string country;
	string cc;
	string sponsor;
	string id;
	string host;
	string path;

	float latency = -1;
public:
	Server(std::string raw) {
		this->url = get_xml_attribute(raw, "url");
		this->lat = std::stof(get_xml_attribute(raw, "lat"));
		this->lon = std::stof(get_xml_attribute(raw, "lon"));
		this->name = get_xml_attribute(raw, "name");
		this->country = get_xml_attribute(raw, "country");
		this->cc = get_xml_attribute(raw, "cc");
		this->sponsor = get_xml_attribute(raw, "sponsor");
		this->id = get_xml_attribute(raw, "id");
		this->host = get_xml_attribute(raw, "host");

		this->path = split(this->url, "/upload.php", 1).at(0);
	};

	string to_string();

	void ping();

	float get_latency() { return this->latency; }
	string get_path() { return this->path; }
	string get_url() { return this->url; }
};

std::vector<Server*> getServerList();
Server getBestServer();
