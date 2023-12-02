#include <iostream>
#include <sstream>
#include <windows.h>
#include <random>

using std::string;
using std::cout;
using std::cin;
using std::to_string;
using std::stoi;
using std::getline;
using std::size;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

void menu();
void displayField(int field[3][3]);
void playerInput(int player, int field[3][3], int& x, int& y);
int count(int arr[3], int val);
int loc(int arr[3], int val);
void botInput(int bid, int pid, int field[3][3], int& x, int& y);
int lineCheck(int line[3]);
void winCheck(int field[3][3]);
void drawCheck(int field[3][3]);
void newFrame(int field[3][3]);
void gameLoop(string mode = "multi", bool playerFirst = false);
void end(string state, int field[3][3], int player = 0);

int main()
{
    system("color 70");
    menu();
    return 0;
}

void menu() {
    system("cls");
    cout << "Tic-tac-toe -> by adobko (in C++)\n" << "Menu:\n1) Singleplayer\n2) Multiplayer\n3) Exit\n";
    string choice;
    cout << "Choose how to proceed: ";
    cin >> choice;
    if (choice == "1") {
        string choice2;
        while (true) {
            cout << "Want to begin first? (y/n): ";
            cin >> choice2;
            if (choice2 == "y") { gameLoop("single", true); }
            if (choice2 == "n") { gameLoop("single"); }
            else { cout << "Please input (y/n)!\n"; }
        }
    }
    else if (choice == "2") { gameLoop(); }
    else if (choice == "3") { exit(0); }
    else {
        cout << "Please input a vallid choice!\n";
        Sleep(1000);
        menu();
    }
}

void displayField(int field[3][3]) {
    string display = "  A B C\n";
    string item, row;
    for (int y = 0; y < 3; y++) {
        row = to_string(y + 1);
        for (int x = 0; x < 3; x++) {
            switch (field[y][x]) {
            case 0: item = "-"; break;
            case 1: item = "X"; break;
            case 2: item = "O"; break;
            }
            row += " " + item;
        }
        display += row + "\n";
    }
    cout << display;
}

void playerInput(int player, int field[3][3], int& x, int& y) {
    string input;
    while (true) {
        cout << "Player " << player << " input (xy): ";
        cin >> input;
        if (size(input) != 2) { cout << "Pleas input the coords in a format (xy)!\n"; continue; }
        int xIn, yIn;
        switch (input[0]) {
        case 'a': xIn = 0; break;
        case 'b': xIn = 1; break;
        case 'c': xIn = 2; break;
        default: cout << "Invalid input, x coord needs to be a letter a,b or c!\n"; continue;
        }
        yIn = stoi(input.substr(1, 1), nullptr, 10) - 1;
        if (yIn < 0 || yIn > 2) { cout << "Invalid input, y coord needs to be a number 1-3!\n"; continue; }
        if (field[yIn][xIn] != 0) { cout << "Please enter coordinates to a not yet used field!\n"; continue; }
        x = xIn; y = yIn;
        break;
    }
}

int count(int arr[3], int val) {
    int num = 0;
    for (int i = 0; i < 3; i++) {
        if (arr[i] == val) { num++; }
    }
    return num;
}

int loc(int arr[3], int val) {
    for (int i = 0; i < 3; i++) {
        if (arr[i] == val) { return i; }
    }
}

void botInput(int bid, int pid, int field[3][3], int& x, int& y) {
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<int> dist(0, 2);
    cout << "Player " << bid << " (BOT)\n";
    int xIn = dist(eng);
    int yIn = dist(eng);
    int row[3], column[3], diagonal1[3], diagonal2[3];
    int xR = 3;
    int ones, twos = 0;
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            row[y] = field[x][y];
            column[y] = field[y][x];
        }
        if (count(row, 0) == 1 && (count(row, bid) == 2 || count(row, pid) == 2)) {
            xIn = loc(row, 0);
            yIn = x;
        }
        if (count(column, 0) == 1 && (count(column, bid) == 2 || count(column, pid) == 2)) {
            xIn = x;
            yIn = loc(column, 0);
        }
        diagonal1[x] = field[x][x];
        xR--;
        diagonal2[x] = field[x][xR];
    }
    if (count(diagonal1, 0) == 1 && (count(diagonal1, bid) == 2 || count(diagonal1, pid) == 2)) {
        xIn = loc(diagonal1, 0);
        yIn = loc(diagonal1, 0);
    }
    if (count(diagonal2, 0) == 1 && (count(diagonal2, bid) == 2 || count(diagonal2, pid) == 2)) {
        xIn = 2 - loc(diagonal2, 0);
        yIn = loc(diagonal2, 0);
    }
    while (field[yIn][xIn] != 0) {
        xIn = dist(eng);
        yIn = dist(eng);
    }
    x = xIn; y = yIn;
}

int lineCheck(int line[3]) {
    int ones = 0, twos = 0;
    for (int i = 0; i < 3; i++) {
        switch (line[i]) {
        case 1: ones++; break;
        case 2: twos++; break;
        }
    }
    if (ones == 3) { return 1; }
    else if (twos == 3) { return 2; }
    return 0;
}

void winCheck(int field[3][3]) {
    int row[3], column[3], diagonal1[3], diagonal2[3], winner;
    int xR = 3;
    for (int x = 0; x < 3; x++) {
        // column/row build/check
        for (int y = 0; y < 3; y++) {
            row[y] = field[x][y];
            column[y] = field[y][x];
        }
        winner = lineCheck(row);
        if (winner != 0) { end("win", field, winner); }
        winner = lineCheck(column);
        if (winner != 0) { end("win", field, winner); }

        // diagonal builds
        diagonal1[x] = field[x][x];
        xR--;
        diagonal2[x] = field[x][xR];
    }
    // diagonal checks
    winner = lineCheck(diagonal1);
    if (winner != 0) { end("win", field, winner); }
    winner = lineCheck(diagonal2);
    if (winner != 0) { end("win", field, winner); }
}

void drawCheck(int field[3][3]) {
    int zeros = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (field[y][x] == 0) { zeros++; }
        }
    }
    if (zeros == 0) { end("draw", field); }
}

void newFrame(int field[3][3]) {
    system("cls");
    displayField(field);
}

void gameLoop(string mode, bool playerFirst) {
    int field[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    const int PL1 = 1;
    const int PL2 = 2;
    int x1, y1, x2, y2;
    while (true) {
        // player 1
        newFrame(field);
        if (mode == "multi" || playerFirst) { playerInput(PL1, field, x1, y1); }
        else { botInput(PL1, PL2, field, x1, y1); }
        field[y1][x1] = PL1;
        winCheck(field);

        drawCheck(field);

        // player 2
        newFrame(field);
        if (mode == "multi" || !playerFirst) { playerInput(PL2, field, x2, y2); }
        else { botInput(PL2, PL1, field, x2, y2); }
        field[y2][x2] = PL2;
        winCheck(field);
    }
}

void end(string state, int field[3][3], int player) {
    newFrame(field);
    if (state == "win") {
        cout << "       _      _                   \n      (_)    | |                  \n__   ___  ___| |_ ___  _ __ _   _ \n\\ \\ / / |/ __| __/ _ \\| '__| | | |\n \\ V /| | (__| || (_) | |  | |_| |\n  \\_/ |_|\\___|\\__\\___/|_|   \\__, |\n                             __/ |\n                            |___/ \n";
        cout << "Player " << player << " has WON!\n";
    }
    else if (state == "draw") {
        cout << "Draw!\n";
        cout << "The field was filled yet noone has won...\n";
    }
    system("pause");
    menu();
}
