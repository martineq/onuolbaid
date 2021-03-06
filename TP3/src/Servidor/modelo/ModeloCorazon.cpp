#include "ModeloItem.h"

using namespace std;

bool ModeloCorazon::aplicar(ModeloJugador* jugador, ListaJugadores* listaJugadores, ListaJugadores* listaEnemigos, ListaJugadores* listaGolems) {
	jugador->recuperarVida(VIDA_RECUPERADA);
	return true;
}

ModeloCorazon::ModeloCorazon(int alto, int ancho, int velocidad, Posicion posicion, int altoNivel, int anchoNivel, int fps, ProxyModeloEntidad* proxyEntidad, int id, string nombreEntidad)
	: ModeloItem(alto, ancho, velocidad, posicion, altoNivel, anchoNivel, fps, proxyEntidad, id, nombreEntidad, TIPO_ITEM_CORAZON) {
}

ModeloCorazon::~ModeloCorazon() {
}
