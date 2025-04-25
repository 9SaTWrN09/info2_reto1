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
