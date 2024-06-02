#include "peripherals.h"

struct JoY JoYAct;


 int gRawKeys[6] = { 0, 0, 0, 0, 0, 0 }; //chyba ghosting na 6 klawiszy

int checkKey(int code) { //sprawdz czy podany przycisk klikniety
	int a = 0;
	while (a < 6) //sprawdza czy ktorys z klawiszy zapisanych w tablicy sie zgadza
		if (gRawKeys[a++] == code)
			return 1; //jesli sie zgadza zwraca 1
	return 0; //jesli nie zwraca 2
}

int getKey() { //zwraca przyciski zapisane w tablicy
	int a = 0;
	while (a < 6)
		if (gRawKeys[a++] != 0)
			return gRawKeys[--a]; //jesli jakis jest to go zwraca
	return 0;
}

