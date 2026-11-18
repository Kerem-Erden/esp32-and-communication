# Mülakat notları

Her günün "Kendin anlat" sorusuna verdiğim cevabın düzeltilmiş hali.

## Gün 2 – Araçlarda neden CAN kullanılıyor? UART ya da RS-485 neden yetmiyor?

Araçta onlarca ECU var. CAN hepsini iki kablolu tek bir hatta bağlıyor: CANH ve CANL. Bilgi iki kablo
arasındaki gerilim farkıyla taşındığı için motor gürültüsünden etkilenmiyor. Hattaki herkes her mesajı duyuyor.
Mesajın ID'si mesajın içeriğini ve önceliğini belirtiyor. İki düğüm aynı anda konuşursa ID'si küçük olan
kazanıyor, kaybedenin verisi bozulmuyor ve sonra tekrar deniyor. CRC, ACK, hata sayaçları ve arızalı düğümü
hattan ayıran bus-off mekanizması çipin içinde hazır geliyor.

UART yalnızca iki cihazı bağlıyor. RS-485 çok cihazı bağlıyor ve diferansiyel, ama sadece fiziksel katmanı
tanımlıyor. Çakışmayı ve hataları sizin yazılımda çözmeniz gerekiyor. CAN bunları donanımda çözdüğü için araç
için daha güvenilir.

**Karıştırma:**
- **Bit monitoring:** gönderen kendi gönderdiği biti hattan geri okur. Arbitration da bu sayede çalışır.
- **ACK:** frame'i doğru alan alıcılar, ACK slot'una dominant bit yazar.
