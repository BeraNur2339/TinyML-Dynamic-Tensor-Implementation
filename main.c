#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Desteklenen veri tipleri.
 * Gömülü sistemlerde bellek yönetimi için tipin bilinmesi kritiktir.
 */
typedef enum
{
    DTYPE_FLOAT32 = 0,
    DTYPE_FLOAT16 = 1, // Half-precision (Simüle edilmiş)
    DTYPE_INT8 = 2     // Quantized (Nicemlenmiş)
} TensorDataType;

/**
 * @brief Dinamik Tensor Yapısı
 * Union kullanarak aynı bellek alanını farklı veri tipleri için paylaştırıyoruz.
 */
typedef struct
{
    TensorDataType type; // Verinin tipi
    uint16_t rows;       // Matris satır sayısı
    uint16_t cols;       // Matris sütun sayısı

    union
    {
        float *f32_ptr;    // 32-bit hassasiyet
        uint16_t *f16_ptr; // 16-bit hassasiyet (FP16/BF16 ham veri)
        int8_t *i8_ptr;    // 8-bit Quantized veri
    } data;
} Tensor;

/**
 * @brief Tensor için bellek tahsis eden fonksiyon.
 */
Tensor *create_tensor(uint16_t r, uint16_t c, TensorDataType type)
{
    Tensor *t = (Tensor *)malloc(sizeof(Tensor));
    t->type = type;
    t->rows = r;
    t->cols = c;
    uint32_t num_elements = r * c;

    // Bellek miktarını tipe göre ayarla
    switch (type)
    {
    case DTYPE_FLOAT32:
        t->data.f32_ptr = (float *)malloc(num_elements * sizeof(float));
        break;
    case DTYPE_FLOAT16:
        t->data.f16_ptr = (uint16_t *)malloc(num_elements * sizeof(uint16_t));
        break;
    case DTYPE_INT8:
        t->data.i8_ptr = (int8_t *)malloc(num_elements * sizeof(int8_t));
        break;
    }
    return t;
}

/**
 * @brief Basit bir Quantization (Nicemleme) işlemi.
 * Float32 değerleri Int8 aralığına (-128, 127) sıkıştırır.
 */
void quantize_to_int8(Tensor *f32_src, Tensor *i8_dest, float scale)
{
    if (f32_src->type != DTYPE_FLOAT32 || i8_dest->type != DTYPE_INT8)
        return;

    int num_elements = f32_src->rows * f32_src->cols;
    for (int i = 0; i < num_elements; i++)
    {
        float val = f32_src->data.f32_ptr[i] / scale;
        // Clamp (Sınırlandırma)
        if (val > 127)
            val = 127;
        if (val < -128)
            val = -128;
        i8_dest->data.i8_ptr[i] = (int8_t)val;
    }
}

/**
 * @brief Tensörü ekrana yazdıran yardımcı fonksiyon.
 */
void print_tensor(Tensor *t)
{
    printf("Tensor [%dx%d] - Tip: %d\n", t->rows, t->cols, t->type);
    int num_elements = t->rows * t->cols;
    for (int i = 0; i < num_elements; i++)
    {
        if (t->type == DTYPE_FLOAT32)
            printf("%.2f ", t->data.f32_ptr[i]);
        else if (t->type == DTYPE_INT8)
            printf("%d ", t->data.i8_ptr[i]);
    }
    printf("\n--------------------------\n");
}

int main()
{
    // 1. Orijinal Float32 Tensor Oluştur (Örn: Model Ağırlıkları)
    Tensor *weights_f32 = create_tensor(1, 4, DTYPE_FLOAT32);
    weights_f32->data.f32_ptr[0] = 0.50f;
    weights_f32->data.f32_ptr[1] = -0.75f;
    weights_f32->data.f32_ptr[2] = 1.20f;
    weights_f32->data.f32_ptr[3] = -0.10f;

    printf("Orijinal Veri (RAM: %zu byte)\n", 4 * sizeof(float));
    print_tensor(weights_f32);

    // 2. Quantized (Int8) Tensor Oluştur
    Tensor *weights_i8 = create_tensor(1, 4, DTYPE_INT8);
    float scale = 0.01f; // Hassasiyet çarpanı

    quantize_to_int8(weights_f32, weights_i8, scale);

    printf("Quantized Veri (RAM: %zu byte)\n", 4 * sizeof(int8_t));
    print_tensor(weights_i8);

    // Bellek Temizliği
    free(weights_f32->data.f32_ptr);
    free(weights_f32);
    free(weights_i8->data.i8_ptr);
    free(weights_i8);

    return 0;
}