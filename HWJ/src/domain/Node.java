package domain;

public interface Node {
	Node getSx(); //null se non esiste il figlio sinistro
	Node getDx(); //null se non esiste il figlio destro
	int getValue(); //restituisce un intero associato al nodo
}
