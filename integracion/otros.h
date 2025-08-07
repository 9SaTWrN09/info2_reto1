#ifndef OTROS_H
#define OTROS_H

void alternar_buffers(unsigned char* bufferA, unsigned char* bufferB, int cont, unsigned char*& actual, unsigned char*& siguiente);
int encontrar_t_con_valor_distinto(const int* coincidencias, int valor_base);
bool detectar_shift(unsigned char* actual,
                    unsigned char* temp,
                    unsigned int* txt_i,
                    int semilla,
                    int largo_mask,
                    unsigned char* IM,
                    int total_bytes,
                    int &t_out,
                    int &coincid_shift);
void copiar_region_mascara(unsigned char* src,
                           unsigned char* dst,
                           int semilla,
                           int largo_mascara,
                           int total_bytes);
bool buscar_xor_rot(unsigned char* actual,
                    unsigned char* temp,
                    unsigned int* txt_i,
                    int semilla,
                    int largo_mask,
                    unsigned char* IM,
                    int total_bytes,
                    int &t_out,
                    int &coincid_ref);
bool buscar_xor_rot2(unsigned char* actual,
                    unsigned char* temp,
                    unsigned int* txt_i,
                    int semilla,
                    int largo_mask,
                    unsigned char* IM,
                    int total_bytes,
                    int &t_out,
                    int &coincid_ref);
void agregar_region_mascara(unsigned int* src,
                            unsigned char* dst,
                            int semilla,
                            int largo_mascara);
int obtener_inverso_t(int t);

#endif // OTROS_H

/*
 * Desarrollado por:
 *      Juan Pauolo Moncada(Juanpa) - [@JuanPa](https://github.com/Paulo848) correo: moncadamoralesjuanpaulo@gmail.com
 *
 *      Emmanuel López Ramírez(Entropy) - [@EmmaNuel-Network](https://github.com/EmmaNuel-Network) correo: entropy.casp9@gmail.com
*/
