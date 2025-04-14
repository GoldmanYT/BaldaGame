#include "game.hpp"
#include <fstream>
#include <random>
#include <ctime>

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
    loadWords();
    srand(unsigned int(time(0)));
    wstring word = startWords[rand() % startWords.size()];
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
    gameState = inGame;
    players.resize(PLAYER_COUNT);
    sentWords.push_back(word);
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
    gameState = inGame;
    players.resize(PLAYER_COUNT);
    sentWords.push_back(word);
    loadWords();
}

void BaldaGame::newGame()
{
    wstring word = startWords[rand() % startWords.size()];
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
    gameState = inGame;
    players.clear();
    players.resize(PLAYER_COUNT);
    sentWords.clear();
    sentWords.push_back(word);
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
        file.get();
        file.get();
        wchar_t c1 = file.get(), c2 = file.get();
        while (file) {
            wchar_t c = c1 | (c2 << 8);
            switch (c) {
            case L'\n':
                words.insert(word);
                if (word.size() == BOARD_SIZE) {
                    startWords.push_back(word);
                }
                word.clear();
                break;
            case L'\r':
                break;
            default:
                word.push_back(c);
                break;
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
    if (gameState != inGame)
        return;
    placedLetter.letter = letter;
}

void BaldaGame::selectLetter(int x, int y)
{
    if (gameState != inGame)
        return;
    if (!onField(x, y))
        return;
    if (placedLetter == BLANK_LETTER)
        return;
    if (!placedLetter) {
        if (getLetter(x, y) != BLANK_LETTER)
            return;
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
    if (gameState != inGame)
        return;
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
    toNextPlayer();

    placedLetter = Cell();
    selectedWord.clear();
    updateGameState();
}

void BaldaGame::missMove()
{
    if (gameState != inGame)
        return;
    Player* player = getCurrentPlayer();
    player->missedMoves++;
    toNextPlayer();
    updateGameState();
}

BaldaGame::Player* BaldaGame::getPlayer(int index)
{
    return &players[index];
}

BaldaGame::Player* BaldaGame::getCurrentPlayer()
{
    return &players[playerMove];
}

int BaldaGame::getCurrentPlayerIndex() const
{
    return playerMove;
}

BaldaGame::GameState BaldaGame::getGameState() const
{
    return gameState;
}

void BaldaGame::toNextPlayer()
{
    playerMove = (playerMove + 1) % PLAYER_COUNT;
}

void BaldaGame::updateGameState()
{
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (players[i].missedMoves == MAX_PLAYER_MISSED_MOVES) {
            gameState = GameState(1 - i);
            break;
        }
    }
    bool hasEmptyCells = 0;
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (getLetter(x, y) == BLANK_LETTER) {
                hasEmptyCells = 1;
                break;
            }
        }
        if (hasEmptyCells)
            break;
    }
    if (!hasEmptyCells) {
        int maxScorePlayerIndex = 0, maxScore = players[0].score;
        for (int i = 1; i < PLAYER_COUNT; i++) {
            if (players[i].score > maxScore) {
                maxScore = players[i].score;
                maxScorePlayerIndex = i;
            } else if (players[i].score == maxScore) {
                maxScorePlayerIndex = -1;
            }
        }
        if (maxScorePlayerIndex == -1) {
            gameState = tie;
        } else {
            gameState = GameState(maxScorePlayerIndex);
        }
    }
}

BaldaGame::Player::Player()
    : score(0)
    , missedMoves(0)
{
}
