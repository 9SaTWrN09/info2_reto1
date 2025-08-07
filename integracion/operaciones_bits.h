#ifndef OPERACIONES_BITS_H
#define OPERACIONES_BITS_H

// TRANSFORMACIONES

void aplicar_xor(unsigned char* in, unsigned char* mask, unsigned char* out, int size);
void rotar_izquierda(unsigned char* in, int bits, unsigned char* out, int size);
void rotar_derecha(unsigned char* in, int bits, unsigned char* out, int size);
void desplazar_izquierda(unsigned char* in, int bits, unsigned char* out, int size);
void desplazar_derecha(unsigned char* in, int bits, unsigned char* out, int size);

// UTILIDADES DE VERIFICACIÓN

int verificar_enmascaramiento(unsigned char* imagen, unsigned int* datos_txt, int semilla, int largo_mascara);
void aplicar_operacion_inversa(unsigned char* in, int t, unsigned char* out, unsigned char* IM, int size);

#endif // OPERACIONES_BITS_H

/*
 * Desarrollado por:
 *      Juan Pauolo Moncada(Juanpa) - [@JuanPa](https://github.com/Paulo848) correo: moncadamoralesjuanpaulo@gmail.com
 *
 *      Emmanuel López Ramírez(Entropy) - [@EmmaNuel-Network](https://github.com/EmmaNuel-Network) correo: entropy.casp9@gmail.com
*/
