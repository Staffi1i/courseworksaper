#include "StatFunc.h"


Mix_Music* fon = NULL;
void loadmusic()
{
	fon = Mix_LoadMUS("fon.wav");
	Mix_PlayMusic(fon, -1);
}
int main(int argc, char** argv)
{
	Mix_Init(0);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

	char text[10];
	int check_set = 0;
	int cnt_bomb = 10;
	int cnt_bomb_tmp = cnt_bomb;
	int rows = 9, cols = 9;
	int custom_bomb = 1, custom_rows = 3, custom_cols = 3;
	int tempForRaiting = 0;
	string NameUser;
	srand(time(NULL));
	setlocale(LC_ALL, "ru_RU.UTF-8");
	TTF_Init();

	TTF_Font* zifra = TTF_OpenFont("Samson.ttf", 100);
	TTF_Font* my_font = TTF_OpenFont("font2.ttf", 100);

	char playText[] = u8"Играть", settText[] = { u8"Настройки" }, ratText[] = { u8"Рейтинг" }, quiText[] = { u8"Выход" }, name[] = { u8"Сапёр" }, nameInputText[] = { u8"Введите имя" };
	SDL_Texture *TextPlay, *TextSett, *TextRat, *TextQuit, *Name;
	
	bool quit_out = false;
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Event event;
	
	SDL_Window* window = SDL_CreateWindow(u8"sapper", 0, 0, 1920, 1080, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Rect play = { 385,280, 750, 180 };
	SDL_Rect settings = { 385,480, 750, 180 };
	SDL_Rect rating = { 385,680, 750, 180 };
	SDL_Rect quit = { 385,880, 750, 180 };
	SDL_Texture* knopka;
	SDL_Surface* knopkaImage = IMG_Load("knop.bmp");
	knopka = SDL_CreateTextureFromSurface(renderer, knopkaImage);
	TextPlay = get_text_texture(renderer, playText, my_font);
	TextSett = get_text_texture(renderer, settText, my_font);
	TextRat = get_text_texture(renderer, ratText, my_font);
	TextQuit = get_text_texture(renderer, quiText, my_font);
	Name = get_text_texture(renderer, name, my_font);
	SDL_Rect TextOut[5];
	TextOut[0] = { 560,295,375, 150 };
	TextOut[1] = { 460,495,600, 150 };
	TextOut[2] = { 510,695,500, 150 };
	TextOut[3] = { 560,900,375, 140 };
	TextOut[4] = { 560, 50, 400, 150 };
	SDL_Texture* upIntputRed = IMG_LoadTexture(renderer, "InputTextRed.bmp");
	SDL_Texture* upIntputGreen = IMG_LoadTexture(renderer, "InputTextGreen.bmp");
	SDL_Rect InputName = {1150, 280, 755, 180};
	SDL_Rect InputNameUp = { 1300, 180, 455, 100 };
	SDL_Rect InputNameText = { 1250, 310,0 ,120 };
	char* tmpText = const_cast<char*>(NameUser.c_str());
	SDL_Texture* InputNameTextTexture = get_text_texture(renderer, tmpText, my_font);
	SDL_Texture* InputNameUpTextTexture = get_text_texture(renderer, nameInputText, my_font);
	loadmusic();
	while (!quit_out)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit_out = true;
			
			if(check_tap(event, play))
				if(NameUser.length() != 0)
					game_process(window, renderer, my_font, zifra, event, check_set, rows, cols, cnt_bomb, custom_rows, custom_cols, custom_bomb, cnt_bomb_tmp,tempForRaiting, NameUser);
					
			if(check_tap(event, settings))
				settings_Game(renderer, my_font, event, check_set, cnt_bomb, rows, cols, custom_bomb, custom_rows, custom_cols, cnt_bomb_tmp);	
			
			if (check_tap(event, rating))
				RaitingMenu(renderer, my_font, event, tempForRaiting);
			
			if(check_tap(event, quit))
				quit_out = true;
			
			if (check_tap(event, InputName))
			{
				SDL_StartTextInput();
				SDL_Event eva;
				bool qInput = false;
				while (qInput == false)
				{
					while (SDL_PollEvent(&eva)) {
						if (eva.type == SDL_KEYDOWN && eva.key.keysym.sym == SDLK_RETURN)
							qInput = true;
						if (eva.type == SDL_TEXTINPUT && InputNameText.x + InputNameText.w < InputNameUp.x+InputNameUp.w)
						{
							NameUser += eva.text.text;
							SDL_DestroyTexture(InputNameTextTexture);
							tmpText = const_cast<char*>(NameUser.c_str());
							InputNameTextTexture = get_text_texture(renderer, tmpText, my_font);
							InputNameText.w += 50;

						}
						if (eva.type == SDL_KEYDOWN && eva.key.keysym.sym == SDLK_BACKSPACE && NameUser.length() > 0)
						{
							NameUser.pop_back();
							SDL_DestroyTexture(InputNameTextTexture);
							tmpText = const_cast<char*>(NameUser.c_str());
							InputNameTextTexture = get_text_texture(renderer, tmpText, my_font);
							InputNameText.w -= 50;
						}
					}
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
					SDL_RenderClear(renderer);
					SDL_RenderCopy(renderer, knopka, NULL, &play);
					SDL_RenderCopy(renderer, knopka, NULL, &settings);
					SDL_RenderCopy(renderer, knopka, NULL, &rating);
					SDL_RenderCopy(renderer, knopka, NULL, &quit);
					SDL_RenderCopy(renderer, TextPlay, NULL, &TextOut[0]);
					SDL_RenderCopy(renderer, TextSett, NULL, &TextOut[1]);
					SDL_RenderCopy(renderer, TextRat, NULL, &TextOut[2]);
					SDL_RenderCopy(renderer, TextQuit, NULL, &TextOut[3]);
					SDL_RenderCopy(renderer, Name, NULL, &TextOut[4]);
					SDL_RenderCopy(renderer, upIntputRed, NULL, &InputName);
					SDL_RenderCopy(renderer, InputNameUpTextTexture, NULL, &InputNameUp);
					SDL_RenderCopy(renderer, InputNameTextTexture, NULL, &InputNameText);
					SDL_RenderPresent(renderer);
				}	
				SDL_StopTextInput();
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, knopka, NULL, &play);
		SDL_RenderCopy(renderer, knopka, NULL, &settings);
		SDL_RenderCopy(renderer, knopka, NULL, &rating);
		SDL_RenderCopy(renderer, knopka, NULL, &quit);
		SDL_RenderCopy(renderer, TextPlay, NULL, &TextOut[0]);
		SDL_RenderCopy(renderer, TextSett, NULL, &TextOut[1]);
		SDL_RenderCopy(renderer, TextRat, NULL, &TextOut[2]);
		SDL_RenderCopy(renderer, TextQuit, NULL, &TextOut[3]);
		SDL_RenderCopy(renderer, Name, NULL, &TextOut[4]);
		if(NameUser.length() == 0)
			SDL_RenderCopy(renderer, upIntputRed, NULL, &InputName);
		else
			SDL_RenderCopy(renderer, upIntputGreen, NULL, &InputName);
		SDL_RenderCopy(renderer, InputNameTextTexture, NULL, &InputNameText);
		SDL_RenderCopy(renderer, InputNameUpTextTexture, NULL, &InputNameUp);
		SDL_RenderPresent(renderer);
		
	}
	Mix_FreeMusic(fon);
	Mix_CloseAudio();
	SDL_DestroyTexture(TextPlay);
	SDL_DestroyTexture(TextQuit);
	SDL_DestroyTexture(TextSett);
	SDL_DestroyTexture(TextRat);
	SDL_DestroyTexture(knopka);

	return 0;
}