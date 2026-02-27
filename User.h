#pragma once
#include <iostream>
#include "Database.h"
using namespace std;

class User {
private:
	string ID;
	string Name;
	string Surname;
	string Role;
	string Password;
public:
	User(string id, string name, string surname, string role, string password) {
		ID = id;
		Name = name;
		Surname = surname;
		Role = role;
		Password = password;
	}

	string GetID() {
		return ID;
	}

	string GetName() {
		return Name;
	}

	string GetSurname() {
		return Surname;
	}

	string GetRole() {
		return Role;
	}

	void Register() {
		registerUser(ID, Name, Surname, Role, Password);
	}

	bool Login() {
		if (loginUser(ID, Name, Role, Password)) {
			cout << "Successfully logged in!" << endl;
			return true;
		}
	}

	void ManageItems() {
		createItemTable();
		while (true) {
			cout << "What do you want to do next?\n1- add item category/ 2 - add amount of items/ 3 - delete item category/ 4 - subtract amount of items / 5 - see items" << endl;
			char choice;
			string choiceString;
			string choiceString2;
			cin >> choice;
			switch (choice) {
			case '1':
				cout << "what item do you want to add" << endl;
				cin >> choiceString;
				addItem(choiceString);
				break;
			case '2':
				cout << "what 1: item do you want to add, 2: amount of the item" << endl;
				cin >> choiceString;
				cin >> choiceString2;
				addItemAmount(choiceString, choiceString2);
				break;
			case '3':
				cout << "Which item do you want to delete" << endl;
				cin >> choiceString;
				deleteItem(choiceString);
				break;
			case '4':
				cout << "Which item do you want to 1: subtract from, 2: how many items do you want to subtract" << endl;
				cin >> choiceString;
				cin >> choiceString2;
				deleteItemAmount(choiceString, choiceString2);
				break;
			case '5':
				showAllItems();
				break;
			default:
				return;
			}
		}
	}
};
