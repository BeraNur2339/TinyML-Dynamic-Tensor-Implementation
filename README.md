# 🧠 TinyML Dynamic Tensor Framework: Low-Level Memory Optimization

![C Version](https://img.shields.io/badge/Language-C99-blue.svg)
![Field](https://img.shields.io/badge/Field-Embedded_AI-orange.svg)
![Efficiency](https://img.shields.io/badge/RAM_Saving-75%25-green.svg)

Bu proje, kaynak kısıtlı gömülü sistemler (Ultra-low power MCUs) için geliştirilmiş, **Dinamik Tensör Yönetimi** ve **Sayısal Nicemleme (Quantization)** altyapısını içermektedir.

---

## 🏗️ Sistem Mimarisi ve Tasarım Prensipleri

### 1. Bellek Optimizasyonu (Union-Based Aliasing)
Gömülü sistemlerde RAM israfını önlemek amacıyla **Zero-Copy** prensibi uygulanmıştır. C dilinin `union` yetenekleri kullanılarak, farklı bit genişliğindeki verilerin aynı fiziksel bellek bloğunu paylaşması sağlanmıştır. 

![Union Layout](https://img.atwiki.jp/p_study/pub/union_memory.png)

* **Verimlilik:** Çalışma anında (runtime) bellek fragmentasyonu %0'a indirgenmiştir.

### 2. Quantization Engine (Linear Symmetric Mapping)
Sistem, sürekli floating-point ağırlıklarını, ayrık 8-bit integer uzayına map eden bir motor içerir.

**Matematiksel Model:**
$$Q = \text{clamp}\left(\text{round}\left(\frac{F}{\text{scale}}\right), -128, 127\right)$$

![Quantization](https://raw.githubusercontent.com/google/gemlowp/master/doc/quantization.png)

---

## 📊 Performans ve Kaynak Analizi (Benchmark)

| Metrik | Orijinal Katman (FP32) | Optimize Edilmiş Katman (INT8) |
| :--- | :---: | :---: |
| **Birim Veri Genişliği** | 32-bit (4 Byte) | 8-bit (1 Byte) |
| **Toplam Bellek Yükü** | 16 Byte | 4 Byte |
| **Sıkıştırma Oranı** | 1x (Referans) | **4x (75% Reduction)** |

---

## 🛠️ Uygulama Bileşenleri
* **Enum Layer:** Veri tipi güvenliği için `TensorDataType` hiyerarşisi.
* **Abstract Struct:** Boyut ve tip verilerini kapsülleyen dinamik `Tensor` yapısı.
* **Memory Allocator:** Tipe özgü dinamik bellek tahsisi.

### Derleme Talimatları:
```bash
gcc main.c -o tiny_tensor_engine
./tiny_tensor_engine
./tiny_tensor_engine
