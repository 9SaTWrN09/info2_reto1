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
    if (bufferA == nullptr)
    {
        cout << "Error: No se pudo cargar 'archivos/I_D.bmp'" << endl;
    }
    else
    {
        cout << "Imagen ID cargada correctamente. Dimensiones: "
             << width << " x " << height << endl;
    }

    if (IM == nullptr)
    {
        cout << "Error: No se pudo cargar 'archivos/I_M.bmp'" << endl;
    }
    else
    {
        cout << "Imagen IM cargada correctamente." << endl;
    }

    if (M == nullptr)
    {
        cout << "Error: No se pudo cargar 'archivos/M.bmp'" << endl;
    }
    else
    {
        cout << "Mascara M cargada correctamente. Dimensiones: "
             << width_m << " x " << height_m << endl;
    }


    // ======== LECTURA Y DESENMASCARAMIENTO DE LOS TXT ========
    int n_txt = 0; // Cantidad de archivos .txt
    int largo_mascara = width_m * height_m * 3; // i * j * 3 (bytes)
    unsigned int** txt_data = nullptr; // Matriz para almacenar los datos desenmascarados

    // cargar_txts() es una función auxiliar que se implementará para cargar y desenmascarar los txts
    txt_data = cargar_txts("archivos/txt/", M, n_txt, largo_mascara);
    delete[] M;

    if (n_txt <= 0)
    {
        cout << "No se encontraron archivos .txt validos." << endl;
        return 1;
    }
    else cout << "Se encontraron " << n_txt << " txts" << endl;

    // ======== PREPARACIÓN DE CONTROL DE FLUJO ========
    int* operaciones = new int[n_txt - 1]; // Arreglo dinámico para guardar operaciones inversas, hacer la función inversa de las inversas!!
    int semilla_actual = 0;
    int flag_shift_detectado = 0;

    unsigned char* actual = bufferA;
    unsigned char* siguiente = bufferB;

    // ======== BUCLE PRINCIPAL DE INVERSIÓN ========
    for (int i = n_txt-1; i >= 0; i--)
    //for (int i = n_txt-1; i >= 0; i--)
    {
        unsigned int* txt_i = txt_data[i];
        semilla_actual = txt_i[0];

        // Alternar buffers
        if (i % 2 == 0) { actual = bufferA; siguiente = bufferB; cout << "alternar buffers" << " actual = bufferA; siguiente = bufferB; " <<  endl;}
        else            { actual = bufferB; siguiente = bufferA; cout << "alternar buffers" << " actual = bufferB; siguiente = bufferA; " <<  endl;}

        // Aplicar transformaciones inversas y verificar
        bool operacion_valida = false;

        for (int t = 1; t <= 33 && !operacion_valida; t++)
        {

            aplicar_operacion_inversa(actual, t, siguiente, IM, width * height * 3);

            int coincidencias = verificar_enmascaramiento(siguiente, txt_i, semilla_actual, largo_mascara);

            if (coincidencias > 0)
            {
                if(coincidencias == largo_mascara)
                {
                    operacion_valida = true;
                    operaciones[i] = t; // Guardar operación usada
                }
                else;
                {
                    if((coincidencias < largo_mascara) && (t > 17) ){
                    flag_shift_detectado++;
                    }
                }
            }

            cout << "txt numero : |" << i << "| t = |" << t << "| Coincidencias : |" << coincidencias << "| flag_shif :" << flag_shift_detectado << endl;
        }

        if (!operacion_valida)
        {
            cout << "Fallo en la inversión en paso " << i << endl;
            break;
        }
    }

    // ======== RESULTADO FINAL (opcional exportación de prueba) ========
    exportImage(siguiente, width, height, "reconstruida-siguiente-.bmp"); // para validar visualmente
    exportImage(actual, width, height, "reconstruida-actual-.bmp"); // para validar visualmente

    // ======== LIBERAR MEMORIA ========
    delete[] bufferA;
    delete[] bufferB;
    delete[] IM;

    for (int i = 0; i < n_txt; i++) delete[] txt_data[i];
    delete[] txt_data;
    delete[] operaciones;

    return 0;
}

