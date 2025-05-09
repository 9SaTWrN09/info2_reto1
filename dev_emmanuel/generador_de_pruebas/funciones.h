#ifndef FUNCIONES_H
#define FUNCIONES_H

// TRANSFORMACIONES INVERSAS
void aplicar_xor(unsigned char* in, unsigned char* mask, unsigned char* out, int size);
void rotar_izquierda(unsigned char* in, int bits, unsigned char* out, int size);
void rotar_derecha(unsigned char* in, int bits, unsigned char* out, int size);
void desplazar_izquierda(unsigned char* in, int bits, unsigned char* out, int size);
void desplazar_derecha(unsigned char* in, int bits, unsigned char* out, int size);

// UTILIDADES
int verificar_enmascaramiento(unsigned char* imagen, unsigned int* datos_txt, int semilla, int largo_mascara);
void aplicar_operacion_inversa(unsigned char* in, int t, unsigned char* out, unsigned char* IM, int size);
void generar_nombre_txt(int numero, char* nombreArchivo);
void generar_nombre_prueba(int numero, char* nombreArchivo);

#endif
