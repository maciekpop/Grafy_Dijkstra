#include <iostream>
#include <fstream>
#include <chrono>
#define VERTICES 100							// ustalanie liczby wierzcholkow w grafie
#define INF  std::numeric_limits<int>::max()

using namespace std;

ofstream zapis("100_50_macierz.txt");

//**************************************************************************
// Lista jednokierunkowa (do list sasiedztwa)							   *
//**************************************************************************

template<typename T>
class LinkedList {

	// deklaracja klasy wewnetrznej w private
	class Node {
		int index;
		T elem;
		Node* next;
		friend class Iterator;
		template<typename T> friend class LinkedList;
	};

public:

	// deklaracja klasy wewnetrznej w public
	class Iterator {
		Node* v;
		Iterator(Node* u) { v = u; }
	public:
		int& operator*() { return v->index; }
		bool operator==(const Iterator& p) const { return v == p.v; }
		bool operator!=(const Iterator& p) const { return v != p.v; }
		Iterator& operator++() { v = v->next; return *this; }
		template <typename T> friend class LinkedList;
	};

	LinkedList();
	bool empty() const;
	const T& frontDist() const { return (head->elem); }
	const int& frontIndex() const { return (head->index); }
	void addFront(const T& e, const int& i);
	void removeFront();
	Iterator begin() const { return Iterator(head); }


private:
	Node* head;
	int n;  // ilosc elementow listy
};

template<typename T>
LinkedList<T>::LinkedList() {
	n = 0;
	head = new Node;
	head->next = NULL;
}

template<typename T>
bool LinkedList<T>::empty() const {
	return (n == 0);
}

template<typename T>
void LinkedList<T>::addFront(const T& e, const int& i) {
	Node* v = new Node;
	v->elem = e;
	v->index = i;
	v->next = head;
	head = v;
	n++;
}

template<typename T>
void LinkedList<T>::removeFront() {
	Node* old = head;
	head = old->next;
	delete old;
	n--;
}

//**************************************************************************
// Lista dwukierunkowa                                                     *
//**************************************************************************

template <typename T>
class DLinkedList {
private:

	class DNode {
		T elem;
		DNode* prev;
		DNode* next;	
		friend class Iterator;
		template <typename T> friend class DLinkedList;
	};

public:

	class Iterator {
		DNode* v;
		Iterator(DNode* u) { v = u; }
	public:
		T& operator*() { return v->elem; }
		bool operator==(const Iterator& p) const { return v == p.v; }
		bool operator!=(const Iterator& p) const { return v != p.v; }
		Iterator& operator++() { v = v->next; return *this; }
		Iterator& operator--() { v = v->prev; return *this; }
		template <typename T> friend class DLinkedList;
	};

	DLinkedList();
	bool empty() const;
	Iterator begin() const { return Iterator(header->next); }
	Iterator end() const { return Iterator(trailer->prev); }
	void add(const Iterator& p, const T& e);
	void addFront(const T& e);	
	void addBack(const T& e);
	void remove(const Iterator& p);
	void removeFront();
	void removeBack();
	void swap(const Iterator& p, const Iterator& q);
	const T& front() const { return (header->next->elem); }
	const T& back() const { return (trailer->prev->elem); }
	const int size() const { return n; }
	void print();

private:
	DNode* header;
	DNode* trailer;
	int n;           // liczba elementow
};

template <typename T>
DLinkedList<T>::DLinkedList() {
	header = new DNode;
	trailer = new DNode;
	header->next = trailer;
	trailer->prev = header;
	n = 0;
}

template <typename T>
bool DLinkedList<T>::empty() const {
	return (header->next == trailer);
}

template <typename T>
void DLinkedList<T>::add(const Iterator& p, const T& e) {
	DNode* w = p.v;
	DNode* u = w->prev;
	DNode* v = new DNode;
	v->elem = e;
	v->next = w;
	w->prev = v;
	v->prev = u;
	u->next = v;
	n++;
}

template <typename T>
void DLinkedList<T>::addFront(const T& e) {
	DNode* v = new DNode;
	DNode* w = header->next;
	v->elem = e;
	v->next = w;
	w->prev = v;
	header->next = v;
	v->prev = header;
	
	n++;
}

template <typename T>
void DLinkedList<T>::addBack(const T& e) {
	DNode* v = new DNode;
	DNode* w = trailer->prev;
	v->elem = e;
	v->prev = w;
	w->next = v;
	trailer->prev = v;
	v->next = trailer;
	
	n++;
}

template <typename T>
void DLinkedList<T>::remove(const Iterator& p) {
	DNode* v = p.v;
	DNode* w = v->next;
	DNode* u = v->prev;
	u->next = w;
	w->prev = u;
	
	delete v;
	n--;
}

template <typename T>
void DLinkedList<T>::removeFront() {
	remove(begin());
}

template <typename T>
void DLinkedList<T>::removeBack() {
	remove(--end());
}

template <typename T>
void DLinkedList<T>::swap(const Iterator& p, const Iterator& q) {
	DNode* w = p.v;
	DNode* v = q.v;

	T temp = w->elem;
	w->elem = v->elem;
	v->elem = temp;
}

template <typename T>
void  DLinkedList<T>::print() {
	DLinkedList<T>::Iterator iter(begin());
	while (iter != trailer) {
		zapis << *iter << ' ';
		++iter;
	}
}

//**************************************************************************
// Kolejka z kopca												           *
//**************************************************************************

template <typename T>
class HeapPriorityQueue {
	DLinkedList<T> tree;
	void heapify();
public:
	bool empty() const { return size() == 0; }
	const T& min();
	void removeMin();
	void insert(const T& e);
	const int size() const { return tree.size(); }
};

template <typename T>
const T& HeapPriorityQueue<T>::min() {
	if (!empty())
		return tree.front();
}

template <typename T>
void HeapPriorityQueue<T>::heapify() {
	typename DLinkedList<T>::Iterator iter1(tree.end());
	typename DLinkedList<T>::Iterator iter2(tree.end());
	int child = size() - 1;
	int parent = (child - 1) / 2;
	
	while (iter1 != tree.begin()) {   // dopoki dziecko nie jest korzeniem
		while (child != parent) {
			--iter2;					// dostep do rodzica
			child--;
		}
		
		if (*iter1 < *iter2)	         // jesli dziecko mniejsze od rodzica
			tree.swap(iter1, iter2);	// zamiana wartosci

		else break;                    // gdy dziecko jest wieksze od rodzica petla przerwana
		
		parent = (child - 1) / 2;      // przejscie do kolejnego rodzica glebiej
		iter1 = iter2;
	}
}

template <typename T>
void HeapPriorityQueue<T>::insert(const T& e) {
	tree.addBack(e);
	if (size() > 1)
		heapify();

}

template <typename T>
void HeapPriorityQueue<T>::removeMin() {
	if (!empty()) {
		tree.removeFront();
		if (size() > 1)
			heapify();
	}
}

//**************************************************************************
// Wierzcholek														       *
//**************************************************************************

template <typename T>
class Vertex {

	int index;
	T distance;

public:

	int& operator*() { return index; }
	bool operator<(const Vertex& V) const { return distance < V.distance; }
	friend ostream& operator<<(ostream& str, const Vertex& V) { return str << V.distance; }
	template <typename T> friend class GrafMacierz;
	template <typename T> friend class GrafLista;
};

//**************************************************************************
// Graf macierz sasiedztwa                                                 *
//**************************************************************************

template <typename T>
class GrafMacierz {
	T** matrix = new T*[VERTICES];									// macierz sąsiedztwa
	Vertex<T>* vertArr = new Vertex<T>[VERTICES];					// tablica wierzcholkow
	HeapPriorityQueue<Vertex<T>> Queue;
	int* path = new int[VERTICES];

public:

	GrafMacierz();
	~GrafMacierz();
	void insertEdge(int V, int W, T value);
	bool areAdjacent(int indexV, int indexW) { return (matrix[indexV][indexW] != INF); }
	void dijkstra(const int& indexStart);
	void printPath(const int& indexStart);

};

template <typename T>
GrafMacierz<T>::GrafMacierz() {

	for (int i = 0; i < VERTICES; i++) {
		matrix[i] = new T[VERTICES];			 // macierz sasiedztwa nie ma jeszcze polaczen
		for (int j = 0; j < VERTICES; j++)    // kazda mozliwa krawedz ma wartosc max dla typu T
			matrix[i][j] = INF;
	}

	for (int i = 0; i < VERTICES; i++) {			// nadanie wszystkim wierzcholkom wartosci max dla typu T
		vertArr[i].index = i;
		vertArr[i].distance = INF;
	}
}

template <typename T>
GrafMacierz<T>::~GrafMacierz() {
	for (int i = 0; i < VERTICES; i++)
		delete[] matrix[i];
	delete[] matrix;
	delete[] vertArr;
	delete[] path;
}

template <typename T>
void GrafMacierz<T>::insertEdge(int V, int W, T value) {
		matrix[V][W] = value;
		matrix[W][V] = value;
}

template <typename T>
void GrafMacierz<T>::dijkstra(const int& indexStart) {
	vertArr[indexStart].distance = 0;
	Vertex<T> Z;
	Queue.insert(vertArr[indexStart]);


	while (!Queue.empty()) {
		int i = Queue.min().index;
		Z = Queue.min();
		Queue.removeMin();

		for (int j = 0; j < VERTICES; j++) {
			if (areAdjacent(i, j)) {
				if (vertArr[j].distance > matrix[i][j] + Z.distance) {
					vertArr[j].distance = matrix[i][j] + Z.distance;
					Queue.insert(vertArr[j]);
					path[j] = i;
				}
			}
		}
	}
	
}

template<typename T>
void GrafMacierz<T>::printPath(const int& indexStart) {
	DLinkedList<int> L;
	zapis << indexStart << endl;
	for (int i = 0; i < VERTICES; i++) {
		zapis << vertArr[i] << ' ';
		int j = i;
		L.addFront(j);
		while (j != indexStart) {	
			j = path[j];
			L.addFront(j);
		}
		L.print();
		zapis << endl;
		while (!L.empty()) L.removeFront();
	}
}

//**************************************************************************
// Graf lista sasiedztwa												   *
//**************************************************************************

template <typename T>
class GrafLista {
	LinkedList<T>* adList = new LinkedList<T>[VERTICES];			// listy sasiedztwa
	Vertex<T>* vertArr = new Vertex<T>[VERTICES];
	HeapPriorityQueue<Vertex<T>> Queue;
	int* path = new int[VERTICES];

public:

	GrafLista();
	~GrafLista();
	void insertEdge(int indexV, int indexW, int value);
	void dijkstra(const int& indexStart);
	void printPath(const int& indexStart);
};

template <typename T>
GrafLista<T>::GrafLista() {
	for (int i = 0; i < VERTICES; i++) {
		vertArr[i].index = i;
		vertArr[i].distance = INF;
	}
}

template <typename T>
GrafLista<T>::~GrafLista() {
	delete[] adList;
	delete[] vertArr;
	delete[] path;
}

template <typename T>
void GrafLista<T>::insertEdge(int indexV, int indexW, int value) {
	adList[indexV].addFront(value, indexW);
	adList[indexW].addFront(value, indexV);
}

template <typename T>
void GrafLista<T>::dijkstra(const int& indexStart) {

	LinkedList<T>* adjacency = adList;   // kopia listy sasiedztwa
	Vertex<T> Z;
	int i, j;

	vertArr[indexStart].distance = 0;      // wierzcholek startowy, odleglosc 0
	Queue.insert(vertArr[indexStart]);     // wierzcholek startowy do kolejki

	while (!Queue.empty()) {               // dopoki w kolejce sa elementy

		i = Queue.min().index;				// pobieranie polozenia najmniejszego elementu
		Z = Queue.min();					// pierwszy wierzcholek kolejki do zmiennej pomocniczej

		Queue.removeMin();					// usuniecie pierwszego wierzcholka kolejki
											// ktory posiada najmniejsza odleglosc
		while(!adjacency[i].empty()) {		// dopoki wszystkie krawedzie dochodzace do danego wierzcholka nie zostana przebadane
			j = adjacency[i].frontIndex();	// pobieranie polozenia sasiedniego wierzcholka, 
											// ktory jest pierwszy na liscie sasiedztwa danego wierzcholka	

			if (vertArr[j].distance > adjacency[i].frontDist() + Z.distance) {	// relaksacja krawedzi
				vertArr[j].distance = adjacency[i].frontDist() + Z.distance;
				Queue.insert(vertArr[j]);										// dodanie zaktualizowanego wierzcholka do kolejki
				path[j] = i;													// dodanie wierzcholka ktory jest najblizej
			}
			adjacency[i].removeFront();		// usuniecie pierwszego elementu z kopii listy sasiedztwa (dostep do kolejnych wierzcholkow sasiednich)
		}
	}
}

template<typename T>
void GrafLista<T>::printPath(const int& indexStart) {
	DLinkedList<int> L;									// lista do gromadzenia sciezki
	zapis << indexStart << endl;
	for (int i = 0; i < VERTICES; i++) {
		zapis << vertArr[i] << ' ';
		int j = i;
		L.addFront(j);
		while (j != indexStart) {
			j = path[j];								// przejscie do poprzedniego, najblizszego wierzcholka
			L.addFront(j);
		}
		L.print();
		zapis << endl;
		while (!L.empty()) L.removeFront();
	}
}

int main()
{
	ifstream plik;
	int iloscKrawedzi, iloscWierzcholkow, wierzcholekStartowy;
	int source, end, value;
	plik.open("100_50.txt");

	if (plik.good() == true) {
		plik >> iloscKrawedzi >> iloscWierzcholkow >> wierzcholekStartowy;
		//GrafLista<int>* G = new GrafLista<int>[100];
		GrafMacierz<int>* G = new GrafMacierz<int>[100];
		int j = 0;
		while (!plik.eof()) {

			for (int i = 0; i < iloscKrawedzi; i++) {
				plik >> source >> end >> value;
				G[j].insertEdge(source, end, value);
			}
			j++;
			cout << j << endl;
		}
		plik.close();
		
		auto start = chrono::steady_clock::now();
		for (j = 0; j < 100; j++) 
			G[j].dijkstra(wierzcholekStartowy);
		auto end = chrono::steady_clock::now();

		cout << "Elapsed time in milliseconds : "
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		cout << "Elapsed time in milliseconds : "
			<< chrono::duration_cast<chrono::seconds>(end - start).count()
			<< " s" << endl;

		for (j = 0; j < 20; j++)
			G[j].printPath(wierzcholekStartowy);

		delete[] G;

	}
	else plik.close();
	zapis.close();
}