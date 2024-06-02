#include "main.h"
#include "peripherals.h"
#include "utilities.h"

//Nasz kod
#include <math.h>
#include <vector>

int preScale=0;

struct Pozycja
{
	unsigned int x;
	unsigned int y;
};

struct Pocisk{
	Pozycja pozycja;
	int szer;
	int wys;
	int predkosc;
	int moc;
	char czyj;
	bool czyRysowac;

	Pocisk()
	{
		szer=5;
		wys=10;
		predkosc=10;
		moc=5;
		czyj='n';
		czyRysowac=true;
	}

	//funkcja rysujaca pocisk
	void RysujPocisk()
	{
		if(czyRysowac==true)
		{
			for(int x=-szer/2; x<szer/2; x++)
			{
				for(int y=-wys/2; y<wys/2; y++)
				{
					int nx = pozycja.x+x;
					int ny = pozycja.y+y;

					if(czyj=='p') {SetPixel(GRAPH,nx,ny,0xF800);}
					else if(czyj=='g') {SetPixel(GRAPH,nx,ny,0xFFFF);}
				}
			}
		}
	}
};

struct Przeciwnik{
	Pozycja pozycja;
	int punktyZycia;

	Przeciwnik()
	{
		pozycja.y=40;
		punktyZycia=100;
	}

	//funkcja rysujaca przeciwnik + pasek zdrowia
	void RysujPrzeciwnik()
	{
		if(punktyZycia>0)
		{
			int promien=15;
			int nx=0, ny=0;
			for(int i=0; i<360; i++)
			{
				nx=(int)(promien*sin(i* (M_PI / 180.0))+pozycja.x);
				ny=(int)(promien*cos(i* (M_PI / 180.0))+pozycja.y);
				SetPixel(GRAPH,nx,ny,0xF800);
			}
			//pasek
			for(int poziom=-15; poziom<15; poziom++)
			{
				SetPixel(GRAPH,(pozycja.x+poziom),(pozycja.y-15),0xFF0000);
				SetPixel(GRAPH,(pozycja.x+poziom),(pozycja.y-25),0xFF0000);
			}
			for(int pion=-15; pion>-25; pion--)
			{
				SetPixel(GRAPH,(pozycja.x-15),(pozycja.y+pion),0xFF0000);
				SetPixel(GRAPH,(pozycja.x+15),(pozycja.y+pion),0xFF0000);
			}
			int zycie=punktyZycia*30/100;
			for(int poziom=-15; poziom<zycie-15; poziom++)
			{
				for(int pion=-15; pion>-25; pion--)
				{
					SetPixel(GRAPH,(pozycja.x-poziom),(pozycja.y+pion),0xFF0000);
				}
			}
		}
	}
};

struct Gracz{
	Pozycja pozycja;
	int punktyZycia;

	Gracz(){
		pozycja.x=width/2;
		pozycja.y=height-40;
		punktyZycia=100;
	}

	//funkcja rysujaca gracz + pasek zdrowia
	void RysujGracz()
	{
		//gracz
		int promien=15;
		int nx=0, ny=0;
		for(int i=0; i<360; i++)
		{
			nx=(int)(promien*sin(i* (M_PI / 180.0))+pozycja.x);
			ny=(int)(promien*cos(i* (M_PI / 180.0))+pozycja.y);
			SetPixel(GRAPH,nx,ny,0xFFFF);
		}

		//pasek
		for(int poziom=-15; poziom<15; poziom++)
		{
			SetPixel(GRAPH,(pozycja.x+poziom),(pozycja.y+15),0xFF0000);
			SetPixel(GRAPH,(pozycja.x+poziom),(pozycja.y+25),0xFF0000);
		}
		for(int pion=15; pion<25; pion++)
		{
			SetPixel(GRAPH,(pozycja.x-15),(pozycja.y+pion),0xFF0000);
			SetPixel(GRAPH,(pozycja.x+15),(pozycja.y+pion),0xFF0000);
		}
		int zycie=punktyZycia*30/100;
		for(int poziom=-15; poziom<zycie-15; poziom++)
		{
			for(int pion=15; pion<25; pion++)
			{
				SetPixel(GRAPH,(pozycja.x-poziom),(pozycja.y+pion),0xFF0000);
			}
		}
	}
};

bool CzyKolizja(Przeciwnik przeciwnik, Pocisk pocisk)
{
	if(przeciwnik.punktyZycia>0)
	{
		if(pocisk.czyRysowac==true)
		{
			int promien=15;
			int xprze=0, yprze=0;
			for(int i=0; i<360; i++)
			{
				xprze=(int)(promien*sin(i* (M_PI / 180.0))+przeciwnik.pozycja.x);
				yprze=(int)(promien*cos(i* (M_PI / 180.0))+przeciwnik.pozycja.y);

				for(int x=-pocisk.szer/2; x<pocisk.szer/2; x++)
				{
					int xp = pocisk.pozycja.x+x;
					int yp = pocisk.pozycja.y-pocisk.wys/2;

					if(xprze==xp && yprze==yp) return true;
				}
			}
		}
	}

	return 0;
}

bool CzyKolizja(Gracz gracz, Pocisk pocisk)
{
	if(pocisk.czyRysowac==true)
	{
		int promien=15;
		int xg=0, yg=0;
		for(int i=0; i<360; i++)
		{
			xg=(int)(promien*sin(i* (M_PI / 180.0))+gracz.pozycja.x);
			yg=(int)(promien*cos(i* (M_PI / 180.0))+gracz.pozycja.y);

			for(int x=-pocisk.szer/2; x<pocisk.szer/2; x++)
			{
				int xp = pocisk.pozycja.x+x;
				int yp = pocisk.pozycja.y+pocisk.wys/2;

				if(xg==xp && yg==yp) return true;
			}
		}
	}
	return 0;
}

void SprawdzKolizje(Gracz &gracz, std::vector<Przeciwnik> &przeciwnicy, std::vector<Pocisk> &pociski)
{
	for ( auto& pocisk : pociski)
	{
		//kolizje pociskow gracza w przeciwnika
		if(pocisk.czyj=='g')
		{
			for ( auto& przeciwnik : przeciwnicy)
			{
				if(CzyKolizja(przeciwnik, pocisk)) przeciwnik.punktyZycia-=pocisk.moc;
			}
		}
		//kolizje pociskow przeciwnika w gracza
		if(pocisk.czyj=='p')
		{
			if(CzyKolizja(gracz, pocisk)) gracz.punktyZycia-=pocisk.moc;
		}
	}
}

void RysujPrzeciwnikow(std::vector<Przeciwnik> przeciwnicy)
{
	for ( auto& przeciwnik : przeciwnicy)
	{
		przeciwnik.RysujPrzeciwnik();
	}
}

void RysujPociski(std::vector<Pocisk> &pociski)
{
	for ( auto& pocisk : pociski)
	{
		if(pocisk.czyj=='g')
		{
			if(pocisk.pozycja.y<=1) pocisk.czyRysowac=false;
			else pocisk.pozycja.y--;
		}

		else if(pocisk.czyj=='p')
		{
			if(pocisk.pozycja.y>=(height-1)) pocisk.czyRysowac=false;
			else pocisk.pozycja.y++;
		}

		//printf("%i\n",pocisk.pozycja.y);
		pocisk.RysujPocisk();
	}
}

//

#define RPiLAB

int main(int argc, char *argv[]) {
	SystemInit();

	Gracz gracz;

	std::vector<Przeciwnik> przeciwnicy;
	for (int i = 0; i < 5; i++) {
		Przeciwnik przeciwnik;
		przeciwnik.pozycja.x = i * (width / 5) + (width / 10);
		przeciwnik.pozycja.y = 30;
		przeciwnicy.push_back(przeciwnik);
	}

	std::vector<Pocisk> pociski;

	bool gameOver=false;

	while (!gameOver) {
		preScale++;

		UpdateIO();

		if(gracz.pozycja.x<width-15 && checkKey(77)) gracz.pozycja.x+=2; //6
		if(gracz.pozycja.x>15 &&checkKey(75))gracz.pozycja.x-=2; //4
		if(checkKey(76)) //wystrzel pocisk (5)
		{
			Pocisk pocisk;
			pocisk.pozycja.x=gracz.pozycja.x;
			pocisk.pozycja.y=gracz.pozycja.y;
			//pocisk.predkosc=20;
			pocisk.czyj='g';
			pociski.push_back(pocisk);
		}

		if(preScale%100==0) //co 1 s przeciwnik strzela pocisk
		{
			int los = rand()%5; //0,4
			if(przeciwnicy[los].punktyZycia>0)
			{
				Pocisk pocisk;
				pocisk.pozycja.x=przeciwnicy[los].pozycja.x;
				pocisk.pozycja.y=przeciwnicy[los].pozycja.y;
				pocisk.predkosc=20;
				pocisk.czyj='p';
				pociski.push_back(pocisk);
			}
		}

		SprawdzKolizje(gracz, przeciwnicy, pociski);

		if(pociski.size()>20) pociski.erase(pociski.begin());

		if(gracz.punktyZycia<=0) {
			gameOver=true;
			printf("Przegrales!\nNastepnym razempostaraj sie bardziej.");
		}
		if(przeciwnicy[0].punktyZycia<=0
				&& przeciwnicy[1].punktyZycia<=0
				&& przeciwnicy[2].punktyZycia<=0
				&& przeciwnicy[3].punktyZycia<=0
				&& przeciwnicy[4].punktyZycia<=0) {
			gameOver=true;
			printf("Wygrales!\nGratuluje i zapraszam ponownie.");
		}

		ClearScreen();

		gracz.RysujGracz();
		RysujPrzeciwnikow(przeciwnicy);
		RysujPociski(pociski);


		if(preScale==10000) preScale=0;
		usleep(10000); //opoznia o 10ms
	}
}

void SystemInit() {
	FrameBufferFD = open("/dev/fb0", O_RDWR);
	ioctl(FrameBufferFD, FBIOGET_FSCREENINFO, &fix_info);
	framebuffer = mmap(NULL, fix_info.line_length * 480, PROT_READ | PROT_WRITE,MAP_SHARED, FrameBufferFD, 0);
	inputEventsFD = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
#ifdef RPiLAB
	inputJoyFD= open("/dev/input/js0", O_RDONLY | O_NONBLOCK);
#else
	inputJoyFD=0;
#endif
}

void UpdateIO() {

	struct input_event ev;
	while (read(inputEventsFD, &ev, sizeof ev) > 0) {
		if (ev.type == EV_KEY) {
			switch (ev.value) {
			case 2:
				gRawKeys[0] = ev.code;
				break;
			case 1:
				gRawKeys[0] = ev.code;
				break;
			case 0:
				gRawKeys[0] = 0;
				break;
			default:
				break;
			}
#ifndef RPiLAB
				switch (ev.code) {
				case KEY_KP4: JoYAct.AxisX =-127*(ev.value&1); break;
				case KEY_KP6: JoYAct.AxisX =127*(ev.value&1); break;
				case KEY_KP8: JoYAct.AxisY =-127*(ev.value&1); break;
				case KEY_KP2: JoYAct.AxisY =127*(ev.value&1); break;
				case KEY_RIGHT:
					if(ev.value == 1) JoYAct.ButtonStates |=BUTTON_2;
					if(ev.value == 0) JoYAct.ButtonStates &=~BUTTON_2;
					break;
				case KEY_LEFT:
					if(ev.value == 1) JoYAct.ButtonStates |=BUTTON_4;
					if(ev.value == 0) JoYAct.ButtonStates &=~BUTTON_4;
					break;
				case KEY_UP:
					if(ev.value == 1) JoYAct.ButtonStates |=BUTTON_1;
					if(ev.value == 0) JoYAct.ButtonStates &=~BUTTON_1;
					break;
				case KEY_DOWN:
					if(ev.value == 1) JoYAct.ButtonStates |=BUTTON_3;
					if(ev.value == 0) JoYAct.ButtonStates &=~BUTTON_3;
					break;
				default:
					break;
				}
#endif
		}

	}
#ifdef RPiLAB
	struct js_event ev2;
			while (read(inputJoyFD, &ev2, sizeof ev2) > 0) {
					if (ev2.type & JS_EVENT_AXIS) {
						switch (ev2.number) {

						case 1:
							JoYAct.AxisY = ev2.value >> 8;		break;
						case 2:
							JoYAct.AxisX = ev2.value >> 8;		break;
						case 3:
							JoYAct.AxisZ = ev2.value >> 8;		break;
						case 4:
							JoYAct.AxisR = ev2.value >> 8;		break;
						default:
							break;
						}

					}
					if (ev2.type & JS_EVENT_BUTTON) {
						if (ev2.value)
							JoYAct.ButtonStates |= (1 << ev2.number);
						else
							JoYAct.ButtonStates &= ~(1 << ev2.number);
					}

}
#endif

}

void ClearScreen() {

	unsigned int bckgmask =getKey()>>2;

	int stage_max= -JRz/20;
	if (stage_max < 1)		stage_max = 1;
	if (stage_max > 9)		stage_max = 9;

	if (JoYAct.ButtonStates & BUTTON_SELECT)
		stage_max = 24;

	stage++;
	for (int a = stage % stage_max; a < (640 * 480); a += stage_max) {
		((u_int32_t*) framebuffer)[fix_info.line_length / 4 * (a / 640)
				+ a % 640] = GRAPH[a];
		GRAPH[a] = ((GRAPH[a] & 0xfefefe) >> 1) | bckgmask;
	}
}

