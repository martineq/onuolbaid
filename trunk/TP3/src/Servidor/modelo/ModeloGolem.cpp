#include "ModeloItem.h"

ModeloGolem::ModeloGolem(int alto, int ancho, int velocidad, Posicion posicion, int altoNivel, int anchoNivel, int fps, ProxyModeloEntidad* proxyEntidad, int id, std::string nombreEntidad)
	: ModeloItem(alto, ancho, velocidad, posicion, altoNivel, anchoNivel, fps, proxyEntidad, id, nombreEntidad, TIPO_ENTIDAD_ITEM) {

}

ModeloGolem::~ModeloGolem() {

}

bool ModeloGolem::inmediato() {
	return false;  // Confirmar si va false. (El Golem se invocar�a de la misma forma que se activa una bomba)
}

bool ModeloGolem::aplicar(ModeloJugador* jugador, ListaJugadores* listaJugadores, ListaJugadores* listaEnemigos, ListaJugadores* listaGolems) {
	
	// Cargo los datos que necesito del jugador que invoca el golem
	this->idDuenio = jugador->modeloEntidad()->id();
	this->velocidad =  jugador->modeloEntidad()->velocidad();
	this->vida = jugador->maximoVida();
	this->danio = (jugador->danioAtaque() / 2);			// El golem tendr� menos da�o de ataque que su creador
	this->pos = jugador->posicionGolem();
	
	ModeloJugador* golem = this->crearGolem();
	
	listaGolems->agregarJugador(golem);
	golem->asignarListaJugadores(listaJugadores);
	//TODO: Falta pasarle la lista de items para que el golem pueda dropear al morir
	golem->asignarListaEnemigos(listaEnemigos);
	golem->enviarEstado();
	
	return true;
}

// Lo usa el ModeloFactory para cargar los datos necesarios para la creaci�n del golem
void ModeloGolem::cargarDatos(int idGolem, int alto, int ancho, int fps, int anchoEscenario, int altoEscenario, void* pSocketServidor){

	this->idGolem = idGolem;
	this->alto = alto;
	this->ancho = ancho;
	this->fps = fps;
	this->anchoEscenario = anchoEscenario;
	this->altoEscenario = altoEscenario;
	this->pSocketServidor = pSocketServidor;
	this->nombreEntidad.assign(ENTIDAD_GOLEM);
	this->mana = 0;

	return void();
}

ModeloJugador* ModeloGolem::crearGolem(){

	// Creo el proxy para este enemigo
	ProxyModeloEntidad* pProxyEntidad = new ProxyModeloEntidad();
	pProxyEntidad->setSocketServidor(pSocketServidor);

	// Creo el enemigo (es un ModeloJugador) y lo agrego al nivel
	ModeloJugador* pGolem = new ModeloJugador(alto,ancho,velocidad,pos,altoEscenario,anchoEscenario,fps,pProxyEntidad,idGolem,nombreEntidad,"",vida,mana,danio,idDuenio,TIPO_ENTIDAD_GOLEM);
	pGolem->autonomo(true);

	return pGolem;
}
