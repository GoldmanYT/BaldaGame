#pragma once

const int PLAYER_COUNT = 2;
const int BOARD_SIZE = 5;
const int MAX_PLAYER_MISSED_MOVES = 3;

const int CONSOLE_WIDTH = 66;
const int CONSOLE_HEIGHT = 35;

const int MENU_BUTTON_GRID_INDEX = 0;
const int MENU_BUTTON_GRID_WIDTH = 3;
const int MENU_BUTTON_GRID_HEIGHT = 1;

const int START_GAME_BUTTON_X = 1;
const int START_GAME_BUTTON_Y = 0;
const int START_GAME_BUTTON_WIDTH = 19;
const int START_GAME_BUTTON_HEIGHT = 3;

const int MISS_MOVE_BUTTON_X = 21;
const int MISS_MOVE_BUTTON_Y = 0;
const int MISS_MOVE_BUTTON_WIDTH = 19;
const int MISS_MOVE_BUTTON_HEIGHT = 3;

const int REMOVE_LETTER_BUTTON_X = 41;
const int REMOVE_LETTER_BUTTON_Y = 0;
const int REMOVE_LETTER_BUTTON_WIDTH = 19;
const int REMOVE_LETTER_BUTTON_HEIGHT = 3;

const int FIELD_BUTTON_GRID_INDEX = 1;
const int FIELD_BUTTON_X = 18;
const int FIELD_BUTTON_Y = 5;
const int FIELD_BUTTON_WIDTH = 5;
const int FIELD_BUTTON_HEIGHT = 3;

const int WORD_BUTTON_GRID_INDEX = 2;
const int WORD_BUTTON_X = 13;
const int WORD_BUTTON_Y = 22;
const int WORD_BUTTON_WIDTH = 35;
const int WORD_BUTTON_HEIGHT = 3;

const int LETTERS_BUTTON_GRID_INDEX = 3;
const int LETTERS_BUTTON_GRID_WIDTH = 11;
const int LETTERS_BUTTON_GRID_HEIGHT = 3;

const int LETTER_BUTTON_X = 3;
const int LETTER_BUTTON_Y = 25;
const int LETTER_BUTTON_WIDTH = 5;
const int LETTER_BUTTON_HEIGHT = 3;

const int PLAYER1_STATS_X = 1;
const int PLAYER1_STATS_Y = 5;

const int PLAYER2_STATS_X = 43;
const int PLAYER2_STATS_Y = 5;

const int PLAYER_STATS_WIDTH = 17;
const int PLAYER_STATS_HEIGHT = 15;

const wchar_t BUTTON_BORDERS[] = L"  ┌╔┐╗┘╝└╚│║─═";

const wchar_t START_GAME_TEXT[] = L"Начать игру";
const wchar_t MISS_MOVE_TEXT[] = L"Пропустить ход";
const wchar_t REMOVE_LETTER_TEXT[] = L"Удалить букву";
const wchar_t PLAYER1_TEXT[] = L"Игрок 1";
const wchar_t PLAYER2_TEXT[] = L"Игрок 2";
const wchar_t PLAYER1_WON_TEXT[] = L"Игрок 1 победил!";
const wchar_t PLAYER2_WON_TEXT[] = L"Игрок 2 победил!";
const wchar_t TIE_TEXT[] = L"Ничья!";
const wchar_t RUSSIAN_LETTERS[] = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
const wchar_t BLANK_LETTER = L' ';