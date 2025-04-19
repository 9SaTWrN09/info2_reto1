#ifndef FUNCIONES_H
#define FUNCIONES_H

// TRANSFORMACIONES INVERSAS

void aplicar_xor(unsigned char* in, unsigned char* mask, unsigned char* out, int size);
void rotar_izquierda(unsigned char* in, int bits, unsigned char* out, int size);
void rotar_derecha(unsigned char* in, int bits, unsigned char* out, int size);
void desplazar_izquierda(unsigned char* in, int bits, unsigned char* out, int size);
void desplazar_derecha(unsigned char* in, int bits, unsigned char* out, int size);

// UTILIDADES DE VERIFICACIÓN

int verificar_enmascaramiento(unsigned char* imagen, unsigned int* datos_txt, int semilla, int largo_mascara);
void aplicar_operacion_inversa(unsigned char* in, int t, unsigned char* out, unsigned char* IM, int size);

// LECTURA Y PROCESAMIENTO DE TXT

unsigned int** cargar_txts(const char* ruta_base, unsigned char* M, int& n_txt, int& largo_mascara);

#endif // FUNCIONES_H
