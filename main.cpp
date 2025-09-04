#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>

// --- DECLARACIÓN DE FUNCIONES AUXILIARES ---
void limpiar_buffer_entrada();
bool es_ip_valida(const std::string& ip_str);


// ===================================================================
// FUNCIÓN PRINCIPAL
// ===================================================================
int main() {
    // --- 1. Declaración de variables ---
    std::string ip_objetivo;
    std::string nombre_archivo = "resultado.txt"; 
    int sensibilidad = 1; 
    std::vector<int> puertos_a_escanear;

    // --- Mensaje de bienvenida ---
    std::cout << "========================================" << std::endl;
    std::cout << "   Herramienta de Escaneo de Puertos" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // --- 2. Pedir la Dirección IP (CON BUCLE DE VALIDACIÓN) ---
    while (true) {
        std::cout << "-> Introduce la direccion IP objetivo (ej. 127.0.0.1): ";
        std::getline(std::cin, ip_objetivo);
        if (es_ip_valida(ip_objetivo)) {
            break; 
        } else {
            std::cerr << "Error: La direccion IP introducida no tiene un formato valido. Intentalo de nuevo." << std::endl;
        }
    }

    // --- 3. Pedir la lista o rango de puertos ---
    int opcion_puertos = 0;
    while (opcion_puertos != 1 && opcion_puertos != 2) {
        std::cout << "\nSelecciona el modo de escaneo de puertos:" << std::endl;
        std::cout << "  1. Escanear un rango de puertos (ej. 1-1024)" << std::endl;
        std::cout << "  2. Escanear una lista especifica de puertos (ej. 80 443 8080)" << std::endl;
        std::cout << "Elige una opcion (1 o 2): ";
        std::cin >> opcion_puertos;

        if (std::cin.fail() || (opcion_puertos != 1 && opcion_puertos != 2)) {
            std::cerr << "Error: Opcion invalida. Por favor, introduce 1 o 2." << std::endl;
            limpiar_buffer_entrada();
            opcion_puertos = 0;
        }
    }

    limpiar_buffer_entrada();

    if (opcion_puertos == 1) { 
        int puerto_inicial = 0, puerto_final = 0;
        bool rango_valido = false;
        while (!rango_valido) {
            std::cout << "Introduce el puerto inicial del rango: ";
            std::cin >> puerto_inicial;
            std::cout << "Introduce el puerto final del rango: ";
            std::cin >> puerto_final;

            if (std::cin.fail() || puerto_inicial <= 0 || puerto_final <= 0 || puerto_final < puerto_inicial || puerto_final > 65535) {
                std::cerr << "Error: Rango invalido. Asegurate de que los puertos esten entre 1 y 65535 y que el inicial sea menor o igual al final." << std::endl;
                limpiar_buffer_entrada();
            } else {
                rango_valido = true;
            }
        }
        
        for (int i = puerto_inicial; i <= puerto_final; ++i) {
            puertos_a_escanear.push_back(i);
        }
        limpiar_buffer_entrada();

    } else { 
        while (true) {
            std::cout << "Introduce los puertos separados por espacios y presiona Enter (ej. 80 443 3306): ";
            std::string linea_puertos;
            std::getline(std::cin, linea_puertos);
            std::stringstream ss(linea_puertos);
            
            int puerto_num;
            puertos_a_escanear.clear(); 

            while (ss >> puerto_num) {
                if (puerto_num > 0 && puerto_num <= 65535) {
                    puertos_a_escanear.push_back(puerto_num);
                }
            }
            
            if (!puertos_a_escanear.empty()) {
                break; 
            } else {
                std::cerr << "Error: Debes introducir al menos un numero de puerto valido. Intentalo de nuevo." << std::endl;
            }
        }
    }
    
    // --- 4. (Opcional) Pedir Nivel de Sensibilidad ---
    std::cout << "\n-> (Opcional) Introduce un nivel de sensibilidad (1-3) o presiona Enter para usar el valor por defecto (1): ";
    std::string linea_sensibilidad;
    std::getline(std::cin, linea_sensibilidad);

    if (!linea_sensibilidad.empty()) {
        try {
            int temp_sensibilidad = std::stoi(linea_sensibilidad);
            if (temp_sensibilidad >= 1 && temp_sensibilidad <= 3) {
                 sensibilidad = temp_sensibilidad;
            } else {
                std::cout << "Valor fuera de rango. Usando sensibilidad por defecto (1)." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Entrada invalida. Usando sensibilidad por defecto (1)." << std::endl;
        }
    }

    // --- 5. Mostrar resumen final para verificar ---
    std::cout << "\n--- VERIFICACION DE DATOS INGRESADOS ---" << std::endl;
    std::cout << "IP Objetivo:         " << ip_objetivo << std::endl;
    std::cout << "Archivo de Salida:   " << nombre_archivo << std::endl;
    std::cout << "Nivel de Sensibilidad: " << sensibilidad << std::endl;
    
    // <<--- CAMBIO APLICADO AQUÍ ---
    std::cout << "Puertos a Escanear (" << puertos_a_escanear.size() << "): ";
    for (size_t i = 0; i < puertos_a_escanear.size(); ++i) {
        std::cout << puertos_a_escanear[i];
        // Añade una coma y un espacio, excepto para el último elemento
        if (i < puertos_a_escanear.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl; // Salto de línea final
    std::cout << "----------------------------------------" << std::endl;
    
    std::cout << "\nIniciando escaneo..." << std::endl;

    // Aquí irían las llamadas a tus otros módulos
    
    return 0;
}



// ===================================================================
// IMPLEMENTACIÓN DE FUNCIONES AUXILIARES
// ===================================================================
void limpiar_buffer_entrada() {
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool es_ip_valida(const std::string& ip_str) {
    std::stringstream ss(ip_str);
    std::string segmento;
    int contador_segmentos = 0;
    
    while(std::getline(ss, segmento, '.')) {
        contador_segmentos++;
        if (contador_segmentos > 4) return false;
        
        try {
            int valor = std::stoi(segmento);
            if (valor < 0 || valor > 255) return false;
            if (std::to_string(valor) != segmento) return false;
        } catch (const std::exception& e) {
            return false;
        }
    }
    
    return contador_segmentos == 4;
}
