#include "game.hpp"
#include <fstream>

using namespace std;

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
    wstring word = L"БАЛДА";
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
    playerMove = 0;
    players.resize(PLAYER_COUNT);
    sentWords.push_back(word);
    loadWords();
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
    playerMove = 0;
    players.resize(PLAYER_COUNT);
    sentWords.push_back(word);
    loadWords();
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

void BaldaGame::loadWords()
{
    wifstream file("words.txt", 'b');
    wstring word;

    if (file.is_open()) {
        wchar_t c1 = file.get(), c2 = file.get();
        while (file) {
            wchar_t c = (c1 << 8) | c2;
            if (c != L'\n') {
                word.push_back(c);
            } else {
                words.insert(word);
                word.clear();
            }
            c1 = file.get();
            c2 = file.get();
        }
    }

    file.close();
}

bool BaldaGame::isNeighbor(Cell c1, Cell c2)
{
    return abs(c1.x - c2.x) + abs(c1.y - c2.y) == 1;
}

void BaldaGame::selectLetter(wchar_t letter)
{
    placedLetter.letter = letter;
}

void BaldaGame::selectLetter(int x, int y)
{
    if (!onField(x, y))
        return;
    if (placedLetter == BLANK_LETTER)
        return;
    if (!placedLetter) {
        field[x][y].letter = placedLetter.letter;
        placedLetter.x = x;
        placedLetter.y = y;
        return;
    }

    Cell cell = getCell(x, y);
    if (cell.letter == BLANK_LETTER)
        return;

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

vector<BaldaGame::Cell> BaldaGame::getSelectedWord()
{
    return selectedWord;
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

BaldaGame::Cell BaldaGame::getPlacedLetter() const
{
    return placedLetter;
}

BaldaGame::Cell BaldaGame::getCell(int x, int y) const
{
    return field[x][y];
}

void BaldaGame::removeLetter()
{
    if (placedLetter) {
        field[placedLetter.x][placedLetter.y].letter = BLANK_LETTER;
    }
    placedLetter = Cell();
    selectedWord.clear();
}

bool BaldaGame::isWord()
{
    wstring word = getWord();
    return words.count(word);
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

    Player* currentPlayer = getCurrentPlayer();
    currentPlayer->words.push_back(sentWords.back());
    currentPlayer->score += int(currentWord.size());
    playerMove = (playerMove + 1) % PLAYER_COUNT;

    placedLetter = Cell();
    selectedWord.clear();
}

void BaldaGame::missMove()
{
}

BaldaGame::Player* BaldaGame::getPlayer(int index)
{
    return &players[index];
}

BaldaGame::Player* BaldaGame::getCurrentPlayer()
{
    return &players[playerMove];
}

BaldaGame::Player::Player()
    : score(0)
    , missedMoves(0)
{
}
