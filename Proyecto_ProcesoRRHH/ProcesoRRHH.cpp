#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <conio.h> // Para _getch()
#include <cctype>
#include <limits>
#include <map>
#include <cstdlib> // Para system("cls") y system("pause")

using namespace std;


// Estructuras de datos para usuarios y empleados
struct Usuario {
    string username;
    string password;
    string tipo;
    string estado;
};

struct Empleado {
    string nombre;
    string identidad;
    double salario;
    double comision;
    double bonificacion;
    double impuesto;
    double seguroSocial;
    double salarioNeto;
};

// Función para ocultar la contraseña
string ocultarContraseña() {
    string contraseña = "";
    char caracter;

    cout << "contraseña: ";
    while ((caracter = _getch()) != '\r') { // '\r' = tecla Enter
        if (caracter == '\b' && contraseña.length() > 0) { // Retroceso
            cout << "\b \b"; // Borra el asterisco anterior
            contraseña.pop_back();
        } else if (caracter != '\b') {
            cout << '*'; // Muestra un asterisco
            contraseña.push_back(caracter);
        }
    }
    cout << endl;
    return contraseña;
}

// Función para autenticar un usuario en la base de datos
bool autenticarUsuario(const string& username, const string& password, string& tipoUsuario) {
    ifstream db("database.txt");
    string linea;
    while (getline(db, linea)) {
        Usuario usuario;
        size_t pos = linea.find(',');
        usuario.username = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        usuario.password = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        usuario.tipo = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        usuario.estado = linea;

        if (usuario.username == username && usuario.password == password && usuario.estado == "Activo") {
            tipoUsuario = usuario.tipo;
            return true;
        }
    }
    return false;
}

// Función para registrar un nuevo usuario en la base de datos
void registrarUsuario() {
    string usuario, contrasena, tipo, estado;
    cout << "Ingrese nombre de usuario: ";
    cin >> usuario;
    contrasena = ocultarContraseña(); // Usar la función para ocultar la contraseña
    cout << "Tipo de usuario (admin/lectura/edicion/estandar): ";
    cin >> tipo;
    cout << "Estado (Activo/Inactivo): ";
    cin >> estado;

    ofstream db("database.txt", ios::app);
    if (db.is_open()) {
        db << usuario << "," << contrasena << "," << tipo << "," << estado << endl;
        db.close();
        cout << "Usuario registrado con éxito.\n";
    } else {
        cout << "Error al abrir la base de datos.\n";
    }
}

// Función para mostrar usuarios creados (incluyendo contraseñas)
void mostrarUsuarios() {
    ifstream db("database.txt");
    if (!db.is_open()) {
        cout << "No hay usuarios registrados.\n";
        return;
    }

    cout << "\n--- Usuarios Registrados ---\n";
    cout << setw(15) << left << "Usuario"
         << setw(15) << "Contraseña"
         << setw(15) << "Tipo"
         << setw(10) << "Estado" << endl;
    cout << string(55, '-') << endl;

    string linea;
    while (getline(db, linea)) {
        Usuario usuario;
        size_t pos = linea.find(',');
        usuario.username = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        usuario.password = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        usuario.tipo = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        usuario.estado = linea;

        cout << setw(15) << left << usuario.username
             << setw(15) << usuario.password
             << setw(15) << usuario.tipo
             << setw(10) << usuario.estado << endl;
    }
    db.close();
}

// Submenú de Gestión de Usuarios
void submenuUsuarios(const vector<Empleado>& empleados) {
    int opcion;
    do {
        cout << "\n=== Gestión de Usuarios ===\n";
        cout << "1. Registrar nuevo usuario\n";
        cout << "2. Mostrar usuarios creados\n";
        cout << "3. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrarUsuario();
                break;
            case 2:
                mostrarUsuarios();
                break;
            case 3:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 3);
}

// Funciones para cálculo de planilla
void calcularPlanilla(vector<Empleado>& empleados) {
    for (auto& emp : empleados) {
        emp.impuesto = emp.comision * 0.15; // Incluir bonificación en el cálculo del impuesto
        emp.seguroSocial = emp.salario * 0.036;
        emp.salarioNeto = emp.salario + emp.comision + emp.bonificacion - emp.impuesto - emp.seguroSocial;
    }
}

// Impresión de la planilla en consola y guardado en archivo
void imprimirPlanilla(const vector<Empleado>& empleados) {
    double totalSalarioBase = 0.0;
    double totalComision = 0.0;
    double totalImpuesto = 0.0;
    double totalSeguroSocial = 0.0;
    double totalBonificacion = 0.0;
    double totalSalarioNeto = 0.0;
    double totalPagosComisiones = 0.0;
    double totalDeducciones = 0.0;
    double totalPagosNetos = 0.0;

    // Encabezados de la tabla
    cout << "--- Detalles de la Planilla ---\n";
      cout << setw(5) << left << "N°"
         << setw(30) << left << "Nombre"
         << setw(15) << "Identidad"
         << setw(15) << right << "Salario Base"
         << setw(15) << "Comisión"
         << setw(10) << "ISV"
         << setw(10) << "IHSS"
         << setw(10) << "Bonos"
         << setw(15) << "Salario Neto" << endl;
    cout << string(125, '-') << endl; // Línea separadora

    // Datos de los empleados
    // Datos de los empleados
    int contador = 1; // Contador para el número de empleado (N°)
    for (const auto& emp : empleados) {
        cout << setw(5) << left << contador++ // Mostrar el número de empleado
             << setw(30) << emp.nombre
             << setw(15) << emp.identidad
             << setw(15) << right << fixed << setprecision(2) << emp.salario
             << setw(15) << emp.comision
             << setw(10) << emp.impuesto
             << setw(10) << emp.seguroSocial
             << setw(10) << emp.bonificacion
             << setw(15) << emp.salarioNeto << endl;

        // Acumular totales de cada columna
        totalSalarioBase += emp.salario;
        totalComision += emp.comision;
        totalImpuesto += emp.impuesto;
        totalSeguroSocial += emp.seguroSocial;
        totalBonificacion += emp.bonificacion;
        totalSalarioNeto += emp.salarioNeto;
    }

    // Línea separadora antes de los totales
    cout << string(125, '-') << endl;

    // Mostrar totales debajo de cada columna
    cout << setw(5) << left << "Totales"
         << setw(28) << "" // Espacio en blanco para la columna Nombre
         << setw(15) << "" // Espacio en blanco para la columna Identidad
         << setw(15) << right << fixed << setprecision(2) << totalSalarioBase
         << setw(15) << totalComision
         << setw(10) << totalImpuesto
         << setw(10) << totalSeguroSocial
         << setw(10) << totalBonificacion
         << setw(15) << totalSalarioNeto << endl;

    // Calcular totales de pagos y deducciones
    totalPagosComisiones = totalSalarioBase + totalComision;
    totalDeducciones = totalImpuesto + totalSeguroSocial;
    totalPagosNetos = totalSalarioNeto;

    // Mostrar totales
    cout << "\n--- Totales ---\n";
    cout << "Total de pagos y comisiones: L." << fixed << setprecision(2) << totalPagosComisiones << endl;
    cout << "Total de deducciones: L." << totalDeducciones << endl;
    cout << "Total de pagos netos: L." << totalPagosNetos << endl;
    
}

// Guardar la planilla en un archivo de texto
void guardarPlanillaEnArchivo(const vector<Empleado>& empleados) {
    ofstream archivo("planilla.txt", ios::out); // Modo de escritura (sobrescribe el archivo)
    if (archivo.is_open()) {
        for (const auto& emp : empleados) {
            archivo << emp.nombre << "," 
                    << emp.identidad << "," 
                    << "L." << emp.salario << "," 
                    << "L." << emp.comision << "," 
                    << "L." << emp.impuesto << "," 
                    << "L." << emp.seguroSocial << "," 
                    << "L." << emp.bonificacion << ","
                    << "L." << emp.salarioNeto << endl;
        }
        archivo.close();
        cout << "Datos de la planilla guardados en el archivo.\n";
    } else {
        cout << "Error al abrir el archivo de planilla.\n";
    }
}

// Funciones para el manejo de empleados
vector<Empleado> cargarEmpleadosDesdeArchivo() {
    vector<Empleado> empleados;
    ifstream archivo("planilla.txt");
    if (!archivo.is_open()) {
        cout << "El archivo de planilla no existe. Se creará uno nuevo.\n";
        return empleados; // Retorna una lista vacía si el archivo no existe
    }

    string linea;
    while (getline(archivo, linea)) {
        Empleado emp;
        size_t pos = linea.find(',');
        emp.nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        emp.identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string salarioStr = linea.substr(0, pos);
        salarioStr.erase(0, 2);
        emp.salario = stod(salarioStr);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string comisionStr = linea.substr(0, pos);
        comisionStr.erase(0, 2);
        emp.comision = stod(comisionStr);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string bonificacionStr = linea.substr(0, pos); // Leer bonificación
        bonificacionStr.erase(0, 2);
        emp.bonificacion = stod(bonificacionStr);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string impuestoStr = linea.substr(0, pos);
        impuestoStr.erase(0, 2);
        emp.impuesto = stod(impuestoStr);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string seguroSocialStr = linea.substr(0, pos);
        seguroSocialStr.erase(0, 2);
        emp.seguroSocial = stod(seguroSocialStr);
        linea.erase(0, pos + 1);

        string salarioNetoStr = linea;
        salarioNetoStr.erase(0, 2); 
        emp.salarioNeto = stod(salarioNetoStr);
        empleados.push_back(emp);
    }
    return empleados;
}

// Funciones para el modificación de empleados y bonificaciones
void modificarEmpleado(vector<Empleado>& empleados) {
    string identidad;
    cout << "Ingrese el número de identidad del empleado a modificar: ";
    cin >> identidad;

    for (auto& emp : empleados) {
        if (emp.identidad == identidad) {
            // Mostrar datos actuales
            cout << "\n--- Datos Actuales ---\n";
            cout << "Nombre: " << emp.nombre 
                 << " | Salario: L." << emp.salario 
                 << " | Comisión: L." << emp.comision
                 << " | Bonificación: L." << emp.bonificacion << endl; // Ahora funciona
            // Modificar los datos
            cout << "Ingrese el nuevo nombre del empleado: ";
            cin.ignore();
            getline(cin, emp.nombre);
            cout << "Ingrese el nuevo salario: ";
            cin >> emp.salario;
            cout << "Ingrese la nueva comisión: ";
            cin >> emp.comision;
            cout << "Ingrese la nueva Bonificación: ";
            cin >> emp.bonificacion;
            cout << "Empleado modificado con éxito.\n";
            return;
        }
    }
    cout << "Empleado no encontrado.\n";
}

// Función para agregar un nuevo empleado a la planilla
void agregarEmpleado(vector<Empleado>& empleados) {
    Empleado nuevoEmpleado;
    cout << "Ingrese el nombre completo del empleado: ";
    cin.ignore();
    getline(cin, nuevoEmpleado.nombre);

    string identidad;
    bool identidadExistente = false;
    do {
        cout << "Ingrese el número de identidad: ";
        cin >> identidad;

        // Verificar si el número de identidad ya existe
        identidadExistente = false;
        for (const auto& emp : empleados) {
            if (emp.identidad == identidad) {
                identidadExistente = true;
                break;
            }
        }
        // Si el número de identidad ya existe, preguntar al usuario qué desea hacer
        if (identidadExistente) {
            cout << "El número de identidad ya existe. ¿Qué desea hacer?\n";
            cout << "1. Modificar los datos del empleado existente.\n";
            cout << "2. Ingresar un nuevo empleado.\n";
            cout << "Seleccione una opción: ";
            int opcion;
            cin >> opcion;

            if (opcion == 1) {
                modificarEmpleado(empleados); // Llamar a la función para modificar empleado
                return; // Salir de la función después de modificar
            } else if (opcion != 2) {
                cout << "Opción no válida. Volviendo al menú principal.\n";
                return;
            }
        }
    } while (identidadExistente);

    // Si el número de identidad no existe, continuar con el ingreso de datos
    nuevoEmpleado.identidad = identidad; // Asignar la identidad al nuevo empleado
    cout << "Ingrese el salario base: ";
    cin >> nuevoEmpleado.salario;
    cout << "Ingrese la comisión: ";
    cin >> nuevoEmpleado.comision;
    cout << "Ingrese la bonificación: ";
    cin >> nuevoEmpleado.bonificacion; // Inicializar la bonificación
    nuevoEmpleado.impuesto = 0;
    nuevoEmpleado.seguroSocial = 0;
    nuevoEmpleado.salarioNeto = 0;

    empleados.push_back(nuevoEmpleado);
    cout << "Empleado agregado con éxito.\n";
}

// Función para agregar bonificaciones a un empleado
void agregarBonificacion(vector<Empleado>& empleados) {
    string identidad;
    cout << "Ingrese identidad del empleado: ";
    cin.ignore();
    getline(cin, identidad);

    // Buscar al empleado por su identidad
    for (auto& emp : empleados) {
        if (emp.identidad == identidad) {
            // Mostrar los datos actuales del empleado
            cout << "\n--- Datos Actuales del Empleado ---\n";
            cout << "Nombre: " << emp.nombre 
                 << " | Bono actual: L." << emp.bonificacion << endl;

            double bonificacion;
            cout << "Ingrese la bonificación a agregar: ";
            cin >> bonificacion;

            // Acumular la bonificación a la comisión actual
            emp.bonificacion += bonificacion;

            cout << "\n--- Datos Actualizados ---\n";
            cout << "Nombre: " << emp.nombre 
                 << " | Nueva Bonificación: L." << emp.bonificacion << endl;
            return; // Salir de la función después de encontrar y actualizar al empleado
        }
    }
    // Si no se encuentra al empleado
    cout << "Empleado no encontrado.\n";
}

// Función para enviar la planilla a Contabilidad
void enviarAContabilidad(const vector<Empleado>& empleados) {
    char confirmacion;
    cout << "\n¿Está seguro que desea enviar el documento con los datos acumulados? (s/n): ";
    cin >> confirmacion;
    if (confirmacion == 's' || confirmacion == 'S') {
        guardarPlanillaEnArchivo(empleados);
        cout << "Documento enviado a Contabilidad!.\n";
    } else {
        cout << "Envío cancelado.\n";
    }
}

struct Candidato {
    string nombre;
    string direccion;
    string identidad;
    string sexo;
    string titulo;
    string telefono;
};

struct Cita {
    string nombreCandidato;
    string identidadCandidato;
    string telefonoCandidato;
    string fecha;
    string hora;
};

struct FaltaDisciplinaria {
    string nombreEmpleado;
    string identidad;
    string tipoFalta; // Leve o Grave
    string accion; // Verbal, Escrito, Suspensión, Despido
};

struct Despido {
    string nombreEmpleado;
    string identidad;
    string motivo; // Justificado, Recorte, Renuncia
};

struct CalculoDerechos {
    string nombreEmpleado;
    string identidad;
    double salarioPendiente;
    double vacacionesNoGozadas;
    double decimoTercero;
    double decimoCuarto;
    double horasExtras;
    double preaviso;
    double auxilioCesantia;
    double total;
};

void mostrarEmpleadosInternos(const vector<Empleado>& empleados) {
    cout << "--- Lista de Empleados Internos ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad" << endl;
    cout << string(50, '-') << endl;

    int contador = 1;
    for (const auto& emp : empleados) {
        cout << setw(5) << left << contador++
             << setw(30) << emp.nombre
             << setw(15) << emp.identidad << endl;
    }

    char respuesta;
    cout << "\n¿Desea elegir algún empleado para cubrir una plaza existente? (s/n): ";
    cin >> respuesta;

    if (respuesta == 's' || respuesta == 'S') {
        string identidad;
        cout << "Ingrese el número de identidad del empleado: ";
        cin >> identidad;

        cin.ignore(); // Limpiar el buffer de entrada antes de getline
        string departamento;
        cout << "Ingrese el nombre del departamento a promover: ";
        getline(cin, departamento);

        // Buscar al empleado por su número de identidad
        bool empleadoEncontrado = true;
        for (const auto& emp : empleados) {
            if (emp.identidad == identidad) {
                empleadoEncontrado = true;

                // Guardar en el archivo "empleadospromovidos.txt"
                ofstream archivoPromovidos("empleadospromovidos.txt", ios::app);
                if (archivoPromovidos.is_open()) {
                    archivoPromovidos << emp.nombre << "," << emp.identidad << "," << departamento << endl;
                    archivoPromovidos.close();
                    cout << "Empleado promovido y guardado con éxito.\n";
                } else {
                    cout << "Error al abrir el archivo.\n";
                }
                break; // Salir del bucle una vez que se encuentra el empleado
            }
        }
        if (!empleadoEncontrado) {
            cout << "No se encontró un empleado con el número de identidad proporcionado.\n";
        }
    }
}

void ingresarCandidato() {
    Candidato candidato;
    cout << "Ingrese nombre completo: ";
    cin.ignore();
    getline(cin, candidato.nombre);
    cout << "Ingrese dirección: ";
    getline(cin, candidato.direccion);
    cout << "Ingrese número de identidad: ";
    getline(cin, candidato.identidad);
    cout << "Ingrese sexo (F: Femenino, M: Masculino): ";
    getline(cin, candidato.sexo);
    cout << "Ingrese título obtenido: ";
    getline(cin, candidato.titulo);
    cout << "Ingrese número de teléfono: ";
    getline(cin, candidato.telefono);

    ofstream archivo("Candidatos.txt", ios::app);
    if (archivo.is_open()) {
        archivo << candidato.nombre << "," << candidato.direccion << "," << candidato.identidad << ","
                << candidato.sexo << "," << candidato.titulo << "," << candidato.telefono << endl;
        archivo.close();
        cout << "Candidato registrado con éxito.\n";
    } else {
        cout << "Error al abrir el archivo.\n";
    }
}

void elegirCandidato() {
    ifstream archivo("Candidatos.txt");
    if (!archivo.is_open()) {
        cout << "No hay candidatos registrados.\n";
        return;
    }

    cout << "--- Lista de Candidatos ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad"
         << setw(10) << "Sexo"
         << setw(20) << "Título"
         << setw(15) << "Teléfono" << endl;
    cout << string(95, '-') << endl;

    string linea;
    int contador = 1;
    while (getline(archivo, linea)) {
        size_t pos = linea.find(',');
        string nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string direccion = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string sexo = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string titulo = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        string telefono = linea;

        cout << setw(5) << left << contador++
             << setw(30) << nombre
             << setw(15) << identidad
             << setw(10) << sexo
             << setw(20) << titulo
             << setw(15) << telefono << endl;
    }
    archivo.close();

    string identidad, fecha, hora;
    cout << "\nIngrese el número de identidad del candidato seleccionado: ";
    cin >> identidad;
    cout << "Ingrese la fecha de la cita (DD/MM/AAAA): ";
    cin >> fecha;
    cout << "Ingrese la hora de la cita (HH:MM): ";
    cin >> hora;

    ofstream citas("CitasProgramadas.txt", ios::app);
    if (citas.is_open()) {
        citas << identidad << "," << fecha << "," << hora << endl;
        citas.close();
        cout << "Cita programada con éxito.\n";
    } else {
        cout << "Error al abrir el archivo.\n";
    }
}

void mostrarCitasProgramadas() {
    ifstream archivo("CitasProgramadas.txt");
    if (!archivo.is_open()) {
        cout << "No hay citas programadas.\n";
        return;
    }

    cout << "--- Citas Programadas ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad"
         << setw(15) << "Teléfono"
         << setw(12) << "Fecha"
         << setw(8) << "Hora" << endl;
    cout << string(85, '-') << endl;

    string linea;
    int contador = 1;
    while (getline(archivo, linea)) {
        size_t pos = linea.find(',');
        string identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string fecha = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        string hora = linea;

        // Buscar el nombre y teléfono del candidato en "Candidatos.txt"
        ifstream candidatos("Candidatos.txt");
        string nombre, telefono;
        while (getline(candidatos, linea)) {
            size_t pos = linea.find(',');
            nombre = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            linea.erase(0, pos + 1); // Saltar dirección

            pos = linea.find(',');
            string id = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            linea.erase(0, pos + 1); // Saltar sexo

            pos = linea.find(',');
            linea.erase(0, pos + 1); // Saltar título

            telefono = linea;

            if (id == identidad) {
                break;
            }
        }
        candidatos.close();

        cout << setw(5) << left << contador++
             << setw(30) << nombre
             << setw(15) << identidad
             << setw(15) << telefono
             << setw(12) << fecha
             << setw(8) << hora << endl;
    }
    archivo.close();
}

void mostrarEmpleadosPromovidos() {
    ifstream archivo("empleadospromovidos.txt");
    if (!archivo.is_open()) {
        cout << "No hay empleados promovidos registrados.\n";
        return;
    }

    cout << "--- Listado de Empleados Promovidos ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad"
         << setw(20) << "Departamento" << endl;
    cout << string(70, '-') << endl;

    string linea;
    int contador = 1;
    while (getline(archivo, linea)) {
    
        size_t pos = linea.find(',');
        if (pos == string::npos) continue; // Saltar líneas incorrectas
        string nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);
    
        pos = linea.find(',');
        if (pos == string::npos) continue;
        string identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);
    
        string departamento = linea;
    
        cout << setw(5) << left << contador++
             << setw(30) << nombre
             << setw(15) << identidad
             << setw(20) << departamento << endl;
    }

    archivo.close();
}

void submenudescripcionanalisispuesto(vector<Empleado>& empleados) {
    int opcion;
    do {
        cout << "\n=== Submenú Descripción y Análisis de Puesto ===\n";
        cout << "1. Elección de empleados internos\n";
        cout << "2. Ingreso de candidatos\n";
        cout << "3. Elección de candidatos\n";
        cout << "4. Mostrar citas programadas\n";
        cout << "5. Mostrar listado de Empleados promovidos\n";
        cout << "6. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                mostrarEmpleadosInternos(empleados);
                break;
            case 2:
                ingresarCandidato();
                break;
            case 3:
                elegirCandidato();
                break;
            case 4:
                mostrarCitasProgramadas();
                break;
            case 5:
                mostrarEmpleadosPromovidos();
                break;
            case 6:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 6);
}

void calcularDerechosDespidoJustificado() {
    ifstream archivo("EmpleadosDespidoJustificado.txt");
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados para despido justificado.\n";
        return;
    }

    cout << "\n--- Cálculo de Derechos por Despido Justificado ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad"
         << setw(20) << "Falta" << endl;
    cout << string(70, '-') << endl;

    string linea;
    vector<string> empleados;  // Para almacenar empleados
    int contador = 1;
    while (getline(archivo, linea)) {
        size_t pos = linea.find(',');
        string nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        string falta = linea;

        // Mostrar empleado
        cout << setw(5) << left << contador
             << setw(30) << nombre
             << setw(15) << identidad
             << setw(20) << falta << endl;

        // Almacenar datos del empleado en el vector
        empleados.push_back(nombre + "," + identidad + "," + falta);
        contador++;
    }
    archivo.close();

    // Pedir al usuario seleccionar un empleado
    int seleccion;
    cout << "\nSeleccione el número del empleado para calcular los derechos (1 a " << empleados.size() << "): ";
    cin >> seleccion;

    if (seleccion < 1 || seleccion > empleados.size()) {
        cout << "Selección inválida.\n";
        return;
    }

    // Obtener los datos del empleado seleccionado
    string empleadoSeleccionado = empleados[seleccion - 1];
    size_t pos1 = empleadoSeleccionado.find(',');
    string nombre = empleadoSeleccionado.substr(0, pos1);
    empleadoSeleccionado.erase(0, pos1 + 1);

    pos1 = empleadoSeleccionado.find(',');
    string identidad = empleadoSeleccionado.substr(0, pos1);
    string falta = empleadoSeleccionado.substr(pos1 + 1);

    // Mostrar empleado seleccionado
    cout << "\nEmpleado seleccionado: " << nombre << endl;

    // Pedir datos para el cálculo
    double salarioPendiente, vacacionesNoGozadas, decimoTercero, decimoCuarto, horasExtras;
    cout << "\nIngrese los siguientes datos para " << nombre << ":\n";
    cout << "Salario pendiente: ";
    cin >> salarioPendiente;
    cout << "Vacaciones no gozadas: ";
    cin >> vacacionesNoGozadas;
    cout << "Décimo tercer mes proporcional: ";
    cin >> decimoTercero;
    cout << "Décimo cuarto mes proporcional: ";
    cin >> decimoCuarto;
    cout << "Horas extras pendientes: ";
    cin >> horasExtras;

    // Calcular total
    double total = salarioPendiente + vacacionesNoGozadas + decimoTercero + decimoCuarto + horasExtras;

    // Guardar en archivo "calculototal.txt"
    ofstream calculo("calculototal.txt", ios::app);
    if (calculo.is_open()) {
        calculo << nombre << "," << identidad << ","
                << salarioPendiente << "," << vacacionesNoGozadas << ","
                << decimoTercero << "," << decimoCuarto << ","
                << horasExtras << "," << total << endl;
        calculo.close();
        cout << "Cálculo guardado en calculototal.txt.\n";
    } else {
        cout << "Error al abrir el archivo calculototal.txt.\n";
    }
}

void calcularDerechosDespidoRecorte() {
    ifstream archivo("planilla.txt");
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados para despido por recorte de personal.\n";
        return;
    }

    cout << "\n--- Cálculo de Derechos por Despido por Recorte de Personal ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad" << endl;
    cout << string(50, '-') << endl;

    string linea;
    vector<string> empleados;  // Para almacenar empleados
    int contador = 1;
    while (getline(archivo, linea)) {
        size_t pos = linea.find(',');
        string nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        cout << setw(5) << left << contador
             << setw(30) << nombre
             << setw(15) << identidad << endl;

        // Almacenar datos del empleado en el vector
        empleados.push_back(nombre + "," + identidad);
        contador++;
    }
    archivo.close();

    // Pedir al usuario seleccionar un empleado
    int seleccion;
    cout << "\nSeleccione el número del empleado para calcular los derechos (1 a " << empleados.size() << "): ";
    cin >> seleccion;

    if (seleccion < 1 || seleccion > empleados.size()) {
        cout << "Selección inválida.\n";
        return;
    }

    // Obtener los datos del empleado seleccionado
    string empleadoSeleccionado = empleados[seleccion - 1];
    size_t pos1 = empleadoSeleccionado.find(',');
    string nombre = empleadoSeleccionado.substr(0, pos1);
    string identidad = empleadoSeleccionado.substr(pos1 + 1);

    // Mostrar empleado seleccionado
    cout << "\nEmpleado seleccionado: " << nombre << endl;

    // Pedir datos para el cálculo
    double salarioPendiente, vacacionesNoGozadas, decimoTercero, decimoCuarto, horasExtras;
    int mesesTrabajados;
    cout << "\nIngrese los siguientes datos para " << nombre << ":\n";
    cout << "Salario pendiente: ";
    cin >> salarioPendiente;
    cout << "Vacaciones no gozadas: ";
    cin >> vacacionesNoGozadas;
    cout << "Décimo tercer mes proporcional: ";
    cin >> decimoTercero;
    cout << "Décimo cuarto mes proporcional: ";
    cin >> decimoCuarto;
    cout << "Horas extras pendientes: ";
    cin >> horasExtras;
    cout << "Meses trabajados: ";
    cin >> mesesTrabajados;

    // Calcular preaviso y auxilio de cesantía
    double preaviso = (mesesTrabajados > 12) ? salarioPendiente : 0;
    double auxilioCesantia = 0;
    if (mesesTrabajados >= 3 && mesesTrabajados <= 6) {
        auxilioCesantia = 10 * salarioPendiente / 30 * mesesTrabajados;
    } else if (mesesTrabajados > 6 && mesesTrabajados <= 12) {
        auxilioCesantia = 20 * salarioPendiente / 30 * mesesTrabajados;
    } else if (mesesTrabajados > 12) {
        auxilioCesantia = salarioPendiente * (mesesTrabajados / 12);
    }

    // Calcular total
    double total = salarioPendiente + vacacionesNoGozadas + decimoTercero + decimoCuarto + horasExtras + preaviso + auxilioCesantia;

    // Guardar en archivo "calculototal.txt"
    ofstream calculo("calculototal.txt", ios::app);
    if (calculo.is_open()) {
        calculo << nombre << "," << identidad << "," << salarioPendiente << "," 
                << vacacionesNoGozadas << ","
                << decimoTercero << "," << decimoCuarto << ","
                << horasExtras << "," << preaviso << ","
                << auxilioCesantia << "," << total << endl;
        calculo.close();
        cout << "Cálculo guardado en calculototal.txt.\n";
    } else {
        cout << "Error al abrir el archivo calculototal.txt.\n";
    }
}

void calcularDerechosRenuncia() {
    ifstream archivo("planilla.txt");
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados para renuncia.\n";
        return;
    }

    cout << "\n--- Cálculo de Derechos por Renuncia ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad" << endl;
    cout << string(50, '-') << endl;

    string linea;
    vector<string> empleados;  // Para almacenar empleados
    int contador = 1;
    while (getline(archivo, linea)) {
        size_t pos = linea.find(',');
        string nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        cout << setw(5) << left << contador
             << setw(30) << nombre
             << setw(15) << identidad << endl;

        // Almacenar datos del empleado en el vector
        empleados.push_back(nombre + "," + identidad);
        contador++;
    }
    archivo.close();

    // Pedir al usuario seleccionar un empleado
    int seleccion;
    cout << "\nSeleccione el número del empleado para calcular los derechos (1 a " << empleados.size() << "): ";
    cin >> seleccion;

    if (seleccion < 1 || seleccion > empleados.size()) {
        cout << "Selección inválida.\n";
        return;
    }

    // Obtener los datos del empleado seleccionado
    string empleadoSeleccionado = empleados[seleccion - 1];
    size_t pos1 = empleadoSeleccionado.find(',');
    string nombre = empleadoSeleccionado.substr(0, pos1);
    string identidad = empleadoSeleccionado.substr(pos1 + 1);

    // Mostrar empleado seleccionado
    cout << "\nEmpleado seleccionado: " << nombre << endl;

    // Pedir datos para el cálculo
    double salarioPendiente, vacacionesNoGozadas, decimoTercero, decimoCuarto;
    cout << "\nIngrese los siguientes datos para " << nombre << ":\n";
    cout << "Salario pendiente: ";
    cin >> salarioPendiente;
    cout << "Vacaciones no gozadas: ";
    cin >> vacacionesNoGozadas;
    cout << "Décimo tercer mes proporcional: ";
    cin >> decimoTercero;
    cout << "Décimo cuarto mes proporcional: ";
    cin >> decimoCuarto;

    // Calcular total
    double total = salarioPendiente + vacacionesNoGozadas + decimoTercero + decimoCuarto;

    // Guardar en archivo "calculototal.txt"
    ofstream calculo("calculototal.txt", ios::app);
    if (calculo.is_open()) {
        calculo << nombre << "," << identidad << "," << salarioPendiente 
                << "," << vacacionesNoGozadas << ","
                << decimoTercero << "," << decimoCuarto << "," << total << endl;
        calculo.close();
        cout << "Cálculo guardado en calculototal.txt.\n";
    } else {
        cout << "Error al abrir el archivo calculototal.txt.\n";
    }
}

void mostrarCalculoTotal() {
    ifstream archivo("calculototal.txt");
    if (!archivo.is_open()) {
        cout << "No hay cálculos registrados.\n";
        return;
    }

    cout << "--- Cálculo Total de Derechos Laborales ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad"
         << setw(15) << "Salario"
         << setw(15) << "Vacaciones"
         << setw(15) << "Décimo 3ro"
         << setw(15) << "Décimo 4to"
         << setw(15) << "Horas Extras"
         << setw(15) << "Preaviso"
         << setw(15) << "Cesantía"
         << setw(15) << "Total" << endl;
    cout << string(160, '-') << endl;

    string linea;
    int contador = 1;
    while (getline(archivo, linea)) {
        size_t pos = linea.find(',');
        string nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        double salarioPendiente = stod(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        double vacacionesNoGozadas = stod(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        double decimoTercero = stod(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        double decimoCuarto = stod(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        double horasExtras = stod(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        double preaviso = stod(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        double auxilioCesantia = stod(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        double total = stod(linea);

        // Mostrar los datos en pantalla
        cout << setw(5) << left << contador++
             << setw(30) << nombre
             << setw(15) << identidad
             << setw(15) << salarioPendiente
             << setw(15) << vacacionesNoGozadas
             << setw(15) << decimoTercero
             << setw(15) << decimoCuarto
             << setw(15) << horasExtras
             << setw(15) << preaviso
             << setw(15) << auxilioCesantia
             << setw(15) << total << endl;
    }
    archivo.close();
}

void submenuegresodepersonal() {
    int opcion;
    do {
        cout << "\n=== Submenú Egreso de Personal ===\n";
        cout << "1. Calculo Despido justificado\n";
        cout << "2. Calculo Despido Recorte Personal\n";
        cout << "3. Calculo por renuncia\n";
        cout << "4. Mostrar cálculo total\n";
        cout << "5. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                calcularDerechosDespidoJustificado();
                break;
            case 2:
                calcularDerechosDespidoRecorte();
                break;
            case 3:
                calcularDerechosRenuncia();
                break;
            case 4:
                mostrarCalculoTotal();
                break;
            case 5:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 5);
}

void registrarFaltaDisciplinaria() {
    ifstream lista("planilla.txt");
    if (!lista.is_open()) {
        cout << "No hay empleados registrados.\n";
        return;
    }

    // Mostrar lista de empleados
    cout << "\n--- Lista de Empleados ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad" << endl;
    cout << string(50, '-') << endl;

    vector<pair<string, string>> empleados; // Para almacenar nombre e identidad
    string linea;
    int contador = 1;
    
    while (getline(lista, linea)) {
        size_t pos = linea.find(',');
        string nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string id = linea.substr(0, pos);
        
        empleados.push_back({nombre, id}); // Guardamos para validación
        
        cout << setw(5) << left << contador++
             << setw(30) << nombre
             << setw(15) << id << endl;
    }
    lista.close();

    // Solicitar identidad del empleado
    string identidad;
    cout << "\nIngrese el número de identidad del empleado: ";
    cin >> identidad;

    // Buscar empleado en la lista
    bool encontrado = false;
    string nombreEmpleado;
    for (const auto& emp : empleados) {
        if (emp.second == identidad) {
            encontrado = true;
            nombreEmpleado = emp.first;
            break;
        }
    }

    if (!encontrado) {
        cout << "Empleado no encontrado.\n";
        return;
    }

    // Registrar la falta disciplinaria
    string tipoFalta, accion;
    cout << "Seleccione el tipo de falta (Leve/Grave): ";
    cin >> tipoFalta;

    if (tipoFalta == "Leve") {
        cout << "Seleccione la acción (Verbal/Escrito): ";
        cin >> accion;
    } else if (tipoFalta == "Grave") {
        cout << "Seleccione la acción (Suspensión/Despido): ";
        cin >> accion;

        if (accion == "Despido") {
            ofstream despido("EmpleadosDespidoJustificado.txt", ios::app);
            if (despido.is_open()) {
                despido << nombreEmpleado << "," << identidad << "," << "Grave" << endl;
                despido.close();
            }
        }
    }



    // Guardar la falta disciplinaria en el archivo "AccionesDisciplinarias.txt"
    ofstream archivo("AccionesDisciplinarias.txt", ios::app);
    if (archivo.is_open()) {
        archivo << nombreEmpleado << "," << identidad << "," << tipoFalta << "," << accion << endl;
        archivo.close();
        cout << "Falta disciplinaria registrada con éxito.\n";
    } else {
        cout << "Error al abrir el archivo.\n";
    }
}

void mostrarFaltasDisciplinarias() {
    ifstream archivo("AccionesDisciplinarias.txt");
    if (!archivo.is_open()) {
        cout << "No hay faltas disciplinarias registradas.\n";
        return;
    }

    cout << "\n--- Faltas Disciplinarias ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Nombre"
         << setw(15) << "Identidad"
         << setw(15) << "Tipo de Falta"
         << setw(15) << "Acción" << endl;
    cout << string(95, '-') << endl;

    string linea;
    int contador = 1;
    while (getline(archivo, linea)) {
        size_t pos = linea.find(',');
        string nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string identidad = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        string tipoFalta = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        string accion = linea;

        cout << setw(5) << left << contador++
             << setw(30) << nombre
             << setw(15) << identidad
             << setw(15) << tipoFalta
             << setw(15) << accion << endl;
    }
    archivo.close();
}

void submenuAccionesDisciplinarias() {
    int opcion;
    do {
        cout << "\n=== Submenú Acciones Disciplinarias ===\n";
        cout << "1. Registrar falta a Empleado\n";
        cout << "2. Mostrar faltas disciplinarias\n";
        cout << "3. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrarFaltaDisciplinaria();
                break;
            case 2:
                mostrarFaltasDisciplinarias();
                break;
            case 3:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 3);
}

void submenugestionempleados(vector<Empleado>& empleados) {
    int opcion;
    do {
        cout << "\n=== Submenú Gestión de Empleados ===\n";
        cout << "1. Descripción y Análisis de Puesto\n";
        cout << "2. Acciones discicplinarias\n";
        cout << "3. Egreso de Personal\n";
        cout << "4. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                submenudescripcionanalisispuesto(empleados);
                break;
            case 2:
                submenuAccionesDisciplinarias();
                break;
            case 3:
                submenuegresodepersonal();
                break;
            case 4:
                cout << "Volviendo al menú principal...\n";
                break;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while (opcion != 4);
}

// Función para aplicar test de evaluación de desempeño
void aplicarTestDesempeno() {
    cout << "\n--- Aplicación de TEST de Evaluación de Desempeño ---\n";
    
    string jefatura, departamento, tema, lugar, fecha;
    vector<string> empleados;
    bool requiereCapacitacion = false;

    // Solicitar datos generales
    cout << "Ingrese nombre de la jefatura solicitante: ";
    getline(cin, jefatura);
    cout << "Ingrese departamento: ";
    getline(cin, departamento);
    // Solicitar empleados y sus notas
    char continuar = 's';
    while(continuar == 's' || continuar == 'S') {
        string nombreEmpleado;
        double notaEmpleado;
        
        cout << "\nIngrese nombre del empleado: ";
        getline(cin, nombreEmpleado);
        cout << "Ingrese nota de evaluación (0-100%): ";
        cin >> notaEmpleado;
        cin.ignore(); // Limpiar el buffer
        
        if(notaEmpleado < 80) {
            empleados.push_back(nombreEmpleado);
            requiereCapacitacion = true;
            cout << "El empleado " << nombreEmpleado << " requiere capacitación de reforzamiento.\n";
        } else {
            cout << "El empleado " << nombreEmpleado << " no requiere capacitación.\n";
        }
        
        cout << "¿Desea ingresar otro empleado? (s/n): ";
        cin >> continuar;
        cin.ignore(); // Limpiar el buffer
    }
    
    // Si se requiere capacitación, solicitar más detalles
    if(requiereCapacitacion) {
        cout << "\n--- PROGRAMAR CAPACITACIÓN DE REFORZAMIENTO ---\n";
        cout << "Ingrese tema de reforzamiento: ";
        getline(cin, tema);
        cout << "Ingrese lugar de la capacitación: ";
        getline(cin, lugar);
        cout << "Ingrese fecha de la capacitación: ";
        getline(cin, fecha);
        
        // Guardar en archivo en formato CSV
        ofstream archivo("programacioncapacitaciones.txt", ios::app);
        if(archivo.is_open()) {
            archivo << "Evaluación de Desempeño," << jefatura << "," << departamento << ",";
            
            // Guardar lista de empleados separados por punto y coma
            for(size_t i = 0; i < empleados.size(); i++) {
                archivo << empleados[i];
                if(i != empleados.size() - 1) archivo << ";";
            }
            
            archivo << "," << tema << "," << lugar << "," << fecha << "\n";
            archivo.close();
            
            cout << "\nCapacitación programada y guardada correctamente.\n";
        } else {
            cout << "Error al abrir el archivo para guardar.\n";
        }
    } else {
        cout << "\nNingún empleado requiere capacitación de reforzamiento.\n";
    }
}

void ingresarSolicitudCapacitacion() {
    cout << "\n--- INGRESO DE SOLICITUD DE CAPACITACIÓN POR JEFATURA ---\n";
    
    string jefatura, departamento, tema, lugar, fecha;
    vector<string> empleados;
    
    // Solicitar datos
    cout << "Ingrese nombre de la jefatura solicitante: ";
    getline(cin, jefatura);
    cout << "Ingrese departamento: ";
    getline(cin, departamento);
    
    // Solicitar empleados
    char continuar = 's';
    while(continuar == 's' || continuar == 'S') {
        string nombreEmpleado;
        
        cout << "Ingrese nombre del empleado: ";
        getline(cin, nombreEmpleado);
        empleados.push_back(nombreEmpleado);
        
        cout << "¿Desea ingresar otro empleado? (s/n): ";
        cin >> continuar;
        cin.ignore(); // Limpiar el buffer
    }
    
    cout << "Ingrese tema de la capacitación: ";
    getline(cin, tema);
    cout << "Ingrese lugar de la capacitación: ";
    getline(cin, lugar);
    cout << "Ingrese fecha de la capacitación: ";
    getline(cin, fecha);
    
    // Guardar en archivo en formato CSV
    ofstream archivo("programacioncapacitaciones.txt", ios::app);
    if(archivo.is_open()) {
        archivo << "Solicitud de Jefatura," << jefatura << "," << departamento << ",";
        
        // Guardar lista de empleados separados por punto y coma
        for(size_t i = 0; i < empleados.size(); i++) {
            archivo << empleados[i];
            if(i != empleados.size() - 1) archivo << ";";
        }
        
        archivo << "," << tema << "," << lugar << "," << fecha << "\n";
        archivo.close();
        
        cout << "\nSolicitud de capacitación guardada correctamente.\n";
    } else {
        cout << "Error al abrir el archivo para guardar.\n";
    }
}

void mostrarCapacitacionesProgramadas() {
    cout << "\n--- CAPACITACIONES PROGRAMADAS ---\n";
    cout << setw(5) << left << "N°"
         << setw(30) << "Jefatura"
         << setw(20) << "Departamento"
         << setw(30) << "Tipo"
         << setw(30) << "Tema"
         << setw(30) << "Lugar"
         << setw(10) << "Fecha" << endl;
    cout << string(130, '-') << endl;

    ifstream archivo("programacioncapacitaciones.txt");
    if (!archivo.is_open()) {
        cout << "No hay capacitaciones programadas.\n";
        return;
    }

    string linea;
    int contador = 1;
    while (getline(archivo, linea)) {
        vector<string> campos;
        size_t pos = 0;
        string token;
        
        // Parsear la línea CSV
        while ((pos = linea.find(',')) != string::npos) {
            token = linea.substr(0, pos);
            campos.push_back(token);
            linea.erase(0, pos + 1);
        }
        campos.push_back(linea); // Último campo

        if(campos.size() >= 6) {
            cout << setw(5) << left << contador++
                 << setw(30) << campos[1]  // Jefatura
                 << setw(20) << campos[2]  // Departamento
                 << setw(30) << campos[0]  // Tipo
                 << setw(30) << campos[4]  // Tema
                 << setw(30) << campos[5]  // Lugar
                 << setw(10) << campos[6]  // Fecha
                 << endl;
        }
    }
    archivo.close();
}

void menuCapacitaciones() {
    int opcion;
    do {
        cout << "\n--- Submenú Capacitaciones ---\n";
        cout << "1. Aplicación de TEST de evaluación de desempeño\n";
        cout << "2. Ingreso de solicitud de capacitación por jefatura\n";
        cout << "3. Mostrar las capacitaciones programadas\n";
        cout << "4. Regresar al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer de entrada

        switch(opcion) {
            case 1:
                aplicarTestDesempeno();
                break;
            case 2:
                ingresarSolicitudCapacitacion();
                break;
            case 3:
                mostrarCapacitacionesProgramadas();
                break;
            case 4:
                cout << "Regresando al menú principal...\n";
                break;
            default:
                cout << "Opción no válida. Intente nuevamente.\n";
        }
    } while(opcion != 4);
}

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <limits>

using namespace std;

// Estructura para representar un detalle de gasto dentro de una factura
struct DetalleGasto {
    string concepto;
    float monto;
};

// Estructura para representar una factura
struct Factura {
    string numeroFactura;
    vector<DetalleGasto> detalles;
    float montoTotal;
};

// Estructura para la caja chica
struct CajaChica {
    float fondoInicial;
    vector<Factura> facturas;
};

// Implementación de funciones

void registrarGastos(CajaChica& caja) {
    char agregarOtraFactura = 's';
    while (agregarOtraFactura == 's' || agregarOtraFactura == 'S') {
        Factura nuevaFactura;
        char agregarMasConceptos = 's';
        nuevaFactura.montoTotal = 0;

        cout << "\n=== REGISTRO DE FACTURA ===" << endl;
        cout << "N° de Factura: ";
        cin.ignore();
        getline(cin, nuevaFactura.numeroFactura);
        DetalleGasto detalle;
            cout << "Concepto del gasto: ";
            getline(cin, detalle.concepto);
            cout << "Monto: L. ";
            cin >> detalle.monto;
            cin.ignore();
            
            nuevaFactura.detalles.push_back(detalle);
            nuevaFactura.montoTotal += detalle.monto;
        caja.facturas.push_back(nuevaFactura);
        cout << "\nFactura registrada exitosamente. Total: L. " << nuevaFactura.montoTotal << endl;
        
        cout << "¿Desea agregar otra factura? (s/n): ";
        cin >> agregarOtraFactura;
    }
}

string obtenerFechaActual() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", now);
    return string(buffer);
}

float calcularTotalGastado(const vector<Factura>& facturas) {
    float total = 0;
    for (const auto& factura : facturas) {
        total += factura.montoTotal;
    }
    return total;
}

void mostrarGastos(const vector<Factura>& facturas) {
    // Encabezados de columna
    cout << left << setw(15) << "FACTURA" 
         << left << setw(30) << "CONCEPTO" 
         << right << setw(15) << "MONTO (L.)" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (const auto& factura : facturas) {
        bool primeraLinea = true;
        
        for (const auto& detalle : factura.detalles) {
            cout << left << setw(15) << (primeraLinea ? factura.numeroFactura : "");
            cout << left << setw(30) << detalle.concepto 
                 << right << setw(15) << fixed << setprecision(2) << detalle.monto << endl;
            
            primeraLinea = false;
        }
    }

    // Mostrar total general
    float totalGeneral = calcularTotalGastado(facturas);
    cout << "------------------------------------------------------------" << endl;
    cout << left << setw(30) << "TOTAL GENERAL:" 
         << right << setw(30) << fixed << setprecision(2) << totalGeneral << endl;
}

void generarArqueo(const CajaChica& caja) {
    float totalGastado = calcularTotalGastado(caja.facturas);
    
    cout << "\n=== ARQUEO DE CAJA CHICA ===\n" << endl;
    mostrarGastos(caja.facturas);
    cout << "\nFondo Inicial:   L. " << fixed << setprecision(2) << caja.fondoInicial << endl;
    cout << "Total Gastado:   L. " << fixed << setprecision(2) << totalGastado << endl;
    cout << "Saldo en Caja:   L. " << fixed << setprecision(2) << (caja.fondoInicial - totalGastado) << endl;
    cout << "============================" << endl;
}

void envioLiquidacion(const CajaChica& caja) {
    ofstream archivo("liquidacion.txt");
    if (!archivo.is_open()) {
        cerr << "Error al crear archivo de liquidacion" << endl;
        return;
    }

    float totalGastado = calcularTotalGastado(caja.facturas);
    archivo << "=== LIQUIDACION DE CAJA CHICA ===" << endl;
    archivo << "Fecha: " << obtenerFechaActual() << "\n\n";
    archivo << "GASTOS REGISTRADOS:" << endl;
    for (const auto& factura : caja.facturas) {
        archivo << "\nFactura #" << factura.numeroFactura << "\n";
        for (const auto& detalle : factura.detalles) {
            archivo << "  - " << detalle.concepto << " (L. " << fixed << setprecision(2) << detalle.monto << ")\n";
        }
        archivo << "Total Factura: L. " << fixed << setprecision(2) << factura.montoTotal << "\n";
    }
    archivo << "\nTotal Gastado: L. " << fixed << setprecision(2) << totalGastado << "\n";
    archivo << "Saldo en caja: L. " << fixed << setprecision(2) << (caja.fondoInicial - totalGastado) << "\n";
    archivo.close();
    cout << "\nLiquidacion enviada con exito a Contabilidad." << endl;
}

void solicitudReembolso(const CajaChica& caja) {
    float totalGastado = calcularTotalGastado(caja.facturas);
    float montoReembolso;
    cout << "\nTotal gastado: L. " << totalGastado << endl;
    cout << "Ingrese la cantidad a reembolsar: L. ";
    cin >> montoReembolso;
    if (montoReembolso > totalGastado) {
        cout << "Error: La cantidad excede el monto gastado" << endl;
        return;
    }
    string firma;
    cout << "Nombre del solicitante: ";
    cin.ignore();
    getline(cin, firma);
    ofstream archivo("solicitud_reembolso.txt");
    archivo << "Fecha: " << obtenerFechaActual() << "\n";
    archivo << "Monto solicitado: L. " << montoReembolso << "\n";
    archivo << "Solicitante: " << firma << "\n";
    archivo.close();
    cout << "\nSolicitud de reembolso guardada." << endl;
}

void mostrarArchivo(const string& nombreArchivo, const string& titulo = "CONTENIDO") {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }

    cout << endl << "=== " << titulo << " ===" << endl;
    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }
    archivo.close();
}

void mostrarSolicitudReembolso() {
    mostrarArchivo("solicitud_reembolso.txt", "SOLICITUD DE REEMBOLSO");
}

void submenuCajaChica(CajaChica& caja) {
    int opcion;
    do {
        cout << "\n=== Sub Menú CAJA CHICA ===" << endl;
        cout << "1. Registrar gastos" << endl; 
        cout << "2. Elaboración arqueo" << endl; 
        cout << "3. Envio de liquidacion a Contabilidad" << endl;
        cout << "4. Elaboración de solicitud de Reembolso" << endl;  
        cout << "5. Mostrar Solicitud de Reembolso" << endl;
        cout << "6. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrarGastos(caja);
                break;
            case 2:
                generarArqueo(caja);
                break;
            case 3:
                envioLiquidacion(caja);
                break;
            case 4:
                solicitudReembolso(caja);
                break;
            case 5:
                mostrarSolicitudReembolso();
                break;
            case 6:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente nuevamente." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (opcion != 6);
}


// Estructura para el producto
struct ProductoLimpieza {
    string nombre;
    int cantidadDisponible;
    int cantidadRequerida;
    bool validado;
};

// Estructura para la solicitud
struct SolicitudLimpieza {
    string encargadoAseo;
    vector<ProductoLimpieza> productos;
    bool validada;
    bool enviadaAContabilidad;
};

// Inventario de productos de limpieza
map<string, ProductoLimpieza> inventarioLimpieza;

// Función para limpiar el buffer de entrada
void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Cargar inventario inicial de limpieza
void cargarInventarioLimpieza() {
    inventarioLimpieza["Jabon"] = {"Jabon", 50, 0, false};
    inventarioLimpieza["Desinfectante"] = {"Desinfectante", 30, 0, false};
    inventarioLimpieza["Escobas"] = {"Escobas", 10, 0, false};
    inventarioLimpieza["Trapeadores"] = {"Trapeadores", 8, 0, false};
    inventarioLimpieza["Guantes"] = {"Guantes", 100, 0, false};
}

// Mostrar inventario de limpieza
void mostrarInventarioLimpieza() {
    cout << "\n--- INVENTARIO ACTUAL DE LIMPIEZA ---\n";
    cout << setw(20) << left << "PRODUCTO" 
         << setw(15) << "DISPONIBLE" 
         << setw(15) << "REQUERIDO" 
         << setw(15) << "FALTANTE" << endl;
    cout << "-----------------------------------------------------------------\n";

    for (const auto& item : inventarioLimpieza) {
        const ProductoLimpieza& p = item.second;
        int faltante = max(0, p.cantidadRequerida - p.cantidadDisponible);
        cout << setw(20) << left << p.nombre 
             << setw(15) << p.cantidadDisponible 
             << setw(15) << p.cantidadRequerida 
             << setw(15) << faltante << endl;
    }
}

// Submenú para gestionar productos de limpieza
void menuProductosLimpieza(SolicitudLimpieza &solicitud) {
    int opcion;
    do {
        cout << "\n--- MENU DE PRODUCTOS DE LIMPIEZA ---\n";
        cout << "1. Agregar producto a la solicitud\n";
        cout << "2. Ver inventario actual\n";
        cout << "3. Finalizar y guardar solicitud\n";
        cout << "4. Cancelar y volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();

        switch (opcion) {
            case 1: {
                ProductoLimpieza p;
                cout << "\nIngrese el nombre del producto: ";
                getline(cin, p.nombre);
                
                if (inventarioLimpieza.find(p.nombre) == inventarioLimpieza.end()) {
                    cout << "Producto no existe en el inventario. Desea agregarlo? (s/n): ";
                    char op;
                    cin >> op;
                    limpiarBuffer();
                    if (tolower(op) == 's') {
                        cout << "Ingrese la cantidad disponible: ";
                        cin >> p.cantidadDisponible;
                        limpiarBuffer();
                        p.cantidadRequerida = 0;
                        inventarioLimpieza[p.nombre] = p;
                    } else {
                        break;
                    }
                }
                
                cout << "Ingrese la cantidad requerida: ";
                cin >> p.cantidadRequerida;
                limpiarBuffer();
                
                inventarioLimpieza[p.nombre].cantidadRequerida += p.cantidadRequerida;
                solicitud.productos.push_back(p);
                
                cout << "Producto agregado correctamente.\n";
                break;
            }
            case 2: {
                mostrarInventarioLimpieza();
                break;
            }
            case 3: {
                ofstream archivo("solicitudes_limpieza.txt", ios::app);
                if (archivo.is_open()) {
                    archivo << "SOLICITUD A NOMBRE DE: " << solicitud.encargadoAseo << "\n";
                    archivo << "PRODUCTOS SOLICITADOS:\n";
                    
                    int totalUnidades = 0;
                    for (const auto& p : solicitud.productos) {
                        archivo << p.nombre << ": " << p.cantidadRequerida << " unidades\n";
                        totalUnidades += p.cantidadRequerida;
                    }
                    
                    archivo << "TOTAL UNIDADES: " << totalUnidades << "\n";
                    archivo << "ESTADO: VALIDADA - NO ENVIADA\n";
                    archivo << "---------------------------------\n";
                    archivo.close();
                    
                    cout << "\nSolicitud de limpieza guardada correctamente.\n";
                    cout << "Total de productos solicitados: " << solicitud.productos.size() << "\n";
                    cout << "Total de unidades solicitadas: " << totalUnidades << "\n";
                } else {
                    cout << "Error al guardar la solicitud.\n";
                }
                return;
            }
            case 4: {
                cout << "Cancelando solicitud...\n";
                return;
            }
            default: {
                cout << "Opcion invalida. Intente de nuevo.\n";
            }
        }
    } while (true);
}

// Registrar solicitud de limpieza
void solicitarImplementosLimpieza() {
    SolicitudLimpieza solicitud;
    cout << "\nIngrese el nombre de la encargada de aseo: ";
    getline(cin, solicitud.encargadoAseo);
    
    solicitud.validada = true;
    solicitud.enviadaAContabilidad = false;
    
    menuProductosLimpieza(solicitud);
}

// Enviar solicitud de limpieza a contabilidad
void enviarLimpiezaAContabilidad() {
    ifstream archivoLectura("solicitudes_limpieza.txt");
    ofstream archivoTemporal("temp_limpieza.txt");
    string linea;
    bool actualizada = false;

    if (!archivoLectura || !archivoTemporal) {
        cout << "Error al abrir archivos.\n";
        return;
    }

    while (getline(archivoLectura, linea)) {
        if (linea.find("NO ENVIADA") != string::npos && !actualizada) {
            size_t pos = linea.find("NO ENVIADA");
            linea.replace(pos, 10, "ENVIADA");
            actualizada = true;
            cout << "Solicitud de limpieza enviada a contabilidad.\n";
            
            for (auto& item : inventarioLimpieza) {
                item.second.cantidadDisponible -= item.second.cantidadRequerida;
                item.second.cantidadRequerida = 0;
            }
        }
        archivoTemporal << linea << endl;
    }

    archivoLectura.close();
    archivoTemporal.close();

    remove("solicitudes_limpieza.txt");
    rename("temp_limpieza.txt", "solicitudes_limpieza.txt");

    if (!actualizada) {
        cout << "\nNo hay solicitudes de limpieza pendientes para enviar.\n";
    }
}

// Supervisar tareas de limpieza
void supervisarLimpieza() {
    ifstream archivo("solicitudes_limpieza.txt");
    string linea;
    bool supervisado = false;

    if (!archivo.is_open()) {  // Mejor forma de verificar si se abrió el archivo
        cout << "No se pudo abrir el archivo de solicitudes de limpieza.\n";
        return;
    }

    cout << "\n--- SUPERVISION DE LIMPIEZA ---\n";
    while (getline(archivo, linea)) {
        if (linea.find("ENVIADA") != string::npos) {
            cout << "Solicitud de limpieza supervisada:\n";
            // Mostrar los detalles de la solicitud
            cout << linea << endl;
            for (int i = 0; i < 5 && getline(archivo, linea); i++) {
                cout << linea << endl;
            }
            supervisado = true;
            // No break para encontrar todas las solicitudes ENVIADAS
        }
    }

    archivo.close();

    if (!supervisado) {
        cout << "No hay solicitudes de limpieza enviadas para supervisar.\n";
    } else {
        cout << "Supervision de limpieza completada.\n";
    }
}

// Mostrar reporte de productos de limpieza
void mostrarReporteLimpieza() {
    cout << "\n--- REPORTE DE PRODUCTOS DE LIMPIEZA ---\n";
    mostrarInventarioLimpieza();
    
    int totalFaltantes = 0;
    int totalUnidades = 0;
    
    for (const auto& item : inventarioLimpieza) {
        const ProductoLimpieza& p = item.second;
        int faltante = max(0, p.cantidadRequerida - p.cantidadDisponible);
        totalFaltantes += faltante;
        totalUnidades += p.cantidadRequerida;
    }
    
    cout << "\nRESUMEN:\n";
    cout << "Total de productos en inventario: " << inventarioLimpieza.size() << endl;
    cout << "Total de unidades requeridas: " << totalUnidades << endl;
    cout << "Total de unidades faltantes: " << totalFaltantes << endl;
}

// Menu principal de limpieza y aseo
void menuLimpiezayAseo() {
    int opcion;
    cargarInventarioLimpieza();

    do {
        cout << "\n--- SUB MENU DE LIMPIEZA Y ASEO ---\n";
        cout << "1. Registrar solicitud de productos\n";
        cout << "2. Enviar solicitud a contabilidad\n";
        cout << "3. Mostrar inventario y reportes\n";
        cout << "4. Supervisar tareas de limpieza\n";
        cout << "5. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        
        cin >> opcion;
        limpiarBuffer();

        switch (opcion) {
            case 1:
                solicitarImplementosLimpieza();
                break;
            case 2:
                enviarLimpiezaAContabilidad();
                break;
            case 3:
                mostrarReporteLimpieza();
                break;
            case 4:
                supervisarLimpieza();
                break;
            case 5:
                cout << "Volviendo al menu principal...\n";
                return;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }

    } while (true);
}


int main() {
    string username, password, tipoUsuario;
    int intentos = 0;
    const int maxIntentos = 2; // Número máximo de intentos permitidos

    do {
        cout << "\n=== BIENVENIDO AL SISTEMA DE RRHH METRONET ===" << endl;
        cout << "\n=== INICIE SECIÓN ===" << endl;
        cout << "Usuario: ";
        cin >> username;
        password = ocultarContraseña(); // Usar la función para ocultar la contraseña

        if (autenticarUsuario(username, password, tipoUsuario)) {
            break; 
        } else {
            intentos++;
            if (intentos < maxIntentos) {
                cout << "Autenticación fallida. Intente nuevamente (" << maxIntentos - intentos << " intento(s) restante(s)).\n";
            } else {
                cout << "Autenticación fallida. Ha excedido el número máximo de intentos.\n";
                return 1; 
            }
        }
    } while (intentos < maxIntentos);

    vector<Empleado> empleados = cargarEmpleadosDesdeArchivo();
    
    int opcion;
    do {
        cout << "\n=== Menú Principal PROCESO RRHH METRONET ===" << endl;
        cout << "1. Gestión de Personal" << endl;
        cout << "2. Caja Chica" << endl;
        cout << "3. Elaboración de Planilla" << endl;
        cout << "4. Limpieza y Aseo" << endl;
        cout << "5. Capacitaciones" << endl;
        cout << "6. Gestión de Usuarios" << endl;
        cout << "7. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;
// Declarar la variable fuera del switch
        CajaChica caja;
        switch (opcion) {
            case 1:
                submenugestionempleados(empleados);
                break;
            case 2: {
                // Inicializar caja chica dentro del bloque
                float fondoInicial;
                cout << "\nIngrese el fondo inicial para la caja chica: L. ";
                cin >> fondoInicial;
        
                caja = {fondoInicial, {}}; // Asignación correcta
                submenuCajaChica(caja);
                break;
            }
            case 3: {
                int subOpcion;
                do {
                    cout << "\n=== Submenú Elaboración de Planilla ===" << endl;
                    cout << "1. Ingreso datos de Empleados a Planilla" << endl;
                    cout << "2. Actualización de Empleados" << endl;
                    cout << "3. Recepción de pago de Bonificaciones" << endl;
                    cout << "4. Enviar Planilla a Contabilidad" << endl;
                    cout << "5. Volver al menú principal" << endl;
                    cout << "Seleccione una opción: ";
                    cin >> subOpcion;
        
                    switch (subOpcion) {
                        case 1:
                            cout << "\n*** Elaboración de Planilla *** " << endl;
                            agregarEmpleado(empleados);
                            calcularPlanilla(empleados);
                            guardarPlanillaEnArchivo(empleados);
                            break;
                        case 2:
                            cout << "=== Actualización de Empleados ===" << endl;
                            modificarEmpleado(empleados);
                            calcularPlanilla(empleados);
                            break;
                        case 3:
                            cout << "=== Recepción de Bonificaciones ===" << endl;
                            agregarBonificacion(empleados);
                            calcularPlanilla(empleados);
                            break;
                        case 4:
                            cout << "\n=== Enviar a Contabilidad ===" << endl;
                            imprimirPlanilla(empleados);
                            enviarAContabilidad(empleados);
                            break;
                        case 5:
                            cout << "Volviendo al menú principal..." << endl;
                            break;
                        default:
                            cout << "Opción no válida. Intente nuevamente." << endl;
                    }
                } while (subOpcion != 5);
                break;
            }
            case 4:
                menuLimpiezayAseo();
                break;
            case 5:
                menuCapacitaciones();
                break;
            case 6:
                submenuUsuarios(empleados);
                break;
            case 7:
                cout << "Saliendo del sistema..." << endl;
                break;
            default:
                cout << "Opción no válida. Intente nuevamente." << endl;
        }
            } while (opcion != 7);
        
            return 0;
}