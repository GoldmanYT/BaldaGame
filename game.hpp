#pragma once
#include "consts.hpp"
#include <iostream>
#include <set>
#include <vector>

using namespace std;

class BaldaGame {
    struct Cell {
        int x, y;
        wchar_t letter;
        Cell();
        Cell(int x, int y);
        Cell(int x, int y, wchar_t letter);
        bool operator==(const Cell& other);
        bool operator==(wchar_t otherLetter);
        operator wchar_t() const;
        explicit operator bool() const;
    };

public:
    struct Player {
        int score, missedMoves;
        vector<wstring> words;
        Player();
    };

    enum GameState {
        inGame = -2,
        tie = -1,
        player1Won = 0,
        player2Won = 1
    };

private:
    Cell field[BOARD_SIZE][BOARD_SIZE];
    vector<Cell> selectedWord;
    Cell placedLetter;
    vector<wstring> sentWords;
    vector<Player> players;
    int playerMove;
    set<wstring> words;
    vector<wstring> startWords;
    GameState gameState;

    bool isNeighbor(Cell c1, Cell c2);
    bool onField(int x, int y);
    void loadWords();
    void toNextPlayer();
    void updateGameState();

public:
    vector<Cell> getSelectedWord();
    wstring getWord();
    wchar_t getLetter(int x, int y) const;
    Cell getPlacedLetter() const;
    Cell getCell(int x, int y) const;
    BaldaGame();
    BaldaGame(wstring word);
    void newGame();
    void selectLetter(wchar_t letter);
    void selectLetter(int x, int y);
    void removeLetter();
    bool isWord();
    void sendSelectedWord();
    void missMove();
    Player* getPlayer(int index);
    Player* getCurrentPlayer();
    int getCurrentPlayerIndex() const;
    GameState getGameState() const;
};