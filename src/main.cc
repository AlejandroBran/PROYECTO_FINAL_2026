#include <cctype>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "gestion_usuarios.h"
#include "mensajes.h"

// Revisa si el texto ingresado tiene solo espacios en blanco
bool EsSoloEspacios(const std::string& str) {
  for (char c : str) {
    if (!std::isspace(static_cast<unsigned char>(c))) {
      return false;
    }
  }
  return true;
}

// Pide un texto y no deja avanzar si esta vacio o son solo espacios
std::string LeerCadenaNoVacia(const std::string& prompt) {
  std::string entrada;
  while (true) {
    std::cout << prompt;
    std::getline(std::cin, entrada);
    if (!entrada.empty() && !EsSoloEspacios(entrada)) {
      return entrada;
    }
    std::cout << "[!!] Error: El campo no puede estar vacio ni contener solo espacios.\n";
  }
}

// Pide un numero entero positivo y limpia el teclado si escriben letras
int LeerCodigoNumerico() {
  int codigo = 0;
  while (true) {
    std::cout << "Ingrese codigo numerico (ej. 503): ";
    if (std::cin >> codigo && codigo > 0 && std::cin.peek() == '\n') {
      std::cin.ignore(1000, '\n');
      return codigo;
    }
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cout << "[!!] Error: Debe ingresar un numero entero positivo valido.\n";
  }
}

// Lee la opcion del menu asegurando que sea un numero
int LeerOpcionMenu() {
  int opcion = 0;
  while (true) {
    std::cout << "== Seleccione una opcion -> ";
    if (std::cin >> opcion) {
      std::cin.ignore(1000, '\n');
      return opcion;
    }
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cout << "[!!] Error: Por favor ingrese solo un numero valido.\n";
  }
}

// Pregunta si desea seguir en el programa aceptando solo S o N
bool PreguntarContinuar() {
  std::string entrada;
  while (true) {
    std::cout << "\nDesea realizar otra operacion? (S/N): ";
    std::getline(std::cin, entrada);
    if (entrada == "S" || entrada == "s") {
      return true;
    } else if (entrada == "N" || entrada == "n") {
      return false;
    }
    std::cout << "[!!] Opcion invalida. Ingrese exclusivamente 'S' para Si o 'N' para No.\n";
  }
}

int main() {
  TablaHashUsuarios sistema_chat;
  bool ejecucion_activa = true;

  // Usuarios iniciales cargados por defecto
  sistema_chat.RegistrarUsuario("anon503", "Anonimo El Salvador", 503);
  sistema_chat.RegistrarUsuario("anon502", "Anonimo Guatemala", 502);

  do {
    std::cout << "\n=============================================\n";
    std::cout << "         SALA CHAT - GESTION COMPLETA        \n";
    std::cout << "=============================================\n";
    std::cout << " [1] Registrar nuevo usuario\n";
    std::cout << " [2] Actualizar informacion de usuario (Alias)\n";
    std::cout << " [3] Buscar / Validar usuario\n";
    std::cout << " [4] Enviar mensaje a usuario\n";
    std::cout << " [5] Consultar historial de mensajes por usuario\n";
    std::cout << " [6] Eliminar ultimo mensaje de usuario\n";
    std::cout << " [7] Eliminar usuario y su historial\n";
    std::cout << " [8] Listar todos los usuarios registrados\n";
    std::cout << " [9] Salir del sistema\n";
    std::cout << "=============================================\n";

    int opcion = LeerOpcionMenu();

    switch (opcion) {
      // Registro de un usuario nuevo en la tabla hash
      case 1: {
        std::cout << "\n--- REGISTRAR NUEVO USUARIO ---\n";
        std::string id = LeerCadenaNoVacia("Ingrese ID de usuario (clave unica): ");
        std::string nombre = LeerCadenaNoVacia("Ingrese Nombre / Alias: ");
        int codigo = LeerCodigoNumerico();

        if (sistema_chat.RegistrarUsuario(id, nombre, codigo)) {
          std::cout << "\n[+] Usuario registrado exitosamente en la Tabla Hash!\n";
        } else {
          std::cout << "\n[!] Error: El ID ingresado ya existe o no es valido.\n";
        }
        break;
      }

      // Cambio de alias de un usuario existente
      case 2: {
        std::cout << "\n--- ACTUALIZAR INFORMACION DE USUARIO ---\n";
        std::string id = LeerCadenaNoVacia("Ingrese ID del usuario a actualizar: ");

        if (!sistema_chat.ExisteUsuario(id)) {
          std::cout << "\n[!] Error: El usuario con ID \"" << id << "\" no existe.\n";
          break;
        }

        std::string nuevo_alias = LeerCadenaNoVacia("Ingrese nuevo Nombre / Alias: ");
        if (sistema_chat.ActualizarUsuario(id, nuevo_alias)) {
          std::cout << "\n[+] Informacion actualizada exitosamente en la Tabla Hash!\n";
        } else {
          std::cout << "\n[!] No se pudo actualizar la informacion del usuario.\n";
        }
        break;
      }

      // Busqueda y verificacion de un usuario por ID
      case 3: {
        std::cout << "\n--- BUSCAR USUARIO ---\n";
        std::string id = LeerCadenaNoVacia("Ingrese ID del usuario a validar: ");

        Usuario* usr = sistema_chat.BuscarUsuario(id);
        if (usr != nullptr) {
          std::cout << "\n[✓] Usuario ENCONTRADO en el sistema:\n";
          std::cout << "    ID: " << usr->id_usuario << "\n";
          std::cout << "    Alias: " << usr->nombre_alias << "\n";
          std::cout << "    Codigo: " << usr->codigo_numerico << "\n";
        } else {
          std::cout << "\n[X] El usuario con ID \"" << id << "\" no existe.\n";
        }
        break;
      }

      // Envio de mensaje asociandolo a la lista del usuario
      case 4: {
        std::cout << "\n--- ENVIAR MENSAJE ---\n";
        std::string id = LeerCadenaNoVacia("Ingrese ID del usuario emisor: ");

        if (!sistema_chat.ExisteUsuario(id)) {
          std::cout << "\n[!] Error: El usuario no existe en la Tabla Hash.\n";
          break;
        }

        std::string texto = LeerCadenaNoVacia("Escriba el mensaje a enviar: ");

        if (sistema_chat.EnviarMensajeUsuario(id, texto)) {
          std::cout << "\n[+] Mensaje enviado y almacenado en su lista enlazada!\n";
          sistema_chat.MostrarHistorialUsuario(id);
        }
        break;
      }

      // Consulta de mensajes guardados de un usuario
      case 5: {
        std::cout << "\n--- CONSULTAR HISTORIAL ---\n";
        std::string id = LeerCadenaNoVacia("Ingrese ID del usuario: ");
        sistema_chat.MostrarHistorialUsuario(id);
        break;
      }

      // Borrado del ultimo mensaje de un usuario
      case 6: {
        std::cout << "\n--- ELIMINAR ULTIMO MENSAJE ---\n";
        std::string id = LeerCadenaNoVacia("Ingrese ID del usuario: ");

        Usuario* usr = sistema_chat.BuscarUsuario(id);
        if (usr != nullptr) {
          if (EliminarUltimoMensaje(usr->historial)) {
            std::cout << "\n[-] Ultimo mensaje eliminado de la lista enlazada.\n";
            sistema_chat.MostrarHistorialUsuario(id);
          } else {
            std::cout << "\n[!] El usuario no tiene mensajes para eliminar.\n";
          }
        } else {
          std::cout << "\n[!] Usuario no encontrado.\n";
        }
        break;
      }

      // Eliminacion completa de un usuario de la tabla
      case 7: {
        std::cout << "\n--- ELIMINAR USUARIO ---\n";
        std::string id = LeerCadenaNoVacia("Ingrese ID del usuario a eliminar: ");

        if (sistema_chat.EliminarUsuario(id)) {
          std::cout << "\n[-] Usuario e historial removidos de la Tabla Hash!\n";
        } else {
          std::cout << "\n[!] No se encontro el usuario para eliminar.\n";
        }
        break;
      }

      // Muestra todos los usuarios registrados
      case 8: {
        sistema_chat.ListarUsuarios();
        break;
      }

      // Cierre del ciclo principal
      case 9: {
        ejecucion_activa = false;
        break;
      }

      default: {
        std::cout << "\n[!!] Por favor, ingrese una opcion valida (1 al 9).\n";
        break;
      }
    }

    if (ejecucion_activa && opcion != 9) {
      ejecucion_activa = PreguntarContinuar();
    }

  } while (ejecucion_activa);

  std::cout << "\n[ Finalizando aplicacion y liberando memoria. . . ]\n";
  std::this_thread::sleep_for(std::chrono::seconds(1));

  return 0;
}
