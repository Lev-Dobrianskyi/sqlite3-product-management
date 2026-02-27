#pragma once
#include <iostream>
#include "sqlite3/sqlite3.h"
using namespace std;

void createUserTable() {
	sqlite3* userDB;
	if (sqlite3_open("Database.db", &userDB) != SQLITE_OK) {
		cout << "Error to open user db" << endl;
		return;
	}
	cout << "UserDB open successfully" << endl;

	string req = "CREATE TABLE IF NOT EXISTS Users(ID TEXT, Name TEXT, Surname TEXT, Role TEXT, Password TEXT, FailedAttempts INTEGER DEFAULT 0, IsBlocked INTEGER DEFAULT 0, UNIQUE(ID, Name, Surname))";
	int result = sqlite3_exec(userDB, req.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		cout << "Error to send req to userDB." << endl;
		return;
	}
	cout << "Req. to userDB Has been sent succesfully" << endl;
}

void registerUser(string ID, string Name, string Surname, string Role, string Password) {
	sqlite3* userDB;
	if (sqlite3_open("Database.db", &userDB) != SQLITE_OK) {
		cout << "Error to open user db" << endl;
		return;
	}
	cout << "UserDB open successfully" << endl;

	string req = "INSERT INTO Users(ID, Name, Surname, Role, Password) VALUES('" + ID + "', '" + Name + "', '" + Surname + "', '" + Role + "', '" + Password + "')";
	int result = sqlite3_exec(userDB, req.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		cout << "Error to send req2 to userDB." << endl;
		return;
	}
	cout << "Req2. Has been sent to userDB succesfully" << endl;
	sqlite3_close(userDB);
}

bool loginUser(string ID, string Name, string Role, string Password) {
	sqlite3* userDB;

	if (sqlite3_open("Database.db", &userDB) != SQLITE_OK) {
		std::cout << "Error opening DB\n";
		return false;
	}

	const char* sql =
		"SELECT Password, FailedAttempts, IsBlocked "
		"FROM Users "
		"WHERE ID = ? AND Name = ? AND Role = ? "
		"LIMIT 1;";

	sqlite3_stmt* stmt = nullptr;

	sqlite3_prepare_v2(userDB, sql, -1, &stmt, nullptr); // я хотів попробувати зробити з SQL а не файлами, бо тоді по факту можна всю програму файлами зробити, понятно, що мені помагав ШІ, і також понятно, що файлами я можу сам, бо я це робив уже 1000 разів

	sqlite3_bind_text(stmt, 1, ID.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, Name.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, Role.c_str(), -1, SQLITE_TRANSIENT);

	int rc = sqlite3_step(stmt);

	if (rc != SQLITE_ROW) {
		sqlite3_finalize(stmt);
		sqlite3_close(userDB);
		cout << "user was not found" << endl;
		return false;
	}

	std::string storedPassword =
		reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

	int failedAttempts = sqlite3_column_int(stmt, 1);
	int isBlocked = sqlite3_column_int(stmt, 2);

	sqlite3_finalize(stmt);

	if (isBlocked) {
		sqlite3_close(userDB);
		cout << "This user is blocked" << endl;
		return false;
	}
	cout << "checking password" << endl;
	if (Password == storedPassword) {
		sqlite3_exec(userDB,
			("UPDATE Users SET FailedAttempts = 0 "
				"WHERE ID='" + ID + "' AND Name = '" + Name + "' AND Role = '" + Role + "'").c_str(),
			nullptr, nullptr, nullptr);

		sqlite3_close(userDB);
		cout << "Logged in successfully" << endl;
		return true;
	}
	else {
		failedAttempts++;

		if (failedAttempts >= 3) {
			sqlite3_exec(userDB,
				("UPDATE Users SET FailedAttempts = 3, IsBlocked = 1 "
					"WHERE ID='" + ID + "' AND Name = '" + Name + "' AND Role = '" + Role + "'").c_str(),
				nullptr, nullptr, nullptr);
			cout << "Login failed too many times, this user is now blocked" << endl;
		}
		else {
			sqlite3_exec(userDB,
				("UPDATE Users SET FailedAttempts = " +
					std::to_string(failedAttempts) +
					" WHERE ID = '" + ID + "' AND Name = '" + Name + "' AND Role = '" + Role + "'").c_str(),
				nullptr, nullptr, nullptr);
			cout << "Login failed" << endl;
		}
	}

	sqlite3_close(userDB);
	return false;
}

void createItemTable() {
	sqlite3* itemDB;

	if (sqlite3_open("Items.db", &itemDB) != SQLITE_OK) {
		cout << "Error to open items db" << endl;
		return;
	}

	string req = "CREATE TABLE IF NOT EXISTS Items(ID INTEGER PRIMARY KEY AUTOINCREMENT, Item TEXT UNIQUE, Amount INTEGER DEFAULT 0)";
	int result = sqlite3_exec(itemDB, req.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		cout << "Error to send req to itemsDB." << endl;
		return;
	}
	cout << "Req. to itemsDB Has been sent succesfully" << endl;
	sqlite3_close(itemDB);
}

void addItem(string item) {
	sqlite3* itemDB;

	if (sqlite3_open("Items.db", &itemDB) != SQLITE_OK) {
		cout << "Error to open items db" << endl;
		return;
	}
	cout << "ItemsDB open successfully" << endl;

	string req = "INSERT INTO Items(Item) VALUES('" + item + "')";
	int result = sqlite3_exec(itemDB, req.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		cout << "Error to send req to itemsDB." << endl;
		return;
	}
	cout << "Req. to itemsDB Has been sent succesfully" << endl;
	sqlite3_close(itemDB);
}

void addItemAmount(string item, string amount) {
	sqlite3* itemDB;

	if (sqlite3_open("Items.db", &itemDB) != SQLITE_OK) {
		cout << "Error to open items db" << endl;
		return;
	}
	cout << "ItemsDB open successfully" << endl;

	string req = "UPDATE Items SET Amount = '" + amount + "' WHERE Item = '" + item + "'";
	int result = sqlite3_exec(itemDB, req.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		cout << "Error to send req to itemsDB." << endl;
		return;
	}
	cout << "Req. to itemsDB Has been sent succesfully" << endl;
	sqlite3_close(itemDB);
}

void deleteItem(string item) {
	sqlite3* itemDB;

	if (sqlite3_open("Items.db", &itemDB) != SQLITE_OK) {
		cout << "Error to open items db" << endl;
		return;
	}
	cout << "ItemsDB open successfully" << endl;

	string req = "DELETE FROM Items WHERE Item = '" + item + "'";
	int result = sqlite3_exec(itemDB, req.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		cout << "Error to send req to itemsDB." << endl;
		return;
	}
	cout << "Req. to itemsDB Has been sent succesfully" << endl;
	sqlite3_close(itemDB);
}

void deleteItemAmount(string item, string amountToRemove) {
	sqlite3* itemDB;

	if (sqlite3_open("Items.db", &itemDB) != SQLITE_OK) {
		cout << "Error opening items db" << endl;
		return;
	}

	const char* sql =
		"UPDATE Items "
		"SET Amount = Amount - ? "
		"WHERE Item = ? AND Amount >= ?;";

	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(itemDB, sql, -1, &stmt, nullptr);

	sqlite3_bind_int(stmt, 1, stoi(amountToRemove));
	sqlite3_bind_text(stmt, 2, item.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 3, stoi(amountToRemove));

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		cout << "Update error: " << sqlite3_errmsg(itemDB) << endl;
	}

	int changes = sqlite3_changes(itemDB);

	if (changes == 0) {
		cout << "Not enough items in stock" << endl;
	}
	else {
		cout << "Amount updated successfully" << endl;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(itemDB);
}

void showAllItems() {
	sqlite3* itemDB;

	if (sqlite3_open("Items.db", &itemDB) != SQLITE_OK) {
		cout << "Error opening items db" << endl;
		return;
	}

	const char* sql = "SELECT Item, Amount FROM Items;";

	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(itemDB, sql, -1, &stmt, nullptr);

	cout << "Items in DB: " << endl;

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string item =
			reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		int amount = sqlite3_column_int(stmt, 1);

		cout << "Item: " << item << ", Amount: " << amount << endl;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(itemDB);
}
