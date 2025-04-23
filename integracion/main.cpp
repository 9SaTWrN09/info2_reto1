#include <iostream>
#include "funciones.h"
#include "imagenes_y_txt.h"

using namespace std;

int main()
{
    // ======== CARGA DE IMÁGENES Y DATOS ========
    int width = 0, height = 0;
    int width_m = 0, height_m = 0;

    unsigned char* bufferA = loadPixels("archivos/I_D.bmp", width, height); // Buffer con I_I
    unsigned char* bufferB = new unsigned char[width * height * 3];         // Buffer para almacenar la imagen transformada

    unsigned char* IM      = loadPixels("archivos/I_M.bmp", width, height);    // IM utilizada en XOR
    unsigned char* M       = loadPixels("archivos/M.bmp", width_m, height_m);  // Mascara
/*
    // ======== VERIFICACIÓN DE CARGA DE ARCHIVOS ========
    if (bufferA == nullptr) cout << "Error: No se pudo cargar 'archivos/I_D.bmp'" << endl;
    else cout << "Imagen ID cargada correctamente. Dimensiones: " << width << " x " << height << endl;

    if (IM == nullptr) cout << "Error: No se pudo cargar 'archivos/I_M.bmp'" << endl;
    else cout << "Imagen IM cargada correctamente." << endl;

    if (M == nullptr) cout << "Error: No se pudo cargar 'archivos/M.bmp'" << endl;
    else cout << "Mascara M cargada correctamente. Dimensiones: " << width_m << " x " << height_m << endl;
*/
    // ======== LECTURA Y DESENMASCARAMIENTO DE LOS TXT ========
    int n_txt = 0;
    int largo_mascara = width_m * height_m * 3;
    unsigned int** txt_data = cargar_txts("archivos/txt/", M, n_txt, largo_mascara);
    delete[] M;

    if (n_txt <= 0) // Verifica si se encontraron txt, en caso contrario se termina el programa
    {
        delete[] bufferA;
        delete[] bufferB;
        delete[] IM;
        cout << "No se encontraron archivos .txt validos." << endl;
        return 0;
    }
    else cout << "Se encontraron " << n_txt << " txts" << endl;

    // ======== PREPARACIÓN DE CONTROL DE FLUJO ========
    int* operaciones = new int[n_txt];  // arreglo dinamico que almacena las operaciones inversas utilizadas con un tamaño de tamaño n_txt
    int cont_buffer = 0;                // parametro de control para intercambio de buffers en cada iteracion del buble principal de inversion

    unsigned char* actual = bufferA;      // apuntador al Buffer con el arreglo dinamico que contiene la informacion del paso a operar para desencriptar
    unsigned char* siguiente = bufferB;   // apuntador al Buffer con el arrglo dinamico en el se van a almacenar las transformaciones del Buffer actual para su confirmacion
    bool operacion_valida = false;        // variable de control que permite saber si ya se encontro la operacion correspondiente al paso actual

    // ======== BUCLE PRINCIPAL DE INVERSIÓN ========
    for (int i = n_txt-1; i >= 0; i--)
    {
        // se itera en la cantidad de txt empezando con el ultimo
        unsigned int* txt_i = txt_data[i];        // apuntador al txt del ciclo correspondiente
        unsigned int semilla_actual = txt_i[0];   // variable con la semilla del txt del ciclo actual

        // Alternar buffers : Se encarga de intercambiar los Buffers en cada iteracion
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

        operacion_valida = false;   // se inicia en false porque no se ha encontrado la transformacion del ciclo actaul
        int coincidencia_1 = 0;     // variable de control para detectar desplazamientos

        for (int t = 1; t <= 33 && !operacion_valida; t++)
        {
            // Se itera en las 33 posible operaciones utilizadas en la encriptacion
            // t = 1 : XOR
            // 2  <= t <= 9  Rotacion de t-1 a la derecha
            // 10 <= t <= 17 Rotacion de t-9 a la izquierda
            // 18 <= t <= 25 Desplazamiento de t-17 a la derecha
            // 26 <= t <= 33 Desplazamiento de t-25 a la izquierda

            aplicar_operacion_inversa(actual, t, siguiente, IM, width * height * 3); // funcion que aplica la operacion correspondiente al t del ciclo al arreglo actual y lo almacena en siguiente
            int coincidencias = verificar_enmascaramiento(siguiente, txt_i, semilla_actual, largo_mascara); // funcion que verfica byte a byte si la operacion realizada a actual y almacenada en siguiente coincide con los detos del txt del ciclo y devuelve que cantas coincidencias hubieron

            if (coincidencias == largo_mascara) // Si hubo un XOR o una Rotaciones como estos no tienen perdida de datos, debe haber una coincidencia en todos los bytes para que la operacion sea la correcta
            {
                operacion_valida = true; // se activa la variable de control
                operaciones[i] = t;      // se almacena la operacion encontrada
            }

            if (t == 17) coincidencia_1 = coincidencias; // se toma la cantidad de coincidencias que hibieron con la operacion de rotacion de 8 bits a izquierda por si esta fallo y se necesita encontrar el valor del desplazamiento

            if (t >= 18)  // Si estamos verificando desplazamientos
            {
                if (coincidencia_1 != coincidencias) // si el desplazamiento corresponde a una coincidencia mayor a la norma
                {
                    cout << "t = |"               << t              << "|" << endl
                         << "coincidencias 1 = |" << coincidencia_1 << "|" << endl
                         << "coincidencias 2 = |" << coincidencias << "|" << endl;
                    operacion_valida = true; // se activa la variable de control
                    operaciones[i] = t;      // se almacena la operacion encontrada
                }
            }

            // Línea de log dentro del if(coincidencias > 0)
            cout << "txt: |"            << i             << "| t = |" << t << "|"
                 << " Coincidencias: |" << coincidencias << "|" << endl;
        }

        if (!operacion_valida) // detecta si no se pudo encontrar una operacion valida y lanza un mensaje
        {
            cout << "Fallo en la inversión en paso " << i << endl;
            return 0;
        }
        cont_buffer++;  // Se suma una unidad al parametro de control de intercambio de buffer
    }

    // ======== EXPORTACIÓN Y LIMPIEZA ========
    if (siguiente != nullptr) exportImage(siguiente, width, height, "I_O_reconstruida.bmp"); // Se exporta la imagen roconstruida
    delete[] bufferA;
    delete[] bufferB;
    delete[] IM;

    for (int i = 0; i < n_txt; i++) delete[] txt_data[i]; // se libera la memoria dedicada a los txt
    delete[] txt_data;                                    // se libera la memoria a los apuntadores de los txt
    delete[] operaciones;                                 // se libera la memoria dedicada a guardar las operaciones encontradas

    return 0;
}
