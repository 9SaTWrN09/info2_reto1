#include <iostream>
#include "funciones.h"
#include "imagenes_y_txt.h"

using namespace std;

int main()
{
    // ======== CARGA DE IMÁGENES Y DATOS ========
    int width = 0, height = 0;
    int width_m = 0, height_m = 0;

    unsigned char* bufferA = loadPixels("archivos/I_D.bmp", width, height);
    unsigned char* bufferB = new unsigned char[width * height * 3];

    unsigned char* IM      = loadPixels("archivos/I_M.bmp", width, height);
    unsigned char* M       = loadPixels("archivos/M.bmp", width_m, height_m);

    // ======== VERIFICACIÓN DE CARGA DE ARCHIVOS ========
    if (bufferA == nullptr) cout << "Error: No se pudo cargar 'archivos/I_D.bmp'" << endl;
    else cout << "Imagen ID cargada correctamente. Dimensiones: " << width << " x " << height << endl;

    if (IM == nullptr) cout << "Error: No se pudo cargar 'archivos/I_M.bmp'" << endl;
    else cout << "Imagen IM cargada correctamente." << endl;

    if (M == nullptr) cout << "Error: No se pudo cargar 'archivos/M.bmp'" << endl;
    else cout << "Mascara M cargada correctamente. Dimensiones: " << width_m << " x " << height_m << endl;

    // ======== LECTURA Y DESENMASCARAMIENTO DE LOS TXT ========
    int n_txt = 0;
    int largo_mascara = width_m * height_m * 3;
    unsigned int** txt_data = cargar_txts("archivos/txt/", M, n_txt, largo_mascara);
    delete[] M;

    if (n_txt <= 0) {
        cout << "No se encontraron archivos .txt validos." << endl;
        return 1;
    }
    else cout << "Se encontraron " << n_txt << " txts" << endl;

    // ======== PREPARACIÓN DE CONTROL DE FLUJO ========
    int* operaciones = new int[n_txt];  // Corregido: tamaño n_txt
    int semilla_actual = 0;
    int flag_shift_detectado = 0;

    unsigned char* actual = bufferA;
    unsigned char* siguiente = bufferB;

    // ======== BUCLE PRINCIPAL DE INVERSIÓN ========
    for (int i = n_txt-1; i >= 0; i--)
    {
        unsigned int* txt_i = txt_data[i];
        semilla_actual = txt_i[0];

        // Alternar buffers
        if (i % 2 == 0) {
            actual = bufferA;
            siguiente = bufferB;
            cout << "Alternar buffers: actual = bufferA; siguiente = bufferB;" << endl;
        }
        else {
            actual = bufferB;
            siguiente = bufferA;
            cout << "Alternar buffers: actual = bufferB; siguiente = bufferA;" << endl;
        }

        bool operacion_valida = false;

        for (int t = 1; t <= 33 && !operacion_valida; t++)
        {
            aplicar_operacion_inversa(actual, t, siguiente, IM, width * height * 3);
            int coincidencias = verificar_enmascaramiento(siguiente, txt_i, semilla_actual, largo_mascara);

            if (coincidencias > 0)
            {
                if (coincidencias == largo_mascara)
                {
                    operacion_valida = true;
                    operaciones[i] = t;
                }
                else
                {
                    if (t > 17)  // Desplazamientos
                    {
                        int bits_perdidos = (t <= 25) ? (t - 17) : (t - 25);
                        int umbral = largo_mascara - bits_perdidos;

                        if (coincidencias >= umbral)
                        {
                            operacion_valida = true;
                            operaciones[i] = t;
                            flag_shift_detectado++;
                        }
                    }
                }

                // Línea de log dentro del if(coincidencias > 0)
                cout << "txt: |" << i << "| t = |" << t
                     << "| Coincidencias: |" << coincidencias
                     << "| flag_shift: " << flag_shift_detectado << endl;
            }
        }

        if (!operacion_valida)
        {
            cout << "Fallo en la inversión en paso " << i << endl;
            break;
        }
    }

    // ======== EXPORTACIÓN Y LIMPIEZA ========
    if (siguiente != nullptr) exportImage(siguiente, width, height, "reconstruida-siguiente.bmp");
    if (actual != nullptr) exportImage(actual, width, height, "reconstruida-actual.bmp");

    delete[] bufferA;
    delete[] bufferB;
    delete[] IM;

    for (int i = 0; i < n_txt; i++) delete[] txt_data[i];
    delete[] txt_data;
    delete[] operaciones;

    return 0;
}
