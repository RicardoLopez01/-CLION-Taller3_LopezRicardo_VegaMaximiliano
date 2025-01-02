#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

// Función para convertir la intensidad de un píxel a un carácter ASCII
char obtenerCarácterASCII(int valor) {
    const char* caracteres = "@#8%&o-+  "; // Caracteres más densos y claros
    int index = (valor * (strlen(caracteres) - 1)) / 255;
    return caracteres[index];
}

// Función para mostrar una imagen como ASCII
void mostrarFrameComoASCII(unsigned char* img, int width, int height, int channels) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int r = img[(y * width + x) * channels + 0];
            int g = img[(y * width + x) * channels + 1];
            int b = img[(y * width + x) * channels + 2];

            int gris = static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b); // Luminancia estándar
            char asciiChar = obtenerCarácterASCII(gris);
            std::cout << asciiChar;
        }
        std::cout << std::endl;
    }
}


void procesarFrame(const std::string& framePath) {
    int width, height, channels;
    unsigned char* img = stbi_load(framePath.c_str(), &width, &height, &channels, 0);

    if (img == nullptr) {
        std::cerr << "No se pudo cargar la imagen: " << framePath << std::endl;
        return;
    }

    mostrarFrameComoASCII(img, width, height, channels);
    stbi_image_free(img);
}


void reproducirVideo(const std::string& carpetaFrames) {
    std::vector<std::string> frames;
    for (const auto& entry : fs::directory_iterator(carpetaFrames)) {
        if (entry.path().extension() == ".jpg") {
            frames.push_back(entry.path().string());
        }
    }

    if (frames.empty()) {
        std::cerr << "No se encontraron archivos JPG en la carpeta de frames." << std::endl;
        return;
    }

    // Calcular el tiempo por frame para 30 fps (aprox 33 por frame)
    double tiempoPorFrame = 1000.0 / 30; // 30 fps


    for (size_t i = 0; i < frames.size(); ++i) {
        system("cls");
        procesarFrame(frames[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tiempoPorFrame)));
    }
}

// Función para mostrar el menú
void mostrarMenu() {
    std::cout << "=== Sistema de visualizacion de video ===" << std::endl;
    std::cout << "Seleccione una opcion:" << std::endl;
    std::cout << "1. Mostrar video" << std::endl;
    std::cout << "2. Salir" << std::endl;
}

int main() {
    int opcion;
    std::string carpetaFrames;

    while (true) {
        mostrarMenu();
        std::cout << "Ingrese una opcion: ";
        std::cin >> opcion;

        if (opcion == 1) {

            std::cout << "Ingrese la ruta de la carpeta con los frames: ";
            std::cin.ignore(); // Limpiar el buffer
            std::getline(std::cin, carpetaFrames);

            if (!fs::exists(carpetaFrames) || !fs::is_directory(carpetaFrames)) {
                std::cerr << "La carpeta con los frames no existe o no es un directorio valido." << std::endl;
                continue;
            }

            // Llamar a la función para reproducir el video
            reproducirVideo(carpetaFrames);
        }
        else if (opcion == 2) {

            std::cout << "Saliendo del sistema..." << std::endl;
            break;
        }
        else {
            std::cerr << "Opcion no válida, intente de nuevo." << std::endl;
        }
    }

    return 0;
}
