#pragma once

#include <fstream>
#include <list>
#include <yaml-cpp/yaml.h>
#include "../Constantes/Constantes.h"
#include "../Log/Log.h"

class ParserYaml{

	public:
		
		///////////////////////
		// Structs del juego //
		///////////////////////

		struct stConexion{
			std::string ip;
			int puerto;
			bool conexionValida;
		};

		struct stPantalla{
			int ancho;
			int alto;
		};

		struct stConfiguracion{
			int velocidadPersonaje;
			int margenScroll;
		};

		struct stEntidad{
			std::string nombre;
			std::list<std::list<std::string>> imagenes;
			int anchoBase;
			int altoBase;
			int pixelReferenciaX;
			int pixelReferenciaY;
			int fps;
			int delay;
		};

		struct stEntidadDefinida{
			int x;
			int y;
			std::string entidad;
		};

		struct stProtagonista{
			int x;
			int y;
			std::string entidad;
			int vida;
			int mana;
			int danio;
			int velocidad;
		};

		struct stEnemigo{
			int x;
			int y;
			std::string entidad;
			int vida;
			int danio;
			int velocidad;
		};

		struct stItem{
			int x;
			int y;
			std::string entidad;
		};

		struct stEscenario{
			std::string nombre;
			int tamanioX;
			int tamanioY;
			std::list <stEntidadDefinida> entidadesDefinidas;
			std::list <stProtagonista> protagonistas;
			std::list <stEnemigo> enemigos;
			std::list <stItem> items;
		};

		struct stJuego{
			stPantalla pantalla;
			stConfiguracion configuracion;
			std::list <stEntidad> entidades;
			std::list <stEscenario> escenarios;
			bool juegoValido;
		};

	private:

		// Estructura para guardar todos los datos parseados
		stJuego juego;
		stConexion conexion;
		bool archivoJuegoYaLeido;
		bool archivoConexionYaLeido;

		// Funciones privadas para la carga de datos parseados
		void cargaStConexion(YAML::Node& nodoRaiz, ParserYaml::stConexion& conexion);
		void cargaStJuego(YAML::Node& nodoRaiz, ParserYaml::stJuego& juego);
		void cargaStPantalla(const YAML::Node& nodo, stPantalla& pantalla);
		void cargaStConfiguracion(const YAML::Node& nodo, ParserYaml::stConfiguracion& configuracion);
		void cargaListaEntidades(const YAML::Node& nodo, std::list <ParserYaml::stEntidad>& entidades);
		void cargaListaEscenarios(const YAML::Node& nodo, std::list <ParserYaml::stEscenario>& escenarios);
		void cargaListaProtagonistas(const YAML::Node& nodo, std::list <ParserYaml::stProtagonista>& protagonistas);
		void cargaListaEnemigos(const YAML::Node& nodo, std::list <ParserYaml::stEnemigo>& enemigos);
		void cargaListaEntidadesDefinidas(const YAML::Node& nodo, std::list <ParserYaml::stEntidadDefinida>& entidadesDefinidas);
		void cargaStEntidad(const YAML::Node& nodo, ParserYaml::stEntidad& entidad);
		void cargaListaImagenes(const YAML::Node& nodo, std::list<std::list<std::string>>& imagenes);
		void cargaDefaultStEntidad(ParserYaml::stEntidad& entidad);
		void cargaStEscenario(const YAML::Node& nodo, ParserYaml::stEscenario& escenario);
		void cargaDefaultStEscenario(ParserYaml::stEscenario& escenario);
		void cargaStEntidadDefinida(const YAML::Node& nodo, ParserYaml::stEntidadDefinida& entidadDefinida);
		void cargaListaItems(const YAML::Node& nodo, std::list <ParserYaml::stItem>& listaItems);
		void cargaStItem(const YAML::Node& nodo, ParserYaml::stItem& item);

		// Funciones de validaci�n
		void validacionDeDatos(void);
		void validaPantalla(void);
		void validaConfiguracion(void);
		void validaEntidades(void);
		void validaEscenarios(void);
		void validaRecorrerListaEntidades(std::list<std::list<ParserYaml::stEntidad>::iterator>& tipoEntidadABorrar);
		void validaDescartarEntidades(std::list<std::list<ParserYaml::stEntidad>::iterator>& tipoEntidadABorrar);
		void validaListaEntidadesVacia(void);
		bool validaEsEntidadRepetida(std::list<stEntidad>::iterator iterador, std::list<std::list<stEntidad>::iterator>& tipoEntidadABorrar);
		int validaCantidadVecesEnEntidadABorrar(std::list<stEntidad>::iterator iteradorBuscado, std::list<std::list<stEntidad>::iterator>& tipoEntidadABorrar);
		bool validaListaImagenes(std::list<std::list<std::string>>& listaImagenes, std::string nombreEntidad);
		void validaRecorrerListaEscenarios(std::list<std::list<ParserYaml::stEscenario>::iterator>& tipoEscenarioABorrar);
		void validaDescartarEscenarios(std::list<std::list<ParserYaml::stEscenario>::iterator>& tipoEscenarioABorrar);
		void validaListaEscenariosVacia(void);
		void validaListaEntidadesDefinidas(std::list <ParserYaml::stEntidadDefinida>& entidadesDefinidas,std::string nombreEscenario, int tamanioX, int tamanioY);
		bool validaExisteEntidad(std::string entidad);
		bool validaListaProtagonistas(std::list <ParserYaml::stProtagonista>& protagonistas,std::string nombreEscenario, int tamanioX, int tamanioY);
		bool validaListaEnemigos(std::list <ParserYaml::stEnemigo>& enemigos,std::string nombreEscenario, int tamanioX, int tamanioY);
		void validaListaItems(std::list <ParserYaml::stItem>& listaItems, std::string nombreEscenario, int tamanioX, int tamanioY);

		// Funciones privadas auxiliares
		int leerNodoYamlInt(const YAML::Node& nodo);
		std::string leerNodoYamlString(const YAML::Node& nodo);
		bool chequeoArchivo(std::string rutaArchivo);
		void notificarErrorLectura(std::string tipoDato,std::string archivo, int linea, std::string msgError, bool& lecturaOk);
		int cantidadDeAnimacionesDeEntidad(std::string entidad);
		bool entidadDefaultCargada(void);
		stProtagonista crearJugadorDefault(void);
		stEnemigo crearEnemigoDefault(void);
		void cargaListasAnimacionesDefault(std::list<std::list<std::string>>& listaAnimaciones);
		void cargaListaImagenesDefault(std::list<std::string>& listaImagenes, int sufijo);
		stJuego crearJuegoDefault(void);

	private:
		// Los constructores permanecen privados, para evitar que sea instanciado multiples veces
		// permitiendo que el archivo YAML se le�do una sola vez
		ParserYaml();
		ParserYaml(const ParserYaml&);
		ParserYaml& operator =(const ParserYaml&);

	public:

		static ParserYaml& getInstance(); // Aplico Singleton
		ParserYaml::stJuego cargarConfiguracionDeJuego(void);
		ParserYaml::stConexion cargarConfiguracionDeConexion(void);
		ParserYaml::stEntidad buscarStEntidad(std::list<ParserYaml::stEntidad>& listaEntidades, std::string nombreEntidad);
		ParserYaml::stProtagonista buscarStProtagonista(ParserYaml::stEscenario escenario,std::string nombreProtagonista);
		
		virtual ~ParserYaml();
};

// La declaracion de la obtencion de instancia se hace en el mismo .h
inline ParserYaml& ParserYaml::getInstance(){
	static ParserYaml objecto;
	return objecto;
}
