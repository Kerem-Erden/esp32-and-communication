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

## Gün 3 – Dominant ve recessive ne demek? Hattın iki ucunda neden 120 Ω direnç var?

CAN hattında iki durum var. Recessive mantık 1'dir: kimse hattı sürmez, CANH ve CANL yaklaşık 2,5 V'ta
eşitlenir, aradaki fark 0'dır. Dominant mantık 0'dır: transceiver hattı aktif olarak sürer, CANH yaklaşık
3,5 V'a çıkar, CANL yaklaşık 1,5 V'a iner, fark yaklaşık 2 V olur. Biri 0, biri 1 gönderirse hatta 0 görünür,
çünkü sürülen hat sürülmeyeni ezer. Arbitration ve ACK bu kurala dayanır.

Hattın iki ucuna birer 120 Ω direnç konur. Kablonun empedansı yaklaşık 120 Ω olduğu için sinyal uçta sönümlenir
ve geri yansıyıp sonraki bitleri bozmaz. Dirençler ayrıca dominant bitten sonra CANH ile CANL'yi birbirine
çekerek hattı hızla recessive'e döndürür. Enerji yokken CANH–CANL arası 60 Ω ölçülmelidir (iki 120 Ω paralel).

**Karıştırma:** ACK'yi gönderen değil, frame'i doğru alan alıcı yazar. Gönderen ACK slot'unu 1 bırakır,
alıcı oraya 0 basar.
