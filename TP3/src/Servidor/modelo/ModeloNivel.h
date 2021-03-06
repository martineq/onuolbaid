#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include "ModeloEntidad.h"
#include "ModeloJugador.h"
#include "ModeloItem.h"
#include "ModeloMision.h"
#include "../../utils/Observador/Identificable.h"
#include "../../utils/Hilos/Mutex.h"

class ModeloNivel {
	private:
		int altoTiles;
		int anchoTiles;

		ListaEntidades listaEntidades;
		ListaJugadores listaJugadores;
		ListaJugadores listaEnemigos;
		ListaJugadores listaGolems;
		ListaItems listaItems;

		int indiceEnemigo;
		int indiceGolem;

		ModeloMision* mision;		

		int jugadoresConectados;
		Mutex mutexJugadoresConectados;

		void actualizarJugadores(std::list<ModeloJugador*>* listaJugadores, std::list<ModeloJugador*>* listaEnemigos, std::list<ModeloJugador*>* listaGolems);
		void actualizarEnemigos(std::list<ModeloJugador*>* listaJugadores, std::list<ModeloJugador*>* listaEnemigos, std::list<ModeloJugador*>* listaGolems);
		void actualizarGolems(std::list<ModeloJugador*>* listaJugadores, std::list<ModeloJugador*>* listaEnemigos, std::list<ModeloJugador*>* listaGolems);
		void actualizarItems();
		void decrementarJugadores(void);

	public:
		ModeloNivel();
		virtual ~ModeloNivel();

		// Getters
		std::list<ModeloJugador*> getJugadores();
		std::list<ModeloJugador*> getEnemigos();
		std::multimap<std::pair<int, int>, ModeloItem*> getItems();

		int getAltoTiles();
		int getAnchoTiles();

		// Mision
		ModeloMision* getMision();
		bool chequearMision();

		// Setters
		void setAltoTiles(int alto);
		void setAnchoTiles(int ancho);

		void agregarJugador(ModeloJugador* jugador);
		void agregarEnemigo(ModeloJugador* enemigo);
		void agregarItem(ModeloItem* item);
		void agregarEntidad(ModeloEntidad* entidad);
		
		void activarBomba(int id);
		void activarHechizoHielo(int id);
		void activarGolem(int id);
		void ejecutarAccionJugador(int tileX, int tileY, int id);
		void desconectarJugador(int id);

		void setMision(ModeloMision*);				

		ModeloJugador* obtenerJugador(int id);
		ModeloJugador* obtenerJugador(Posicion posicion);
		ModeloJugador* obtenerEnemigo(Posicion posicion);
		ModeloJugador* obtenerGolem(Posicion posicion);
		ModeloItem* obtenerItem(Posicion posicion);

		bool actualizar();

		bool posicionOcupada(Posicion);

		bool chequearConexion();

		int cantidadJugadores(void);
		
		void incrementarJugadores(void);

		void iniciarNuevosJugadores(void);

		void destruirListas();

		void reset(void);
};
