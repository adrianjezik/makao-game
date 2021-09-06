/***************************************************
*												   *
*			  COPYRIGHT ADRIAN JEZIK			   *
*												   *
*												   *
*												   *
****************************************************/

#include <iostream>
#include <time.h>
#include <Windows.h>

using namespace std;


enum wartosci
{
	dwojka = 2, trojka,
	czworka, piatka, szostka,
	siodemka, osemka, dziewiatka,
	dzisiatka, jopek, dama,
	krol, as, joker
};
enum kolory
{
	pik,
	kier,
	trefl,
	karo
};

struct karta
{
	kolory kolor;
	wartosci wartosc;
	int pierwsza;
	int ostatnia;
	int na_stole; // Na stole w przypadku talii oznacza ze ta karta zostala zagrana; Na stole w przypadku reki oznacza ze trzeba ta karte z reki wyjebac 
	int ilosc_kart;
	int indeks_karty;
};

karta talia[56];
int licznik_koniec = 55;
int licznik_poczatek = 0;
string mapa_kolorow[5] = { "P","K","T","k","JR" };
string mapa_wartosci[14] = { "2","3","4","5", "6","7", "8","9", "10","J", "Q","K","A","JR" };
void wypelnij_talie()
{
	//wczytuje karty od dwojki do asa do tablicy 'talia'

	for (int i = 0; i < 4; i++) // petla odpowiedzialna za kolory
	{
		for (int j = 0; j <= 12; j++) // petla odpowiedzialna za wartosci kart od dwojki do asa
		{
			talia[((52 * i) / 4) + j].kolor = (enum kolory)i; //rzutuje na 'i' kolory - czyli zmieniam typ tak jakby
			talia[((52 * i) / 4) + j].wartosc = (enum wartosci)(dwojka + j); // tutaj robie to samo
		}
	}
	//karty od dwojki do asa zostaly juz wpisane, brakuje tylko jokerow ktore nie musza miec konkretnego koloru
	//standardowo w strukturze wartosci sa inicjalizowane jako 0, przynajmniej tak bylo w debugerze wiec nie 
	//trzeba dodawac oddzielnego koloru w petli dla jokerow
	for (int i = 0; i < 4; i++)
	{
		talia[52 + i].wartosc = joker;
	}

	talia->pierwsza = 0;
	talia->ostatnia = 55;
	talia->na_stole = 0;

}

void tasuj()

{
	int ilosc_tasowan = rand() % 56 + 1;
	

	int x, y;
	for (int i = 0; i < (56 + ilosc_tasowan); i++)
	{
		x = rand() % 56;
		y = rand() % 56;
		swap(talia[x], talia[y]);

	}
	for (int i = 0; i < 56; i++)
	{
		talia[i].indeks_karty = i;
	}
}



void rozdaj(karta reka_gracza[54], karta reka_bota[54])
{

	int licznik_gracz = 0, licznik_bot = 0;
	talia->pierwsza = 0;
	talia->ostatnia = 55;
	for (int i = 1; i < 13; i++) // i =1 bo pierwsza karta idzie na stol 
	{
		// te dwa bloki kodu sie powtarzaja - musze wrzucic je do funkcji
		if (i % 2 == 0)
		{
			reka_gracza[licznik_gracz].kolor = talia[i].kolor;
			reka_gracza[licznik_gracz].wartosc = talia[i].wartosc;
			reka_gracza[licznik_gracz].indeks_karty = talia[i].indeks_karty;
			talia->pierwsza++;
			reka_gracza->ostatnia = licznik_gracz;
			licznik_gracz++;
			reka_gracza->ilosc_kart++; // gracz dostal dodatkowa karte wiec trzeba zwiekszyc licznik posiadanych kart o 1 
		}
		else
		{
			reka_bota[licznik_bot].kolor = talia[i].kolor;
			reka_bota[licznik_bot].wartosc = talia[i].wartosc;
			reka_bota[licznik_gracz].indeks_karty = talia[i].indeks_karty;
			talia->pierwsza++;
			reka_bota->ostatnia = licznik_bot;
			licznik_bot++;
			reka_bota->ilosc_kart++; //gracz dostal dodatkowa karte wiec trzeba zwiekszyc licznik posiadanych kart o 1
		}
	}

}
void pobierz_karte(karta reka[54]) // Trzeba to poprawic; chodzi o to ze teraz pobiera na pa³e kazda dostepna karte - jest mozliwosc ze oboje beda mieli te same karty
{
	reka->ostatnia++; // pobrana karte bedziemy ustawiac na ostatnia pozycje wiec trzeba to zwiekszyc 
	talia->pierwsza++; // numer pierwszej dostepnej karty z talii do pobrania 
	reka[reka->ostatnia].kolor = talia[talia->pierwsza].kolor; // do ostatniej karty w rece przypisuje pierwsza dostepna z talii
	reka[reka->ostatnia].wartosc = talia[talia->pierwsza].wartosc;
	reka[reka->ostatnia].indeks_karty = talia[talia->pierwsza].indeks_karty;

	if (reka[reka->ostatnia].na_stole == 1)
	{
		reka[reka->ostatnia].na_stole = 0; // karty z podstawowej talii sie skonczyly to bierzemy z dolu (nie bylo to zabronione wiec mozna)
	}

	talia->pierwsza = (talia->pierwsza) % 56;
	if (talia->pierwsza == talia->ostatnia)
	{
		talia->ostatnia++;
		talia->ostatnia = (talia->ostatnia) % 56;
	}
	reka->ilosc_kart++;
}

void koryguj_po_zagraniu(karta reka[54]) // jak zagram karte to dalej pokazuje w cout karte ktora zagralem  
{
	int ilosc_kart = reka->ilosc_kart - 1;
	int ostatnia = reka->ostatnia - 1;
	//sortujemy karty; w rece jakby zagrane karty jeszcze sa ale nie s¹ one dostepne do zagrania 
	//- nie usuwam ich bo beda potem zastepowane przez kopie wartosci z oryginalnej talii
	for (int i = 0; i < reka->ilosc_kart; i++) {
		for (int j = 1; j < reka->ilosc_kart - i; j++) {
			if (reka[j - 1].na_stole > reka[j].na_stole) {
				//zamiana miejscami
				swap(reka[j - 1], reka[j]);
			}
		}
	}
	reka->ostatnia = ostatnia;
	reka->ilosc_kart = ilosc_kart;

}
void zagraj_karte(karta reka[54], bool gracz)
{
	int numer = -1;
	if (gracz == true)
	{

		cout << "Podaj numer karty ktora chcesz zagrac: ";
		cin >> numer;
		while (numer > reka->ilosc_kart || numer < 1)
		{
			cout << "Nie masz tyle kart - podaj liczbe calkowita z zakresu: " << 0 << ";" << reka->ilosc_kart << endl;
			cout << "Wpisz pozycje: ";
			cin >> numer;
		}
		numer = numer - 1;

	}
	else
	{
		numer = rand() % reka->ilosc_kart; // wylosuje liczbe od 0 do ilosc_kart - 1 
	}
	if (talia[talia->na_stole].wartosc == reka[reka->pierwsza + numer].wartosc || talia[talia->na_stole].kolor == reka[reka->pierwsza + numer].kolor)
	{
		talia->na_stole = reka[reka->pierwsza + numer].indeks_karty;
		reka[reka->pierwsza + numer].na_stole = 1;
		koryguj_po_zagraniu(reka);
	}
	else if (reka[reka->pierwsza + numer].wartosc == joker)
	{
		talia->na_stole = reka[reka->pierwsza + numer].indeks_karty;
		reka[reka->pierwsza + numer].na_stole = 1;
		koryguj_po_zagraniu(reka);
	}
	else if (talia[talia->na_stole].wartosc == joker)
	{
		talia->na_stole = reka[reka->pierwsza + numer].indeks_karty;
		reka[reka->pierwsza + numer].na_stole = 1;
		koryguj_po_zagraniu(reka);
	}
	else
	{
		pobierz_karte(reka);
		if (talia[talia->na_stole].wartosc == reka[reka->ostatnia].wartosc || talia[talia->na_stole].kolor == reka[reka->ostatnia].kolor)
		{
			talia->na_stole = reka[reka->ostatnia].indeks_karty;
			reka[reka->ostatnia].na_stole = 1;
			koryguj_po_zagraniu(reka);
		}
		else if (reka[reka->ostatnia].wartosc == joker)
		{
			talia->na_stole = reka[reka->ostatnia].indeks_karty;
			reka[reka->ostatnia].na_stole = 1;
			koryguj_po_zagraniu(reka);
		}
	}
}

void wyswietl_interfejs(karta reka_gracza[54], karta reka_bota[54])
{
	char akcja = 'x';
	bool kolej_gracza = true;
	for (;;)
	{
		cout << "Reka ( P - pik, K - kier, T - trefl, k - karo, JR - joker):" << endl;
		for (int i = (reka_gracza->pierwsza) % 54; i <= (reka_gracza->ostatnia) % 54; i++)
		{
			if (reka_gracza[i].wartosc != joker)
			{
				cout << mapa_kolorow[reka_gracza[i].kolor] << mapa_wartosci[reka_gracza[i].wartosc - 2] << "(" << i + 1 << ") ";
			}
			else
			{
				cout << mapa_wartosci[reka_gracza[i].wartosc - 2] << "(" << i + 1 << ") ";
			}
		}
		cout << endl << endl;;
		cout << "Odkryta karta: " << mapa_kolorow[talia[talia->na_stole].kolor] << mapa_wartosci[talia[talia->na_stole].wartosc - 2] << endl; // ostatnia karta na stole
		cout << "Liczba kart komputera: " << reka_bota->ilosc_kart << endl << endl; // << liczba_kart_komputera << endl; 
		cout << "Akcja (P - pobierz karte, G - zagraj karte): ";
		while (akcja != 'P' || akcja != 'G')
		{
			if (kolej_gracza)
			{
				cin >> akcja;
			}
			else
			{
				int losowa_akcja_indeks = rand() % 2;
				char akcja_bota[2] = { 'P','G' };
				akcja = akcja_bota[losowa_akcja_indeks];
			}
			if (akcja == 'P')
			{
				if (kolej_gracza) {
					pobierz_karte(reka_gracza);
				}
				else
				{
					pobierz_karte(reka_bota);
				}
				kolej_gracza = !kolej_gracza;
				break;
			}
			else if (akcja == 'G')
			{
				if (kolej_gracza) {
					zagraj_karte(reka_gracza, kolej_gracza);
				}
				else
				{
					zagraj_karte(reka_bota, kolej_gracza);
				}
				kolej_gracza = !kolej_gracza;
				break;
			}
			else
			{
				cout << endl << "Niepoprawna komenda, prosze wpisac 'P' lub 'G'" << endl;
			}
		}
		if (reka_gracza->ilosc_kart == 0)
		{
			cout << "Gratulacje Czlowiek wygral z botem !" << endl;
			exit(0);
		}
		else if (reka_bota->ilosc_kart == 0)
		{
			cout << "Niestety Bot wygral z czlowiekiem :(" << endl;
			exit(0);
		}
		system("cls");

	}
}

void wyswietl_karty(karta reka_gracza[54], karta reka_bota[54])
{
	for (int i = 0; i < 6; i++)
	{
		cout << reka_gracza[i].wartosc << " GRACZ";
		cout << endl;
	}
	cout << reka_gracza->ostatnia << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << reka_bota[i].wartosc << " BOT";
		cout << endl;
	}
	cout << reka_bota->ostatnia << endl;

} // do sprawdzania poprawnosci
int main()
{
	srand(time(NULL)); // ustawiam ziarno do losowania - tak jak generator mapy w minecrafcie 
	karta reka_gracza[54] = {}, reka_bota[54] = {};
	//karta stol[56];
	wypelnij_talie(); // wypelniamy tablice globalna 'talia' Globalna bo nie chcialem przekazywac non stop do funkcji tego samego argumentu
	tasuj(); // tasujemy zainicjalizowana talie
	rozdaj(reka_gracza, reka_bota); // rozdajemy po 6 kart graczom
	wyswietl_interfejs(reka_gracza, reka_bota); // wlasciwie to gra juz

	return 0;
}
