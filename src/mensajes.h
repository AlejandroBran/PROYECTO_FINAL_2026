#ifndef MENSAJES_H_
#define MENSAJES_H_

#include <memory>
#include <string>

// Guarda los datos de un mensaje enviado en el chat
struct Mensaje {
  int id_emisor;
  std::string texto;
  std::string fecha_hora;
};

// Nodo de la lista enlazada de mensajes usando puntero inteligente
struct Nodo {
  Mensaje dato;
  std::unique_ptr<Nodo> siguiente;
};

// Funciones para crear, guardar, mostrar y borrar mensajes
std::unique_ptr<Nodo> CrearMensaje(int id_emisor, const std::string& texto);
void AlmacenarEnLista(std::unique_ptr<Nodo>& cabeza,
                      std::unique_ptr<Nodo> nuevo_nodo);
void RecorrerLista(const Nodo* cabeza);
bool EliminarUltimoMensaje(std::unique_ptr<Nodo>& cabeza);
void LimpiarLista(std::unique_ptr<Nodo>& cabeza);

#endif  // MENSAJES_H_
