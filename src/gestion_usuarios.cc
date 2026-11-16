#include "gestion_usuarios.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Deja todas las cubetas de la tabla hash vacias al iniciar
TablaHashUsuarios::TablaHashUsuarios() {
  for (int i = 0; i < kCapacidadTabla; ++i) {
    tabla_[i] = nullptr;
  }
}

// Libera la memoria de todos los usuarios y sus mensajes al cerrar el programa
TablaHashUsuarios::~TablaHashUsuarios() {
  for (int i = 0; i < kCapacidadTabla; ++i) {
    while (tabla_[i]) {
      LimpiarLista(tabla_[i]->usuario.historial);
      tabla_[i] = std::move(tabla_[i]->siguiente);
    }
  }
}

// Convierte el ID de texto en un indice valido dentro del arreglo de la tabla
int TablaHashUsuarios::CalcularHash(const std::string& clave) const {
  unsigned long hash = 5381;
  for (char c : clave) {
    hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
  }
  return static_cast<int>(hash % kCapacidadTabla);
}

// Guarda un nuevo usuario en la tabla si su ID no esta repetido
bool TablaHashUsuarios::RegistrarUsuario(const std::string& id,
                                          const std::string& nombre,
                                          int codigo_num) {
  if (id.empty() || ExisteUsuario(id)) {
    return false;
  }

  int indice = CalcularHash(id);

  auto nuevo_nodo = std::make_unique<NodoUsuario>();
  nuevo_nodo->usuario.id_usuario = id;
  nuevo_nodo->usuario.nombre_alias = nombre;
  nuevo_nodo->usuario.codigo_numerico = codigo_num;
  nuevo_nodo->usuario.historial = nullptr;

  // Inserta el usuario al principio de la cubeta correspondiente
  nuevo_nodo->siguiente = std::move(tabla_[indice]);
  tabla_[indice] = std::move(nuevo_nodo);

  return true;
}

// Cambia el alias de un usuario que ya esta registrado
bool TablaHashUsuarios::ActualizarUsuario(const std::string& id,
                                           const std::string& nuevo_alias) {
  if (id.empty() || nuevo_alias.empty()) {
    return false;
  }

  Usuario* usr = BuscarUsuario(id);
  if (usr == nullptr) {
    return false;
  }

  usr->nombre_alias = nuevo_alias;
  return true;
}

// Busca un usuario por su ID y devuelve su direccion en memoria
Usuario* TablaHashUsuarios::BuscarUsuario(const std::string& id) {
  if (id.empty()) return nullptr;

  int indice = CalcularHash(id);
  NodoUsuario* actual = tabla_[indice].get();

  // Recorre la lista de esa cubeta hasta encontrar el ID exacto
  while (actual != nullptr) {
    if (actual->usuario.id_usuario == id) {
      return &(actual->usuario);
    }
    actual = actual->siguiente.get();
  }

  return nullptr;
}

// Comprueba si un ID ya esta guardado en la tabla
bool TablaHashUsuarios::ExisteUsuario(const std::string& id) const {
  if (id.empty()) return false;

  int indice = CalcularHash(id);
  const NodoUsuario* actual = tabla_[indice].get();

  while (actual != nullptr) {
    if (actual->usuario.id_usuario == id) {
      return true;
    }
    actual = actual->siguiente.get();
  }

  return false;
}

// Borra un usuario de la tabla junto con todos sus mensajes
bool TablaHashUsuarios::EliminarUsuario(const std::string& id) {
  if (id.empty()) return false;

  int indice = CalcularHash(id);

  if (!tabla_[indice]) {
    return false;
  }

  // Si el usuario es el primero de la cubeta
  if (tabla_[indice]->usuario.id_usuario == id) {
    LimpiarLista(tabla_[indice]->usuario.historial);
    tabla_[indice] = std::move(tabla_[indice]->siguiente);
    return true;
  }

  // Si el usuario esta mas adelante en la lista de esa cubeta
  NodoUsuario* actual = tabla_[indice].get();
  while (actual->siguiente) {
    if (actual->siguiente->usuario.id_usuario == id) {
      LimpiarLista(actual->siguiente->usuario.historial);
      actual->siguiente = std::move(actual->siguiente->siguiente);
      return true;
    }
    actual = actual->siguiente.get();
  }

  return false;
}

// Crea un mensaje y lo guarda en el historial propio del usuario
bool TablaHashUsuarios::EnviarMensajeUsuario(const std::string& id,
                                              const std::string& texto) {
  Usuario* usr = BuscarUsuario(id);
  if (usr == nullptr) {
    return false;
  }

  auto nuevo_msg = CrearMensaje(usr->codigo_numerico, texto);
  AlmacenarEnLista(usr->historial, std::move(nuevo_msg));
  return true;
}

// Muestra todos los mensajes que ha enviado un usuario especifico
void TablaHashUsuarios::MostrarHistorialUsuario(const std::string& id) {
  Usuario* usr = BuscarUsuario(id);
  if (usr == nullptr) {
    std::cout << "\n[!] Usuario \"" << id << "\" no encontrado en el sistema.\n";
    return;
  }

  std::cout << "\n=============================================\n";
  std::cout << " HISTORIAL DE MENSAJES DE: " << usr->nombre_alias
            << " (ID: " << usr->id_usuario << ")\n";
  std::cout << "=============================================\n";
  RecorrerLista(usr->historial.get());
}

// Muestra todos los usuarios guardados indicando en que cubeta estan
void TablaHashUsuarios::ListarUsuarios() const {
  std::cout << "\n=============================================\n";
  std::cout << "       LISTA DE USUARIOS REGISTRADOS         \n";
  std::cout << "=============================================\n";

  bool hay_usuarios = false;
  for (int i = 0; i < kCapacidadTabla; ++i) {
    const NodoUsuario* actual = tabla_[i].get();
    while (actual != nullptr) {
      hay_usuarios = true;
      std::cout << " - Cubeta [" << i << "] -> ID: " << actual->usuario.id_usuario
                << " | Alias: " << actual->usuario.nombre_alias
                << " | Codigo: " << actual->usuario.codigo_numerico << "\n";
      actual = actual->siguiente.get();
    }
  }

  if (!hay_usuarios) {
    std::cout << "   (No hay usuarios registrados en la tabla hash)\n";
  }
}
