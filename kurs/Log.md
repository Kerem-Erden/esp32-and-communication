# ESP32 Kurs Log

## Gün 3 — Struct ve Pointer (21.11.2026)
- **Padding buyukten kucuge siralama daha optimize oluyor tipki mavlink reordering gibi**
- **gcc -Wall -Wextra -o main.x -main.c**

## Gün 4 — Function Pointer (23.11.2026)
- **Öğrendim:** Fonksiyonun adresi tablo içinde tutulup ismiyle çağrılabiliyor (callback). Tabloya parantezsiz yazılır, çağırırken func(veri).
- **Patladı:** func(&s) → pointer'ın pointer'ı, program çöktü. & sadece bir kere, main'de alınır. printf'i döngünün içine koyunca her satırda "bilinmeyen" bastı.
- **Yarına:** Gün 5 bit işlemleri + volatile. 

## Gün 5 — Bits (25.11.2026)
- **Bit mask islemleri ve volatile kavramini ogrendim**