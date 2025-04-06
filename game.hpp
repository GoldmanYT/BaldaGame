#pragma once
#include "consts.hpp"
#include <iostream>
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

    Cell field[BOARD_SIZE][BOARD_SIZE];
    vector<Cell> selectedWord;
    Cell placedLetter;
    vector<wstring> sentWords;

    bool isNeighbor(Cell c1, Cell c2);
    bool onField(int x, int y);

public:
    wstring getWord();
    wchar_t getLetter(int x, int y) const;
    Cell getCell(int x, int y) const;
    BaldaGame();
    BaldaGame(wstring word);
    void print() const;
    void selectLetter(wchar_t letter);
    void placeLetter(int x, int y);
    void selectLetter(int x, int y);
    void removeLetter();
    bool isWord();
    void sendSelectedWord();
};