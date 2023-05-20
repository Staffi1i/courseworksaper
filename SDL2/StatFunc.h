#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define GAME_WIDGHT 1440
#define GAME_HEIGHT 980
#define WIDGHT_WINDOW 1920
#define HEIGHT_WINDOW 1080
#pragma comment(lib, "SDL2_mixer.lib")
#include "SDL.h"
#include<math.h>
#include "stdlib.h"
#include"SDL_ttf.h"
#include"SDL_image.h"
#include"SDL_mixer.h"
#include<time.h>
#include<iostream>
#include <string>
#include <fstream>
using namespace std;

struct level
{
	bool bomb = false;;
	bool up_surface = false;
	bool check_number = false;
	int number = 0;
	bool check_open = false;
	bool check_flag = true;
	SDL_Rect cell;
};

bool check_tap(SDL_Event& event, SDL_Rect rect);
void init_clear_cell(level** easy, int rows, int cols);
void init_bomb(level** easy, int rows, int cols, int cnt_bomb);
void init_number(level** easy, int rows, int cols);
void init_rect_cell(level** easy, int rows, int cols);
void check_tap_cell(level** Lev, SDL_Event event, int rows, int cols, bool& play, bool change, bool& checktimer, int& firstTime, int& cnt_bomb_tmp);
SDL_Texture* get_text_texture(SDL_Renderer*& renderer, char* text, TTF_Font* font);
void settings_Game(SDL_Renderer *& renderer, TTF_Font *my_font, SDL_Event event, int & check_set, int& cnt_bomb, int& rows, int& cols, int& custom_bomb, int& custom_rows, int& custom_cols, int& cnt_bomb_tmp);
void game_process(SDL_Window * window, SDL_Renderer*& renderer, TTF_Font* my_font, TTF_Font* zifra, SDL_Event& event, int& check_set, int& rows, int& cols, int& cnt_bomb, int& custom_rows, int& custom_cols, int& custom_bomb, int& custom_bomb_tmp,int&tempForRaiting, string &NameUser);
void settings_Game_process(SDL_Renderer*& renderer, TTF_Font* my_font, SDL_Event event, int& check_set, bool& quitMenu, int& cnt_bomb, int& rows, int& cols, int& custom_bomb, int& custom_rows, int& custom_cols, int&cnt_bomb_tmp);
bool check_win(level** Lev, int rows, int cols, int cnt_bomb, int cnt_bomb_tmp, bool &play);
void open_clean(int rows, int cols, int i, int j, level** Lev);
void custom_settings(SDL_Renderer*& renderer, TTF_Font* my_font, SDL_Event& event, int check_set, int& cnt_bomb, int& rows, int& cols, int& custom_bomb, int& custom_rows, int& custom_cols, int& cnt_bomb_tmp);
void addNewRecord(string &NameUser, int time, int&temp_cnt);
void RaitingMenu(SDL_Renderer*& renderer, TTF_Font* my_font, SDL_Event& event, int& tempForRaiting);
void win_window(SDL_Renderer*& renderer, level** Lev, TTF_Font* my_font, int& time, int& rows, int& cols, int& cnt_bomb, int& cnt_bomb_tmp, int& time_s, bool& play, bool& checktimer);
void checkSet(int check_set, int& rows, int& cols, int& cnt_bomb, int& cnt_bomb_tmp);
