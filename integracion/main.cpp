#include <iostream>
#include "operaciones_bits.h"
#include "imagenes_y_txt.h"
#include "otros.h"

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

    int n_txt = 0;
    int tamaño_IM = width * height * 3;
    int largo_mascara = width_m * height_m * 3;
    unsigned int** txt_data = cargar_txts(M, n_txt, largo_mascara);
    delete[] M;

    if (n_txt <= 0) // Verifica si se encontraron txt, en caso contrario se termina el programa
    {
        delete[] bufferA;
        delete[] bufferB;
        delete[] IM;
        return 0;
    }

    // ======== PREPARACIÓN DE CONTROL DE FLUJO ========
    int* operaciones = new int[n_txt];  // arreglo dinamico que almacena las operaciones inversas utilizadas con un tamaño de tamaño n_txt
    int* operaciones_original = new int[n_txt];  // arreglo dinamico que almacena las operaciones inversas utilizadas con un tamaño de tamaño n_txt

    unsigned char* actual = bufferA;      // apuntador al Buffer con el arreglo dinamico que contiene la informacion del paso a operar para desencriptar
    unsigned char* siguiente = bufferB;   // apuntador al Buffer con el arrglo dinamico en el se van a almacenar las transformaciones del Buffer actual para su confirmacion
    int cont_buffer = 0;                // parametro de control para intercambio de buffers en cada iteracion del buble principal de inversion
    int coincid_ref = largo_mascara;
    int coincid_shift = 0;


    // ======== BUCLE PRINCIPAL DE INVERSIÓN ========
    for (int i = n_txt - 1; i >= 0; i--)
    {
        alternar_buffers(bufferA, bufferB, cont_buffer, actual, siguiente);

        unsigned int* txt_i        = txt_data[i];
        int semilla_actual         = txt_i[0];
        bool operacion_valida      = false;        // variable de control que permite saber si ya se encontro la operacion correspondiente al paso actual
        int  t_detect              = 0;

        bool es_shift = detectar_shift(actual,siguiente,
                                       txt_i,semilla_actual,
                                       largo_mascara,
                                       IM,
                                       tamaño_IM,t_detect,coincid_shift );

        if (es_shift)
        {
            operaciones[i] = t_detect;

            aplicar_operacion_inversa(actual,t_detect,siguiente,
                                      IM,tamaño_IM);
            agregar_region_mascara(txt_i,siguiente,semilla_actual,
                                  largo_mascara);

            operacion_valida = true;
            coincid_ref = coincid_shift;
        }
        else if (t_detect == -1)
        {
            operaciones[i] = -1;

            copiar_region_mascara(actual,siguiente,semilla_actual,
                                  largo_mascara,tamaño_IM);
            operaciones[i]  = -2;

            operacion_valida = true;
        }
        else
        {
            operacion_valida = buscar_xor_rot( actual,siguiente,txt_i,semilla_actual,largo_mascara,
                                              IM,tamaño_IM,operaciones[i],coincid_ref);
        }

        if (!operacion_valida)
        {
            delete[] bufferA; delete[] bufferB; delete[] IM;

            delete[] operaciones;
            delete[] operaciones_original;

            for (int k = 0; k < n_txt; k++)delete[] txt_data[k];
            delete[] txt_data;

            return 0;
        }
        operaciones_original[i] = obtener_inverso_t(operaciones[i]);

        cont_buffer++;
    }
    for (int i = 0; i <= n_txt-1 ; i++)
    {
        cout << "La operacion original del paso " << i+1 << " puede ser : " << operaciones_original[i] << endl;
    }


    // ======== EXPORTACIÓN Y LIMPIEZA ========
    if (siguiente != nullptr) exportImage(siguiente, width, height, "I_O_reconstruida.bmp"); // Se exporta la imagen roconstruida
    delete[] bufferA;
    delete[] bufferB;
    delete[] IM;

    for (int i = 0; i < n_txt; i++) delete[] txt_data[i]; // se libera la memoria dedicada a los txt
    delete[] txt_data;                                    // se libera la memoria a los apuntadores de los txt
    delete[] operaciones;                                 // se libera la memoria dedicada a guardar las operaciones encontradas
    delete[] operaciones_original;

    return 0;
}
