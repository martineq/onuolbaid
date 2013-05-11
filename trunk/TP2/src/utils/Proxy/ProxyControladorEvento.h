#pragma once

#include "../Sockets/SocketCliente.h"
#include "../Sockets/SocketServidor.h"

class ProxyControladorEvento{

	public:
		struct stEvento{
			// Info para el proxy
			int id;
			bool errorEnSocket; // No hace falta serializar este valor. Esta atributo lo setea el proxy cuando recibe datos (al hidratar). Si el socket me da errror seteo ac� en true, sino lo dejo en false
			
			// Datos para actualizar en el evento
			int mouseX;
			int mouseY;
			bool teclaA;
			bool teclaS;
			bool mouseClickIzquierdo;
			bool mouseDentroPantalla;	// TODO: Confirmar si se puede borrar, o si todavia hace falta
			bool finalizoElJuego;
	};

	private:
		SocketCliente* pCliente;
		SocketServidor* pServidor;
	
	public:
		ProxyControladorEvento(void);
		~ProxyControladorEvento(void);

		// Para setear al principio
		void setSocketCliente(SocketCliente* pCliente);
		void setSocketServidor(SocketServidor* pServidor);

		// Para usar en el momento de juego. Debe estar seteado el modo Masivo
		bool enviarEvento(ProxyControladorEvento::stEvento entidad);			// Lo usa el lado Cliente
		bool recibirEvento(ProxyControladorEvento::stEvento& evento);			// Lo usa el lado Servidor

		// Auxiliares
		static void cargarStEvento(ProxyControladorEvento::stEvento& evento,int id,bool errorEnSocket,int mouseX,int mouseY,bool teclaA,bool teclaS,bool mouseClickIzquierdo,bool mouseDentroPantalla,bool finalizoElJuego);
		
};