﻿#include "consts.hpp"
#include "game.hpp"
#include <Windows.h>
#include <iostream>
#include <vector>

using namespace std;

class UserInterface {
    struct Coord {
        int x, y;
        Coord();
        Coord(int x, int y);
    };
    class Button {
        Coord pos;
        Coord size;
        wstring text;
        bool selected;

    public:
        Button();
        Button(int x, int y, int width, int height);
        Button(int x, int y, int width, int height, wstring text);
        void draw(HANDLE& handle) const;
        void setSelected(bool state);
        Coord getPos() const;
        Coord getSize() const;
    };
    class ButtonGrid {
        Coord size;
        vector<vector<Button>> grid;

    public:
        ButtonGrid(int width, int height);
        void draw(HANDLE& handle);
        void placeButton(int x, int y, Button& button);
        Button* getButton(int x, int y);
        Coord getSize();
    };
    HANDLE handleInput;
    HANDLE handleOutput;
    BaldaGame* game;
    int tabIndex;
    Coord buttonGridPos;
    vector<ButtonGrid> buttonGrids;
    void mouseEvent(MOUSE_EVENT_RECORD);
    void keyboardEvent(KEY_EVENT_RECORD);

public:
    UserInterface(BaldaGame* game, HANDLE hStdIn, HANDLE hStdOut);
    void update();
    void draw();
};
