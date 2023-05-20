#include "StatFunc.h"
Mix_Chunk* Sound = NULL;
void soundCell()
{
	Sound = Mix_LoadWAV("bomb.wav");
	Mix_PlayChannel(-1, Sound, 0);
}
void soundFlag()
{
	Sound = Mix_LoadWAV("flag.wav");
	Mix_PlayChannel(-1, Sound, 0);
}
SDL_Texture* get_text_texture(SDL_Renderer*& renderer, char* text, TTF_Font* font)
{
	SDL_Surface* textSurface = NULL;
	SDL_Color fore_color = { 0,0,0 };
	textSurface = TTF_RenderUTF8_Blended(font, text, fore_color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	return texture;
}
void init_clear_cell(level** easy, int rows, int cols)
{
	for(int i = 0; i < rows; i ++)
		for (int j = 0; j < cols; j++)
		{
			easy[i][j].bomb = false;
			easy[i][j].check_number = false;
			easy[i][j].number = 0;
			easy[i][j].up_surface = false;
			easy[i][j].check_open = false;
			easy[i][j].check_flag = false;
		}
}
void init_bomb(level **easy, int rows, int cols, int cnt_bomb)
{
	int k = rand() % rows, k1 = rand() % cols;
	while(cnt_bomb != 0)
	{
		
		if (easy[k][k1].bomb == false)
		{
			easy[k][k1].bomb = true;
			cnt_bomb--;
		}
		else
		{
			k = rand() % rows;
			k1 = rand() % cols;
		}
	}
}


void init_number(level **easy, int rows, int cols)
{
	for(int i = 0; i< rows; i++)
		for (int j = 0; j < cols; j++)
		{
			if (easy[i][j].bomb == true)
			{
				if (j + 1 < cols && easy[i][j + 1].bomb == false) {
					easy[i][j + 1].check_number = true;
					easy[i][j + 1].number++;
				}
				if (j + 1 < cols && i - 1 >=0 && easy[i - 1][j + 1].bomb == false){
					easy[i - 1][j + 1].check_number = true;
					easy[i - 1][j + 1].number++;
				}
				if (j + 1 < cols && i + 1 < rows && easy[i + 1][j + 1].bomb == false){
					easy[i + 1][j + 1].check_number = true;
					easy[i + 1][j + 1].number++;
				}
				if (i + 1 < rows && easy[i + 1][j].bomb == false) {
					easy[i + 1][j].check_number = true;
					easy[i + 1][j].number++;
				}
				if (i - 1 >= 0 && easy[i - 1][j].bomb == false) {
					easy[i - 1][j].check_number = true;
					easy[i - 1][j].number++;
				}
				if (i + 1 < rows && j - 1 >= 0 && easy[i + 1][j - 1].bomb == false) {
					easy[i + 1][j - 1].check_number = true;
					easy[i + 1][j - 1].number++;
				}
				if (j - 1 >= 0 && easy[i][j - 1].bomb == false) {
					easy[i][j - 1].check_number = true;
					easy[i][j - 1].number++;
				}
				if (i - 1 >= 0 && j - 1 >= 0 && easy[i - 1][j - 1].bomb == false) {
					easy[i - 1][j - 1].check_number = true;
					easy[i - 1][j - 1].number++;
				}
			}
		}
}
void init_rect_cell(level** easy, int rows, int cols)
{
	int tmp1 = GAME_WIDGHT / cols, tmp2 = GAME_HEIGHT / rows;
	int tmp;
	if (tmp1 > tmp2)
		tmp = tmp2;
	else
		tmp = tmp1;
	tmp1 = (WIDGHT_WINDOW - tmp * cols) / 2;
	tmp2 = (HEIGHT_WINDOW - tmp * rows) / 2;
	for(int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			easy[i][j].cell = {tmp1 + tmp * j, tmp2 + tmp * i, tmp, tmp};
		}
}
void draw_game_cell(level ** easy, TTF_Font* zifra, SDL_Renderer* renderer, SDL_Texture* down_cell,SDL_Texture *bomb,SDL_Texture *up_cell,SDL_Texture *zifraT,SDL_Texture *flag,SDL_Rect tmp_rect, int rows, int cols)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_RenderClear(renderer);
	char text[10];
	for(int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			
			if (easy[i][j].bomb == false)
				SDL_RenderCopy(renderer, down_cell, NULL, &easy[i][j].cell);
			else
				SDL_RenderCopy(renderer, bomb, NULL, &easy[i][j].cell);
			if (easy[i][j].check_number == true)
			{
				tmp_rect = easy[i][j].cell;
				tmp_rect = { tmp_rect.x + tmp_rect.w / 4, tmp_rect.y, tmp_rect.w / 2, tmp_rect.h };
				_itoa_s(easy[i][j].number, text, 10);
				SDL_DestroyTexture(zifraT);
				zifraT = get_text_texture(renderer, text, zifra);
				SDL_RenderCopy(renderer, zifraT, NULL, &tmp_rect);
			}
			if (easy[i][j].check_open == false)
				SDL_RenderCopy(renderer, up_cell, NULL, &easy[i][j].cell);
			if(easy[i][j].check_flag == true)
				SDL_RenderCopy(renderer, flag, NULL, &easy[i][j].cell);
		}
}

bool check_win(level ** Lev, int rows, int cols, int cnt_bomb, int cnt_bomb_tmp, bool &play)
{
	int tmp = 0;
	int tmp_cnt = 0;
	for(int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			if (Lev[i][j].bomb == true && Lev[i][j].check_flag == true)
				tmp++;
			if (Lev[i][j].bomb == false && Lev[i][j].check_open == true)
				tmp_cnt++;
		}
	if (tmp == cnt_bomb && cnt_bomb_tmp == 0 && tmp_cnt == rows * cols - cnt_bomb)
		return false;
	else
		return true;
}

void addNewRecord(string& NameUser, int time, int &temp_cnt)
{
	ifstream ListName("ListNameRecord.txt");
	ifstream ListRecord("ListRecord.txt");
	string tmp_string_name[100];
	string tmp_string_record[100];
	int cnt = 0;
	int check = 0;
	char tmpScanName[50];
	char tmpScanRecord[50];

	while (ListName.getline(tmpScanName, sizeof(tmpScanName)))
	{
		tmp_string_name[cnt] = tmpScanName;
		ListRecord.getline(tmpScanRecord, sizeof(tmpScanRecord));
		tmp_string_record[cnt] = tmpScanRecord;
		if (tmp_string_name[cnt] == NameUser)
		{
			if (stoi(tmp_string_record[cnt]) > time)
				tmp_string_record[cnt] = to_string(time);
			check = 1;
		}
		cnt++;
	}
	ListName.close();
	ListRecord.close();
	ofstream updatedListRecord("ListRecord.txt");
	for (int i = 0; i < cnt; i++)
	{
		updatedListRecord << tmp_string_record[i] << endl;
	}
	updatedListRecord.close();

	if (check == 0)
	{
		ofstream ListName("ListNameRecord.txt", ios::app);
		ListName << NameUser << endl;
		ListName.close();

		ofstream ListRecord("ListRecord.txt", ios::app);
		ListRecord << time << endl;
		ListRecord.close();
	}
	
}




void open_clean(int rows, int cols, int i, int j, level** Lev)
{
	if ((i >= 0) && (i < rows)) {
		if ((j >= 0) && (j < cols)){
			if (Lev[i][j].check_open == false && Lev[i][j].check_flag == false) {
				Lev[i][j].check_open = true;
				if (Lev[i][j].check_number == false && Lev[i][j].bomb == false && Lev[i][j].check_flag == false) {
					open_clean(rows, cols, i - 1, j - 1, Lev);
					open_clean(rows, cols, i - 1, j, Lev);
					open_clean(rows, cols, i - 1, j + 1, Lev);
					open_clean(rows, cols, i, j - 1, Lev);
					open_clean(rows, cols, i, j + 1, Lev);
					open_clean(rows, cols, i + 1, j - 1, Lev);
					open_clean(rows, cols, i + 1, j, Lev);
					open_clean(rows, cols, i + 1, j + 1, Lev);
				}
				else
				{
					if(i - 1 >= 0 && j - 1 >= 0)
						if (Lev[i - 1][j - 1].bomb == false && Lev[i - 1][j - 1].check_number == false && Lev[i - 1][j - 1].check_flag == false)
							open_clean(rows, cols, i - 1, j - 1, Lev);
					if(i - 1 >= 0)
						if (Lev[i - 1][j].bomb == false && Lev[i - 1][j].check_number == false && Lev[i - 1][j].check_flag == false)
							open_clean(rows, cols, i - 1, j, Lev);
					if(i - 1 >= 0 && j + 1 < cols)
						if (Lev[i - 1][j + 1].bomb == false && Lev[i - 1][j + 1].check_number == false && Lev[i - 1][j + 1].check_flag == false)
							open_clean(rows, cols, i - 1, j + 1, Lev);
					if(j - 1 >= 0)
						if (Lev[i][j - 1].bomb == false && Lev[i][j - 1].check_number == false && Lev[i][j - 1].check_flag == false)
							open_clean(rows, cols, i, j - 1, Lev);
					if(j + 1 < cols)
						if (Lev[i][j + 1].bomb == false && Lev[i][j + 1].check_number == false && Lev[i][j + 1].check_flag == false)
							open_clean(rows, cols, i, j + 1, Lev);
					if(i + 1 < rows && j - 1 >= 0)
						if (Lev[i + 1][j - 1].bomb == false && Lev[i + 1][j - 1].check_number == false && Lev[i + 1][j - 1].check_flag == false)
							open_clean(rows, cols, i + 1, j - 1, Lev);
					if(i + 1 < rows)
						if (Lev[i + 1][j].bomb == false && Lev[i + 1][j].check_number == false && Lev[i + 1][j].check_flag == false)
							open_clean(rows, cols, i + 1, j, Lev);
					if(i + 1 < rows && j + 1 < cols)
						if (Lev[i + 1][j + 1].bomb == false && Lev[i + 1][j + 1].check_number == false && Lev[i + 1][j + 1].check_flag == false)
							open_clean(rows, cols, i + 1, j + 1, Lev);
				}
			}
		}
	}
}
void check_tap_cell(level** Lev,SDL_Event event, int rows, int cols, bool &play, bool change, bool &checktimer, int &firstTime, int &cnt_bomb_tmp)
{
	/*Mix_Init(0);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);*/
	for(int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{		
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				if (event.button.x > Lev[i][j].cell.x && event.button.x < Lev[i][j].cell.x + Lev[i][j].cell.w)
					if (event.button.y > Lev[i][j].cell.y && event.button.y < Lev[i][j].cell.y + Lev[i][j].cell.h)
					{
						
						if (checktimer == false)
						{
							checktimer = true;
							firstTime = time(NULL);
						}
						if (change == true)
						{
							soundCell();
							if (Lev[i][j].bomb == false && Lev[i][j].check_number == false)
							{
								int x = i, y = j;
								open_clean(rows, cols, x, y, Lev);
								
							}
							Lev[i][j].check_open = true;
							if (Lev[i][j].bomb == true)
							{
								checktimer = false;
								play = false;
								for (int k = 0; k < rows; k++)
									for (int f = 0; f < cols; f++)
										if (Lev[k][f].bomb == true)
											Lev[k][f].check_open = true;
							}
						}
						else
						{
							soundFlag();
							if (Lev[i][j].check_open == false)
							{
								if (Lev[i][j].check_flag == true)
								{
									Lev[i][j].check_flag = false;
									cnt_bomb_tmp++;
								}
								else
								{
									Lev[i][j].check_flag = true;
									cnt_bomb_tmp--;
								}
								
							}
						}
					}
		}
}
void settings_Game(SDL_Renderer*& renderer, TTF_Font* my_font, SDL_Event event, int &check_set, int& cnt_bomb, int& rows, int& cols, int& custom_bomb, int& custom_rows, int& custom_cols, int& cnt_bomb_tmp)
{
	char playText[] = { u8"Новичёк" }, settText[] = { u8"Любитель" }, ratText[] = { u8"Профессионал" }, quiText[] = { u8"Назад" }, name[] = { u8"Настройки" }, Textcustom[] = {u8"Пользовательский"};
	SDL_Texture* TextPlay, * TextSett, * TextRat, * TextQuit, * Name, *knopka_2, *Custom;
	bool quit_out = false;
	SDL_Rect play = { 585,280, 750, 142 };
	SDL_Rect settings = { 585,442, 750, 142 };
	SDL_Rect rating = { 585,604, 750, 142 };
	SDL_Rect custom = {585, 766, 750, 142};
	SDL_Rect quit = { 585,928, 750, 142 };
	SDL_Texture* knopka;
	knopka_2 = IMG_LoadTexture(renderer, "knop2.bmp");
	knopka = IMG_LoadTexture(renderer, "knop.bmp");
	TextPlay = get_text_texture(renderer, playText, my_font);
	TextSett = get_text_texture(renderer, settText, my_font);
	TextRat = get_text_texture(renderer, ratText, my_font);
	TextQuit = get_text_texture(renderer, quiText, my_font);
	Name = get_text_texture(renderer, name, my_font);
	Custom = get_text_texture(renderer, Textcustom, my_font);
	SDL_Rect TextOut[6];
	TextOut[0] = { 760,291,375, 120 };
	TextOut[1] = { 760,453,350, 120 };
	TextOut[2] = { 760,615,375, 120 };
	TextOut[3] = { 660,777,600, 120 };
	TextOut[4] = { 760,939,375, 120 };
	TextOut[5] = { 700, 50, 500, 140 };
	while (!quit_out)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit_out = true;
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
				if (event.button.x > play.x && event.button.x < play.x + play.w)
					if (event.button.y > play.y && event.button.y < play.y + play.h)
						check_set = 0;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
				if (event.button.x > settings.x && event.button.x < settings.x + settings.w)
					if (event.button.y > settings.y && event.button.y < settings.y + settings.h)
						check_set = 1;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
				if (event.button.x > rating.x && event.button.x < rating.x + rating.w)
					if (event.button.y > rating.y && event.button.y < rating.y + rating.h)
						check_set = 2;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > custom.x && event.button.x < custom.x + custom.w)
					if (event.button.y > custom.y && event.button.y < custom.y + custom.h)
					{
						check_set = 3;
						custom_settings(renderer, my_font, event, check_set, cnt_bomb, rows, cols, custom_bomb, custom_rows, custom_cols, cnt_bomb_tmp);
						checkSet(check_set, rows, cols, cnt_bomb, cnt_bomb_tmp);
					}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
				if (event.button.x > quit.x && event.button.x < quit.x + quit.w)
					if (event.button.y > quit.y && event.button.y < quit.y + quit.h)
						quit_out = true;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);
		if(check_set == 0)
			SDL_RenderCopy(renderer, knopka_2, NULL, &play);
		else
		SDL_RenderCopy(renderer, knopka, NULL, &play);
		if (check_set == 1)
			SDL_RenderCopy(renderer, knopka_2, NULL, &settings);
		else
		SDL_RenderCopy(renderer, knopka, NULL, &settings);
		if (check_set == 2)
			SDL_RenderCopy(renderer, knopka_2, NULL, &rating);
		else
		SDL_RenderCopy(renderer, knopka, NULL, &rating);
		if (check_set == 3)
			SDL_RenderCopy(renderer, knopka_2, NULL, &custom);
		else
			SDL_RenderCopy(renderer, knopka, NULL, &custom);

		SDL_RenderCopy(renderer, knopka, NULL, &quit);
		SDL_RenderCopy(renderer, TextPlay, NULL, &TextOut[0]);
		SDL_RenderCopy(renderer, TextSett, NULL, &TextOut[1]);
		SDL_RenderCopy(renderer, TextRat, NULL, &TextOut[2]);
		SDL_RenderCopy(renderer, TextQuit, NULL, &TextOut[4]);
		SDL_RenderCopy(renderer, Custom, NULL, &TextOut[3]);
		SDL_RenderCopy(renderer, Name, NULL, &TextOut[5]);
		SDL_RenderPresent(renderer);
	}
	TextPlay = NULL;
	TextQuit = NULL;
	TextSett = NULL;
	TextRat = NULL;
	knopka = NULL;
	knopka_2 = NULL;
	Custom = NULL;
}
void settings_Game_process(SDL_Renderer*& renderer, TTF_Font* my_font, SDL_Event event, int& check_set, bool& quitMenu, int &cnt_bomb, int&rows, int&cols, int&custom_bomb, int&custom_rows, int&custom_cols, int &cnt_bomb_tmp)
{
	char playText[] = { u8"Новичёк" }, settText[] = { u8"Любитель" }, ratText[] = { u8"Профессионал" }, quiText[] = { u8"Назад" }, name[] = { u8"Настройки" }, Textcustom[] = { u8"Пользовательский" }, TextQuitMain[]={u8"Выход"};
	SDL_Texture* TextPlay, * TextSett, * TextRat, * TextQuit, * Name, * knopka_2, * Custom, *quitMainMenu;
	bool quit_out = false;
	SDL_Rect play = { 585,280, 750, 120 };
	SDL_Rect settings = { 585,410, 750, 120 };
	SDL_Rect rating = { 585,540, 750, 120 };
	SDL_Rect custom = { 585, 670, 750, 120 };
	SDL_Rect back = { 585,800, 750, 120 };
	SDL_Rect quitmain = { 585,930,750, 120 };
	SDL_Texture* knopka;
	knopka_2 = IMG_LoadTexture(renderer,"knop2.bmp");
	knopka = IMG_LoadTexture(renderer, "knop.bmp");
	TextPlay = get_text_texture(renderer, playText, my_font);
	TextSett = get_text_texture(renderer, settText, my_font);
	TextRat = get_text_texture(renderer, ratText, my_font);
	TextQuit = get_text_texture(renderer, quiText, my_font);
	Name = get_text_texture(renderer, name, my_font);
	Custom = get_text_texture(renderer, Textcustom, my_font);
	quitMainMenu = get_text_texture(renderer, TextQuitMain, my_font);
	SDL_Rect TextOut[7];
	TextOut[0] = { 760,290,375, 100 };
	TextOut[1] = { 660,420,600, 100 };
	TextOut[2] = { 710,550,500, 100 };
	TextOut[3] = { 660,680,600, 100 };
	TextOut[4] = { 760,810,375, 100 };
	TextOut[6] = { 760,940,375, 100 };
	TextOut[5] = { 700, 50, 500, 140 };

	while (!quit_out)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit_out = true;
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > play.x && event.button.x < play.x + play.w)
					if (event.button.y > play.y && event.button.y < play.y + play.h)
						check_set = 0;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > settings.x && event.button.x < settings.x + settings.w)
					if (event.button.y > settings.y && event.button.y < settings.y + settings.h)
						check_set = 1;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > rating.x && event.button.x < rating.x + rating.w)
					if (event.button.y > rating.y && event.button.y < rating.y + rating.h)
						check_set = 2;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > custom.x && event.button.x < custom.x + custom.w)
					if (event.button.y > custom.y && event.button.y < custom.y + custom.h)
					{
						check_set = 3;
						custom_settings(renderer, my_font, event, check_set, cnt_bomb, rows, cols, custom_bomb, custom_rows, custom_cols, cnt_bomb_tmp);
					}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > back.x && event.button.x < back.x + back.w)
					if (event.button.y > back.y && event.button.y < back.y + back.h)
						quit_out = true;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > quitmain.x && event.button.x < quitmain.x + quitmain.w)
					if (event.button.y > quitmain.y && event.button.y < quitmain.y + quitmain.h)
					{
						quit_out = true;
						quitMenu = true;
					}
			}
		}
		if (quit_out == false)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			SDL_RenderClear(renderer);
			if (check_set == 0)
				SDL_RenderCopy(renderer, knopka_2, NULL, &play);
			else
				SDL_RenderCopy(renderer, knopka, NULL, &play);
			if (check_set == 1)
				SDL_RenderCopy(renderer, knopka_2, NULL, &settings);
			else
				SDL_RenderCopy(renderer, knopka, NULL, &settings);
			if (check_set == 2)
				SDL_RenderCopy(renderer, knopka_2, NULL, &rating);
			else
				SDL_RenderCopy(renderer, knopka, NULL, &rating);
			if (check_set == 3)
				SDL_RenderCopy(renderer, knopka_2, NULL, &custom);
			else
				SDL_RenderCopy(renderer, knopka, NULL, &custom);

			SDL_RenderCopy(renderer, knopka, NULL, &back);
			SDL_RenderCopy(renderer, knopka, NULL, &quitmain);
			SDL_RenderCopy(renderer, TextPlay, NULL, &TextOut[0]);
			SDL_RenderCopy(renderer, TextSett, NULL, &TextOut[1]);
			SDL_RenderCopy(renderer, TextRat, NULL, &TextOut[2]);
			SDL_RenderCopy(renderer, TextQuit, NULL, &TextOut[4]);
			SDL_RenderCopy(renderer, Custom, NULL, &TextOut[3]);
			SDL_RenderCopy(renderer, quitMainMenu, NULL, &TextOut[6]);
			SDL_RenderCopy(renderer, Name, NULL, &TextOut[5]);
			SDL_RenderPresent(renderer);
		}
	}
	TextPlay = NULL;
	TextQuit = NULL;
	TextSett = NULL;
	TextRat = NULL;
	knopka = NULL;
	knopka_2 = NULL;
	Custom = NULL;
	quitMainMenu = NULL;
	Name = NULL;
}
void checkSet(int check_set, int& rows, int& cols, int& cnt_bomb, int&cnt_bomb_tmp)
{
	if (check_set == 0) {
		rows = 9;
		cols = 9;
		cnt_bomb = 10;
		cnt_bomb_tmp = cnt_bomb;
	}
	else if (check_set == 1) {
		rows = 16;
		cols = 16;
		cnt_bomb = 40;
		cnt_bomb_tmp = cnt_bomb;
	}
	else if (check_set == 2) {
		rows = 16;
		cols = 30;
		cnt_bomb = 99;
		cnt_bomb_tmp = cnt_bomb;
	}
}
bool check_tap(SDL_Event& event, SDL_Rect rect)
{
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
	{
		if (event.button.x > rect.x && event.button.x < rect.x + rect.w)
		{
			if (event.button.y > rect.y && event.button.y < rect.y + rect.h)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

void custom_settings(SDL_Renderer*& renderer, TTF_Font* my_font, SDL_Event& event, int check_set,  int& cnt_bomb, int &rows, int &cols, int&custom_bomb, int&custom_rows, int &custom_cols, int&cnt_bomb_tmp)
{
	bool quit = false;
	SDL_Texture* centrTexture = IMG_LoadTexture(renderer, "centr.bmp");
	SDL_Texture* leftButtonTexture = IMG_LoadTexture(renderer, "left.bmp");
	SDL_Texture* rightButtonTexture = IMG_LoadTexture(renderer, "right.bmp");
	char cntBombNumber[10], bombText[] = u8"Бомбы", cntRowsNumber[10], rowsText[] = u8"Строки", cntColsNumber[10], colsText[] = u8"Столбцы", customText[] = u8"Пользовательский режим", backText[] = u8"Назад";
	SDL_Rect Name = { 300, 30, 1320, 170 };
	SDL_Texture* NameTexture = get_text_texture(renderer, customText, my_font);

	SDL_Rect but_back = { 610,780, 700, 120 };
	SDL_Rect text_back = { 760,790, 400, 100 };
	SDL_Texture* knop = IMG_LoadTexture(renderer, "knop.bmp");
	SDL_Texture* TextureBack = get_text_texture(renderer, backText, my_font);

	SDL_Rect leftRectButtonBomb = { 240, 480, 140, 140 };
	SDL_Rect centrRectButtonBomb = {385, 480, 140, 140 };
	SDL_Rect rightRectButtonBomb = { 530, 480, 140, 140 };
	SDL_Rect centrTextNumberBomb = {405, 485, 100, 130};
	SDL_Rect TextBomb = {310, 340,290 ,130};
	_itoa_s(custom_bomb, cntBombNumber, 10);
	SDL_Texture* TextureTextBombNumber = get_text_texture(renderer, cntBombNumber, my_font);
	SDL_Texture* TextureTextBomb = get_text_texture(renderer, bombText, my_font);

	SDL_Rect leftRectButtonRows = { 750, 480, 140, 140 };
	SDL_Rect centrRectButtonRows = { 895, 480, 140, 140 };
	SDL_Rect rightRectButtonRows = { 1040, 480, 140, 140 };
	SDL_Rect centrTextNumberRows = { 915, 485, 100, 130 };
	SDL_Rect TextRows = {820, 340,290 ,130};
	_itoa_s(custom_rows, cntRowsNumber, 10);
	SDL_Texture* TextureTextRowsNumber = get_text_texture(renderer, cntRowsNumber, my_font);
	SDL_Texture* TextureTextRows = get_text_texture(renderer, rowsText, my_font);

	SDL_Rect leftRectButtonCols = { 1250, 480, 140, 140 };
	SDL_Rect centrRectButtonCols = { 1395, 480, 140, 140 };
	SDL_Rect rightRectButtonCols = { 1540, 480, 140, 140 };
	SDL_Rect centrTextNumberCols = { 1415, 485, 100, 130 };
	SDL_Rect TextCols = { 1320, 340, 290, 130 };
	_itoa_s(custom_cols, cntColsNumber, 10);
	SDL_Texture* TextureTextColsNumber = get_text_texture(renderer, cntColsNumber, my_font);
	SDL_Texture *TextureTextCols = get_text_texture(renderer, colsText, my_font);

	while (!quit)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = true;
			if (check_tap(event, leftRectButtonBomb))
				if (custom_bomb > 1 && custom_bomb < custom_rows * custom_cols)
				{
					custom_bomb--;
					
					TextureTextBombNumber =  NULL;
					_itoa_s(custom_bomb, cntBombNumber, 10);
					TextureTextBombNumber = get_text_texture(renderer, cntBombNumber, my_font);
				}
			if (check_tap(event, leftRectButtonRows))
				if (custom_rows > 3 && custom_bomb < custom_rows * custom_cols)
				{
					custom_rows--;
					
					TextureTextRowsNumber = NULL;
					_itoa_s(custom_rows, cntRowsNumber, 10);
					TextureTextRowsNumber = get_text_texture(renderer, cntRowsNumber, my_font);
					
				}
			if (check_tap(event, leftRectButtonCols))
				if (custom_cols > 3 && custom_bomb < custom_rows * custom_cols)
				{
					custom_cols--;
					
					TextureTextColsNumber = NULL;
					_itoa_s(custom_cols, cntColsNumber, 10);
					TextureTextColsNumber = get_text_texture(renderer, cntColsNumber, my_font);
				}
			if (check_tap(event, rightRectButtonBomb))
				if (custom_bomb < 200 && custom_bomb < custom_rows * custom_cols)
				{
					custom_bomb++;
					
					TextureTextBombNumber = NULL;
					_itoa_s(custom_bomb, cntBombNumber, 10);
					TextureTextBombNumber = get_text_texture(renderer, cntBombNumber, my_font);
				}
			if (check_tap(event, rightRectButtonRows))
				if (custom_rows < 30)
				{
					custom_rows++;
					
					TextureTextRowsNumber = NULL;
					_itoa_s(custom_rows, cntRowsNumber, 10);
					TextureTextRowsNumber = get_text_texture(renderer, cntRowsNumber, my_font);
				}
			if (check_tap(event, rightRectButtonCols))
				if (custom_cols < 30)
				{
					custom_cols++;
					
					TextureTextColsNumber = NULL;
					_itoa_s(custom_cols, cntColsNumber, 10);
					TextureTextColsNumber = get_text_texture(renderer, cntColsNumber, my_font);
				}
			if (check_tap(event, but_back))
				quit = true;
		}
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, NameTexture, NULL, &Name);

		SDL_RenderCopy(renderer, knop, NULL, &but_back);
		SDL_RenderCopy(renderer, TextureBack, NULL, &text_back);

		SDL_RenderCopy(renderer, TextureTextBomb, NULL, &TextBomb);
		SDL_RenderCopy(renderer, leftButtonTexture, NULL, &leftRectButtonBomb);
		SDL_RenderCopy(renderer, centrTexture, NULL, &centrRectButtonBomb);
		SDL_RenderCopy(renderer, rightButtonTexture, NULL, &rightRectButtonBomb);
		SDL_RenderCopy(renderer, TextureTextBombNumber, NULL, &centrTextNumberBomb);

		SDL_RenderCopy(renderer, TextureTextRows, NULL, &TextRows);
		SDL_RenderCopy(renderer, leftButtonTexture, NULL, &leftRectButtonRows);
		SDL_RenderCopy(renderer, centrTexture, NULL, &centrRectButtonRows);
		SDL_RenderCopy(renderer, rightButtonTexture, NULL, &rightRectButtonRows);
		SDL_RenderCopy(renderer, TextureTextRowsNumber, NULL, &centrTextNumberRows);
		
		SDL_RenderCopy(renderer, TextureTextCols, NULL, &TextCols);
		SDL_RenderCopy(renderer, leftButtonTexture, NULL, &leftRectButtonCols);
		SDL_RenderCopy(renderer, centrTexture, NULL, &centrRectButtonCols);
		SDL_RenderCopy(renderer, rightButtonTexture, NULL, &rightRectButtonCols);
		SDL_RenderCopy(renderer, TextureTextColsNumber, NULL, &centrTextNumberCols);
		SDL_RenderPresent(renderer);

	}
	
	NameTexture = NULL;
	knop = NULL;
	TextureBack = NULL;
	TextureTextBomb = NULL;
	leftButtonTexture = NULL;
	centrTexture = NULL;
	rightButtonTexture = NULL;
	TextureTextBombNumber = NULL;
	TextureTextRows = NULL;
	TextureTextRowsNumber = NULL;
	TextureTextCols = NULL;
	TextureTextColsNumber = NULL;
	cnt_bomb = custom_bomb;
	rows = custom_rows;
	cols = custom_cols;
	cnt_bomb_tmp = custom_bomb;
}
void game_process(SDL_Window *window, SDL_Renderer*& renderer, TTF_Font* my_font, TTF_Font* zifra, SDL_Event& event, int& check_set, int &rows, int &cols, int &cnt_bomb, int& custom_rows ,int & custom_cols, int &custom_bomb, int&cnt_bomb_tmp,int& tempForRaiting, string&NameUser)
{
	bool change = true, play = true, checktimer = false;
	int time_s = 0, time_first, time_process;
	int w_but = GAME_WIDGHT / 16;
	int butttonChange = GAME_WIDGHT / 9;
	char text[10], time_text[10], flag_text[10];
	int prom = check_set;
	int promBobm = cnt_bomb, promRows = rows, promCols = cols;
	_itoa_s(0, text, 10);
	bool quit = false;

	checkSet(check_set, rows, cols, cnt_bomb, cnt_bomb_tmp);
	level** Lev = (level**)malloc(sizeof(level*) * rows);
	for (int i = 0; i < rows; i++)
		Lev[i] = (level*)malloc(sizeof(level) * cols);
	init_clear_cell(Lev, rows, cols);
	init_bomb(Lev, rows, cols, cnt_bomb);
	init_number(Lev, rows, cols);
	init_rect_cell(Lev, rows, cols);

	SDL_Texture* up_cell = IMG_LoadTexture(renderer, "Cell.bmp");
	SDL_Texture* down_surface = IMG_LoadTexture(renderer, "CellDown.bmp");
	SDL_Texture* bomb = IMG_LoadTexture(renderer, "Bomb.bmp");
	SDL_Texture* zifraT = get_text_texture(renderer, text, zifra);
	SDL_Texture* flag = IMG_LoadTexture(renderer, "FlagBomb.bmp");

	SDL_Rect Restart = { 5, 5, w_but,w_but };
	SDL_Rect Sett = { 10 + Restart.w , 5, w_but,w_but };
	SDL_Rect Rait = { 15 + Sett.w * 2, 5, w_but, w_but };
	SDL_Texture* RestartTexture = IMG_LoadTexture(renderer, "StrelkaFin.bmp");
	SDL_Texture* SettTexture = IMG_LoadTexture(renderer, "Settings.bmp");
	SDL_Texture* RaitTexture = IMG_LoadTexture(renderer, "Crown1.bmp");

	_itoa_s(cnt_bomb_tmp, flag_text, 10);
	SDL_Rect FlagCnt = {5, w_but + 10, 200, 100};
	SDL_Rect TextFlagCnt = { 55, w_but + 10+ 12, 100, 75 };
	SDL_Texture* textFlagTexture = get_text_texture(renderer, flag_text, my_font);


	SDL_Rect ChangeItem = { GAME_WIDGHT + (WIDGHT_WINDOW - GAME_WIDGHT) - butttonChange * 1.2, GAME_HEIGHT / 2 - butttonChange / 4, butttonChange, butttonChange };
	SDL_Texture* ChangeButtonTexture1 = IMG_LoadTexture(renderer, "Change1.bmp");
	SDL_Texture* ChangeButtonTexture2 = IMG_LoadTexture(renderer, "Change2.bmp");

	_itoa_s(time_s, time_text, 10);
	SDL_Texture * timeCellTexture = IMG_LoadTexture(renderer, "timer.bmp");
	SDL_Rect timeCell = { GAME_WIDGHT + (WIDGHT_WINDOW - GAME_WIDGHT) - 300 - butttonChange * 0.2, 30, 300, 100 };
	SDL_Rect timeTextRect = { GAME_WIDGHT + (WIDGHT_WINDOW - GAME_WIDGHT) - 225 - butttonChange*0.2, 42,150, 75 };
	SDL_Texture* textTimerTexture = get_text_texture(renderer, time_text, my_font);


	SDL_Rect tmp_rect = { 0, 0, 0, 0 };
	

	while (!quit)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = true;
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > Sett.x && event.button.x < Sett.x + Sett.w)
					if (event.button.y > Sett.y && event.button.y < Sett.y + Sett.h)
					{
						prom = check_set;
						promBobm=cnt_bomb, promRows=rows, promCols = cols;
						settings_Game_process(renderer, my_font, event, check_set, quit, cnt_bomb, rows, cols, custom_bomb, custom_rows, custom_cols, cnt_bomb_tmp);
						if (prom != check_set || (check_set == 3 && (promBobm != cnt_bomb || promRows != rows || promCols != cols)))
						{
							checkSet(check_set, rows, cols, cnt_bomb, cnt_bomb_tmp);
							Lev = (level**)malloc(sizeof(level*) * rows);
							for (int i = 0; i < rows; i++)
								Lev[i] = (level*)malloc(sizeof(level) * cols);
							init_clear_cell(Lev, rows, cols);
							init_bomb(Lev, rows, cols, cnt_bomb);
							init_number(Lev, rows, cols);
							init_rect_cell(Lev, rows, cols);
							play = true;
							checktimer = false;
							time_s = 0;
							cnt_bomb_tmp = cnt_bomb;
							SDL_DestroyTexture(textTimerTexture);
							_itoa_s(time_s, time_text, 10);
							textTimerTexture = get_text_texture(renderer, time_text, my_font);
							SDL_DestroyTexture(textFlagTexture);
							_itoa_s(cnt_bomb_tmp, flag_text, 10);
							textFlagTexture = get_text_texture(renderer, flag_text, my_font);
						}
					}	
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > Restart.x && event.button.x < Restart.x + Restart.w)
					if (event.button.y > Restart.y && event.button.y < Restart.y + Restart.h)
					{
						init_clear_cell(Lev, rows, cols);
						init_bomb(Lev, rows, cols, cnt_bomb);
						init_number(Lev, rows, cols);
						init_rect_cell(Lev, rows, cols);
						play = true;
						checktimer = false;
						time_s = 0;
						cnt_bomb_tmp = cnt_bomb;
						SDL_DestroyTexture(textTimerTexture);
						_itoa_s(time_s, time_text, 10);
						textTimerTexture = get_text_texture(renderer, time_text, my_font);
						SDL_DestroyTexture(textFlagTexture);
						_itoa_s(cnt_bomb_tmp, flag_text, 10);
						textFlagTexture = get_text_texture(renderer, flag_text, my_font);
						
					}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > Rait.x && event.button.x < Rait.x + Rait.w)
					if (event.button.y > Rait.y && event.button.y < Rait.y + Rait.h)
						RaitingMenu(renderer, my_font, event, tempForRaiting);
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (event.button.x > ChangeItem.x && event.button.x < ChangeItem.x + ChangeItem.w)
					if (event.button.y > ChangeItem.y && event.button.y < ChangeItem.y + ChangeItem.h)
					{
						if (play == true)
						{
							if (change == true)
								change = false;
							else
								change = true;
						}
					}
			}
			if (play == true)
			{

				if (!check_win(Lev, rows, cols, cnt_bomb, cnt_bomb_tmp, play) && check_set == 3)
				{
					addNewRecord(NameUser, time_s, tempForRaiting);
					
				}
				if (!check_win(Lev, rows, cols, cnt_bomb, cnt_bomb_tmp, play))
				{
					win_window(renderer, Lev, my_font, time_s, rows, cols, cnt_bomb, cnt_bomb_tmp, time_s, play, checktimer);
					if (play == true) {
						SDL_DestroyTexture(textTimerTexture);
							_itoa_s(time_s, time_text, 10);
							textTimerTexture = get_text_texture(renderer, time_text, my_font);
							SDL_DestroyTexture(textFlagTexture);
							_itoa_s(cnt_bomb_tmp, flag_text, 10);
							textFlagTexture = get_text_texture(renderer, flag_text, my_font);
					}
				}
				play = check_win(Lev, rows, cols, cnt_bomb, cnt_bomb_tmp, play);
				check_tap_cell(Lev, event, rows, cols, play, change, checktimer, time_first, cnt_bomb_tmp);
				SDL_DestroyTexture(textFlagTexture);
				_itoa_s(cnt_bomb_tmp, flag_text, 10);
				textFlagTexture = get_text_texture(renderer, flag_text, my_font);
			}
			
		}
		
		if (play == false)
			checktimer = false;
		if (checktimer == true)
		{
			time_process = time(NULL);
			if (time_process > time_first)
			{
				time_s++;
				time_first = time(NULL);
				SDL_DestroyTexture(textTimerTexture);
				_itoa_s(time_s, time_text, 10);
				textTimerTexture = get_text_texture(renderer, time_text, my_font);
			}
		}
		if (quit == false)
		{
			draw_game_cell(Lev, zifra, renderer, down_surface, bomb, up_cell, zifraT, flag, tmp_rect, rows, cols);
			SDL_RenderCopy(renderer, RestartTexture, NULL, &Restart);
			SDL_RenderCopy(renderer, SettTexture, NULL, &Sett);
			SDL_RenderCopy(renderer, RaitTexture, NULL, &Rait);
			if (change == true)
				SDL_RenderCopy(renderer, ChangeButtonTexture1, NULL, &ChangeItem);
			else
				SDL_RenderCopy(renderer, ChangeButtonTexture2, NULL, &ChangeItem);
			SDL_RenderCopy(renderer, timeCellTexture, NULL, &timeCell);
			SDL_RenderCopy(renderer, textTimerTexture, NULL, &timeTextRect);
			SDL_RenderCopy(renderer, timeCellTexture, NULL, &FlagCnt);
			SDL_RenderCopy(renderer, textFlagTexture, NULL, &TextFlagCnt);
			SDL_RenderPresent(renderer);
		}
	}
	SDL_DestroyTexture(up_cell);
	SDL_DestroyTexture(down_surface);
	SDL_DestroyTexture(bomb);
	SDL_DestroyTexture(zifraT);
	SDL_DestroyTexture(RestartTexture);
	SDL_DestroyTexture(SettTexture);
	SDL_DestroyTexture(RaitTexture);
	SDL_DestroyTexture(ChangeButtonTexture1);
	SDL_DestroyTexture(ChangeButtonTexture2);
	SDL_DestroyTexture(timeCellTexture);
	SDL_DestroyTexture(textTimerTexture);
	SDL_DestroyTexture(timeCellTexture);
	SDL_DestroyTexture(textFlagTexture);
	SDL_DestroyTexture(flag);
	for (int i = 0; i < rows; i++)
		free(Lev[i]);
	free(Lev);
}

void RaitingMenu(SDL_Renderer*& renderer, TTF_Font* my_font, SDL_Event& event, int& tempForRaiting)
{
	char NameUpText[] = { u8"Имя" }, RecordUpText[] = { u8"Рекорд" };
	bool quit = false;
	SDL_Rect BackButton = { 10, 10, 100, 100 };
	SDL_Texture* TextureButtonBack = IMG_LoadTexture(renderer, "BackRaiting.bmp");
	FILE* ListName = fopen("ListNameRecord.txt", "r+");
	char linesName[100][100];
	char linesRecord[100][5];
	int lineCount = 0;
	char buffer[100];
	while (fgets(buffer, sizeof(buffer), ListName) != NULL) {
		if (buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = '\0';
		}
		strcpy(linesName[lineCount], buffer);
		lineCount++;
	}
	fclose(ListName);
	lineCount = 0;
	FILE* ListRecord = fopen("ListRecord.txt", "r+");
	while (fgets(buffer, sizeof(buffer), ListName) != NULL) {
		if (buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = '\0';
		}
		strcpy(linesRecord[lineCount], buffer);
		lineCount++;
	}
	fclose(ListRecord);
	SDL_Rect NameUpRect = { 450, 50, 200, 100 };
	SDL_Rect RecordUpRect = { 1110, 50, 500, 100 };
	SDL_Rect* NameRect = (SDL_Rect*)malloc(sizeof(SDL_Rect) * lineCount);
	SDL_Rect* RecordRect = (SDL_Rect*)malloc(sizeof(SDL_Rect) * lineCount);
	for (int i = 0; i < lineCount; i++)
	{
		NameRect[i] = { 305, 200 + 100 * i, 500, 80 };
		RecordRect[i] = { 1325, 200 + 100 * i, 70, 80 };
	}
	
	SDL_Texture* TextureUpName = get_text_texture(renderer, NameUpText, my_font);
	SDL_Texture* TextureUpRecord = get_text_texture(renderer, RecordUpText, my_font);
	SDL_Texture* TextureName = get_text_texture(renderer, linesName[0], my_font);
	SDL_Texture* TextureRecord = get_text_texture(renderer, linesRecord[0], my_font);
	while (quit == false)
	{
		while (SDL_PollEvent(&event))
		{
			if(check_tap(event, BackButton))
				quit = true;
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);
		for (int i = 0; i < lineCount; i++) {
			SDL_DestroyTexture(TextureName);
			TextureName = get_text_texture(renderer, linesName[i], my_font);
			SDL_RenderCopy(renderer, TextureName, NULL, &NameRect[i]);
			SDL_DestroyTexture(TextureRecord);
			TextureRecord = get_text_texture(renderer, linesRecord[i], my_font);
			SDL_RenderCopy(renderer, TextureRecord, NULL, &RecordRect[i]);
		}
		SDL_RenderCopy(renderer, TextureButtonBack, NULL, &BackButton);
		SDL_RenderCopy(renderer, TextureUpName, NULL, &NameUpRect);
		SDL_RenderCopy(renderer, TextureUpRecord, NULL, &RecordUpRect);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(TextureButtonBack);
	SDL_DestroyTexture(TextureUpRecord);
	SDL_DestroyTexture(TextureUpName);
	SDL_DestroyTexture(TextureRecord);
	SDL_DestroyTexture(TextureName);
}
void win_window(SDL_Renderer *&render ,level ** Lev,TTF_Font* my_font, int& time, int& rows, int& cols, int& cnt_bomb, int&cnt_bomb_tmp,int&time_s, bool &play, bool &checktimer)
{
	char WinMessage[] = { u8"Поздравляем, вы победили!" }, ResultMessage[] = { u8"Ваш результат:" }, Result[10];
	
	SDL_Rect WindowRect = { 510, 300, 900, 480 };
	SDL_Rect CloseRect = {810,640 ,100, 100};
	SDL_Rect RestartRect = { 1010, 640, 100, 100 };
	SDL_Rect WinMessageRect = {560, 340, 800, 80};
	SDL_Rect ResultMessageRect = { 660, 440, 500, 80 };
	SDL_Rect ResultRect = { 1200, 440, 70, 80 };
	SDL_Event even;
	SDL_Texture* WindowRectTexture = IMG_LoadTexture(render, "winWindow.bmp");
	SDL_Texture* CloseRectTexture = IMG_LoadTexture(render, "close.bmp");
	SDL_Texture* RestartRectTexture = IMG_LoadTexture(render, "StrelkaFin.bmp");
	_itoa_s(time_s, Result, 10);
	SDL_Texture* ResultTexture = get_text_texture(render, Result, my_font);
	SDL_Texture* WinMessageTexture = get_text_texture(render, WinMessage, my_font);
	SDL_Texture* ResultMessageTexture = get_text_texture(render, ResultMessage, my_font);
	bool quit = false;
	while (quit == false)
	{
		while (SDL_PollEvent(&even))
		{
			if (check_tap(even, CloseRect))
				quit = true;
		}
		if (check_tap(even, RestartRect))
		{
			char time_text[10];
			init_clear_cell(Lev, rows, cols);
			init_bomb(Lev, rows, cols, cnt_bomb);
			init_number(Lev, rows, cols);
			init_rect_cell(Lev, rows, cols);
			play = true;
			checktimer = false;
			time_s = 0;
			cnt_bomb_tmp = cnt_bomb;
			
			
			quit = true;
		}
		
		SDL_RenderCopy(render, WindowRectTexture, NULL, &WindowRect);
		SDL_RenderCopy(render, CloseRectTexture, NULL, &CloseRect);
		SDL_RenderCopy(render, RestartRectTexture, NULL, &RestartRect);
		SDL_RenderCopy(render, ResultMessageTexture, NULL, &ResultMessageRect);
		SDL_RenderCopy(render, WinMessageTexture, NULL, &WinMessageRect);
		SDL_RenderCopy(render, ResultTexture, NULL, &ResultRect);
		SDL_RenderPresent(render);
	}
	SDL_DestroyTexture(WindowRectTexture);
	SDL_DestroyTexture(CloseRectTexture);
	SDL_DestroyTexture(RestartRectTexture);
	SDL_DestroyTexture(ResultMessageTexture);
	SDL_DestroyTexture(WinMessageTexture);
	SDL_DestroyTexture(ResultTexture);
}