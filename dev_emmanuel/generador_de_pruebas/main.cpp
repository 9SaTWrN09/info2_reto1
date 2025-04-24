#include <iostream>
#include <fstream>
#include "funciones.h"
#include "imagenes_y_txt.h"

using namespace std;

void generar_txt_enmascarado_lineal
    (unsigned char* imagen,unsigned char* mascara,int semilla,int largo_mascara,char* nombre_txt);

void generar_prueba_completa
    (const char* ruta_I_O,const char* ruta_mascara,unsigned int* operaciones,int num_transform,int* semillas);

int main()
{
    int longit = 7;
    unsigned int operac[] = {1, 13, 20, 15, 1, 15,8};
    int semillas[7] = {4067790, 797394, 1189581, 3753699, 3444696,3749487,3036228};

    generar_prueba_completa
        (
        "archivos/I_O.bmp",    // Imagen original
        "archivos/M.bmp",      // Máscara
        operac,       // operaciones
        longit,       // cantidad de transformaciones
        semillas         // Semilla
        );

    return 0;
}

void generar_prueba_completa
    (
    const char* ruta_I_O,
    const char* ruta_mascara,
    unsigned int* operaciones,
    int num_transform,
    int* semillas
    )
{
    // Cargar imágenes

    int width, height, mask_width, mask_height;

    unsigned char* bufferA = loadPixels(QString(ruta_I_O), width, height); // Buffer con I_I
    if (bufferA != nullptr) cout << "I_O cargada correctamente" << endl;else cout << "Fallo en cargo de I_D" << endl;
    unsigned char* bufferB = new unsigned char[width * height * 3];         // Buffer para almacenar la imagen transformada

    unsigned char* IM      = loadPixels("archivos/I_M.bmp", width, height);    // IM utilizada en XOR

    unsigned char* mascara = loadPixels(QString(ruta_mascara), mask_width, mask_height);
    if (mascara != nullptr) cout << "mascara cargada correctamente" << endl;else cout << "Fallo en cargo de mascara" << endl;

    unsigned char* actual = bufferA;      // apuntador al Buffer con el arreglo dinamico que contiene la informacion del paso a operar para desencriptar
    unsigned char* siguiente = bufferB;   // apuntador al Buffer con el arrglo dinamico en el se van a almacenar las transformaciones del Buffer actual para su confirmacion

    int largo_mascara = mask_width * mask_height * 3; cout << "mask large" << largo_mascara << endl;
    unsigned int transform_actual = 0;
    int cont_buffer = 0;

    int t = 0;

    char ruta_dinamic[20];
    generar_nombre_prueba(t,ruta_dinamic);

    char nombre_archivos[32];
    generar_nombre_txt(t,nombre_archivos);

    exportImage(actual, width, height, QString(ruta_dinamic));
    generar_txt_enmascarado_lineal(actual, mascara, 3036228, largo_mascara, nombre_archivos);

    cout << "t = |" << t << "|" << endl;

    for (t = 0; t <= num_transform-1; t++ )
    {
        if (cont_buffer % 2)
        {
            actual = bufferB;
            siguiente = bufferA;
            // cout << "Alternar buffers: actual = bufferB; siguiente = bufferA;" << endl;
        }
        else
        {
            actual = bufferA;
            siguiente = bufferB;
            // cout << "Alternar buffers: actual = bufferA; siguiente = bufferB;" << endl;
        }
        transform_actual = operaciones[t];
        int semilla_actual = semillas[t];

        char ruta_dinamic[20];
        generar_nombre_prueba(t+1,ruta_dinamic);

        char nombre_archivos[32];
        generar_nombre_txt(t+1,nombre_archivos);

        if (semilla_actual + largo_mascara > width * height * 3)
        {
            cerr << "Error: la semilla + tamaño de máscara excede el tamaño de la imagen." << endl;
            return;
        }

        aplicar_operacion_inversa(actual, transform_actual, siguiente, IM, width * height * 3); // funcion que aplica la operacion correspondiente al t del ciclo al arreglo actual y lo almacena en siguiente


        if (t < num_transform-1)
        {
            generar_txt_enmascarado_lineal(siguiente, mascara, semilla_actual, largo_mascara, nombre_archivos);
            exportImage(siguiente, width, height, QString(ruta_dinamic));
        }

        cout << "t = |" << t << "|" << endl;
        cont_buffer++;
    }
    exportImage(siguiente, width, height, "archivos/pruebas/I_D.bmp");
    // Liberar memoria
    delete[] bufferA;
    delete[] bufferB;
    delete[] IM;
    delete[] mascara;
}

void generar_txt_enmascarado_lineal
    (
        unsigned char* imagen,
        unsigned char* mascara,
        int semilla,
        int largo_mascara,
        char* nombre_txt
        )
{
    ofstream archivo(nombre_txt);
    if (!archivo.is_open())
    {
        cerr << "No se pudo crear " << nombre_txt << endl;
        return;
    }

    archivo << semilla << endl;

    for (int i = 0; i < largo_mascara; i += 3)
    {
        int r = imagen[semilla + i]     + mascara[i];
        int g = imagen[semilla + i + 1] + mascara[i + 1];
        int b = imagen[semilla + i + 2] + mascara[i + 2];

        archivo << r << " " << g << " " << b << endl;
    }

    archivo.close();
}
