#include "ModeloItem.h"

bool ModeloEscudo::aplicar(ModeloJugador* jugador, ListaJugadores* listaJugadores, ListaJugadores* listaEnemigos, ListaJugadores* listaGolems) {
	jugador->recuperarEscudo(ESCUDO_RECUPERADO);
	return true;
}

ModeloEscudo::ModeloEscudo(int alto, int ancho, int velocidad, Posicion posicion, int altoNivel, int anchoNivel, int fps, ProxyModeloEntidad* proxyEntidad, int id, std::string nombreEntidad)
	: ModeloItem(alto, ancho, velocidad, posicion, altoNivel, anchoNivel, fps, proxyEntidad, id, nombreEntidad, TIPO_ENTIDAD_ITEM) {
}

ModeloEscudo::~ModeloEscudo() {
}

bool ModeloEscudo::inmediato() {
	return true;
}
