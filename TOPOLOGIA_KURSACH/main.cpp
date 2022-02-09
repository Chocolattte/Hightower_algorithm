#include <iostream>
#include <iomanip>
#include "glut.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

/* 
Коддыыы
4 3 4 5 5 4 3 4 3 1 9 9 1 7 7 10 10 4 4 9 9
- Что нибудь сделать
Препреисточники и приемники: -333
Препятствие: -2 (254)
Граница: -1
Пустота: 0 (.)
Источник S: -5 (Когда найден путь: -6)
Приемник T: -10 (Когда найден путь: -11)
Линия источника (S): -3 (253)
Конечный путь: -7
*/

using namespace std;

int field[12][12] =
{ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 },
{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } };


double dz = -40;
double x = 0, y = 0, z = 0;

int *masCord;
int saveTreeH[10] = { 0 }, saveTreeV[10] = { 0 }; // В конце массивов хранится X и Y
int saveOldTreeH[10] = { 0 }, saveOldTreeV[10] = { 0 }; // Массивы-копии на шаг назад
int saveLengthH[10] = { 0 }, saveLengthV[10] = { 0 };
int goodIndexH = 0, goodIndexV = 0;
int countVetv = 1;
int Ways = -20;

// Функция для установки источника
void setS(int x, int y, char mode)
{
	field[x][y] = -5;
	if (mode == 'F') field[x][y] = -6;
}

// Функция для установки приемника
void setT(int x, int y, char mode)
{
	field[x][y] = -10;
	if (mode == 'F') field[x][y] = -11;
}

// Функция для ввода координат всех источников и приемников
int initCord()
{
	tryAgain:
	int countSource;
	int temp;
	int k = 0;
	bool flag = false;

	cout << "Enter the number of sources: " << endl;
	cin >> countSource;
	masCord = new int[countSource * 4];

	for (int i = 1; i <= countSource; i++)
	{
		cout << "Enter coordinates S" << i << "(x, y) and T" << i << "(x, y): " << endl;
		for (int j = 0 + 4*k; j < 4 + 4*k; j++)
		{
			cin >> temp;
			masCord[j] = temp;
		}
		k++;
	}

	// Всевозможные проверки
	for (int i = 0; i < countSource * 4; i = i + 2)
	{
		if (masCord[i] < 1 || masCord[i] > 10)
		{
			cout << "ERROR: X out of range [1; 10]" << endl;
			flag = true;
		}

		if (masCord[i + 1] < 1 || masCord[i + 1] > 10)
		{
			cout << "ERROR: Y out of range [1; 10]" << endl;
			flag = true;
		}

		if (field[masCord[i]][masCord[i + 1]] == -2)
		{
			cout << "ERROR: You set S or T on the block" << endl;
			flag = true;
		}
	}

	for (int i = 0; i < countSource * 4; i = i + 2)
	{
		if (field[masCord[i]][masCord[i + 1]] == -333)
		{
			cout << "ERROR: One of the S(x, y) or T(x, y) the same as next S or T" << endl;
			flag = true;
		}
		field[masCord[i]][masCord[i + 1]] = -333;
	}

	if (flag == true)
	{
		for (int i = 1; i < 11; i++)
		for (int j = 1; j < 11; j++)
		{
			if (field[i][j] == -2) continue;
			field[i][j] = 0;
		}
		goto tryAgain;
	}
	return countSource;
}

// Функция луний =.= 
int printLines(int x, int y, char mode)
{
	int temp;
	int i = 0;
	bool flag = false;

	if (mode == 'H')
	{
		saveTreeH[9] = x;

		// Горизонтальная линия идем вправо
		temp = y + 1;
		while (field[x][temp] == 0 || field[x][temp] == -3 || field[x][temp] == -10)
		{
			if (field[x][temp] == -10) return 1;
			saveTreeH[i] = temp;
			field[x][temp] = -3;
			temp++;
			i++;
		}

		// Горизонтальная линия идем влево
		temp = y - 1;
		while (field[x][temp] == 0 || field[x][temp] == -3 || field[x][temp] == -10)
		{
			if (field[x][temp] == -10) return 1;
			saveTreeH[i] = temp;
			field[x][temp] = -3;
			temp--;
			i++;
		}
	}

	if (mode == 'V')
	{
		saveTreeV[9] = y;

		// Вертикальная линия идем вниз
		temp = x + 1;
		while (field[temp][y] == 0 || field[temp][y] == -3 || field[temp][y] == -10)
		{
			if (field[temp][y] == -10) return 1;
			saveTreeV[i] = temp;
			field[temp][y] = -3;
			temp++;
			i++;
		}

		// Вертикальная линия идем вверх
		temp = x - 1;
		while (field[temp][y] == 0 || field[temp][y] == -3 || field[temp][y] == -10)
		{
			if (field[temp][y] == -10) return 1;
			saveTreeV[i] = temp;
			field[temp][y] = -3;
			temp--;
			i++;
		}
	}

	return 0;
}

// Функция мнимых линий. От указанной точки и направления считает длины всех мнимых линий от каждой клетки
bool ilines(int x, int y, int countVetv, char mode)
{
	int tempG, tempL;
	int length = 0;
	int i = 0;

	if (mode == 'H')
	{
		// Горизонтальная линия идем вправо
		tempG = y + 1;
		while (field[x][tempG] == 0 || field[x][tempG] == -3)
		{
			// По вертикали идем вниз
			tempL = x + 1;
			while (field[tempL][tempG] == 0 || field[tempL][tempG] == -3)
			{
				length++;
				tempL++;
			}
			if (field[tempL][tempG] == -10)
			{
				field[x][tempG] = countVetv;
				return false;
			}

			// По вертикали идем вверх
			tempL = x - 1;
			while (field[tempL][tempG] == 0 || field[tempL][tempG] == -3)
			{
				length++;
				tempL--;
			}
			if (field[tempL][tempG] == -10)
			{
				field[x][tempG] = countVetv;
				return false;
			}

			saveLengthH[i] = length + 1;
			length = 0;
			tempG++;
			i++;
		}

		// Горизонтальная линия идем влево
		tempG = y - 1;
		while (field[x][tempG] == 0 || field[x][tempG] == -3)
		{
			// По вертикали идем вниз
			tempL = x + 1;
			while (field[tempL][tempG] == 0 || field[tempL][tempG] == -3)
			{
				length++;
				tempL++;
			}
			if (field[tempL][tempG] == -10)
			{
				field[x][tempG] = countVetv;
				return false;
			}

			// По вертикали идем вверх
			tempL = x - 1;
			while (field[tempL][tempG] == 0 || field[tempL][tempG] == -3)
			{
				length++;
				tempL--;
			}
			if (field[tempL][tempG] == -10)
			{
				field[x][tempG] = countVetv;
				return false;
			}

			saveLengthH[i] = length + 1;
			length = 0;
			tempG--;
			i++;
		}
	}

	if (mode == 'V')
	{
		// Вертикальная линия идем вниз
		tempG = x + 1;
		while (field[tempG][y] == 0 || field[tempG][y] == -3)
		{
			// По горизонтали идем вправо
			tempL = y + 1;
			while (field[tempG][tempL] == 0 || field[tempG][tempL] == -3)
			{
				length++;
				tempL++;
			}
			if (field[tempG][tempL] == -10)
			{
				field[tempG][y] = countVetv;
				return false;
			}

			// По горизонтали идем влево
			tempL = y - 1;
			while (field[tempG][tempL] == 0 || field[tempG][tempL] == -3)
			{
				length++;
				tempL--;
			}
			if (field[tempG][tempL] == -10)
			{
				field[tempG][y] = countVetv;
				return false;
			}

			saveLengthV[i] = length + 1;
			length = 0;
			tempG++;
			i++;
		}

		// Вертикальная линия идем вверх
		tempG = x - 1;
		while (field[tempG][y] == 0 || field[tempG][y] == -3)
		{
			// По горизонтали идем вправо
			tempL = y + 1;
			while (field[tempG][tempL] == 0 || field[tempG][tempL] == -3)
			{
				length++;
				tempL++;
			}
			if (field[tempG][tempL] == -10)
			{
				field[tempG][y] = countVetv;
				return false;
			}

			// По горизонтали идем влево
			tempL = y - 1;
			while (field[tempG][tempL] == 0 || field[tempG][tempL] == -3)
			{
				length++;
				tempL--;
			}
			if (field[tempG][tempL] == -10)
			{
				field[tempG][y] = countVetv;
				return false;
			}

			saveLengthV[i] = length + 1;
			length = 0;
			tempG--;
			i++;
		}
	}
	return true;
}

// Функция печати поля в цифрах
void printField()
{
	for (int i = 1; i < 11; i++)
	{
		for (int j = 1; j < 11; j++)
		{
			cout << setw(5) << field[i][j];
		}
		cout << endl;
	}
}

// Функция печати поля в символах
void printSymbolField()
{
	for (int i = 1; i < 11; i++)
	{
		//cout << "   ";
		for (int j = 1; j < 11; j++)
		{
			switch (field[i][j])
			{
			case 0: cout << setw(2) << '.'; break;
			case -2: cout << setw(2) << (char)254; break;
			case -3: cout << setw(2) << '.'; break;
			case -5:
			case -6: cout << setw(2) << 'S'; break;
			case -11:
			case -10: cout << setw(2) << 'T'; break;
			case -20: cout << setw(2) << '*'; break;
			case -21: cout << setw(2) << '+'; break;
			case -22: cout << setw(2) << '='; break;
			default: cout << setw(2) << field[i][j];
			}
		}
		cout << endl;
	}
	
}

// Функция поиска максимальной длины мнимых линий. X и Y - координаты источника
void findMaxLength(char mode)
{
	int max = -1;

	if (mode == 'H')
	{
		for (int i = 0; i < 9; i++)
		{
			if (max < saveLengthH[i])
			{
				max = saveLengthH[i];
				goodIndexH = i;
			}
		}
	}

	if (mode == 'V')
	{
		for (int i = 0; i < 9; i++)
		{
			if (max < saveLengthV[i])
			{
				max = saveLengthV[i];
				goodIndexV = i;
			}
		}
	}
}

// Функция установки блоков
void setBlock(int count)
{
	int x, y;
	for (int i = 0; i < count; i++)
	{
		cout << "Set block's (x, y): " << endl;
		cin >> x >> y;
		field[x][y] = -2;
	}
}

// Массив копирования массива
void massCopy(int *massOld, int *mass, int N)
{
	for (int i = 0; i < N; i++)
	{
		massOld[i] = mass[i];
	}
}

// Функция поиска пути
void findWay(int x, int y, char mode)
{
	int temp;
	if (mode == 'H')
	{
		// Горизонтальная линия идем вправо
		temp = y + 1;
		while (field[x][temp] == 0 || field[x][temp] == -3 || field[x][temp] == countVetv || field[x][temp] == -5)
		{
			if (field[x][temp] == countVetv)
			{
				if (field[x][temp] == -5) countVetv = 1;

				for (int i = y + 1; i <= temp; i++)
				{
					// DELETED WAYS
					field[x][i] = Ways;
				}
				if (countVetv == 1) countVetv = -4;
				if (countVetv == -5) return;
				countVetv--;
				findWay(x, temp, 'V');
			}
			temp++;
		}

		// Горизонтальная линия идем влево
		temp = y - 1;
		while (field[x][temp] == 0 || field[x][temp] == -3 || field[x][temp] == countVetv || field[x][temp] == -5)
		{
			if (field[x][temp] == countVetv || field[x][temp] == -5)
			{
				if (field[x][temp] == -5) countVetv = 1;

				for (int i = temp; i <= y - 1; i++)
				{
					field[x][i] = Ways;
				}
				if (countVetv == 1) countVetv = -4;
				if (countVetv == -5) return;
				countVetv--;
				findWay(x, temp, 'V');
			}
			temp--;
		}
	}

	if (mode == 'V')
	{
		// Вертикальная линия идем вниз
		temp = x + 1;
		while (field[temp][y] == 0 || field[temp][y] == -3 || field[temp][y] == countVetv || field[temp][y] == -5)
		{
			if (field[temp][y] == countVetv || field[temp][y] == -5)
			{
				if (field[temp][y] == -5) countVetv = 1;

				for (int i = x + 1; i <= temp; i++)
				{
					field[i][y] = Ways;
				}
				if (countVetv == 1) countVetv = -4;
				if (countVetv == -5) return;
				countVetv--;
				findWay(temp, y, 'H');
			}
			temp++;
		}

		// Вертикальная линия идем вверх
		temp = x - 1;
		while (field[temp][y] == 0 || field[temp][y] == -3 || field[temp][y] == countVetv || field[temp][y] == -5)
		{
			if (field[temp][y] == countVetv || field[temp][y] == -5)
			{
				if (field[temp][y] == -5) countVetv = 1;

				for (int i = temp; i <= x - 1; i++)
				{
					field[i][y] = Ways;
				}
				if (countVetv == 1) countVetv = -4;
				if (countVetv == -5) return;
				countVetv--;
				findWay(temp, y, 'H');
			}
			temp--;
		}
	}
}

// Функция очистки поля после поиска пути
void clearField(int countSource)
{
	for (int i = 1; i < 11; i++)
	{
		for (int j = 1; j < 11; j++)
		{
			if (field[i][j] == -2 || field[i][j] == -10 || field[i][j] == -11 || field[i][j] == -5 || field[i][j] == -6 || field[i][j] == -20 || field[i][j] == -21 || field[i][j] == -22) continue;
			else field[i][j] = 0;
		}
	}

	for (int i = 0; i < countSource; i = i + 2)
	{
		if (field[masCord[i]][masCord[i + 1]] == -6 || field[masCord[i]][masCord[i + 1]] == -11) continue;
		field[masCord[i]][masCord[i + 1]] = -333;
	}

	for (int i = 0; i < 10; i++)
	{
		saveTreeH[i] = 0;
		saveTreeV[i] = 0;
		saveOldTreeH[i] = 0;
		saveOldTreeV[i] = 0;
		saveLengthH[i] = 0;
		saveLengthV[i] = 0;
	}

	goodIndexH = 0;
	goodIndexV = 0;
	countVetv = 1;
}

// рисование на экране
void renderScene()
{
	float w = glutGet(GLUT_SCREEN_WIDTH);
	float h = glutGet(GLUT_SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(w, h, w / 2, -h / 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, dz);
	glRotatef(x, 1, 0, 0);
	glRotatef(y, 0, 1, 0);
	glRotatef(z, 0, 0, 1);
	glRotatef(-90, 0, 0, 1); // кос-фич

	for (int i = 1; i < 11; i++)
	{
		for (int j = 1; j < 11; j++)
		{
			switch (field[i][j])
			{
				// Препятствие
			case -2:
				glColor3f(0.3, 0.04, 0.44);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();
				break;

				// Источник S
			case -5:
			case -6:
				glColor3f(0, 0, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glColor3f(0.8, 0.8, 0.8);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3f(0.035, 0.678, 0.38);
				glTranslatef(i - 11 + 0.5, j - 11 + 0.5, 0);
				glutSolidTorus(0.2, 0.35, 6, 10);

				glLoadIdentity();
				glTranslatef(0, 0, dz);
				glRotatef(x, 1, 0, 0);
				glRotatef(y, 0, 1, 0);
				glRotatef(z, 0, 0, 1);
				glRotatef(-90, 0, 0, 1);
				break;

				// Приемник T
			case -10:
			case -11:
				glColor3f(0, 0, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glColor3f(0.8, 0.8, 0.8);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3f(1, 1, 0);
				glTranslatef(i - 11 + 0.5, j - 11 + 0.5, 0);
				glutSolidTorus(0.2, 0.35, 6, 10);

				glLoadIdentity();
				glTranslatef(0, 0, dz);
				glRotatef(x, 1, 0, 0);
				glRotatef(y, 0, 1, 0);
				glRotatef(z, 0, 0, 1);
				glRotatef(-90, 0, 0, 1);
				break;

				// Путь квадратами
			case -20:
				glColor3f(0, 0, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glColor3f(0.8, 0.8, 0.8);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3f(0.98, 0.05, 0.35);
				glTranslatef(i - 11 + 0.5, j - 11 + 0.5, 0);
				glutSolidSphere(0.3, 50, 50);

				glLoadIdentity();
				glTranslatef(0, 0, dz);
				glRotatef(x, 1, 0, 0);
				glRotatef(y, 0, 1, 0);
				glRotatef(z, 0, 0, 1);
				glRotatef(-90, 0, 0, 1);
				break;

			case -21:
				glColor3f(0, 0, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glColor3f(0.8, 0.8, 0.8);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3f(0.98, 0.05, 0.35);
				glTranslatef(i - 11 + 0.5, j - 11 + 0.5, 0);
				glutSolidSphere(0.3, 50, 50);

				glLoadIdentity();
				glTranslatef(0, 0, dz);
				glRotatef(x, 1, 0, 0);
				glRotatef(y, 0, 1, 0);
				glRotatef(z, 0, 0, 1);
				glRotatef(-90, 0, 0, 1);
				break;

			case -22:
				glColor3f(0, 0, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glColor3f(0.8, 0.8, 0.8);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor3f(0.98, 0.05, 0.35);
				glTranslatef(i - 11 + 0.5, j - 11 + 0.5, 0);
				glutSolidSphere(0.3, 50, 50);

				glLoadIdentity();
				glTranslatef(0, 0, dz);
				glRotatef(x, 1, 0, 0);
				glRotatef(y, 0, 1, 0);
				glRotatef(z, 0, 0, 1);
				glRotatef(-90, 0, 0, 1);
				break;

				// Поле
			default:
				glColor3f(0, 0, 0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();

				glColor3f(0.8, 0.8, 0.8);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);
				glVertex3f(i - 11, j - 11, 0);
				glVertex3f(i - 11, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11 + 1, 0);
				glVertex3f(i - 11 + 1, j - 11, 0);
				glEnd();
				break;
			}
		}
	}
	glutSwapBuffers();
}

void readKeyboard(unsigned char key, int, int)
{
	// Масштаб
	if (key == '-') dz -= 1;
	if ((key == '=') || (key == '+')) dz += 1;

	// Вращение
	if ((key == 'w') || (key == 'ц')) x -= 6;
	if ((key == 's') || (key == 'ы')) x += 6;
	if ((key == 'a') || (key == 'ф')) y -= 6;
	if ((key == 'd') || (key == 'в')) y += 6;
	if ((key == 'q') || (key == 'й')) z += 6;
	if ((key == 'e') || (key == 'у')) z -= 6;

	if (key == 27) exit(0);

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(w, h);
	glutCreateWindow("Algprithm HauHauHauuuuu");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.8, 0.8, 0.8, 1);

	bool end = true;
	int countSource;
	int j = 0;

	// Количество блоков
	setBlock(4);
	countSource = initCord();

	for (int i = 0; i < countSource; i++)
	{
		setS(masCord[j], masCord[j + 1], 'S');
		setT(masCord[j + 2], masCord[j + 3], 'S');

		saveTreeH[9] = saveTreeV[goodIndexV] = masCord[j];
		saveTreeH[goodIndexH] = saveTreeV[9] = masCord[j + 1];

		while (end)
		{
			massCopy(saveOldTreeH, saveTreeH, 10);
			massCopy(saveOldTreeV, saveTreeV, 10);

			
			if (printLines(saveOldTreeH[9], saveOldTreeH[goodIndexH], 'V')) break;
			if (printLines(saveOldTreeV[goodIndexV], saveOldTreeV[9], 'H')) break;

			end = ilines(saveOldTreeH[9], saveOldTreeH[goodIndexH], countVetv, 'V');
			if (end == false) break;
			end = ilines(saveOldTreeV[goodIndexV], saveOldTreeV[9], countVetv, 'H');
			if (end == false) break;

			findMaxLength('H');
			findMaxLength('V');

			field[saveTreeH[9]][saveTreeH[goodIndexH]] = countVetv;
			field[saveTreeV[goodIndexV]][saveTreeV[9]] = countVetv;

			countVetv++;

			if (countVetv == 100) goto Next;
		}
		end = true;

		findWay(masCord[j + 2], masCord[j + 3], 'H');
		findWay(masCord[j + 2], masCord[j + 3], 'V');

		Next:
		setS(masCord[j], masCord[j + 1], 'F');
		setT(masCord[j + 2], masCord[j + 3], 'F');

		clearField(countSource * 4);

		printField();
		cout << endl;
		printSymbolField();
		cout << endl;

		Ways--;
		j = j + 4;
	}


	glutDisplayFunc(renderScene);
	glutKeyboardFunc(readKeyboard);
	glutMainLoop();

	return 0;
}