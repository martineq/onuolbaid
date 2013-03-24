#pragma once

#include <fstream>
#include <list>
#include <yaml-cpp/yaml.h>
#include "../Constantes.h"
#include "../Log/Log.h"

class ParserYaml{

	public:
		
		///////////////////////
		// Structs del juego //
		///////////////////////

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
			std::string imagen;
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
		};

		struct stEscenario{
			std::string nombre;
			int tamanioX;
			int tamanioY;
			std::list <stEntidadDefinida> entidadesDefinidas;
			std::list <stProtagonista> protagonistas;
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

		// Funciones privadas para la carga de datos parseados
		void cargaStJuego(YAML::Node& nodoRaiz, ParserYaml::stJuego& juego);
		void cargaStPantalla(const YAML::Node& nodo, stPantalla& pantalla);
		void cargaStConfiguracion(const YAML::Node& nodo, ParserYaml::stConfiguracion& configuracion);
		void cargaListaEntidades(const YAML::Node& nodo, std::list <ParserYaml::stEntidad>& entidades);
		void cargaListaEscenarios(const YAML::Node& nodo, std::list <ParserYaml::stEscenario>& escenarios);
		void cargaListaProtagonistas(const YAML::Node& nodo, std::list <ParserYaml::stProtagonista>& protagonistas);
		void cargaListaEntidadesDefinidas(const YAML::Node& nodo, std::list <ParserYaml::stEntidadDefinida>& entidadesDefinidas);
		void cargaStEntidad(const YAML::Node& nodo, ParserYaml::stEntidad& entidad);
		void cargaDefaultStEntidad(ParserYaml::stEntidad& entidad);
		void cargaStEscenario(const YAML::Node& nodo, ParserYaml::stEscenario& escenario);
		void cargaDefaultStEscenario(ParserYaml::stEscenario& escenario);
		void cargaStEntidadDefinida(const YAML::Node& nodo, ParserYaml::stEntidadDefinida& entidadDefinida);

		// Funciones de validación
		void validacionDeDatos(void);
		void validaPantalla(void);
		void validaConfiguracion(void);
		void validaEntidades(void);
		void validaEscenarios(void);
		void validaRecorrerListaEntidades(std::list<std::list<ParserYaml::stEntidad>::iterator>& tipoEntidadABorrar);
		void validaDescartarEntidades(std::list<std::list<ParserYaml::stEntidad>::iterator>& tipoEntidadABorrar);
		void validaListaEntidadesVacia(void);
		bool validaEsEntidadRepetida(std::string nombreEntidad);
		bool validaListaAnimaciones(std::list<std::string> listaAnimaciones);
		void validaRecorrerListaEscenarios(std::list<std::list<ParserYaml::stEscenario>::iterator>& tipoEscenarioABorrar);
		void validaDescartarEscenarios(std::list<std::list<ParserYaml::stEscenario>::iterator>& tipoEscenarioABorrar);
		void validaListaEscenariosVacia(void);
		void validaListaEntidadesDefinidas(std::list <ParserYaml::stEntidadDefinida>& entidadesDefinidas,std::string nombreEscenario, int tamanioX, int tamanioY);
		bool validaExisteEntidad(std::string entidad);
		bool validaListaProtagonistas(std::list <ParserYaml::stProtagonista>& protagonistas,std::string nombreEscenario, int tamanioX, int tamanioY);

		// Funciones privadas auxiliares
		int leerNodoYamlInt(const YAML::Node& nodo);
		std::string leerNodoYamlString(const YAML::Node& nodo);
		bool chequeoArchivo(std::string rutaArchivo);
		void notificarErrorLectura(std::string tipoDato,std::string archivo, int linea, std::string msgError, bool& lecturaOk);

	public:

		ParserYaml(void);
		~ParserYaml(void);
		ParserYaml::stJuego cargarConfiguracionDeJuego(void);

};

