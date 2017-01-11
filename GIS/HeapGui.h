#pragma once
#include "SFML.h"
#include "FibonacciHeap.h"
#include "Links.h"
#include "Graph.h"
#include "HeapNodeGui.h"

typedef vector<HeapNodeId> Row;
typedef vector<Row> DrawMatrix;
typedef pair<VertexPtr, VertexPtr> VertexPair;
typedef vector<VertexPair> VertexPairs;

//Klasa obslugujaca interfejs graficzny
class HeapGui
{
public:
	//Konstruktor
	HeapGui(FibonacciHeap& heap, Graph& graph, Font& font);

	//Uruchomienie wyswietlania
	void showGui();

private:

	//Okno i widok
	RenderWindow m_Window;
	View m_ActiveView;

	//Kopiec Fibbonaciego oraz skojazony Graf
	FibonacciHeap m_Heap;
	Graph m_Graph;

	//Wyznacznik pustego kafelka
	static HeapNodeId m_EmptyTile;

	//Rozmiary okna i widoku
	unsigned m_WidthDefault = 800;
	unsigned m_HeightDefault = 600;
	unsigned m_ViewWidth = 800;
	unsigned m_ViewHeight = 600;

	//Licznik polaczen miedzy wezlami (id)
	LinkId m_NextFreeLinkId;

	//Wymiary kafelka
	Vector2u m_TileSize;

	//Maciez rysowania
	DrawMatrix m_DrawMatrix;

	//Czcionka
	Font m_Font;

	//Lacza miedzy wezlami
	DLinksParChild m_DLinksParChild;
	SLinksChildPar m_SLinksChildPar;
	DLinksSiblings m_DLinksSiblings;

	//Zapis i czyszczenie macierzy rysowania
	void writeToMatrix(HeapNodeId val, unsigned x, unsigned y);
	void clearMatrix();
	
	//Rekurencyjna funkcja dodajaca wezly do macierzy rysowania
	long addNodeToDrawMatrix(NodePtr node, DrawMatrix& m, unsigned x, unsigned y);

	//Rejestracja polaczen miedzy wezlami
	void registerDLinkSiblings(NodePtr& first, NodePtr& second);
	void registerSLinkParentChild(NodePtr& node);
	void registerDLinkParentChild(NodePtr& node);

	//Pomocnicza funkcja do okreslania przesuniecia miedzy rysowanymi wezlami
	long xOffset(unsigned x);

	//Zwraca wspolzedne centralnego punktu okna
	Vector2u getWindowCenter();

	//Reakcja na zdarzenia przyciskow klawiatury
	void processEvents();

	//Rysowanie kopca
	void drawHeap();

	//Rysuje pojedynczy krok, przedstawia zmiane w kopcu po dokonaniu na nim jakiejs operacji
	void drawStep();
};

