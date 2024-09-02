#include <graphics.h>
#include <ImageHlp.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <mmsystem.h>
#include <string.h>
#include <time.h>
#pragma comment(lib,"winmm.lib")
#define maxnum 50

int width = 1536, height = 960;
int dif;

float jueduizhi(int x) {
	if (x < 0) {
		x = -x;
	}
	return x;
}

float randbetween(float min, float max) {
	return (max - min) * (rand() / (double)RAND_MAX) + min;
}

class player {
public:
	float widthp, heightp, widthp_die, heightp_die;
	float x, y;
	//float mx, my; //mouse new position
	IMAGE pic, pic2, pic_died, pic_died2;
	IMAGE pic_health,pic_health2;
	int health;

	void draw() {
		if (health > 0) {
			putimage(x - widthp / 2, y - heightp / 2, &pic2, SRCAND);
			putimage(x - widthp / 2, y - heightp / 2, &pic, SRCPAINT);
		}
		else {
			putimage(x - widthp / 2, y - heightp / 2, &pic_died, SRCAND);
			putimage(x - widthp / 2, y - heightp / 2, &pic_died2, SRCPAINT);
		}
	}

	void health_draw() {
		for (int i = 1; i <= health; i++) {
			putimage(x - widthp / 2+10, y - heightp / 2 +10*(2*i-0.5), &pic_health, SRCAND);
			putimage(x - widthp / 2+10, y - heightp / 2 + 10 * (2*i - 0.5), &pic_health2, SRCPAINT);
		}
	}

	void position_update(float mx, float my) {
		x = mx;
		y = my;
	}

	int isdied() {
		if (health <= 0) return 1;
		else return 0;
	}
};

class ball {
public:
	float x, y;
	float vx, vy;
	float radius;
	IMAGE pic, pic2;

	void draw() {
		putimage(x - radius, y - radius, &pic, SRCAND);
		putimage(x - radius, y - radius, &pic2, SRCPAINT);
	}

	void update() {
		x += vx;
		y += vy;
		if (x <= 0 || x >= width) {
			vx = -vx;
		}
		if (y <= 0 || y >= height) {
			vy = -vy;
		}
	}

	int iscollision(player p) {
		if (jueduizhi(x - p.x) <= p.widthp / 2 && jueduizhi(y - p.y) <= p.heightp / 2) return 1;
		else return 0;
	}
};

player p;
ball s[maxnum];
IMAGE bk_pic;
int cnt = 0;
int num = 0;

void startup() {
	srand(time(0));
	initgraph(width, height);
	setbkmode(TRANSPARENT);
	settextstyle(60, 36, _T("Consolas"));
	for (int i = 0; i < maxnum; i++) {
		s[i].x = width / 2;
		s[i].y = 0;
		s[i].vx = randbetween(-3, 3);
		s[i].vy = randbetween(0.5, 3);
		s[i].radius = 30;
		loadimage(&s[i].pic, _T("C:\\Users\\User\\Desktop\\pic\\bas2.png"), 2 * s[i].radius, 2 * s[i].radius);
		loadimage(&s[i].pic2, _T("C:\\Users\\User\\Desktop\\pic\\bas3.png"), 2 * s[i].radius, 2 * s[i].radius);
	}
	p.widthp = 377 / 4;
	p.heightp = 662 / 4;
	p.widthp_die = 600 / 5;
	p.heightp_die = 900 / 5;
	p.x = width / 2;
	p.y = height / 2;
	p.health = 3;
	int health_w = 32;
	float health_h = 29.25;

	loadimage(&bk_pic, _T("C:\\Users\\User\\Desktop\\pic\\bk.jpg"), width, height);
	loadimage(&p.pic, _T("C:\\Users\\User\\Desktop\\pic\\kun3.png"), p.widthp, p.heightp);
	loadimage(&p.pic2, _T("C:\\Users\\User\\Desktop\\pic\\kun2.png"), p.widthp, p.heightp);
	loadimage(&p.pic_died, _T("C:\\Users\\User\\Desktop\\pic\\kundied2.png"), p.widthp_die, p.heightp_die);
	loadimage(&p.pic_died2, _T("C:\\Users\\User\\Desktop\\pic\\kundied3.png"), p.widthp_die, p.heightp_die);
	loadimage(&p.pic_health, _T("C:\\Users\\User\\Desktop\\pic\\health2.jpg"), health_w, health_h);
	loadimage(&p.pic_health2, _T("C:\\Users\\User\\Desktop\\pic\\health3.jpg"), health_w, health_h);
	mciSendString(_T("open C:\\Users\\User\\Desktop\\muc\\kou.mp3 alias kou"), NULL, NULL, NULL);
	mciSendString(_T("open C:\\Users\\User\\Desktop\\muc\\die.mp3 alias die"), NULL, NULL, NULL);
	mciSendString(_T("open C:\\Users\\User\\Desktop\\muc\\ori1.mp3 alias ori"), NULL, NULL, NULL);
	mciSendString(_T("play ori repeat"), NULL, NULL, NULL);
	BeginBatchDraw();
}

void updatewithoutinput() {
	for (int i = 0; i <= num; i++) {
		s[i].update();
		if (s[i].iscollision(p)) {
			mciSendString(_T("play kou"), NULL, NULL, NULL);
			if (p.health == 2) {
				mciSendString(_T("open C:\\Users\\User\\Desktop\\muc\\kou.mp3 alias kou1"), NULL, NULL, NULL);
				mciSendString(_T("play kou1"), NULL, NULL, NULL);
			}
			if (p.health == 1) {
				mciSendString(_T("open C:\\Users\\User\\Desktop\\muc\\kou.mp3 alias kou2"), NULL, NULL, NULL);
				mciSendString(_T("play kou2"), NULL, NULL, NULL);
			}
			p.health--;
			s[i].x = -9999;
			s[i].y = -9999;
		}
	}
}

void updatewithinput() {
	MOUSEMSG m;
	if (MouseHit()) {
		m = GetMouseMsg();
		if (m.uMsg == WM_MOUSEMOVE) {
			p.position_update(m.x, m.y);
		}
	}
}

void show() {
	putimage(0, 0, &bk_pic);
	p.draw();
	p.health_draw();
	for (int i = 0; i <= num; i++) {
		s[i].draw();
	}
	FlushBatchDraw();
	cleardevice();
	Sleep(5);
}

void numincrease() {
	cnt++;
	if (cnt == 200) {
		num++;
		cnt = 0;
	}
}

int main() {
	here:
	startup();
	while (1) {
		numincrease();
		updatewithinput();
		updatewithoutinput();
		show();
		if (p.isdied()) {
			mciSendString(_T("play die"), NULL, NULL, NULL);
			break;
		}
	}
	mciSendString(_T("close ori"), NULL, NULL, NULL);
	while (1) {
		char c = _getch();
		if (c == 'y') goto here;
		else if (c == 'x') closegraph();
	}
	_getch();

	return 0;
}