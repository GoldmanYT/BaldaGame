#include "interface.hpp"
#include "game.hpp"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

UserInterface::Coord::Coord()
{
    x = 0;
    y = 0;
}

UserInterface::Coord::Coord(int x, int y)
{
    this->x = x;
    this->y = y;
}

UserInterface::Button::Button()
    : pos()
    , size()
    , selected(0)
{
}

UserInterface::Button::Button(int x, int y, int width, int height)
    : pos(x, y)
    , size(width, height)
    , selected(0)
{
}

UserInterface::Button::Button(int x, int y, int width, int height, wstring text)
    : pos(x, y)
    , size(width, height)
    , selected(0)
{
    this->text = text;
}

void UserInterface::Button::draw(HANDLE& handle) const
{
    COORD coord {};
    DWORD _;
    Coord textCoord = Coord(pos.x + (size.x - int(text.size())) / 2, pos.y + (size.y - 1) / 2);
    for (int x = pos.x; x < pos.x + size.x; x++) {
        coord.X = x;
        for (int y = pos.y; y < pos.y + size.y; y++) {
            coord.Y = y;
            if (x >= textCoord.x && x < textCoord.x + text.size() && y == textCoord.y) {
                WriteConsoleOutputCharacter(handle, &text[size_t(x - textCoord.x)], 1, coord, &_);
            } else {
                wchar_t border = BUTTON_BORDERS[selected + 2 * (1 * (x == pos.x && y == pos.y) + 2 * (x == pos.x + size.x - 1 && y == pos.y) + 3 * (x == pos.x + size.x - 1 && y == pos.y + size.y - 1) + 4 * (x == pos.x && y == pos.y + size.y - 1) + 5 * ((x == pos.x || x == pos.x + size.x - 1) && pos.y < y && y < pos.y + size.y - 1) + 6 * (pos.x < x && x < pos.x + size.x - 1 && (y == pos.y || y == pos.y + size.y - 1)))];
                WriteConsoleOutputCharacter(handle, &border, 1, coord, &_);
            }
        }
    }
}

void UserInterface::Button::setSelected(bool state)
{
    selected = state;
}

UserInterface::Coord UserInterface::Button::getPos() const
{
    return pos;
}

UserInterface::Coord UserInterface::Button::getSize() const
{
    return size;
}

UserInterface::StartGameButton::StartGameButton(BaldaGame* game)
    : Button(START_GAME_BUTTON_X, START_GAME_BUTTON_Y,
          START_GAME_BUTTON_WIDTH, START_GAME_BUTTON_HEIGHT,
          START_GAME_TEXT)
{
    this->game = game;
}

void UserInterface::StartGameButton::onClick()
{
    game->newGame();
}

UserInterface::MissMoveButton::MissMoveButton(BaldaGame* game)
    : Button(MISS_MOVE_BUTTON_X, MISS_MOVE_BUTTON_Y,
          MISS_MOVE_BUTTON_WIDTH, MISS_MOVE_BUTTON_HEIGHT,
          MISS_MOVE_TEXT)
{
    this->game = game;
}

void UserInterface::MissMoveButton::onClick()
{
    game->missMove();
}

UserInterface::RemoveLetterButton::RemoveLetterButton(BaldaGame* game)
    : Button(REMOVE_LETTER_BUTTON_X, REMOVE_LETTER_BUTTON_Y,
          REMOVE_LETTER_BUTTON_WIDTH, REMOVE_LETTER_BUTTON_HEIGHT,
          REMOVE_LETTER_TEXT)
{
    this->game = game;
}

void UserInterface::RemoveLetterButton::onClick()
{
    game->removeLetter();
}

UserInterface::FieldButton::FieldButton(int x, int y, int fieldX, int fieldY, wchar_t letter, BaldaGame* game)
    : Button(x, y, FIELD_BUTTON_WIDTH, FIELD_BUTTON_HEIGHT, wstring(1, letter))
    , fieldPos(fieldX, fieldY)
{
    this->game = game;
}

void UserInterface::FieldButton::onClick()
{
    game->selectLetter(fieldPos.x, fieldPos.y);
}

void UserInterface::FieldButton::setLetter(wchar_t letter)
{
    text = wstring(1, letter);
}

UserInterface::WordButton::WordButton(BaldaGame* game)
    : Button(WORD_BUTTON_X, WORD_BUTTON_Y, WORD_BUTTON_WIDTH, WORD_BUTTON_HEIGHT, wstring())
{
    this->game = game;
}

void UserInterface::WordButton::onClick()
{
    game->sendSelectedWord();
}

void UserInterface::WordButton::update()
{
    text.clear();
    switch (game->getGameState()) {
    case game->GameState::player1Won:
        text = PLAYER1_WON_TEXT;
        return;
    case game->GameState::player2Won:
        text = PLAYER2_WON_TEXT;
        return;
    case game->GameState::tie:
        text = TIE_TEXT;
        return;
    default:
        break;
    }
    wstring word = game->getWord();
    if (!word.size())
        return;
    for (wchar_t letter : word) {
        text.push_back(letter);
    }
    text.append(L" (");
    wstring score = to_wstring(int(word.size()));
    text.append(score);
    text.append(L")");
}

UserInterface::LetterButton::LetterButton(int x, int y, wchar_t letter, BaldaGame* game)
    : Button(x, y, LETTER_BUTTON_WIDTH, LETTER_BUTTON_HEIGHT, wstring(1, letter))
{
    this->game = game;
}

void UserInterface::LetterButton::onClick()
{
    game->selectLetter(text[0]);
}

UserInterface::ButtonGrid::ButtonGrid(int width, int height)
{
    size = Coord(width, height);
    grid.resize(width);
    for (int x = 0; x < width; x++) {
        grid[x].resize(height);
    }
}

void UserInterface::ButtonGrid::draw(HANDLE& handle)
{
    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            Button* button = grid[x][y];
            if (button) {
                button->draw(handle);
            }
        }
    }
}

void UserInterface::ButtonGrid::placeButton(int x, int y, Button* button)
{
    grid[x][y] = button;
}

UserInterface::Button* UserInterface::ButtonGrid::getButton(int x, int y)
{
    return grid[x][y];
}

UserInterface::Coord UserInterface::ButtonGrid::getSize()
{
    return Coord(int(grid.size()), int(grid[0].size()));
}

void UserInterface::mouseEvent(MOUSE_EVENT_RECORD mouseEventRecord)
{
    COORD mousePos = mouseEventRecord.dwMousePosition;
    int selectedButtonGridIndex;
    Coord selectedButtonGridPos;
    Button* selectedButton = 0;
    for (int buttonGridIndex = 0; buttonGridIndex < buttonGrids.size(); buttonGridIndex++) {
        ButtonGrid& buttonGrid = buttonGrids[buttonGridIndex];
        Coord gridSize = buttonGrid.getSize();
        for (int gridX = 0; gridX < gridSize.x; gridX++) {
            for (int gridY = 0; gridY < gridSize.y; gridY++) {
                Button* button = buttonGrid.getButton(gridX, gridY);
                Coord pos = button->getPos(), size = button->getSize();
                if (pos.x <= mousePos.X && mousePos.X < pos.x + size.x && pos.y <= mousePos.Y && mousePos.Y < pos.y + size.y) {
                    selectedButtonGridIndex = buttonGridIndex;
                    selectedButtonGridPos = Coord(gridX, gridY);
                    selectedButton = button;
                }
            }
        }
    }
    switch (mouseEventRecord.dwEventFlags) {
    case 0:
        switch (mouseEventRecord.dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            if (selectedButton) {
                selectedButton->onClick();
            }
            break;
        default:
            break;
        }
    case MOUSE_MOVED:
        if (selectedButton) {
            tabIndex = selectedButtonGridIndex;
            buttonGridPos = selectedButtonGridPos;
        }
        break;
    default:
        break;
    }
}

void UserInterface::keyboardEvent(KEY_EVENT_RECORD keyEventRecord)
{
    Coord currentGridSize = buttonGrids[tabIndex].getSize();
    if (keyEventRecord.bKeyDown) {
        switch (keyEventRecord.wVirtualKeyCode) {
        case VK_RIGHT:
            buttonGridPos.x = (buttonGridPos.x + 1) % currentGridSize.x;
            break;
        case VK_LEFT:
            buttonGridPos.x = (buttonGridPos.x - 1 + currentGridSize.x) % currentGridSize.x;
            break;
        case VK_DOWN:
            buttonGridPos.y = (buttonGridPos.y + 1) % currentGridSize.y;
            break;
        case VK_UP:
            buttonGridPos.y = (buttonGridPos.y - 1 + currentGridSize.y) % currentGridSize.y;
            break;
        case VK_TAB:
            buttonGridPos = Coord();
            if (keyEventRecord.dwControlKeyState & SHIFT_PRESSED) {
                tabIndex = (tabIndex - 1 + int(buttonGrids.size())) % int(buttonGrids.size());
            } else {
                tabIndex = (tabIndex + 1) % int(buttonGrids.size());
            }
            break;
        case VK_SPACE: {
        case VK_RETURN:
            ButtonGrid& buttonGrid = buttonGrids[tabIndex];
            Button* button = buttonGrid.getButton(buttonGridPos.x, buttonGridPos.y);
            button->onClick();
            break;
        }
        default:
            break;
        }
    }
}

UserInterface::UserInterface(BaldaGame* gamePtr, HANDLE hStdIn, HANDLE hStdOut)
    : game(gamePtr)
{
    handleInput = hStdIn;
    handleOutput = hStdOut;
    tabIndex = 0;
    buttonGridPos = Coord();

    COORD size = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
    SetConsoleScreenBufferSize(hStdOut, size);
    HWND hWindowConsole = GetConsoleWindow();
    MoveWindow(hWindowConsole, 0, 0, CONSOLE_WIDTH * 8, CONSOLE_HEIGHT * 18, true);
    SetConsoleMode(handleInput, ENABLE_MOUSE_INPUT);

    StartGameButton* startGameButton = new StartGameButton(game);
    MissMoveButton* missMoveButton = new MissMoveButton(game);
    RemoveLetterButton* removeLetterButton = new RemoveLetterButton(game);

    ButtonGrid menuButtonGrid = ButtonGrid(3, 1);
    menuButtonGrid.placeButton(0, 0, startGameButton);
    menuButtonGrid.placeButton(1, 0, missMoveButton);
    menuButtonGrid.placeButton(2, 0, removeLetterButton);

    ButtonGrid fieldButtonGrid = ButtonGrid(BOARD_SIZE, BOARD_SIZE);
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            FieldButton* fieldButton = new FieldButton(
                FIELD_BUTTON_X + x * FIELD_BUTTON_WIDTH,
                FIELD_BUTTON_Y + y * FIELD_BUTTON_HEIGHT,
                x, y,
                game->getLetter(x, y),
                game);
            fieldButtonGrid.placeButton(x, y, fieldButton);
        }
    }

    ButtonGrid wordButtonGrid = ButtonGrid(1, 1);
    WordButton* wordButton = new WordButton(game);
    wordButtonGrid.placeButton(0, 0, wordButton);

    ButtonGrid letterButtonGrid = ButtonGrid(LETTERS_BUTTON_GRID_WIDTH, LETTERS_BUTTON_GRID_HEIGHT);
    for (int x = 0; x < LETTERS_BUTTON_GRID_WIDTH; x++) {
        for (int y = 0; y < LETTERS_BUTTON_GRID_HEIGHT; y++) {
            LetterButton* letterButton = new LetterButton(
                LETTER_BUTTON_X + x * LETTER_BUTTON_WIDTH,
                LETTER_BUTTON_Y + y * LETTER_BUTTON_HEIGHT,
                RUSSIAN_LETTERS[x + y * LETTERS_BUTTON_GRID_WIDTH],
                game);
            letterButtonGrid.placeButton(x, y, letterButton);
        }
    }

    buttonGrids.push_back(menuButtonGrid);
    buttonGrids.push_back(fieldButtonGrid);
    buttonGrids.push_back(wordButtonGrid);
    buttonGrids.push_back(letterButtonGrid);

    PlayerStats* player1Stats = new PlayerStats(PLAYER1_STATS_X, PLAYER1_STATS_Y, PLAYER1_TEXT, game->getPlayer(0));
    PlayerStats* player2Stats = new PlayerStats(PLAYER2_STATS_X, PLAYER2_STATS_Y, PLAYER2_TEXT, game->getPlayer(1));

    playerStats.push_back(player1Stats);
    playerStats.push_back(player2Stats);

    for (int i = 0; i < CONSOLE_HEIGHT; i++) {
        cout << endl;
    }
}

void UserInterface::update()
{
    draw();

    for (ButtonGrid& buttonGrid : buttonGrids) {
        Coord size = buttonGrid.getSize();
        for (int x = 0; x < size.x; x++) {
            for (int y = 0; y < size.y; y++) {
                buttonGrid.getButton(x, y)->setSelected(false);
            }
        }
    }

    int playerIndex = game->getCurrentPlayerIndex();
    for (int i = 0; i < playerStats.size(); i++) {
        playerStats[i]->setSelected(playerIndex == i);
    }

    ButtonGrid& fieldButtonGrid = buttonGrids[FIELD_BUTTON_GRID_INDEX];
    for (auto& cell : game->getSelectedWord()) {
        Button* button = fieldButtonGrid.getButton(cell.x, cell.y);
        button->setSelected(true);
    }

    auto cell = game->getPlacedLetter();
    for (int x = 0; x < LETTERS_BUTTON_GRID_WIDTH; x++) {
        for (int y = 0; y < LETTERS_BUTTON_GRID_HEIGHT; y++) {
            ButtonGrid& buttonGrid = buttonGrids[LETTERS_BUTTON_GRID_INDEX];
            if (cell.letter == RUSSIAN_LETTERS[x + y * LETTERS_BUTTON_GRID_WIDTH]) {
                buttonGrid.getButton(x, y)->setSelected(true);
                break;
            }
        }
    }

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            ButtonGrid& buttonGrid = buttonGrids[FIELD_BUTTON_GRID_INDEX];
            FieldButton* button = (FieldButton*)buttonGrid.getButton(x, y);
            button->setLetter(game->getLetter(x, y));
        }
    }

    ButtonGrid& wordButtonGrid = buttonGrids[WORD_BUTTON_GRID_INDEX];
    WordButton* wordButton = (WordButton*)wordButtonGrid.getButton(0, 0);
    wordButton->update();

    DWORD eventCount;
    INPUT_RECORD inputRecordBuffer[128];
    ReadConsoleInput(handleInput, inputRecordBuffer, 128, &eventCount);

    for (DWORD i = 0; i < eventCount; i++) {
        if (inputRecordBuffer[i].EventType == KEY_EVENT) {
            keyboardEvent(inputRecordBuffer[i].Event.KeyEvent);
        } else if (inputRecordBuffer[i].EventType == MOUSE_EVENT) {
            mouseEvent(inputRecordBuffer[i].Event.MouseEvent);
        }
    }

    if (tabIndex != -1) {
        ButtonGrid& currentButtonGrid = buttonGrids[tabIndex];
        currentButtonGrid.getButton(buttonGridPos.x, buttonGridPos.y)->setSelected(true);
    }
}

void UserInterface::draw()
{
    for (ButtonGrid& buttonGrid : buttonGrids) {
        buttonGrid.draw(handleOutput);
    }
    for (PlayerStats* playerNStats : playerStats) {
        playerNStats->draw(handleOutput);
    }
}

void UserInterface::PlayerStats::onClick()
{
}

UserInterface::PlayerStats::PlayerStats(int x, int y, wstring text, BaldaGame::Player* playerPtr)
    : Button(x, y, PLAYER_STATS_WIDTH, PLAYER_STATS_HEIGHT)
    , player(playerPtr)
{
    this->text = text;
}

void UserInterface::PlayerStats::draw(HANDLE& handle) const
{
    COORD coord {};
    DWORD _;
    Coord playerCoord = Coord(pos.x + 2, pos.y + 1);
    Coord wordsCoord = Coord(pos.x + 2, pos.y + size.y - 3 - int(player->words.size()));
    Coord scoreCoord = Coord(pos.x + 2, pos.y + size.y - 2);
    wstring scoreText = to_wstring(player->score);
    wstring missedText = L"";
    for (int i = 0; i < player->missedMoves; i++) {
        missedText += L"X ";
    }
    Coord missedCoord = Coord(pos.x + size.x - 1 - int(missedText.size()), pos.y + size.y - 2);
    for (int x = pos.x; x < pos.x + size.x; x++) {
        coord.X = x;
        for (int y = pos.y; y < pos.y + size.y; y++) {
            coord.Y = y;
            if (x >= playerCoord.x && x < playerCoord.x + text.size() && y == playerCoord.y) {
                WriteConsoleOutputCharacter(handle, &text[size_t(x - playerCoord.x)], 1, coord, &_);
            } else if (x >= scoreCoord.x && x < scoreCoord.x + scoreText.size() && y == scoreCoord.y) {
                WriteConsoleOutputCharacter(handle, &scoreText[size_t(x - scoreCoord.x)], 1, coord, &_);
            } else if (y > pos.y + 1 && y >= wordsCoord.y && y < wordsCoord.y + int(player->words.size()) && x >= wordsCoord.x && x < wordsCoord.x + player->words[size_t(y - wordsCoord.y)].size()) {
                wstring& word = player->words[size_t(y - wordsCoord.y)];
                WriteConsoleOutputCharacter(handle, &word[size_t(x - scoreCoord.x)], 1, coord, &_);
            } else if (x >= missedCoord.x && x < missedCoord.x + missedText.size() && y == missedCoord.y) {
                WriteConsoleOutputCharacter(handle, &missedText[size_t(x - missedCoord.x)], 1, coord, &_);
            } else {
                wchar_t border = BUTTON_BORDERS[selected + 2 * (1 * (x == pos.x && y == pos.y) + 2 * (x == pos.x + size.x - 1 && y == pos.y) + 3 * (x == pos.x + size.x - 1 && y == pos.y + size.y - 1) + 4 * (x == pos.x && y == pos.y + size.y - 1) + 5 * ((x == pos.x || x == pos.x + size.x - 1) && pos.y < y && y < pos.y + size.y - 1) + 6 * (pos.x < x && x < pos.x + size.x - 1 && (y == pos.y || y == pos.y + size.y - 1)))];
                WriteConsoleOutputCharacter(handle, &border, 1, coord, &_);
            }
        }
    }
}