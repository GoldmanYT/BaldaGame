#include "consts.hpp"
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
    protected:
        Coord pos;
        Coord size;
        wstring text;
        bool selected;

    public:
        Button();
        Button(int x, int y, int width, int height);
        Button(int x, int y, int width, int height, wstring text);
        virtual void draw(HANDLE& handle) const;
        void setSelected(bool state);
        Coord getPos() const;
        Coord getSize() const;
        virtual void onClick() = 0;
    };
    class StartGameButton : public Button {
        BaldaGame* game;

    public:
        StartGameButton(BaldaGame* game);
        void onClick() override;
    };
    class MissMoveButton : public Button {
        BaldaGame* game;

    public:
        MissMoveButton(BaldaGame* game);
        void onClick() override;
    };
    class RemoveLetterButton : public Button {
        BaldaGame* game;

    public:
        RemoveLetterButton(BaldaGame* game);
        void onClick() override;
    };
    class FieldButton : public Button {
        BaldaGame* game;
        Coord fieldPos;

    public:
        FieldButton(int x, int y, int fieldX, int fieldY, wchar_t letter, BaldaGame* game);
        void onClick() override;
        void setLetter(wchar_t letter);
        // virtual void draw(HANDLE& handle) const override;
    };
    class WordButton : public Button {
        BaldaGame* game;

    public:
        WordButton(BaldaGame* game);
        void onClick() override;
        void update();
    };
    class LetterButton : public Button {
        BaldaGame* game;

    public:
        LetterButton(int x, int y, wchar_t letter, BaldaGame* game);
        void onClick() override;
    };
    class PlayerStats : Button {
        BaldaGame::Player* player;
        void onClick();

    public:
        PlayerStats(int x, int y, BaldaGame::Player* playerPtr);
        void draw(HANDLE& handle) const;
    };
    class ButtonGrid {
        Coord size;
        vector<vector<Button*>> grid;

    public:
        ButtonGrid(int width, int height);
        void draw(HANDLE& handle);
        void placeButton(int x, int y, Button* button);
        Button* getButton(int x, int y);
        Coord getSize();
    };
    HANDLE handleInput;
    HANDLE handleOutput;
    BaldaGame* game;
    int tabIndex;
    Coord buttonGridPos;
    vector<ButtonGrid> buttonGrids;
    vector<PlayerStats> playerStats;
    void mouseEvent(MOUSE_EVENT_RECORD);
    void keyboardEvent(KEY_EVENT_RECORD);
    void draw();

public:
    UserInterface(BaldaGame* game, HANDLE hStdIn, HANDLE hStdOut);
    void update();
};
