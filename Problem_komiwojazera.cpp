#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

const int A = 5;
const int B = 1000;

struct genom
{
	double x;
	double y;
	int id;
};


void podaj(genom *cele,  genom pierwszy , int n)
{
	cout << "Podaj lokalizacje startowa" << endl;
	cout << "Podaj x" << endl;
	cin >> pierwszy.x;
	cout << "Podaj y" << endl;
	cin >> pierwszy.y;
	pierwszy.id = -1;
	for (int i = 0; i < n; i++)
	{
		cout << " Podaj wspolrzedne lokalizacji " << i +1 << endl;
		cout << " x = ";
		cin >> cele[i].x;
		cout << " y = ";
		cin >> cele[i].y;
		cele[i].id = i;
	}
}
void policz_odleglosci(genom cele[], int n, double **odleglosci)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (j == i)
				odleglosci[i][j] = 0; // Po przekątnych są te same cele więc dajemy 0
			else
			{
				odleglosci[i][j] = sqrt((cele[i].x - cele[j].x)*(cele[i].x - cele[j].x) + (cele[i].y - cele[j].y)*(cele[i].y - cele[j].y)); // Liczenie odległości
			}
		}
	}
}

void losuj(genom *p, genom pierwszy , int n)
{
	pierwszy.x  = rand() % (B - A + 1) + A;
	pierwszy.y  = rand() % (B - A + 1) + A;
	pierwszy.id = -1;
	for (int i = 0; i < n; i++)
	{
		p[i].x = rand() % (B - A + 1) + A; // losowanie kolejnych lokalizacji x
		p[i].y = rand() % (B - A + 1) + A;  // losowanie kolejnych lokalizacji y
		p[i].id = i;
	}
}

double sumowanieDrogi(genom *p, genom pierwszy , int n, double **odleglosci)
{
	double suma = sqrt((pierwszy.x - p[0].x)*(pierwszy.x - p[0].x) + (pierwszy.y - p[0].y)*(pierwszy.y - p[0].y)); // Uwgledniamy sume odelglosci miedzy startowa lokalizaja a 1 w kombinacjach
	for (int i = 0; i < n - 1; i++)
	{
		suma = suma + odleglosci[p[i].id][p[i + 1].id];
	}
	return suma;
}

void mutacja(genom *X_base, int n)
{
	int mutowany_gen = rand() % n;
	int drugi_mutowany_gen;

	do {
		drugi_mutowany_gen = rand() % n;
	} while (drugi_mutowany_gen == mutowany_gen);

	int id = X_base[mutowany_gen].id;
	double x = X_base[mutowany_gen].x;
	double y = X_base[mutowany_gen].y;

	X_base[mutowany_gen] = X_base[drugi_mutowany_gen];
	X_base[drugi_mutowany_gen].id = id;
	X_base[drugi_mutowany_gen].x = x;
	X_base[drugi_mutowany_gen].y = y;
}

genom *krzyzowanie(genom *X1, genom *X2, int n)
{
	double r;
	genom *nowy = new genom[n];
	r = ceil(n / 2);

	for (int i = 0; i < r; i++)
	{
		nowy[i] = X1[i];
	}

	int licznikZmian = r;
	bool czyPowtorzona = false;

	for (int i = 0; i < n; i++) {
		czyPowtorzona = false;

		for (int j = 0; j < licznikZmian; j++) {
			if (X2[i].id == nowy[j].id) {
				czyPowtorzona = true;
				break;
			}
		}

		if (!czyPowtorzona) {
			nowy[licznikZmian] = X2[i];
			licznikZmian++;
		}
	}
	return nowy;
}

void stworz_kombinacje(int n, genom *przodek, int N, genom **kombinacje) // Tworzy populacje losowych kombinacji zapisując je do dwuwymiarowej tablicy kolumnami
{
	int los1;
	int los2;                  // losowanie odbywa się przez losową zamiane elementów
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < n; j++)
		{
			kombinacje[i][j] = przodek[j];      // numer komórki = numer wiersza
		}
	}
	genom schowek;

	for (int i = 0; i < N; i++)
	{
		for (int j = 1; j < n; j++)  // od jedynki bo zawsze startujemy w pierwszym
		{
			los1 = rand() % n;
			los2 = rand() % n;
			if (!(los1 == los2)) {
				schowek = kombinacje[i][los1];
				kombinacje[i][los1] = kombinacje[i][los2];
				kombinacje[i][los2] = schowek;
			}
		}
	}
}

int main()
{
	int n; // ilość lokalizacji
	int decyzja;
	srand(time(NULL));

	cout << "Ile miejsc oprocz lokalizacji startowej chcesz podac ?" << endl;
	cin >> n;

	while (n < 2)
	{
		cout << "Musisz podac conajmniej 2 lokacje" << endl;
		cin >> n;
	}
	genom *cele;    // Tablica na przechowanie wszystkich lokalizacji
	cele = new genom[n];
	genom pierwszy;
	pierwszy.x = 0;
	pierwszy.y = 0;
	pierwszy.id = -1;

	cout << "Pierwsze wybrane miejsce bedzie lokalizacja startowa " << endl;
	cout << "Wylosowac miejsca (1) czy podac (2)? ";
	cin >> decyzja;
	while (decyzja != 1 && decyzja != 2)
	{
		cout << " Bledna odowiedz, podaj jeszcze raz: " << endl;
		cin >> decyzja;
	}

	if (decyzja == 1)
	{
		losuj(cele, pierwszy , n);
	}
	else podaj(cele, pierwszy , n);


	int liczbaPokolen = 400;  // Domyślne parametry
	float prawdopodobienstwoMutacji = 0.001;
	float prawdopodobienstwoKrzyzowania = 0.9;
	int N = 2 * n; // liczba kombinacji

	cout << "Uzyc domyslnych ustawien (1) czy wprowadzic recznie parametry algorytmu(2)? ";
	cin >> decyzja;
	while (decyzja != 1 && decyzja != 2)
	{
		cout << " Bledna odowiedz, podaj jeszcze raz: " << endl;
		cin >> decyzja;
	}

	if (decyzja == 2)
	{
		cout << "Podaj liczbe iteracji= ";
		cin >> liczbaPokolen;
		while (liczbaPokolen < 1)
		{
			cout << " Liczba iteracji musi wynosic conajmniej 2. Podaj jeszcze raz:  ";
			cin >> liczbaPokolen;
		}
		cout << "Podaj liczebnosc populacji = ";
		cin >> N;
		while (N < 1)
		{
			cout << " Liczebnosc populacji musi wynosic conajmniej 2. Podaj jeszcze raz:  ";
			cin >> N;
		}
		cout << "Podaj czestotliwosc mutacji <0 ; 1> = ";
		cin >> prawdopodobienstwoMutacji;
		while (prawdopodobienstwoMutacji < 0 || prawdopodobienstwoMutacji > 1)
		{
			cout << " Bledy zakres. Podaj jeszcze raz:  ";
			cin >> prawdopodobienstwoMutacji;
		}
		cout << "Podaj prawdopodobienstwo krzyzowania < 0 ; 1> = ";
		cin >> prawdopodobienstwoKrzyzowania;
		while (prawdopodobienstwoKrzyzowania < 0 || prawdopodobienstwoKrzyzowania > 1)
		{
			cout << " Bledy zakres. Podaj jeszcze raz:  ";
			cin >> prawdopodobienstwoKrzyzowania;
		}
	}

	double ** odleglosci = new double *[n];     // tablica dwuwymiarowa na przechowanie wszystkich odleglosci poszczegolnych miast
	for (int i = 0; i < n; i++)
		odleglosci[i] = new double[n];       // koniec tworzenia tablicy

	policz_odleglosci(cele, n, odleglosci);  // Liczymy odleglosci pomiedzy poszczególnymi miastami i zapisujemy do 2 wymiarowej tablicy


	genom **kombinacje;    // tablica dwuwymiarowa na przechowanie kombinacji
	kombinacje = new genom *[N];
	for (int i = 0; i < N; i++)
		kombinacje[i] = new genom[n];

	stworz_kombinacje(n, cele, N, kombinacje);


	for (int i = 1; i < liczbaPokolen; i++) {
		for (int j = 0; j < N; j++) {
			if (rand() % 101 > (100 - prawdopodobienstwoKrzyzowania * 100)) {
				int krzyzowanyOsobnik = rand() % N;
				while (krzyzowanyOsobnik == j)
					krzyzowanyOsobnik = rand() % N;
				genom *nowyOsobnik;
				genom *rodzic1;
				genom *rodzic2;
				rodzic1 = new genom[n];
				rodzic2 = new genom[n];

				for (int k = 0; k < n; k++) {
					rodzic1[k] = kombinacje[j][k];
					rodzic2[k] = kombinacje[krzyzowanyOsobnik][k];
				}
				nowyOsobnik = krzyzowanie(rodzic1, rodzic2, n);

				double sumaRodzica1 = sumowanieDrogi(rodzic1, pierwszy , n, odleglosci);
				double sumaRodzica2 = sumowanieDrogi(rodzic2, pierwszy, n, odleglosci);
				double sumaNowego = sumowanieDrogi(nowyOsobnik, pierwszy, n, odleglosci);

				if (sumaNowego < sumaRodzica1) {
					if (sumaNowego < sumaRodzica2)
						for (int l = 0; l < n; l++)
							kombinacje[krzyzowanyOsobnik][l] = nowyOsobnik[l];
					else
						for (int m = 0; m < n; m++)
							kombinacje[j][m] = nowyOsobnik[m];
				}
				else if (sumaNowego < sumaRodzica2)
					for (int l = 0; l < n; l++)
						kombinacje[krzyzowanyOsobnik][l] = nowyOsobnik[l];

				delete[]nowyOsobnik;
				delete[]rodzic1;
				delete[]rodzic2;
			}

			if (rand() % 101 > (100 - prawdopodobienstwoMutacji * 100)) {
				genom *mutowanyOsobnik;
				mutowanyOsobnik = new genom[n];
				for (int k = 0; k < n; k++) {
					mutowanyOsobnik[k] = kombinacje[j][k];
				}
				mutacja(mutowanyOsobnik, n);
				for (int l = 0; l < n; l++) {
					kombinacje[j][l] = mutowanyOsobnik[l];
				}
			}
		}
	}
	genom *schowek2;
	schowek2 = new genom[n];

	for (int i = 0; i < n; i++) {
		schowek2[i] = kombinacje[0][i];
	}
	double najkrotszaDroga = sumowanieDrogi(schowek2, pierwszy, n, odleglosci);
	int numer = 0;
	cout << endl;
	cout << " Sumy drog najlepszych znalezionych opcji: " << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < n; j++) {
			schowek2[j] = kombinacje[i][j];
		}
		double droga = sumowanieDrogi(schowek2, pierwszy, n, odleglosci);
		cout << droga << endl;
		if (droga < najkrotszaDroga) {
			najkrotszaDroga = droga;
			numer = i;
		}
	}

	delete[]schowek2;
	delete[]cele;

	cout << "Najkrotsza droga bedzie gdy wybierzemy kolejno miasta: " << endl;
	cout << "1" << endl;
	for (int i = 0; i < n; i++)
		cout << kombinacje[numer][i].id + 2 << endl;

	for (int i = 0; i < n; i++)
		delete[]kombinacje[i];
	delete[]kombinacje;
	cout << endl;
	cout << "Laczna droga dla wybranej kombinacji wynosi = ";
	cout << najkrotszaDroga;
	cout << endl;
	return 0;
}

