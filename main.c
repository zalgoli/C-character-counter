#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const char* karakterek = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Kulon fuggveny, mert a 'feldolgoz' kezdett tul hosszu lenni
FILE* beolvas(const char* file_nev)
{
	FILE* szovegem = fopen("szoveg.txt", "r");

	if (szovegem == NULL)
	{
		perror("Nem sikerult megnyitni a fajlt.\n");
		return NULL;
	}

	return szovegem;
}

// Kulon fuggveny, mert a 'feldolgoz' kezdett tul hosszu lenni
bool angol_abece(FILE* szovegem)
{
	bool van = false; // 10 percig gondolkoztam, miert nem tudok bool tipust irni.. stdbool.h :)
	char ch;

	rewind(szovegem);
	while ((ch = fgetc(szovegem)) != EOF) // A fajlban levo osszes karaktert
	{
		for (const char* p = karakterek; *p != '\0'; ++p)  // Osszehasonlitjuk az abece osszes karakterevel..
															//.. es a C string-ek vegen levo \0 karakter jelzi hogy a szoveg vegere ertunk
			if (ch == *p)
			{
				van = true;
				break;
			}
		if (van) break;
	}
	return van;
}

// Igyekeztem hangsulyt fektetni arra, hogy az 1. ures, 2. egysoros, 3. tobbsoros fajlokat is kezelje a program
char* feldolgoz(FILE* szovegem)
{
	fseek(szovegem, 0, SEEK_END); // Pointer a fajl vegere megy
	int file_size = ftell(szovegem); // Meghatarozzuk, hol van az, hogy "a fajl vege"
	rewind(szovegem); // A pointer-t visszaallitjuk a kezdoallasba

	// 1. Egyaltalan van-e barmi a fajlban
	{
		
		if (file_size == 0)
		{
			fclose(szovegem);
			perror("A fajl ures. Nincs szoveg.\n");
			fclose(szovegem);
			return NULL;
		}
	}

	// 2. Es ha van, az resze-e az angol ABC-nek
	{
		if (!angol_abece(szovegem))
		{
			perror("Nincs ertelmezheto karakter a szovegben.\n");
			fclose(szovegem);
			return NULL;
		}
	}


	// 3. Memoria allokacio
	char* buffer = malloc(file_size + 1);
	if (buffer == NULL)
	{
		perror("Nem sikerult lefoglalni a memoriat.\n");
		fclose(szovegem);
		return NULL;
	}

	// 4. Tenyleges beolvasas es \0 a vegere
	rewind(szovegem); // Kell egy extra rewind, kulonben a pointer onnantol olvassa a fajlt ahol az elso angol ABC karakter talalta
	fread(buffer, 1, file_size, szovegem);
	buffer[file_size] = '\0';

	return buffer;
}



void megszamol(FILE* szovegem, int char_count[52]) {
	rewind(szovegem); // Itt is vissza kell tenni a pointert az elejere
	for (int i = 0; i < 52; i++) {
		char_count[i] = 0;
	}

	char ch;
	while ((ch = fgetc(szovegem)) != EOF) {
		if (ch >= 'a' && ch <= 'z') {
			char_count[ch - 'a']++;
		}
		else if (ch >= 'A' && ch <= 'Z') {
			char_count[ch - 'A' + 26]++;
		}
	}
}

void kijelez(const int char_count[52]) {
	printf("Az alabbi karakterek fordulnak elo a szovegben:\n");
	for (int i = 0; i < 52; i++) {
		if (char_count[i] > 0) {
			if (i < 26) {
				printf("'%c' : %d\n", 'a' + i, char_count[i]);
			}
			else {
				printf("'%c' : %d\n", 'A' + (i - 26), char_count[i]);
			}
		}
	}
}

int main()
{
	FILE* szovegem = fopen("szoveg.txt", "r");
	if (!szovegem) return EXIT_FAILURE;
	
	char* tartalom = feldolgoz(szovegem);
	if (tartalom)
	{
		int karakterek_szama[52];
		megszamol(szovegem, karakterek_szama);
		fclose(szovegem);
		kijelez(karakterek_szama);
		free(tartalom);
	}

	
	return 0;
}