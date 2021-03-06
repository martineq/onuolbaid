#pragma once

#include "./Servidor/Servidor.h"
#include "./Cliente/Cliente.h"
#include "./utils/Observador/Observador.h"
#include "./utils/Log/Log.h"

// Include para las pruebas. Para agregar pruebas nuevas, agregarlas en esa clase.
#include "./utils/Pruebas/Pruebas.h"

class Administrador{

	private:
		Servidor servidor;
		Cliente cliente;
		void vincularObservadores(void);	// Vincula todos los patron observer

	public:
		Administrador(void);
		~Administrador(void);
		bool iniciar(void);					// Inicia el servidor y cliente
		void loop(void);					// Inicia los loops de servidor y cliente

		// Solo para correr pruebas, no influye en el proyecto del juego
		void correrPruebas(void);

};
