#include "interface.hpp"

int main()
{
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    BaldaGame game(L"ПАЛКА");
    UserInterface UI = UserInterface(&game, hStdIn, hStdOut);

    while (1) {
        UI.update();
    }
}