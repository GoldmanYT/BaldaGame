#include "consts.hpp"
#include <fcntl.h>
#include <io.h>
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

int main()
{
    int _ = _setmode(_fileno(stdin), _O_U16TEXT);
    _ = _setmode(_fileno(stdout), _O_U16TEXT);

    wstring testWord = L"�����";
    BaldaGame game(testWord);

    game.selectLetter(L'�');
    game.placeLetter(1, 1);
    game.print();

    game.selectLetter(1, 1);
    for (int i = 1; i < 5; i++) {
        game.selectLetter(i, 2);
    }

    wstring word;
    word = game.getWord();

    wcout << word << endl;
    game.sendSelectedWord();

    word = game.getWord();
    wcout << word << endl;

    return 0;
}

BaldaGame::Cell::Cell()
{
    x = -1;
    y = -1;
    letter = BLANK_LETTER;
}

BaldaGame::Cell::Cell(int x, int y)
{
    this->x = x;
    this->y = y;
    letter = BLANK_LETTER;
}

BaldaGame::Cell::Cell(int x, int y, wchar_t letter)
{
    this->x = x;
    this->y = y;
    this->letter = letter;
}

bool BaldaGame::Cell::operator==(const Cell& other)
{
    return (x == other.x) && (y == other.y);
}

bool BaldaGame::Cell::operator==(wchar_t otherLetter)
{
    return letter == otherLetter;
}

BaldaGame::Cell::operator wchar_t() const
{
    return letter;
}

BaldaGame::Cell::operator bool() const
{
    return (letter != BLANK_LETTER) && (x != -1) && (y != -1);
}

BaldaGame::BaldaGame()
{
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            field[x][y] = Cell(x, y);
        }
    }
    placedLetter = Cell();
}

BaldaGame::BaldaGame(wstring word)
{
    for (int y = 0; y < BOARD_SIZE; y++) {
        if (y == BOARD_SIZE / 2) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                field[x][y] = Cell(x, y, word[x]);
            }
        } else {
            for (int x = 0; x < BOARD_SIZE; x++) {
                field[x][y] = Cell(x, y);
            }
        }
    }
    placedLetter = Cell();
}

void BaldaGame::print() const
{
    for (int x = 0; x < BOARD_SIZE; x++) {
        wcout << L"+---";
    }
    wcout << L"+\n";
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            wcout << L"| " << field[x][y].letter << L" ";
        }
        wcout << L"|\n";
        for (int x = 0; x < BOARD_SIZE; x++) {
            wcout << L"+---";
        }
        wcout << L"+\n";
    }
}

bool BaldaGame::onField(int x, int y)
{
    return (0 <= x) && (x < BOARD_SIZE) && (0 <= y) && (y < BOARD_SIZE);
}

bool BaldaGame::isNeighbor(Cell c1, Cell c2)
{
    return abs(c1.x - c2.x) + abs(c1.y - c2.y) == 1;
}

void BaldaGame::selectLetter(wchar_t letter)
{
    placedLetter.letter = letter;
}

void BaldaGame::placeLetter(int x, int y)
{
    if (!onField(x, y))
        return;
    if (placedLetter == BLANK_LETTER)
        return;
    if (placedLetter) {
        field[placedLetter.x][placedLetter.y].letter = BLANK_LETTER;
    }
    field[x][y].letter = placedLetter.letter;
    placedLetter.x = x;
    placedLetter.y = y;
}

void BaldaGame::selectLetter(int x, int y)
{
    if (!placedLetter)
        return;
    if (!onField(x, y))
        return;
    if (placedLetter == BLANK_LETTER)
        return;

    Cell cell = getCell(x, y);
    if (selectedWord.size()) {
        Cell lastCell = selectedWord.back();
        if (lastCell == cell) {
            selectedWord.pop_back();
        } else if (isNeighbor(lastCell, cell)) {
            bool notInSelectedWord = 1;
            for (Cell wordCell : selectedWord) {
                if (wordCell == cell) {
                    notInSelectedWord = 0;
                }
            }
            if (notInSelectedWord) {
                selectedWord.push_back(cell);
            }
        }
    } else {
        selectedWord.push_back(cell);
    }
}

wstring BaldaGame::getWord()
{
    wstring word;
    for (Cell wordCell : selectedWord) {
        word.push_back(wordCell);
    }
    return word;
}

wchar_t BaldaGame::getLetter(int x, int y) const
{
    return field[x][y].letter;
}

BaldaGame::Cell BaldaGame::getCell(int x, int y) const
{
    return field[x][y];
}

void BaldaGame::removeLetter()
{
    placedLetter = Cell();
}

bool BaldaGame::isWord()
{
    wstring word = getWord();
    return true;
}

void BaldaGame::sendSelectedWord()
{
    if (!isWord())
        return;

    bool hasPlacedLetter = 0;
    for (Cell coord : selectedWord) {
        if (coord == placedLetter) {
            hasPlacedLetter = 1;
            break;
        }
    }
    if (!hasPlacedLetter)
        return;

    wstring currentWord = getWord();
    for (wstring word : sentWords) {
        if (word == currentWord) {
            return;
        }
    }

    sentWords.push_back(currentWord);
    removeLetter();
    selectedWord.clear();
}
