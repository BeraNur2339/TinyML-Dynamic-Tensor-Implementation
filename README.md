# 🧠 TinyML Dynamic Tensor Framework: Low-Level Memory Optimization

![C Version](https://img.shields.io/badge/Language-C99-blue.svg)
![Field](https://img.shields.io/badge/Field-Embedded_AI-orange.svg)
![Efficiency](https://img.shields.io/badge/RAM_Saving-75%25-green.svg)

Bu proje, kaynak kısıtlı gömülü sistemler (Ultra-low power MCUs) için geliştirilmiş, **Dinamik Tensör Yönetimi** ve **Sayısal Nicemleme (Quantization)** altyapısını içermektedir. Derin öğrenme modellerinin mikrodenetleyiciler üzerindeki bellek ayak izini minimize etmeye yönelik ileri düzey C tekniklerini temel alır.

---

## 🏗️ Sistem Mimarisi ve Tasarım Prensipleri

### 1. Bellek Optimizasyonu (Union-Based Aliasing)
Gömülü sistemlerde RAM israfını önlemek amacıyla **Zero-Copy** prensibi uygulanmıştır. C dilinin `union` yetenekleri kullanılarak, farklı bit genişliğindeki verilerin aynı fiziksel bellek bloğunu paylaşması sağlanmıştır. 

![Union Memory](https://media.geeksforgeeks.org/wp-content/uploads/20230221124401/Difference-between-Structure-and-Union.png)

* **Verimlilik:** Çalışma anında (runtime) bellek fragmentasyonu %0'a indirgenmiştir.
* **Esneklik:** Tek bir yapı üzerinden hibrit veri tipleri yönetilebilmektedir.

### 2. Quantization Engine (Linear Symmetric Mapping)
Sistem, sürekli floating-point ağırlıklarını, ayrık 8-bit integer uzayına map eden bir motor içerir. Bu işlem, donanımın FPU yükünü azaltırken, bellek verimliliğini maksimize eder.

**Matematiksel Model:**
$$Q = \text{clamp}\left(\text{round}\left(\frac{F}{\text{scale}}\right), -128, 127\right)$$

---

## 📊 Performans ve Kaynak Analizi (Benchmark)
Aşağıdaki tablo, 4 elemanlı bir tensörün dönüşüm sonrası elde edilen tasarruf verilerini göstermektedir:

| Metrik | Orijinal Katman (FP32) | Optimize Edilmiş Katman (INT8) |
| :--- | :---: | :---: |
| **Birim Veri Genişliği** | 32-bit (4 Byte) | 8-bit (1 Byte) |
| **Toplam Bellek Yükü** | 16 Byte | 4 Byte |
| **Sıkıştırma Oranı** | 1x (Referans) | **4x (75% Reduction)** |

![Quantization Diagram](https://miro.medium.com/v2/resize:fit:1400/1*d0N_W159zG6U7v7q8-6B6A.png)

---

## 🛠️ Uygulama Bileşenleri

### Bileşen Hiyerarşisi:
* **Enum Layer:** Veri tipi güvenliği için `TensorDataType` hiyerarşisi.
* **Abstract Struct:** Boyut ve tip verilerini kapsülleyen dinamik `Tensor` yapısı.
* **Memory Allocator:** Tipe özgü dinamik bellek tahsisi ve **Pointer Aliasing** yönetimi.

### Derleme Talimatları:
```bash
gcc main.c -o tiny_tensor_engine
./tiny_tensor_engine
