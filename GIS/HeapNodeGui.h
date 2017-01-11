#pragma once
#include "SFML.h"
#include "FibonacciHeap.h"

//Klasa odpowiedzialna za wyglad graficzny wezla
class HeapNodeGui
{
public:
	//Konstruktor
	HeapNodeGui(Vector2u tileSize, Font& font);

	//Ustaw wskaznik na wezel kopca
	void setNode(NodePtr ptr);

	//Narysuj korzystajac z danych kopca i okna
	void draw(RenderWindow& window, FibonacciHeap& heap);

	//Ustaw pozycje
	void setPosition(Vector2f pos);

private:
	
	//Numer wezla
	Text m_NodeNumberSprite;

	//Okrag jako graficzna reprezentacja wezla
	CircleShape m_NodeSprite;

	//Pozycja (ekranowa) nie w macierzy rysowania
	Vector2f m_Position;

	//Rozmiar w pikselach
	Vector2u m_TileSize;

	//Czcionka do priorytetu
	Font m_Font;

	//priorytet wezla, tekst graficzny
	Text m_PriorityText;

	//Wskaznik na odpowiadajacy wezel w kopcu
	HeapNodePtr m_NodePtr;

	//Kolory: element minimalny, zwyczajny, aktywny
	static Color m_MinPtr;
	static Color m_Default;
	static Color m_Active;
};

