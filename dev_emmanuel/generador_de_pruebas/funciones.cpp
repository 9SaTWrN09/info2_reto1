#include "iostream"
#include "funciones.h"
#include "imagenes_y_txt.h"

using namespace std;

void aplicar_xor(unsigned char* in, unsigned char* mask, unsigned char* out, int size) {
    for (int i = 0; i < size; ++i)
    {
        out[i] = in[i] ^ mask[i];
    }
}

void rotar_izquierda(unsigned char* in, int bits, unsigned char* out, int size) {
    for (int i = 0; i < size; ++i)
    {
        out[i] = (in[i] << bits) | (in[i] >> (8 - bits));
    }
}

void rotar_derecha(unsigned char* in, int bits, unsigned char* out, int size) {
    for (int i = 0; i < size; ++i)
    {
        out[i] = (in[i] >> bits) | (in[i] << (8 - bits));
    }
}

void desplazar_izquierda(unsigned char* in, int bits, unsigned char* out, int size) {
    for (int i = 0; i < size; ++i)
    {
        out[i] = in[i] << bits;
    }
}

void desplazar_derecha(unsigned char* in, int bits, unsigned char* out, int size) {
    for (int i = 0; i < size; ++i)
    {
        out[i] = in[i] >> bits;
    }
}

/**
 * @brief Genera el nombre de archivo con prefijo "archivos/txt/M" seguido del número sin ceros a la izquierda.
 *
 * @param numero Número entre 0 y 100 inclusive.
 * @param nombreArchivo Puntero a arreglo dinámico de mínimo 32 caracteres donde se escribirá el nombre.
 */
void generar_nombre_txt(int numero, char* nombreArchivo)
{
    // Borrar contenido previo
    for (int i = 0; i < 32; i++)
    {
        nombreArchivo[i] = '\0';
    }

    // Prefijo fijo
    const char* prefijo = "archivos/txt/M";
    int i = 0;

    while (prefijo[i] != '\0')
    {
        nombreArchivo[i] = prefijo[i];
        i++;
    }

    // Conversión del número (sin ceros a la izquierda)
    // Usamos una técnica sin STL ni sprintf: construimos el número como texto


    if (numero == 0)
    {
        nombreArchivo[i++] = '0';
    }
    else
    {
        int temp = numero;
        int cifras = 0;
        char buffer[4]; // Hasta 3 cifras + '\0'

        // Obtener las cifras al revés
        while (temp > 0)
        {
            buffer[cifras++] = (temp % 10) + '0';
            temp /= 10;
        }

        // Copiar en orden correcto
        for (int j = cifras - 1; j >= 0; j--)
        {
            nombreArchivo[i++] = buffer[j];
        }
    }

    // Sufijo
    nombreArchivo[i++] = '.';
    nombreArchivo[i++] = 't';
    nombreArchivo[i++] = 'x';
    nombreArchivo[i++] = 't';
    nombreArchivo[i]   = '\0'; // Fin del string
}

/**
 * @brief Genera el nombre de archivo con prefijo "archivos/pruebas/P" seguido de número con dos cifras.
 *
 * @param numero Número entre 0 y 99 inclusive.
 * @param nombreArchivo Puntero a arreglo dinámico de mínimo 32 caracteres donde se escribirá el nombre.
 */
void generar_nombre_prueba(int numero, char* nombreArchivo)
{
    // Borrar contenido previo
    for (int i = 0; i < 32; i++)
    {
        nombreArchivo[i] = '\0';
    }

    // Prefijo fijo
    const char* prefijo = "archivos/pruebas/P";
    int i = 0;

    while (prefijo[i] != '\0')
    {
        nombreArchivo[i] = prefijo[i];
        i++;
    }

    // Añadir número con ceros a la izquierda si es necesario
    if (numero < 10)
    {
        nombreArchivo[i++] = '0';
        nombreArchivo[i++] = (numero % 10) + '0';
    }
    else
    {
        int decenas = numero / 10;
        int unidades = numero % 10;
        nombreArchivo[i++] = decenas + '0';
        nombreArchivo[i++] = unidades + '0';
    }

    // Sufijo
    nombreArchivo[i++] = '.';
    nombreArchivo[i++] = 'b';
    nombreArchivo[i++] = 'm';
    nombreArchivo[i++] = 'p';
    nombreArchivo[i]   = '\0'; // Fin del string
}


unsigned int** cargar_txts(const char* ruta_base, unsigned char* M, int& n_txt, int& largo_mascara)
{
    char nombreArchivo[32] = {
        'a','r','c','h','i','v','o','s','/',
        't','x','t','/',
        'M','0','0','0','.','t','x','t','\0'
    };
    //A más de 100 no carga
    unsigned int** txts = new unsigned int*[101];
    n_txt = 0;
    bool flag = false;

    // M0.txt a M9.txt
    for (char u = '0'; u <= '9' && !flag; u++)
    {
        nombreArchivo[14] = u;
        nombreArchivo[15] = '.';
        nombreArchivo[16] = 't';
        nombreArchivo[17] = 'x';
        nombreArchivo[18] = 't';
        nombreArchivo[19] = '\0';

        int seed = 0, n_pixels = 0;
        unsigned int* txtactual = loadSeedMasking(nombreArchivo, seed, n_pixels);

        if (txtactual == nullptr)
        {
            flag = true;
            break;
        }

        unsigned int* txtnuevo = new unsigned int[largo_mascara + 1];
        txtnuevo[0] = seed;
        for (int k = 0; k < largo_mascara; k++)
        {
            txtnuevo[k + 1] = txtactual[k] - M[k];
        }
        txts[n_txt++] = txtnuevo;
        delete[] txtactual;
    }

    // M10.txt a M99.txt
    for (char d = '1'; d <= '9' && !flag; d++)
    {
        for (char u = '0'; u <= '9' && !flag; u++)
        {
            nombreArchivo[14] = d;
            nombreArchivo[15] = u;
            nombreArchivo[16] = '.';
            nombreArchivo[17] = 't';
            nombreArchivo[18] = 'x';
            nombreArchivo[19] = 't';
            nombreArchivo[20] = '\0';

            int seed = 0, n_pixels = 0;
            unsigned int* txtactual = loadSeedMasking(nombreArchivo, seed, n_pixels);

            if (txtactual == nullptr)
            {
                flag = true;
                break;
            }

            unsigned int* txtnuevo = new unsigned int[largo_mascara + 1];
            txtnuevo[0] = seed;
            for (int k = 0; k < largo_mascara; k++)
            {
                txtnuevo[k + 1] = txtactual[k] - M[k];
            }
            txts[n_txt++] = txtnuevo;
            delete[] txtactual;
        }
    }

    // M100.txt
    if (!flag)
    {
        nombreArchivo[14] = '1';
        nombreArchivo[15] = '0';
        nombreArchivo[16] = '0';
        nombreArchivo[17] = '.';
        nombreArchivo[18] = 't';
        nombreArchivo[19] = 'x';
        nombreArchivo[20] = 't';
        nombreArchivo[21] = '\0';

        int seed = 0, n_pixels = 0;
        unsigned int* txtactual = loadSeedMasking(nombreArchivo, seed, n_pixels);

        if (txtactual != nullptr)
        {
            unsigned int* txtnuevo = new unsigned int[largo_mascara + 1];
            txtnuevo[0] = seed;
            for (int k = 0; k < largo_mascara; k++)
            {
                txtnuevo[k + 1] = txtactual[k] - M[k];
            }
            txts[n_txt++] = txtnuevo;
            delete[] txtactual;
        }
    }

    return txts;
}



int verificar_enmascaramiento(unsigned char* imagen, unsigned int* datos_txt, int semilla, int largo_mascara)
{
    int cont = 0;
    for (int i = 0; i < largo_mascara; i++)
    {
        if (imagen[semilla+i] == datos_txt[i+1])
        {
            cont++;
        }
    }
    return cont;
}

void aplicar_operacion_inversa(unsigned char* in, int t, unsigned char* out, unsigned char* IM, int size)
{
    if (t == 1)
    {
        aplicar_xor(in, IM, out, size);
    }
    else if (t >= 2 && t <= 9)
    {
        int bits = t - 1;
        rotar_derecha(in, bits, out, size);  // inversa de ROT_L es ROT_R
    }
    else if (t >= 10 && t <= 17)
    {
        int bits = t - 9;
        rotar_izquierda(in, bits, out, size);  // inversa de ROT_R es ROT_L
    }
    else if (t >= 18 && t <= 25)
    {
        int bits = t - 17;
        desplazar_derecha(in, bits, out, size);  // SHIFT_L destruye MSB → se rellena con ceros
    }
    else if (t >= 26 && t <= 33)
    {
        int bits = t - 25;
        desplazar_izquierda(in, bits, out, size);  // SHIFT_R destruye LSB → también irrecuperable
    }
}

