# Tekrar metinleri

Her dersin sonunda WhatsApp'a atılacak kısa tekrar metni.

## Gün 2

*Gün 2 – CAN neden var?*

• Araçtaki tüm ECU'lar 2 kabloya bağlı: CANH ve CANL
• Mesaj tabanlı: ID *içeriği* ve *önceliği* söyler, adres değildir. Herkes her şeyi duyar
• Multi-master: merkezi yönetici yok
• Çakışmada *küçük ID kazanır*, kaybedenin verisi bozulmaz, sonra tekrar dener
• Diferansiyel: gürültü iki kabloya eşit biner, fark değişmez
• Hata yönetimi *donanımda*: CRC, ACK, bit monitoring, bus-off
• UART: sadece 2 cihaz
• RS-485: çok cihaz + diferansiyel ama sadece fiziksel katman. Çakışma/hata yazılımda çözülür
• OBD soketi: pin 6 CANH, pin 14 CANL, 500k veya 250k

❓ Bit monitoring ile ACK farkı ne?

## Gün 3

*Gün 3 – Fiziksel katman*

• Recessive = 1 → kimse sürmez, CANH ≈ CANL ≈ 2,5 V, fark ≈ 0
• Dominant = 0 → transceiver sürer, CANH ≈ 3,5 V, CANL ≈ 1,5 V, fark ≈ 2 V
• 0 ile 1 çakışırsa hatta *0* görünür (sürülen, sürülmeyeni ezer)
• Arbitration ve ACK bu kurala dayanır
• ACK: gönderen ACK slot'unu 1 bırakır, doğru alan *alıcı* 0 basar
• 120 Ω × 2, hattın *uçlarına*:
  1) kablo empedansı ≈ 120 Ω → yansıma olmaz
  2) hattı hızla recessive'e geri çeker
• Enerji yokken CANH–CANL: 60 Ω doğru, 120 Ω tek direnç, ∞ direnç yok
• Masadaki arıza: CRX hep 1 → ESP32 kendi 0'ını göremez → bit hatası → TEC +8 → bus-off

❓ Hattın iki ucunda neden 120 Ω var, iki sebebi ne?
