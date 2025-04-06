#include "interface.hpp"
#include "game.hpp"
#include <Windows.h>
#include <iostream>
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
{
    pos = Coord();
    size = Coord();
    selected = 0;
}

UserInterface::Button::Button(int x, int y, int width, int height)
{
    pos = Coord(x, y);
    size = Coord(width, height);
    selected = 0;
}

UserInterface::Button::Button(int x, int y, int width, int height, wstring text)
{
    pos = Coord(x, y);
    size = Coord(width, height);
    this->text = text;
    selected = 0;
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
            wchar_t border = BUTTON_BORDERS[selected + 2 * (1 * (x == pos.x && y == pos.y) + 2 * (x == pos.x + size.x - 1 && y == pos.y) + 3 * (x == pos.x + size.x - 1 && y == pos.y + size.y - 1) + 4 * (x == pos.x && y == pos.y + size.y - 1) + 5 * ((x == pos.x || x == pos.x + size.x - 1) && pos.y < y && y < pos.y + size.y - 1) + 6 * (pos.x < x && x < pos.x + size.x - 1 && (y == pos.y || y == pos.y + size.y - 1)))];
            WriteConsoleOutputCharacter(handle, &border, 1, coord, &_);
            if (x >= textCoord.x && x < textCoord.x + text.size() && y == textCoord.y) {
                WriteConsoleOutputCharacter(handle, &text[size_t(x - textCoord.x)], 1, coord, &_);
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
            Button& button = grid[x][y];
            button.draw(handle);
        }
    }
}

void UserInterface::ButtonGrid::placeButton(int x, int y, Button& button)
{
    grid[x][y] = button;
}

UserInterface::Button* UserInterface::ButtonGrid::getButton(int x, int y)
{
    return &grid[x][y];
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
            // Обработчик нажатия кнопки
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
        case VK_SPACE:
        case VK_RETURN:
            // Обработчик нажатия кнопки
            break;
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

    Button startGameButton = Button(
        START_GAME_BUTTON_X,
        START_GAME_BUTTON_Y,
        START_GAME_BUTTON_WIDTH,
        START_GAME_BUTTON_HEIGHT,
        START_GAME_TEXT);
    Button missMoveButton = Button(
        MISS_MOVE_BUTTON_X,
        MISS_MOVE_BUTTON_Y,
        MISS_MOVE_BUTTON_WIDTH,
        MISS_MOVE_BUTTON_HEIGHT,
        MISS_MOVE_TEXT);
    Button deleteLetterButton = Button(
        DELETE_LETTER_BUTTON_X,
        DELETE_LETTER_BUTTON_Y,
        DELETE_LETTER_BUTTON_WIDTH,
        DELETE_LETTER_BUTTON_HEIGHT,
        DELETE_LETTER_TEXT);

    ButtonGrid menuButtonGrid = ButtonGrid(3, 1);
    menuButtonGrid.placeButton(0, 0, startGameButton);
    menuButtonGrid.placeButton(1, 0, missMoveButton);
    menuButtonGrid.placeButton(2, 0, deleteLetterButton);

    ButtonGrid fieldButtonGrid = ButtonGrid(BOARD_SIZE, BOARD_SIZE);
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            Button fieldButton = Button(
                FIELD_BUTTON_X + x * FIELD_BUTTON_WIDTH,
                FIELD_BUTTON_Y + y * FIELD_BUTTON_HEIGHT,
                FIELD_BUTTON_WIDTH,
                FIELD_BUTTON_HEIGHT,
                wstring(1, game->getLetter(x, y)));
            fieldButtonGrid.placeButton(x, y, fieldButton);
        }
    }

    ButtonGrid letterButtonGrid = ButtonGrid(LETTERS_BUTTON_GRID_WIDTH, LETTERS_BUTTON_GRID_HEIGHT);
    for (int x = 0; x < LETTERS_BUTTON_GRID_WIDTH; x++) {
        for (int y = 0; y < LETTERS_BUTTON_GRID_HEIGHT; y++) {
            Button letterButton = Button(
                LETTERS_BUTTON_X + x * LETTERS_BUTTON_WIDTH,
                LETTERS_BUTTON_Y + y * LETTERS_BUTTON_HEIGHT,
                LETTERS_BUTTON_WIDTH,
                LETTERS_BUTTON_HEIGHT,
                wstring(1, RUSSIAN_LETTERS[x + y * LETTERS_BUTTON_GRID_WIDTH]));
            letterButtonGrid.placeButton(x, y, letterButton);
        }
    }

    buttonGrids.push_back(menuButtonGrid);
    buttonGrids.push_back(fieldButtonGrid);
    buttonGrids.push_back(letterButtonGrid);

    for (int i = 0; i < CONSOLE_HEIGHT; i++) {
        cout << endl;
    }
}

void UserInterface::update()
{
    draw();

    ButtonGrid& previousButtonGrid = buttonGrids[tabIndex];
    previousButtonGrid.getButton(buttonGridPos.x, buttonGridPos.y)->setSelected(false);

    DWORD eventCount;
    INPUT_RECORD inputRecordBuffer[128];
    ReadConsoleInput(handleInput, inputRecordBuffer, 128, &eventCount);

    for (DWORD i = 0; i < eventCount; i++) {
        switch (inputRecordBuffer[i].EventType) {
        case KEY_EVENT:
            keyboardEvent(inputRecordBuffer[i].Event.KeyEvent);
            break;
        case MOUSE_EVENT:
            mouseEvent(inputRecordBuffer[i].Event.MouseEvent);
            break;
        default:
            break;
        }
    }

    if (tabIndex != -1) {
        ButtonGrid& currentButtonGrid = buttonGrids[tabIndex];
        currentButtonGrid.getButton(buttonGridPos.x, buttonGridPos.y)->setSelected(true);
    }
}

void UserInterface::draw()
{
    for (ButtonGrid buttonGrid : buttonGrids) {
        buttonGrid.draw(handleOutput);
    }
}