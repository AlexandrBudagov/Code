#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ncurses.h>

///////////////////////////////////////////////// MAP
int width = 29, height = 19;

char **Map, **map;

///////////////////////////////////////////////// STRUCT
struct coin
{
	int x, y;

	bool b;
};

///////////////////////////////////////////////// GLOBAL VARIABLES
bool isWork;
bool isVictory;
bool isDead;

int x, y;
int dx, dy;

int coinsCount;

const int STEP = 1;
const int MAX_COINS = 50;

struct coin coins[50];

const int NUMBER_EN = 3;
const int TIME_LIMIT = 5000;

int time;

int posEn[2][3];
int dirEn[2][3];

///////////////////////////////////////////////// FUNCTIONS PROTOTYPES
void initialization();
void dispose();
void updatePlayer();
void updateEnemies();
void checkBoxes();
void updateKey();

void initMap();

///////////////////////////////////////////////// MAIN FUNCTION
int main()
{
	initscr();
	raw();
	noecho();
	nodelay(stdscr, true);

	initialization();

	///////////////////////////////////////////// MAIN LOOP
	while (isWork)
	{
		if (isVictory == false && isDead == false)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					map[i][j] = Map[i][j];

			for (int i = 0; i < MAX_COINS; i++)
				if (coins[i].b == 1)
					map[coins[i].y][coins[i].x] = '.';

			for (int i = 0; i < NUMBER_EN; i++)
				map[posEn[0][i]][posEn[1][i]] = '^';

			map[y][x] = 'O';

			dx = dy = 0;
		}

			updateKey();

		if (isVictory == false && isDead == false)
		{
			updatePlayer();

			updateEnemies();

			if (coinsCount == MAX_COINS)
				isVictory = true;
		}

		///////////////////////////////////// CLEAR
		clear();

		///////////////////////////////////// OUTPUT
		printw("1 - Restart	2 - Exit           Coins: %i\n", coinsCount);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				printw("%c", map[i][j]);

			printw("\n");
		}

		if (isVictory == true)
		{
			isVictory = false;

			system("./mapGen.out");

			initialization();
		}
		
		if (isDead == true)
			printw("%s", "\tGAME OVER\n");
	}
	/////////////////////////////////////////////
	dispose();

	endwin();
}
///////////////////////////////////////////////// FUNCTIONS
void initialization()
{
	isWork = true;
	isVictory = false;
	isDead = false;

	coinsCount = 0;

	/////////////////////////////////////////////
	initMap();

	for (int i = 0, n = 0, e = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (Map[i][j] == '.')
			{
				Map[i][j] = ' ';

				coins[n].x = j;
				coins[n].y = i;

				coins[n].b = true;

				n++;
			}
			else if (Map[i][j] == 'O')
			{
				Map[i][j] = ' ';

				x = j;
				y = i;
			}
			else if (Map[i][j] == '^')
			{
				Map[i][j] = ' ';

				posEn[0][e] = i; posEn[1][e] = j;

				dirEn[0][e] = -STEP; dirEn[1][e] = 0;

				e++;
			}
		}

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			map[i][j] = Map[i][j];

	map[y][x] = 'O';
}

void dispose()
{
	for (int i = 0; i < height; i++)
	{
		free(Map[i]);
		free(map[i]);
	}

	free(Map);
	free(map);
}

void updatePlayer()
{
	if (map[y + dy][x + dx] == '^')
		isDead = true;

	if (map[y + dy][x + dx] == '.')
		coinsCount++;

	if (map[y + dy][x + dx] == ' ' || map[y + dy][x + dx] == '.')
	{
		x += dx;
		y += dy;
	}

	for (int i = 0; i < MAX_COINS; i++)
		if (coins[i].x == x && coins[i].y == y)
			coins[i].b = false;
}

void updateEnemies()
{
	time++;

	if (time >= TIME_LIMIT)
	{
		for (int i = 0; i < NUMBER_EN; i++)
		{
			while (map[posEn[0][i] + dirEn[0][i]][posEn[1][i] + dirEn[1][i]] == '#')
			{
				int d = rand() % 4;

				if (d == 0)
				{
					dirEn[0][i] = STEP;
					dirEn[1][i] = 0;
				}
				else if (d == 1)
				{
					dirEn[0][i] = -STEP;
					dirEn[1][i] = 0;
				}
				else if (d == 2)
				{
					dirEn[0][i] = 0;
					dirEn[1][i] = STEP;
				}
				else if (d == 3)
				{
					dirEn[0][i] = 0;
					dirEn[1][i] = -STEP;
				}
			}

			if (map[posEn[0][i] + dirEn[0][i]][posEn[1][i] + dirEn[1][i]] == 'O')
				isDead = true;

			posEn[0][i] += dirEn[0][i];
			posEn[1][i] += dirEn[1][i];
		}

		time = 0;
	}
}

void updateKey()
{
	char key = getch();

	if (key == 'w')
	{
		dx = 0;
		dy = -STEP;
	}
	else if (key == 's')
	{
		dx = 0;
		dy = STEP;
	}
	else if (key == 'a')
	{
		dx = -STEP;
		dy = 0;
	}
	else if (key == 'd')
	{
		dx = STEP;
		dy = 0;
	}
	else if (key == '1')
	{
		dispose();

		initialization();
	}
	else if (key == '2')
		isWork = false;
}

void initMap()
{
	FILE *file = fopen("Map.map", "r");

	char c;

	fscanf(file, "%i", &width);
	fscanf(file, "%i", &height);

	fscanf(file, "%c", &c);

	printw("%i %i", width, height);

	Map = (char**)malloc(height * sizeof(char*));
	map = (char**)malloc(height * sizeof(char*));

	for (int i = 0; i < height; i++)
	{
		Map[i] = (char*)malloc(width * sizeof(char));
		map[i] = (char*)malloc(width * sizeof(char));
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			fscanf(file, "%c", &Map[i][j]);

		char c;

		fscanf(file, "%c", &c);
	}

	fclose(file);
}
/////////////////////////////////////////////////
