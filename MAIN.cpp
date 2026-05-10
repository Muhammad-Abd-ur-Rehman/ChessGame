#include"GAME.h"
int main()
{
    GAME g;
    int choice;
    g.showMenu();
    cin >> choice;
    while (choice != 1 && choice != 2)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        g.showMessage("     INVALID! ENTER 1 OR 2     ", 12);
        g.showMenu();
        cin >> choice;
    }
    if (choice == 1)
    {
        system("cls");
        g.Start_Game();
    }
    else
    {
        system("cls");
        setColor(14, 0);
        gotoxy(30, 8);
        cout << "==============================";
        gotoxy(30, 9);
        cout << "|                            |";
        gotoxy(30, 10);
        cout << "|    BETTER LUCK NEXT TIME!  |";
        gotoxy(30, 11);
        cout << "|       GOODBYE CAPTAIN:)    |";
        gotoxy(30, 12);
        cout << "|                            |";
        gotoxy(30, 13);
        cout << "==============================";
        setColor(15, 0);
        gotoxy(30, 15);
        cin.get();
    }
    return 0;
}
