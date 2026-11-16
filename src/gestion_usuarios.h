#ifndef GESTION_USUARIOS_H_
#define GESTION_USUARIOS_H_

#include <memory>
#include <string>
#include "mensajes.h"

// Datos de cada usuario registrado junto con su propia lista de mensajes
struct Usuario {
  std::string id_usuario;
  std::string nombre_alias;
  int codigo_numerico;
  std::unique_ptr<Nodo> historial;
};

// Nodo usado dentro de cada cubeta de la tabla hash para manejar colisiones
struct NodoUsuario {
  Usuario usuario;
  std::unique_ptr<NodoUsuario> siguiente;
};

// Tabla hash manual para administrar a los usuarios del chat
class TablaHashUsuarios {
 public:
  static const int kCapacidadTabla = 11;

  TablaHashUsuarios();
  ~TablaHashUsuarios();

  // Calcula en que posicion (cubeta) de la tabla va cada ID
  int CalcularHash(const std::string& clave) const;

  // Operaciones principales sobre los usuarios
  bool RegistrarUsuario(const std::string& id, const std::string& nombre,
                        int codigo_num);
  bool ActualizarUsuario(const std::string& id, const std::string& nuevo_alias);
  Usuario* BuscarUsuario(const std::string& id);
  bool ExisteUsuario(const std::string& id) const;
  bool EliminarUsuario(const std::string& id);

  // Operaciones para mensajes e historial de cada usuario
  bool EnviarMensajeUsuario(const std::string& id, const std::string& texto);
  void MostrarHistorialUsuario(const std::string& id);
  void ListarUsuarios() const;

 private:
  std::unique_ptr<NodoUsuario> tabla_[kCapacidadTabla];
};

#endif  // GESTION_USUARIOS_H_
