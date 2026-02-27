#include <iostream>
#include "sqlite3/sqlite3.h"
#include "httplib.h"
#include "Server.h"
using namespace std;
using namespace httplib;

int main()
{
    User user = startServer();
    char choice;
    while (true) {
        createUserTable();
        cout << "Hello, " << user.GetName() << ", what do you want to do?" << endl;
        cout << "1 - Register/ 2 - Login" << endl;
        cin >> choice;
        switch (choice) {
        case '1':
            user.Register();
            break;
        case '2':
            if (user.Login()) {
                user.ManageItems();
            }
            break;
        default:
            return 0;
        }
    }
}
