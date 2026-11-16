# SALACHAT

**SALACHAT** es una aplicación de consola desarrollada en **C++** que simula una sala de chat. El sistema permite administrar usuarios registrados y gestionar sus conversaciones de forma dinámica, implementando estructuras de datos desde cero sin utilizar librerías externas y administrando la memoria de manera segura mediante punteros inteligentes.

---

## Requisitos

* Tener instalado un compilador de **C++** con soporte para **C++17** o superior (como **GCC / MinGW** o **Clang**).
* Contar con una **terminal** (Git Bash, PowerShell, CMD o terminal de Linux/macOS) para compilar y ejecutar el programa.
* **Git** para el control de versiones.

---

## Estructura del proyecto

El proyecto está organizado de manera modular dentro de la carpeta `src/`, separando las declaraciones (`.h`) de las implementaciones (`.cc`):

```text
SalaChat/
├── src/
│   ├── main.cc                # Menú principal y validaciones de entrada del usuario
│   ├── mensajes.h             # Estructuras y prototipos para la lista de mensajes
│   ├── mensajes.cc            # Implementación de la lista simplemente enlazada
│   ├── gestion_usuarios.h     # Definición de la Tabla Hash y estructura de usuarios
│   └── gestion_usuarios.cc    # Implementación de la Tabla Hash y manejo de colisiones
├── .gitignore                 # Ignora archivos compilados y temporales
└── README.md                  # Documentación del sistema
```

---

## Compilación y ejecución

1. Abre una terminal y ubícate dentro de la carpeta `src`:
   ```bash
   cd src
   ```

2. Compila todos los archivos fuente del proyecto:
   ```bash
   g++ -std=c++17 main.cc mensajes.cc gestion_usuarios.cc -o salachat.exe
   ```

3. Ejecuta el programa:
   * En **Linux / macOS / Git Bash**:
     ```bash
     ./salachat.exe
     ```
   * En **Windows PowerShell / CMD**:
     ```powershell
     .\salachat.exe
     ```

---

## Explicación técnica de las estructuras de datos

### 1. Lista simplemente enlazada (`Nodo` y `Mensaje`)
Se utiliza una lista dinámica lineal para guardar los mensajes enviados por cada usuario sin depender de un tamaño fijo:
* Cada nodo almacena el código del emisor (`id_emisor`), el contenido del mensaje (`texto`), la fecha y hora exacta de envío (`fecha_hora`) y un puntero al siguiente nodo.
* Permite insertar nuevos mensajes al final de la conversación, recorrer todo el historial en orden cronológico y eliminar el último mensaje registrado.

### 2. Tabla Hash con encadenamiento separado (`TablaHashUsuarios`)
Para administrar a los participantes del chat de forma rápida se implementó una tabla hash manual de capacidad prima (`11` cubetas):
* **Función de dispersión:** Utiliza el algoritmo *djb2* sobre el identificador único de texto (`id_usuario`) para calcular en qué cubeta de la tabla debe ubicarse cada usuario.
* **Manejo de colisiones:** Se resuelve mediante **encadenamiento separado (hash abierto)**. Cada cubeta actúa como el inicio de una lista enlazada de `NodoUsuario`, permitiendo que varios usuarios que caigan en el mismo índice se almacenen sin perder información.
* **Historial asociado:** Cada `Usuario` dentro de la tabla hash posee su propia lista enlazada de mensajes, lo que permite consultar o eliminar conversaciones de forma independiente por participante.

### 3. Gestión segura de memoria (`std::unique_ptr`)
Todo el manejo de memoria dinámica del proyecto se realiza con punteros inteligentes (`std::unique_ptr`) en lugar de punteros crudos (`new` y `delete`):
* Garantiza que cada nodo de usuario y cada mensaje tenga un único propietario en memoria.
* Evita fugas de memoria (*memory leaks*), ya que al eliminar un mensaje, borrar un usuario o cerrar la aplicación, la memoria se libera automáticamente y de manera iterativa.

---

## Funcionalidades principales

* **Gestión de usuarios:** Registrar nuevos participantes con clave única, actualizar su alias, buscar y validar si existen, listar todos los usuarios mostrando su cubeta y eliminar usuarios junto con su historial.
* **Gestión de mensajes:** Enviar mensajes asociados a un usuario registrado con captura automática de fecha y hora, consultar el historial completo de un usuario específico y eliminar su último mensaje enviado.
* **Validación de entradas:** Control de errores en consola para evitar fallos cuando se ingresan opciones fuera de rango, letras en campos numéricos o textos vacíos.
