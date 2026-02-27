#pragma once
#include <iostream>
#include "sqlite3/sqlite3.h"
#include "httplib.h"
#include "Database.h"
#include "User.h"
using namespace std;
using namespace httplib;

User startServer() {
	string ID;
	string Name;
	string Surname;
	string Role;
	string Password;
	cout << "Tryna start a server" << endl;
	Server server;
	server.Get("/stu", [&](const Request& req, Response& res) {
		if (!req.has_param("ID") || !req.has_param("Name") || !req.has_param("Surname") || !req.has_param("Role") || !req.has_param("Password")) {
			res.set_content("not enough parameters in the url", "text/plain");
			cout << "yo not enough things" << endl;
			return;
		}

		ID = req.get_param_value("ID");
		Name = req.get_param_value("Name");
		Surname = req.get_param_value("Surname");
		Role = req.get_param_value("Role");
		Password = req.get_param_value("Password");
		res.set_content("Got info, look into the console now", "text/plain");
		server.stop();
	});
	std::cout << "Started local server with port 8080, localhost" << std::endl;

	server.listen("0.0.0.0", 8080);
	return User(ID, Name, Surname, Role, Password);
}
