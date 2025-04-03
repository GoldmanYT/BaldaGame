#include "consts.hpp"
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <vector>

using namespace std;

class BaldaGame {
    struct Coord {
        int x, y;
        bool operator==(Coord other)
        {
            return (x == other.x) && (y == other.y);
        }
    };

    wchar_t field[BOARD_SIZE][BOARD_SIZE];
    vector<Coord> selectedWord;
    wchar_t selectedLetter;
    Coord placedLetterCoord;
    vector<wstring> sentWords;

    bool isNeighbor(Coord c1, Coord c2);
    bool onField(int x, int y);

public:
    wstring getWord();
    BaldaGame();
    BaldaGame(wstring word);
    void print() const;
    void selectLetter(int x, int y);
    void selectLetter(wchar_t letter);
    void placeLetter(int x, int y);
    void removeLetter();
    bool isWord();
    void sendSelectedWord();
};

int main()
{
    int _ = _setmode(_fileno(stdin), _O_U16TEXT);
    _ = _setmode(_fileno(stdout), _O_U16TEXT);

    wstring testWord = L"¡¿Àƒ¿";
    BaldaGame game(testWord);

    game.print();
    game.selectLetter(L'¿');
    game.placeLetter(0, 0);

    for (int i = 0; i < 5; i++) {
        game.selectLetter(i, 2);
    }

    wstring word;
    word = game.getWord();

    wcout << word;

    return 0;
}

BaldaGame::BaldaGame()
{
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
            field[x][y] = BLANK_LETTER;
    selectedLetter = BLANK_LETTER;
    placedLetterCoord.x = -1;
    placedLetterCoord.y = -1;
}

BaldaGame::BaldaGame(wstring word)
{
    for (int y = 0; y < BOARD_SIZE; y++) {
        if (y == BOARD_SIZE / 2) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                field[x][y] = word[x];
            }
        } else {
            for (int x = 0; x < BOARD_SIZE; x++) {
                field[x][y] = BLANK_LETTER;
            }
        }
    }
    selectedLetter = BLANK_LETTER;
    placedLetterCoord.x = -1;
    placedLetterCoord.y = -1;
}

void BaldaGame::print() const
{
    for (int x = 0; x < BOARD_SIZE; x++) {
        wcout << L"+---";
    }
    wcout << L"+\n";
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            wcout << L"| " << field[x][y] << L" ";
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

bool BaldaGame::isNeighbor(Coord c1, Coord c2)
{
    return abs(c1.x - c2.x) + abs(c1.y - c2.y) == 1;
}

void BaldaGame::selectLetter(int x, int y)
{
    if (selectedLetter == BLANK_LETTER)
        return;
    if ((placedLetterCoord.x == -1) || (placedLetterCoord.y == -1))
        return;

    Coord coord = { x, y };
    if (!onField(x, y))
        return;
    if (selectedWord.size()) {
        Coord lastCoord = selectedWord.back();
        if (lastCoord == coord) {
            selectedWord.pop_back();
        } else if (isNeighbor(lastCoord, coord)) {
            bool notInSelectedWord = 1;
            for (Coord wordCoord : selectedWord) {
                if (wordCoord == coord) {
                    notInSelectedWord = 0;
                }
            }
            if (notInSelectedWord) {
                selectedWord.push_back(coord);
            }
        }
    } else {
        selectedWord.push_back(coord);
    }
}

wstring BaldaGame::getWord()
{
    wstring word(BOARD_SIZE * BOARD_SIZE + 1, L'\0');
    for (int i = 0; i < selectedWord.size(); i++) {
        Coord coord = selectedWord[i];
        word[i] = field[coord.x][coord.y];
    }
    return word;
}

void BaldaGame::selectLetter(wchar_t letter)
{
    selectedLetter = letter;
}

void BaldaGame::placeLetter(int x, int y)
{
    if (!onField(x, y))
        return;
    if (selectedLetter == BLANK_LETTER)
        return;
    if ((placedLetterCoord.x != -1) && (placedLetterCoord.y != -1)) {
        field[placedLetterCoord.x][placedLetterCoord.y] = BLANK_LETTER;
    }
    field[x][y] = selectedLetter;
    placedLetterCoord.x = x;
    placedLetterCoord.y = y;
}

void BaldaGame::removeLetter()
{
    selectedLetter = BLANK_LETTER;
    placedLetterCoord.x = -1;
    placedLetterCoord.y = -1;
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
    for (Coord coord : selectedWord) {
        if (coord == placedLetterCoord) {
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
    selectedLetter = BLANK_LETTER;
    placedLetterCoord.x = -1;
    placedLetterCoord.y = -1;
    selectedWord.clear();
}