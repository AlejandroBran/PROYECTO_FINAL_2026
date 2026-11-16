#include "mensajes.h"

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Crea un nuevo nodo de mensaje y le agrega la fecha y hora actual
std::unique_ptr<Nodo> CrearMensaje(int id_emisor, const std::string& texto) {
  auto nuevo_nodo = std::make_unique<Nodo>();
  nuevo_nodo->dato.id_emisor = id_emisor;
  nuevo_nodo->dato.texto = texto;

  // Obtiene la hora del reloj de la computadora
  std::time_t tiempo_actual = std::time(nullptr);
  std::tm* fecha_desglosada = std::localtime(&tiempo_actual);

  char fecha_hora_buffer[30];

  if (fecha_desglosada == nullptr) {
    nuevo_nodo->dato.fecha_hora = "+-Fecha no disponible-+";
  } else {
    // Da formato dia/mes/año y hora:minutos:segundos
    std::size_t formateado_ok = std::strftime(
        fecha_hora_buffer, sizeof(fecha_hora_buffer), "%d/%m/%Y %H:%M:%S",
        fecha_desglosada);

    if (formateado_ok == 0) {
      nuevo_nodo->dato.fecha_hora = "+-Fecha no disponible-+";
    } else {
      nuevo_nodo->dato.fecha_hora = fecha_hora_buffer;
    }
  }

  nuevo_nodo->siguiente = nullptr;
  return nuevo_nodo;
}

// Agrega el nuevo mensaje al final de la lista enlazada
void AlmacenarEnLista(std::unique_ptr<Nodo>& cabeza,
                      std::unique_ptr<Nodo> nuevo_nodo) {
  if (!nuevo_nodo) return;

  // Si la lista esta vacia, este mensaje pasa a ser el primero
  if (!cabeza) {
    cabeza = std::move(nuevo_nodo);
  } else {
    // Recorre la lista hasta encontrar el ultimo nodo
    Nodo* temporal = cabeza.get();
    while (temporal->siguiente) {
      temporal = temporal->siguiente.get();
    }
    temporal->siguiente = std::move(nuevo_nodo);
  }
}

// Recorre la lista desde el inicio y muestra cada mensaje en pantalla
void RecorrerLista(const Nodo* cabeza) {
  if (cabeza == nullptr) {
    std::cout << "   (No hay mensajes registrados en esta conversacion)\n";
    return;
  }

  const Nodo* actual = cabeza;
  int contador = 1;
  while (actual != nullptr) {
    std::cout << "   [" << contador << "] Usuario ID "
              << actual->dato.id_emisor << ": \"" << actual->dato.texto << "\"\n"
              << "       Enviado: " << actual->dato.fecha_hora << "\n";
    actual = actual->siguiente.get();
    contador++;
  }
}

// Borra el ultimo mensaje de la lista y libera su memoria
bool EliminarUltimoMensaje(std::unique_ptr<Nodo>& cabeza) {
  if (!cabeza) {
    return false;
  }

  // Si solo existe un mensaje en la lista
  if (!cabeza->siguiente) {
    cabeza.reset();
    return true;
  }

  // Avanza hasta llegar al penultimo mensaje
  Nodo* temporal = cabeza.get();
  while (temporal->siguiente->siguiente) {
    temporal = temporal->siguiente.get();
  }

  temporal->siguiente.reset();
  return true;
}

// Vacia toda la lista nodo por nodo de forma segura
void LimpiarLista(std::unique_ptr<Nodo>& cabeza) {
  while (cabeza) {
    cabeza = std::move(cabeza->siguiente);
  }
}
