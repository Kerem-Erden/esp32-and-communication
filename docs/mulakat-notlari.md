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

## Gün 4 – CAN frame'inde hangi alanlar var? Struct'ta neden CRC ya da ACK yok?

Klasik CAN frame'i şu alanlardan oluşur: SOF (1 bit, başlangıç ve senkron), ID (11 bit, içerik ve öncelik;
arbitration burada olur), RTR (veri frame'i mi istek frame'i mi), IDE (ID 11 bit mi 29 bit mi), r0, DLC
(4 bit, kaç byte veri var), DATA (0–8 byte), CRC (15 bit + ayraç, hata kontrolü), ACK (slot + ayraç, alıcının
"aldım" işareti) ve EOF (7 recessive bit).

SOF, CRC, ACK ve EOF'u CAN denetleyicisi donanımda ekler. Yazılım yalnızca ID, IDE, RTR, DLC ve data'yı verir.
Bu yüzden `can_bus_frame_t` sadece bu alanları tutar. `timestamp_us` frame'de yoktur; yazılım, frame'in ne
zaman geldiğini bilmek için ekler. `id` 29 bit'e kadar çıkabildiği için `uint32_t`. `dlc` ile `data_length`
ayrı, çünkü DLC bir koddur: CAN FD'de 9–15 değerleri 12–64 byte demektir.

**Takip soruları:**
- 29 bitlik frame gelirse: IDE = 1 olur. Projedeki OBD kodu bu frame'leri **atar** (`obd_protocol.c:48`),
  çünkü yalnızca 11 bitlik `0x7DF` / `0x7E8` ile çalışır. Bu bir sınırlamadır: OBD-II 29 bitlik ID'lere de izin
  verir (`0x18DB33F1` gibi) ve bazı ağır vasıtalar bunu kullanır.
- 8 byte'tan uzun veri: ISO-TP ile parçalara bölünür: First Frame, alıcıdan Flow Control, ardından Consecutive
  Frame'ler.

## 🔧 Donanım ölçümü 1 – Sonlandırma direnci (enerji yok, DT830D, 200 Ω kademesi)

- SN65-A tek başına: ≈ 70,6 Ω, SN65-B tek başına: ≈ 70,4 Ω, A ve B bağlı: ≈ 34–35,6 Ω.
- Paralel bağlantı tam yarıya düşüyor, yani iki modülde de sonlandırma var ama ≈ 70 Ω, standart 120 Ω değil.
- Hat ≈ 35 Ω oluyor (beklenen 60 Ω): transceiver'a daha fazla yük biner. Kısa masa hattında sorun olmaz;
  sorun çıkarsa bir direnç sökülür ya da 120 Ω ile değiştirilir.
- Mülakat cümlesi: "Ucuz modüllerde sonlandırma 120 Ω değildi, ölçerek fark ettim."
