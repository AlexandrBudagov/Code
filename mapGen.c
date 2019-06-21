#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

///////////////////////////////////////////////// GLOBAL VARIABLES
const int WIDTH = 29, HEIGHT = 19;

const char MAP[19][29] =
{
	"#############################",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#                           #",
	"#############################"
};

char Map[19][29];

char map[19][29];

const int MAX_COINS = 50;

///////////////////////////////////////////////// FUNCTION PROTOTYPES
void initMap();
void mapGen();
void objectGen();

_Bool mapTest();

///////////////////////////////////////////////// MAIN FUNCTION
int main()
{
	srand(time(NULL));

	_Bool isRight = 0;

	while (isRight == 0)
	{
		mapGen();

		isRight = mapTest();
	}

	objectGen();

	initMap();
}
///////////////////////////////////////////////// FUNCTIONS
void initMap()
{
	FILE *file = fopen("Map.map", "w");

	fprintf(file, "%i ", WIDTH);

	fprintf(file, "%i\n", HEIGHT);

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
			fprintf(file, "%c", Map[i][j]);

		fprintf(file, "\n");
	}

	fclose(file);
}

void mapGen()
{
	memcpy(Map, MAP, sizeof(MAP));

	for (int i = 0; i < 30; i++)
	{
		int x = rand() % (WIDTH - 2) + 1;
		int y = rand() % (HEIGHT - 2) + 1;
		int n = rand() % 5 + 3;
		
		int dx = rand() % 4 - 1;
		int dy = 0;

		if (dx == 0 || dx == 3)
		{
			dx = 0;

			dy = rand() % 2;

			if (dy == 0)
				dy--;
		}

		while (n > 0)
		{
			if (Map[y][x] == '#')
				break;

			Map[y][x] = '#';

			x += dx;
			y += dy;

			n--;
		}
	}
}

void r(int x, int y)
{
	map[y][x] = '1';

	if (map[y + 1][x] == ' ')
		r(x, y + 1);

	if (map[y - 1][x] == ' ')
		r(x, y - 1);

	if (map[y][x + 1] == ' ')
		r(x + 1, y);

	if (map[y][x - 1] == ' ')
		r(x - 1, y);
}

void objectGen()
{
	int i = 0, x = 0, y = 0;

	for (int i = 0; i < MAX_COINS; i++)
	{
		while (Map[y][x] != ' ')
		{
			x = rand() % (WIDTH - 2) + 1;
			y = rand() % (HEIGHT - 2) + 1;
		}

		Map[y][x] = '.';
	}

	while (Map[y][x] != ' ')
	{
		x = rand() % (WIDTH - 2) + 1;
		y = rand() % (HEIGHT - 2) + 1;
	}

	Map[y][x] = 'O';

	for (int i = 0; i < 3; i++)
	{
		while (Map[y][x] != ' ')
		{
			x = rand() % (WIDTH - 2) + 1;
			y = rand() % (HEIGHT - 2) + 1;
		}

		Map[y][x] = '^';
	}
}

_Bool mapTest()
{
	_Bool isRight = 1;

	memcpy(map, Map, sizeof(Map));

	int x = 1, y = 1;

	while (map[y][x] == '#')
	{
		x = rand() % (WIDTH - 2) + 1;
		y = rand() % (HEIGHT - 2) + 1;
	}

	r(x, y);

	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			if (map[i][j] == ' ')
				isRight = 0;

	return isRight;
}

/////////////////////////////////////////////////
