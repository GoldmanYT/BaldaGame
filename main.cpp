#include <iostream>
#include <Windows.h>

using namespace std;

const int CONSOLE_WIDTH = 120;
const int CONSOLE_HEIGHT = 30;

const int BOARD_SIZE = 5;
const int DEFAULT_BUTTON_WIDTH = 5;
const int DEFAULT_BUTTON_HEIGHT = 3;

const TCHAR LEFT_ARROW = L'←';
const TCHAR UP_ARROW = L'↑';
const TCHAR RIGHT_ARROW = L'→';
const TCHAR DOWN_ARROW = L'↓';

const TCHAR TOP_LEFT_CORNER = L'┌';
const TCHAR TOP_RIGHT_CORNER = L'┐';
const TCHAR BOTTOM_LEFT_CORNER = L'└';
const TCHAR BOTTOM_RIGHT_CORNER = L'┘';
const TCHAR VERTICAL_BORDER = L'│';
const TCHAR HORIZONTAL_BORDER = L'─';

const TCHAR TOP_LEFT_CORNER_SELECT = L'╔';
const TCHAR TOP_RIGHT_CORNER_SELECT = L'╗';
const TCHAR BOTTOM_LEFT_CORNER_SELECT = L'╚';
const TCHAR BOTTOM_RIGHT_CORNER_SELECT = L'╝';
const TCHAR VERTICAL_BORDER_SELECT = L'║';
const TCHAR HORIZONTAL_BORDER_SELECT = L'═';

TCHAR START_GAME_TEXT[] = L"Начать игру";
TCHAR CANCEL_MOVE_TEXT[] = L"Отменить ход";
TCHAR MISS_MOVE_TEXT[] = L"Пропустить ход";
TCHAR RUSSIAN_LETTERS[] = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

DWORD dw;

void MouseEventProc(MOUSE_EVENT_RECORD mer);
void draw(HANDLE out);

class Button
{
public:
	int x;
	int y;
	int width;
	int height;
	int len;
	TCHAR* text;
	bool selected;
	Button()
	{
		x = 0;
		y = 0;
		width = DEFAULT_BUTTON_WIDTH;
		height = DEFAULT_BUTTON_HEIGHT;
		len = 0;
		text = nullptr;
		selected = 0;
	}

	Button(
		int button_x,
		int button_y,
		int button_width, 
		int button_height,
		int text_len,
		TCHAR* button_text)
	{
		x = button_x;
		y = button_y;
		width = button_width;
		height = button_height;
		len = text_len;
		text = button_text;
		selected = 0;
	}

	void draw(HANDLE& hStdOut) const
	{
		COORD coord = { 0, 0 };
		TCHAR ch = {};
		for (int x = 0; x < this->width; x++)
		{
			coord.X = this->x + x;
			for (int y = 0; y < this->height; y++)
			{
				coord.Y = this->y + y;
				if (x == 0 && y == 0) {
					ch = (this->selected) ? TOP_LEFT_CORNER_SELECT : TOP_LEFT_CORNER;
				}
				else if (x == 0 && y == this->height - 1) {
					ch = (this->selected) ? BOTTOM_LEFT_CORNER_SELECT : BOTTOM_LEFT_CORNER;
				}
				else if (x == this->width - 1 && y == this->height - 1) {
					ch = (this->selected) ? BOTTOM_RIGHT_CORNER_SELECT : BOTTOM_RIGHT_CORNER;
				}
				else if (x == this->width - 1 && y == 0) {
					ch = (this->selected) ? TOP_RIGHT_CORNER_SELECT : TOP_RIGHT_CORNER;
				}
				else if (x == 0 || x == this->width - 1) {
					ch = (this->selected) ? VERTICAL_BORDER_SELECT : VERTICAL_BORDER;
				}
				else if (y == 0 || y == this->height - 1) {
					ch = (this->selected) ? HORIZONTAL_BORDER_SELECT : HORIZONTAL_BORDER;
				}
				else if (y == this->height / 2 &&
					x >= this->width / 2 - len / 2 &&
					x < this->width / 2 - len / 2 + len) {
					ch = text[x - this->width / 2 + len / 2];
				}
				else { ch = ' '; }
				WriteConsoleOutputCharacter(hStdOut, &ch, 1, coord, &dw);
			}
		}
	}

	void setSelected(bool state)
	{
		selected = state;
	}

	void setX(int x)
	{
		this->x = x;
	}

	void setY(int y)
	{
		this->y = y;
	}

	void setText(TCHAR* text, int len)
	{
		this->text = text;
		this->len = len;
	}

	bool isOnButton(SHORT x, SHORT y) const
	{
		return (this->x <= x) && (x < this->x + this->width) && (this->y <= y) && (y < this->y + this->height);
	}
};

Button StartGameButton(0, 0, 19, 3, 11, START_GAME_TEXT);
Button CancelMoveButton(20, 0, 19, 3, 12, CANCEL_MOVE_TEXT);
Button MissMoveButton(40, 0, 19, 3, 14, MISS_MOVE_TEXT);

Button Board[BOARD_SIZE][BOARD_SIZE];
Button Letters[33];

Button* AllButtons[] = { &StartGameButton, &CancelMoveButton, &MissMoveButton,
						 & Board[0][0],& Board[0][1],& Board[0][2],& Board[0][3],& Board[0][4], 
						 & Board[1][0],& Board[1][1],& Board[1][2],& Board[1][3],& Board[1][4], 
						 & Board[2][0],& Board[2][1],& Board[2][2],& Board[2][3],& Board[2][4], 
						 & Board[3][0],& Board[3][1],& Board[3][2],& Board[3][3],& Board[3][4], 
						 & Board[4][0],& Board[4][1],& Board[4][2],& Board[4][3],& Board[4][4],
						 & Letters[0],& Letters[1],& Letters[2],& Letters[3],& Letters[4],
						 & Letters[5],& Letters[6],& Letters[7],& Letters[8],& Letters[9],
						 & Letters[10],& Letters[11],& Letters[12],& Letters[13],& Letters[14],
						 & Letters[15],& Letters[16],& Letters[17],& Letters[18],& Letters[19],
						 & Letters[20],& Letters[21],& Letters[22],& Letters[23],& Letters[24],
						 & Letters[25],& Letters[26],& Letters[27],& Letters[28],& Letters[29],
						 & Letters[30],& Letters[31],& Letters[32]
};
int ButtonCount = 3 + BOARD_SIZE * BOARD_SIZE + 33;

int main()
{
	DWORD cNumRead;
	INPUT_RECORD irInBuf[128];

	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);	
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleMode(hStdIn, ENABLE_MOUSE_INPUT);
		
	int x0 = 17, y0 = 4;

	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			Board[x][y].setX(x0 + x * DEFAULT_BUTTON_WIDTH);
			Board[x][y].setY(y0 + y * DEFAULT_BUTTON_HEIGHT);
		}
	}

	for (int i = 0; i < 33; i++)
	{
		int x = 2 + (i % 11) * DEFAULT_BUTTON_WIDTH, y = 20 + (i / 11) * DEFAULT_BUTTON_HEIGHT;
		Letters[i].setX(x);
		Letters[i].setY(y);
		Letters[i].setText(RUSSIAN_LETTERS + i, 1);
	}

	while (1)
	{
		draw(hStdOut);

		ReadConsoleInput(hStdIn, irInBuf, 128, &cNumRead);

		for (DWORD i = 0; i < cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case MOUSE_EVENT:
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;
			}
		}
	}

	return 0;
}

void draw(HANDLE out)
{
	for (int i = 0; i < ButtonCount; i++)
	{
		Button& btn = *AllButtons[i];
		btn.draw(out);
	}
}

void MouseEventProc(MOUSE_EVENT_RECORD mer)
{
	SHORT x = mer.dwMousePosition.X, y = mer.dwMousePosition.Y;
	switch (mer.dwEventFlags)
	{
	case 0:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			for (int i = 0; i < ButtonCount; i++)
			{
				Button& btn = *AllButtons[i];
				if (btn.isOnButton(x, y))
				{
					btn.setSelected(not btn.selected);
				}
			}
		}
		break;

	default:
		break;
	}
}