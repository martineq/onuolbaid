#pragma once

#include <math.h>
#include <Windows.h>
#include <WinSock.h>
#include <iostream>
#include <map>

#include "../../utils/Constantes/Constantes.h"
#include "../../utils/Observador/Observable.h"
#include "../../utils/Observador/Observador.h"
#include "../../utils/Hilos/Mutex.h"
#include "../../utils/Proxy/ProxyModeloEntidad.h"
#include "Posicion.h"
#include "Tile.h"
#include "EstadoNivel.h"

typedef enum Direccion { NOROESTE, NORTE, NORESTE, ESTE, SUDESTE, SUR, SUDOESTE, OESTE };
typedef enum Accion { CAMINANDO, ATACANDO, DEFENDIENDO, QUIETO };

class ModeloEntidad {
	private:
		class ModeloMovimiento : public Observable {
			private:
				int _altoNivel;
				int _anchoNivel;
				ModeloEntidad* _modeloEntidad;
				std::list<ModeloEntidad*>* _jugadores;
				Mutex* _mutexJugadores;
				std::multimap<std::pair<int, int>, ModeloEntidad*>* _entidades;
				Mutex* _mutexEntidades;
				std::list<Posicion> _posiciones;
				DWORD _instanteUltimoCambioEstado;

				bool agregarTile(char* mapaTilesCerrados, std::list<Tile>* tilesAbiertos, Posicion posicion, Posicion posicionDestino, Tile* padre, int distancia);

				Posicion calcularPosicionDestino(Posicion posicionDestino);

				ModeloEntidad* detectarColision(Posicion posicion);

				int obtenerAlto(int y, ModeloEntidad* modeloEntidad);

				int obtenerAncho(int x, ModeloEntidad* modeloEntidad);

				Direccion obtenerDireccion(Posicion posicionOrigen, Posicion posicionDestino);

				Posicion obtenerPosicionSiguiente();

				int obtenerX(ModeloEntidad* modeloEntidad);

				int obtenerY(ModeloEntidad* modeloEntidad);

				ModeloMovimiento(const ModeloMovimiento &modeloMovimiento);

				ModeloMovimiento& operator=(const ModeloMovimiento &modeloMovimiento);

			public:
				ModeloMovimiento(int altoNivel, int anchoNivel, ModeloEntidad* modeloEntidad);

				virtual ~ModeloMovimiento();

				void actualizar(Posicion posicion);

				void asignarJugadores(Mutex* mutexJugadores, std::list<ModeloEntidad*>* jugadores);

				void asignarEntidades(Mutex* mutexEntidades, std::multimap<std::pair<int, int>, ModeloEntidad*>* entidades);

				void cambiarEstado();

				void detener();
		};

		class VistaMovimiento : public Observador {
			private:
				ModeloEntidad* _modeloEntidad;
				Posicion _posicionOrigen;
				Posicion _posicionDestino;
				int _altoMapa;
				int _anchoMapa;
				int _cantidadCuadros;
				int _desplazamiento;
				DWORD _espera;
				int _cuadroActual;
				std::list<Posicion> _posiciones;
				DWORD _instanteUltimoCambioEstado;
				
				VistaMovimiento(const VistaMovimiento &vistaMovimiento);

				VistaMovimiento& operator=(const VistaMovimiento &vistaMovimiento);

			public:
				VistaMovimiento(ModeloEntidad* modeloEntidad, int altoMapa, int anchoMapa, int fps);

				virtual ~VistaMovimiento();
				
				void actualizar(Observable* observable);

				void cambiarEstado();

				bool terminado() const;
		};

		Accion _accion;
		int _alto;
		int _altoNivel;
		int _ancho;
		int _anchoNivel;
		Direccion _direccion;
		bool _esJugador;
		bool _estaCongelado;
		EstadoNivel* _estadoNivel;
		int _id;
		ModeloMovimiento* _modeloMovimiento;
		Mutex _mutex;
		std::string _nombreEntidad;
		std::string _nombreJugador;
		Posicion _posicionActual;
		Posicion _posicionSiguiente;
		ProxyModeloEntidad* _proxyEntidad;
		int _velocidad;
		
		bool _esUltimoMovimiento;
		Posicion _pixelActual;
		Posicion _pixelSiguiente;
		VistaMovimiento* _vistaMovimiento;

		ModeloEntidad(const ModeloEntidad &modeloEntidad);

		ModeloEntidad& operator=(const ModeloEntidad &modeloEntidad);

	protected:
		void accion(Accion accion);
		
		void direccion(Direccion direccion);

		void esUltimoMovimiento(bool esUltimoMovimiento);

		void posicionActual(Posicion posicionActual);

		void posicionSiguiente(Posicion posicionSiguiente);

		void pixelActual(Posicion pixelActual);

		void pixelSiguiente(Posicion pixelSiguiente);
		
	public:
		ModeloEntidad(int alto, int ancho, int velocidad, Posicion posicion, bool esJugador, int altoNivel, int anchoNivel, int fps, ProxyModeloEntidad* proxyEntidad, int id, std::string nombreEntidad, std::string mote);

		virtual ~ModeloEntidad();

		Accion accion();

		int alto();

		int ancho();
		
		Direccion direccion();

		bool esJugador();

		bool estaCongelado();

		bool esUltimoMovimiento();

		int id();

		std::string nombreEntidad();

		std::string nombreJugador();

		Posicion posicionActual();

		Posicion posicionSiguiente();

		ProxyModeloEntidad::stEntidad stEntidad();
		
		int velocidad();

		void estaCongelado(bool estaCongelado);

		void asignarJugadores(Mutex* mutexJugadores, std::list<ModeloEntidad*>* jugadores);

		void asignarEntidades(Mutex* mutexEntidades, std::multimap<std::pair<int, int>, ModeloEntidad*>* entidades);

		void atacar();

		bool chequearConexion();

		void defender();

		void enviarMensaje(ModeloEntidad* remitente, std::string mensaje);

		void cambiarEstado();

		void mover(Posicion posicion);

		bool ocupaPosicion(Posicion posicion);

		Posicion pixelActual();

		Posicion pixelSiguiente();

		void notificar();
	
		void setNombreJugador(std::string nombre);

		void cargarMatriz(ProxyModeloEntidad::stEntidad& entidad);

		bool operator==(const ModeloEntidad &modeloEntidad) const;
};
