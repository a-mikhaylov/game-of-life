#include <stdio.h>
#include <glut.h>
#include <time.h>
#include <stdlib.h>
#include <string.h> 

#define FH 100
#define FW 100
#define SIZE 6

typedef unsigned char byte;
byte Frame[FH][FW][3];
byte Frame2[FH][FW][3]; //unsigned char

void Noize(void)
{
	int x, y;
	for (y = 0; y < FH; y++)
		for (x = 0; x < FW; x++)
		{
			if (rand() % 2 == 1)
				Frame[y][x][1] = 255;
			else
				Frame[y][x][1] = 0;
		}
}

void NextFrame(void)
{
	int x, y, x0, y0;
	int cnt = 0;

	for (y0 = 0; y0 < FH; y0++)
		for (x0 = 0; x0 < FW; x0++)
		{
			cnt = 0;
			if (Frame[x0][y0][1] > 0)
			{
				for (y = y0 - 1; y <= y0 + 1; y++)
					for (x = x0 - 1; x <= x0 + 1; x++)
					{
						if (0 <= y && y < FH && 0 <= x && x < FW)
							if (Frame[x][y][1] > 0)
								cnt++;
					}
				cnt--;
				if (cnt != 2 && cnt != 3)
					Frame2[x0][y0][1] = 0;
				else
					Frame2[x0][y0][1] = 255;
			}
			else
			{
				for (y = y0 - 1; y <= y0 + 1; y++)
					for (x = x0 - 1; x <= x0 + 1; x++)
					{
						if (0 <= y && y < FH && 0 <= x && x < FW)
							if (Frame[x][y][1] > 0)
								cnt++;
					}
				if (cnt == 3)
					Frame2[x0][y0][1] = 255;
			}
		}

}

void Glider(int x0, int y0)
{
	int x, y;

	for (y = y0 - 1; y <= y0 + 1; y++)
		for (x = x0 - 1; x <= x0 + 1; x++)
			if ((y == y0 + 1) || (y == y0 - 1 && x == x0) || (y == y0 && x == x0 + 1))
				Frame2[x][y][1] = 255;
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 27)
		exit(0);
	if (key == 'g')
		Glider(10, 100);
	if (key == 'r')
		Noize();
	if (key == 'b')
	{
		for (y = FH / 2 - 30; y <= FH / 2 + 30; y++)
			for (x = FW / 2 - 30; x <= FW / 2 + 30; x++)
				if (rand() % 2 == 1)
					Frame2[x][y][1] = 255;
	}
}

void Mouse(int button, int state, int x, int y)
{
	int a = x / SIZE, b = y / SIZE;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
			Glider(b, a);
	}
		
}



void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2d(-1, 1);
	glPixelZoom(SIZE, -SIZE); //size

	glDrawPixels(FW, FH, GL_RGB, GL_UNSIGNED_BYTE, Frame);

	glFinish();
	//glutPostRedisplay();
}

void Idle(void)
{
	double dt;

	static long l_t = -1;

	if (l_t == -1)
		l_t = clock();
	dt = (double)(clock() - l_t) / CLOCKS_PER_SEC;
	if (dt > 0.05)
	{
		l_t = clock();
		printf("%f\n", dt);
		NextFrame();
		memcpy(Frame, Frame2, sizeof(Frame));
		glutPostRedisplay();
		//NextFrame();
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(FW * SIZE, FH * SIZE); //size
	glutInitWindowPosition(3, 3);
	glutCreateWindow("wuzzup?!");

	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutIdleFunc(Idle);

	Noize();

	glutMainLoop();

	return 0;
}