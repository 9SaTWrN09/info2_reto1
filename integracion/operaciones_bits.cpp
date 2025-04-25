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
