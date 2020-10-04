#include <iostream>
#include <windows.h>
#include <conio.h>

#include "Account.h"

using namespace std;

int main()
{
     cout << "\n\n\n\n\n\n \t\t\t\t\t" << "Helder Batalha" << endl;

     Sleep(500);

     system("cls");

    Account& acc = Account::getOnlyInstance();
    acc.mainMenu();

    return 0;
}


