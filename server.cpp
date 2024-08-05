#include <iostream>
#include <vector>
#include <format>
#include <array>

#include <cstdio>
#include <cstdlib>

#include "requester.h"
#include "server.h"
#include "string_utils.h"

#define SPEEDTEST_SERVERS_URL "https://c.speedtest.net/speedtest-servers-static.php"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

std::vector<Server*> getServerList() {
	std::vector<Server*> server_objs;

	std::vector<std::string> servers = split(split(split(get(SPEEDTEST_SERVERS_URL), (std::string)"<servers>", 1).at(1), "</servers>", 1).at(0), "\n");

	// Only get content within the <servers></servers> tags.
	for (int i = 0; i < servers.size()-1; i++) {
		std::string server = servers.at(i);
		Server* server_obj = new Server(server);
		server_obj->ping();
		if (server_obj->get_latency() > 0.0f)
			server_objs.push_back(server_obj);
	}

	std::sort(server_objs.begin(), server_objs.end(), [](Server* a, Server* b) {
		return a->get_latency() < b->get_latency();
		});
	return server_objs;
}


Server getBestServer() {
	return *getServerList().at(0);
}


std::string Server::to_string() {
	return std::format("<server url=\"{}\" lat=\"{}\" lon=\"{}\" name=\"{}\" country=\"{}\" cc=\"{}\" sponsor=\"{}\" id=\"{}\" host=\"{}\" latency=\"{}\">",
		this->url, this->lat, this->lon, this->name, this->country, this->cc, this->sponsor, this->id, this->host, this->latency);
}

void Server::ping() {
	std::string host = split(this->host, ":", 1).at(0);
	std::string command = "ping -n 1 " + host;

	std::array<char, 128> buffer;
	std::string result;

	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe) {
		std::cerr << "popen() failed!" << std::endl;
		return;
	}

	while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
		result += buffer.data();
	}
	pclose(pipe);

	std::vector<string> lines = split(result, "\n");
	std::string results = lines.at(lines.size() - 2);
	
	if (results.find("Average") == std::string::npos) {
		this->latency = -999.0f;
		return;
	}

	float avg = std::stof(split(split(results, "Average = ", 1).at(1), "ms").at(0)) / 1000.0f;
	this->latency = avg;
}