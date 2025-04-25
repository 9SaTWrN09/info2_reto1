#include "operaciones_bits.h"

/**
 * @brief Devuelve el valor de t (18-33) cuyo conteo de coincidencias
 *        es distinto del valor de referencia (meseta).
 *
 * @param coincidencias  Arreglo de 16 enteros con los conteos
 *                       (ordenados t = 18 .. 33).
 * @param valor_base     Conteo considerado “meseta” (el más frecuente).
 * @return               Número de operación t que produjo el valor distinto.
 *                       Devuelve 0 si no encuentra ninguno (no debería ocurrir).
 */
int encontrar_t_con_valor_distinto(const int* coincidencias, int valor_base)
{
    for (int i = 0; i < 16; i++)
    {
        int t = i+18;
        if (coincidencias[i] == valor_base)return t;
    }

    return 0;   // Caso de seguridad (no debería alcanzarse)
}



/**
 * @brief Copia la fraccion de la imagen cubierta por la mascara M
 *        desde el buffer fuente (actual) al buffer destino (siguiente).
 *
 *        Se usa  cuando se detecta un desplazamiento de 8 bits:
 *        fuera de la region cubierta por la mascara los datos originales
 *        se han perdido, asi que solo vale la pena conservar esta seccion.
 *
 * @param src            Puntero al buffer con la imagen actual (fuente).
 * @param dst            Puntero al buffer destino donde se escribira la copia.
 * @param semilla        Offset inicial (en bytes) donde comienza la mascara.
 * @param largo_mascara  Cantidad de bytes que cubre la mascara
 *                       (width_m * height_m * 3).
 * @param total_bytes    Tamano total de la imagen (width * height * 3).
 */
void copiar_region_mascara(unsigned char* src,
                           unsigned char* dst,
                           int semilla,
                           int largo_mascara,
                           int total_bytes)
{
    /* --- 1. Inicializa dst con ceros para evitar basura visual --------- */
    for (int i = 0; i < total_bytes; i++)
    {
        dst[i] = 0;
    }

    /* --- 2. Verificacion de limites ----------------------------------- */
    if (semilla < 0 ||
        semilla + largo_mascara > total_bytes)
    {
        // Si la mascara se sale de rango no se copia nada
        return;
    }

    /* --- 3. Copia byte a byte la region cubierta por la mascara -------- */
    for (int i = 0; i < largo_mascara; i++)
    {
        dst[semilla + i] = src[semilla + i];
    }
}

void agregar_region_mascara(unsigned int* mask,
                           unsigned char* dst,
                           int semilla,
                           int largo_mascara)
{
    for (int i = 0; i < largo_mascara; i++)
    {
        dst[semilla + i] = mask[i+1];
    }
}



/**
 * @brief Busca la operación XOR (t = 1) o rotaciones (t = 2..17) que
 *        produce coincidencia plena con el .txt actual.
 *
 * @param actual         Buffer fuente (imagen del paso i + 1).
 * @param siguiente      Buffer destino (resultado provisional).
 * @param txt_i          Valores desenmascarados del .txt (incluye semilla en txt_i[0]).
 * @param semilla        Semilla del .txt actual.
 * @param largo_mask     Cantidad de bytes cubiertos por la máscara.
 * @param IM             Imagen IM (necesaria para XOR).
 * @param total_bytes    Tamaño completo de la imagen (width * height * 3).
 * @param t_out          Devuelve el t válido (1..17) si existe, 0 en caso contrario.
 *
 * @return true  si encontró XOR o una rotación correcta.
 *         false en caso contrario.
 */

bool buscar_xor_rot(unsigned char* actual,
                    unsigned char* siguiente,
                    unsigned int* txt_i,
                    int semilla,
                    int largo_mask,
                    unsigned char* IM,
                    int total_bytes,
                    int &t_out,
                    int &coincid_ref)
{
    int t_min = 0;
    int coincidencias_min_dist = largo_mask;
    for (int t = 1; t <= 17 ; t++)
    {
        aplicar_operacion_inversa(actual, t, siguiente, IM, total_bytes);

        int coincid = verificar_enmascaramiento(siguiente,txt_i,semilla,largo_mask);

        int temp = coincid_ref - coincid;                 // distancia entre el ultimo displazamiento y las coincidencias actuales
        unsigned int diferencia_actual = temp < 0 ? -temp : temp;   // aplicacion del valor absoluto

        temp = coincid_ref - coincidencias_min_dist;              // distancia entre el ultimo displazamiento y las coincidencias con el menor desplazamiento encontrado
        unsigned int diferencia_anterior = temp < 0 ? -temp : temp;  // aplicacion del valor absoluto

        if ( diferencia_actual == 0)//cout << "dif act = 0" << endl;
        if ( diferencia_anterior == 0)//cout << "dif ant = 0" << endl;
        if ( diferencia_actual <= diferencia_anterior)   // verifica si la diferencia actual es menor a la aterior mejor diferencia
        {
            coincidencias_min_dist = coincid;        // aplica las diferencias
            t_min = t;                                     // guarda la t de la menor diferencia
        }



    }
    if (t_min)
    {
        t_out = t_min;
        coincid_ref = coincidencias_min_dist;
        aplicar_operacion_inversa(actual, t_out, siguiente, IM, total_bytes);
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * @brief Intenta determinar si la operación correcta del paso actual es un SHIFT.
 *
 * @param actual         Buffer con la imagen del paso i+1.
 * @param siguiente      Buffer auxiliar (del mismo tamaño que actual).
 * @param txt_i          Datos desenmascarados del .txt (incluye semilla en txt_i[0]).
 * @param semilla        Semilla leída del .txt actual.
 * @param largo_mask     Cantidad de bytes cubiertos por la máscara.
 * @param IM             Imagen IM (para XOR, aunque aquí no se usa).
 * @param total_bytes    width * height * 3  (tamaño completo de la imagen).
 * @param t_out          Devuelve:
 *                         ▸ 18-33 → SHIFT válido (1-7 bits).
 *                         ▸  0    → No es SHIFT.
 *                         ▸ −1    → SHIFT de 8 bits (pérdida total).
 *
 * @return true  si encontró un SHIFT 1-7 bits, false en cualquier otro caso.
 */
bool detectar_shift(unsigned char* actual,
                    unsigned char* siguiente,
                    unsigned int* txt_i,
                    int semilla,
                    int largo_mask,
                    unsigned char* IM,
                    int total_bytes,
                    int &t_out,
                    int &coincid_shift)
{
    int coincidencias[16];

    /* ---------- 1. Barrido de los 16 posibles desplazamientos ---------- */
    int idx = 0;
    for (int t = 18; t <= 33; t++, idx++)
    {
        aplicar_operacion_inversa(actual, t, siguiente, IM, total_bytes);

        coincidencias[idx] = verificar_enmascaramiento
                                                        (
                                                            siguiente,
                                                            txt_i,
                                                            semilla,
                                                            largo_mask
                                                        );
    }

    /* ---------- 2. Analizar patrón de coincidencias ---------- */
    int valor_comp_1  = coincidencias[0];
    int cont_1 = 0;
    int valor_comp_2  = -1;
    int cont_2 = 0;

    for (int i = 0; i < 16; i++)
    {
        if (coincidencias[i] == valor_comp_1 ) cont_1++;
        else if (coincidencias[i] == valor_comp_2) cont_2++;
        else if (valor_comp_2 == -1){ valor_comp_2 = coincidencias[i]; cont_2++; }
    }
    if ( cont_1 + cont_2 < 16)
    {
        t_out = 0;
        return false;
    }
    /* Caso A: todos los valores son iguales  →  SHIFT de 8 bits o no-SHIFT */
    if (cont_1 == 16 || cont_2 == 16)
    {
        t_out = -1;        // marca pérdida total
        return false;
    }

    if (cont_1 == 1 || cont_2 == 1)
    {
        if (cont_1 == 1)
        {
            t_out = encontrar_t_con_valor_distinto(coincidencias, valor_comp_1);
            coincid_shift = valor_comp_1;
        }
        else
        {

            t_out = encontrar_t_con_valor_distinto(coincidencias, valor_comp_2);
            coincid_shift = valor_comp_2;
        }
    }

    return (t_out >= 18 && t_out <= 33);
}

/**
 * @brief Alterna entre dos buffers dinámicos (double buffering)
 *
 * @param bufferA Primer buffer (puntero base)
 * @param bufferB Segundo buffer (puntero base)
 * @param cont Número de control (generalmente contador de iteraciones)
 * @param actual Referencia al puntero que apuntará al buffer actual
 * @param siguiente Referencia al puntero que apuntará al siguiente buffer
 */
void alternar_buffers(unsigned char* bufferA, unsigned char* bufferB, int cont, unsigned char*& actual, unsigned char*& siguiente)
{
    if (cont % 2)
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
}


/**
 * @brief Devuelve el valor de t correspondiente a la operación inversa,
 *        incluyendo SHIFTs como inversión hipotética (solo por simetría de bits).
 *
 * @param t Valor de operación original (1 <= t <= 33).
 * @return t inverso correspondiente. -1 si t es inválido.
 */
int obtener_inverso_t(int t)
{
    if (t == 1)
    {
        return 1; // XOR es simétrica
    }
    else if (t >= 2 && t <= 9)
    {
        return t + 8; // ROT_R → ROT_L
    }
    else if (t >= 10 && t <= 17)
    {
        return t - 8; // ROT_L → ROT_R
    }
    else if (t >= 18 && t <= 25)
    {
        return t + 8; // SHIFT_R → SHIFT_L
    }
    else if (t >= 26 && t <= 33)
    {
        return t - 8; // SHIFT_L → SHIFT_R
    }
    else
    {
        return -1; // Valor fuera de rango
    }
}

