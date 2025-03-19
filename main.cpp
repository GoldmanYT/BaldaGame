//#include <iostream>
//#include <Windows.h>
//
//using namespace std;
//
//const int CONSOLE_WIDTH = 120;
//const int CONSOLE_HEIGHT = 30;
//
//const int BOARD_SIZE = 5;
//const int DEFAULT_BUTTON_WIDTH = 5;
//const int DEFAULT_BUTTON_HEIGHT = 3;
//
//const TCHAR LEFT_ARROW = L'←';
//const TCHAR UP_ARROW = L'↑';
//const TCHAR RIGHT_ARROW = L'→';
//const TCHAR DOWN_ARROW = L'↓';
//
//const TCHAR TOP_LEFT_CORNER = L'┌';
//const TCHAR TOP_RIGHT_CORNER = L'┐';
//const TCHAR BOTTOM_LEFT_CORNER = L'└';
//const TCHAR BOTTOM_RIGHT_CORNER = L'┘';
//const TCHAR VERTICAL_BORDER = L'│';
//const TCHAR HORIZONTAL_BORDER = L'─';
//
//const TCHAR TOP_LEFT_CORNER_SELECT = L'╔';
//const TCHAR TOP_RIGHT_CORNER_SELECT = L'╗';
//const TCHAR BOTTOM_LEFT_CORNER_SELECT = L'╚';
//const TCHAR BOTTOM_RIGHT_CORNER_SELECT = L'╝';
//const TCHAR VERTICAL_BORDER_SELECT = L'║';
//const TCHAR HORIZONTAL_BORDER_SELECT = L'═';
//
//TCHAR START_GAME_TEXT[] = L"Начать игру";
//TCHAR CANCEL_MOVE_TEXT[] = L"Отменить ход";
//TCHAR MISS_MOVE_TEXT[] = L"Пропустить ход";
//
//DWORD dw;
//
//class Button
//{
//	int x;
//	int y;
//	int height;
//	int len;
//	TCHAR* text;
//	bool selected;
//public:
//	int width;
//	Button()
//	{
//		x = 0;
//		y = 0;
//		width = DEFAULT_BUTTON_WIDTH;
//		height = DEFAULT_BUTTON_HEIGHT;
//		len = 0;
//		text = nullptr;
//		selected = 0;
//	}
//
//	Button(
//		int button_x,
//		int button_y,
//		int button_width, 
//		int button_height,
//		int text_len,
//		TCHAR* button_text)
//	{
//		x = button_x;
//		y = button_y;
//		width = button_width;
//		height = button_height;
//		len = text_len;
//		text = button_text;
//		selected = 0;
//	}
//
//	void draw(HANDLE& hStdOut) const
//	{
//		COORD coord = { 0, 0 };
//		TCHAR ch = {};
//		for (int x = 0; x < this->width; x++)
//		{
//			coord.X = this->x + x;
//			for (int y = 0; y < this->height; y++)
//			{
//				coord.Y = this->y + y;
//				if (x == 0 && y == 0) {
//					ch = (this->selected) ? TOP_LEFT_CORNER_SELECT : TOP_LEFT_CORNER;
//				}
//				else if (x == 0 && y == this->height - 1) {
//					ch = (this->selected) ? BOTTOM_LEFT_CORNER_SELECT : BOTTOM_LEFT_CORNER;
//				}
//				else if (x == this->width - 1 && y == this->height - 1) {
//					ch = (this->selected) ? BOTTOM_RIGHT_CORNER_SELECT : BOTTOM_RIGHT_CORNER;
//				}
//				else if (x == this->width - 1 && y == 0) {
//					ch = (this->selected) ? TOP_RIGHT_CORNER_SELECT : TOP_RIGHT_CORNER;
//				}
//				else if (x == 0 || x == this->width - 1) {
//					ch = (this->selected) ? VERTICAL_BORDER_SELECT : VERTICAL_BORDER;
//				}
//				else if (y == 0 || y == this->height - 1) {
//					ch = (this->selected) ? HORIZONTAL_BORDER_SELECT : HORIZONTAL_BORDER;
//				}
//				else if (y == this->height / 2 &&
//					x >= this->width / 2 - len / 2 &&
//					x < this->width / 2 - len / 2 + len) {
//					ch = text[x - this->width / 2 + len / 2];
//				}
//				else { ch = ' '; }
//				WriteConsoleOutputCharacter(hStdOut, &ch, 1, coord, &dw);
//			}
//		}
//	}
//
//	void setSelected(bool state)
//	{
//		selected = state;
//	}
//
//	void setX(int x)
//	{
//		this->x = x;
//	}
//
//	void setY(int y)
//	{
//		this->y = y;
//	}
//};

//int main()
//{
//	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);	
//	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//
//	SetConsoleMode(hStdIn, ENABLE_MOUSE_INPUT);
//
//	Button StartGameButton(0, 0, 15, 3, 11, START_GAME_TEXT);
//	Button CancelMoveButton(15, 0, 16, 3, 12, CANCEL_MOVE_TEXT);
//	Button MissMoveButton(31, 0, 18, 3, 14, MISS_MOVE_TEXT);
//
//	Button Board[BOARD_SIZE][BOARD_SIZE];
//	int x0 = 10, y0 = 10;
//
//	for (int x = 0; x < BOARD_SIZE; x++)
//	{
//		for (int y = 0; y < BOARD_SIZE; y++)
//		{
//			Board[x][y].setX(x0 + x * DEFAULT_BUTTON_WIDTH);
//			Board[x][y].setY(y0 + y * DEFAULT_BUTTON_HEIGHT);
//			Board[x][y].draw(hStdOut);
//		}
//	}
//
//	StartGameButton.draw(hStdOut);
//	CancelMoveButton.draw(hStdOut);
//	MissMoveButton.draw(hStdOut);
//	cin.get();
//
//	StartGameButton.setSelected(true);
//	StartGameButton.draw(hStdOut);
//	cin.get();
//
//	return 0;
//}