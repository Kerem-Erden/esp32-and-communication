# Isuzu CAN Projesi – Anlama Planı (31 gün)

## Hedef

CV'deki Isuzu teşhis projesini **anlayarak savunabilmek**: projedeki her kararın *neden* öyle verildiğini,
CAN protokolünün ilgili parametresini ve alternatiflerini kendi cümlelerimle anlatabilmek; bunu yaparken
gömülü C mülakatlarında sorulan C konularını da öğrenmek.
Hedef: projeyle ilgili soruların **%85–90'ına** ezber değil, anlayarak cevap vermek.

* Kapsam: ESP32 firmware'i (C), CAN, OBD-II, ISO-TP, donanım ve tasarım tercihleri, gömülü C.
* Kapsam dışı: C# masaüstü uygulaması.
* İncelenen kod: `../isuzu-diagnostic-system` → `firmware/esp32-gateway/main/` (main = `b98ccd5`, Codex öncesi).
  O repoda yalnızca okuma yapılır.

## Her gün (30 dk, en fazla 1 saat)

1. **Kavram (≈15 dk):** konu, proje hiç yokmuş gibi sıfırdan öğrenilir.
2. **Projede (≈10 dk):** kavram firmware'de nerede, nasıl ve **neden böyle** uygulanmış. C, kodun içinde
   ihtiyaç duyuldukça öğrenilir (tablodaki **C** sütunu).
3. **Kendin anlat (≈5 dk):** konuyu kendi cümlelerimle anlatırım. Ezber cevap yok; anlatamadığım yer
   eksik olan yerdir. Düzeltilmiş anlatım `docs/mulakat-notlari.md` dosyasına eklenir.

* ✍️ işaretli günlerde projeyle ilgili kısa bir parçayı **kendim yazarım**, sonra projedeki halle karşılaştırırım.
  Kod `gunler/gunXX/` klasörüne konur, `gcc -std=c11 -Wall -Wextra` ile derlenir.
* Her bölümün son günü **toparlama**dır: bölümün tamamını baştan sona anlatma provası.
* 🔧 işaretli adımlar karta dokunur ve yalnızca o gün ayrıca verilen izinle yapılır. Araca bağlanmak ve araca
  aktif CAN mesajı göndermek bu planın parçası değildir.
* C yazma pratiğinin asıl yeri **learnesp32** kursudur; bu plan onu projeyle bağlar.

## Gün 1 – Başlangıç ✅

Proje haritası (araç → CAN → transceiver → ESP32 → UART → PC); derleme süreci (ön işlemci, derleyici,
assembler, linker); `gcc -E / -S / -c`; `printf` → `puts` optimizasyonu; dinamik (PC) ve statik (ESP32) bağlama.

## Bölüm A – CAN'in temelleri

| Gün | Kavram | Projede / tercih sorusu | C |
|---|---|---|---|
| 2 ✅ | CAN neden var: araç içi ağ, çok efendili (multi-master) bus, OBD konnektörü (pin 6 CANH, pin 14 CANL) | "Araçlarda neden UART ya da RS-485 değil de CAN?" | — |
| 3 | Fiziksel katman: diferansiyel sinyal, dominant/recessive, CANH/CANL gerilimleri, 2 × 120 Ω sonlandırma | Masadaki ölçümler hangi değerleri göstermeli | — |
| 4 | Frame yapısı: SOF, ID, RTR, IDE, DLC, data, CRC, ACK, EOF; 11 bit / 29 bit ID | `can_bus_frame_t` alanları neden bu tiplerde | `stdint` tipleri, işaretli/işaretsiz, taşma; **struct padding ve hizalama** (`sizeof(can_bus_frame_t)` kaç?) |
| 5 | Arbitration: düşük ID'nin bit bit kazanması | "İki ECU aynı anda konuşursa ne olur?" | bit kaydırma ve maske ile ID'nin bitlerini okumak |
| 6 | Bit timing, bitrate (500k / 250k), sample point | Projede bitrate'in otomatik denenmesi (`probe_can_bitrate`) – neden? | `enum` (`can_bus_bitrate_t`), `#define` |
| 7 | ACK ve hata yönetimi: hata türleri, TEC/REC, error-active/passive, bus-off | `can_bus_print_diagnostics`; "ACK'siz bir hatta ne olur?" | `switch` ile enum'dan metne çevirme |
| 8 | Listen-only mod: hattı ACK vermeden dinlemek | Projede varsayılan modun **passive** olması – güvenlik tercihi | — |
| 9 | **Toparlama** | CAN bölümünü baştan sona anlatma | — |

## Bölüm B – Donanım ve tercihler

| Gün | Kavram | Projede / tercih sorusu | C |
|---|---|---|---|
| 10 | Transceiver nedir; SN65HVD230 pinleri (D, R, Rs, Vref), Rs modları | Modül bağlantısı (D21 → CTX, D22 → CRX) | — |
| 11 | 3,3 V ve 5 V lojik, GPIO toleransı | "Neden TJA1050 / MCP2551 değil de SN65HVD230?" | — |
| 12 | Dahili CAN denetleyicisi ve harici denetleyici (SPI) | "Neden MCP2515 değil de ESP32'nin dahili TWAI'si?", "Neden STM32 ya da Arduino değil?" | — |
| 13 | Hata ayıklama metodolojisi: katman katman daraltma | CRX'in hep recessive kalması, GPIO echo testi (`can_bus_run_transceiver_test`), loopback'in yalnızca denetleyicinin içini test etmesi, multimetre ölçümleri | GPIO okuma/yazma kodu |
| 14 | **Toparlama** | "Donanımı nasıl seçtin, arızayı nasıl buldun?" | — |

## Bölüm C – ESP32 firmware'i

| Gün | Kavram | Projede / tercih sorusu | C |
|---|---|---|---|
| 15 | ESP-IDF, Arduino ve proje yapısı; C mi C++ mı; build → `.bin` → flash | "Neden Arduino değil de ESP-IDF?", "Neden C?" | **bellek düzeni**: flash, `.data`, `.bss`, stack, heap; `.h`/`.c` ayrımı, include guard |
| 16 | FreeRTOS temelleri: task, tick, `app_main` | Ana döngünün akışı (`esp32_gateway.c`) | `static` fonksiyon ve `static` yerel değişken |
| 17 | TWAI sürücüsünü başlatma | `can_bus_init`: modlar, `tx_gpio = -1` tasarımı | struct, designated initializer, modül durumu (`s_` değişkenleri) |
| 18 | Kesme (ISR) ve kuyruk; ISR'de neden iş yapılmaz | `can_bus_rx_callback` → `s_rx_queue` → `can_bus_receive` | pointer, **function pointer** (callback), `volatile`; **`malloc`/heap** – `xQueueCreate` heap kullanır, gömülüde neden dikkatli olunur |
| 19 | Gönderme yolu, otomatik tekrar gönderme (retransmission) | `fail_retry_cnt = 0` – neden tekrar yok?; passive modda gönderme yasağı | **`const` ve pointer** (`const can_bus_frame_t *frame`; `const int *` / `int * const`) |
| 20 | Hata yolları ve kaynak temizleme | `can_bus_init` / `deinit` hata yolları | hata kodları (`esp_err_t`), erken `return`, `NULL` kontrolü |
| 21 | Pasif trafik analizi | `can_stats.c`: hangi ID ne sıklıkla geliyor – neden gerekli? | sabit boyutlu dizi, lineer arama |
| 22 | **Toparlama** | "Firmware'in mimarisini anlat" | — |

## Bölüm D – OBD-II ve ISO-TP

| Gün | Kavram | Projede / tercih sorusu | C |
|---|---|---|---|
| 23 | OBD-II: servisler (modlar), fonksiyonel `0x7DF` / fiziksel `0x7E0` istek, `0x7E8` cevap | Projede neden `0x7DF` kullanılmış; artısı ve eksisi | out-parametre ile frame doldurma |
| 24 | Mode 01 PID'leri ve formülleri | `obd_decode_pid_response`: RPM = (A·256 + B) / 4 | bit işlemleri, hex; **endianness** (A·256 + B neden big-endian?); **`union`** ile aynı byte'lara iki farklı bakış ✍️ RPM decoder |
| 25 | Mode 03: DTC'nin 2 byte'tan `P0301` gibi bir koda çevrilmesi | `obd_decode_dtc` | bit maskeleri, `char` dizisi, `'\0'` ✍️ DTC decoder |
| 26 | ISO-TP: Single/First/Consecutive Frame, Flow Control; neden gerekli | Projenin tek frame sınırı (`OBD_MAX_DTCS_SINGLE_FRAME 3`) ve sonucu | buffer yönetimi, durum makinesi ✍️ çok frame'li bir cevabı birleştirme |
| 27 | Zaman aşımı, negatif cevap (NRC `0x7F`, `0x78`), birden çok ECU'nun cevap vermesi | `obd_wait_pid_response` – neyi doğru, neyi eksik yapıyor | `int64_t` zaman hesabı, taşma |
| 28 | **Toparlama** | "Araçtan RPM'i nasıl okuyorsun? Uçtan uca anlat" | — |

## Bölüm E – Mimari ve kapanış

| Gün | Kavram | Projede / tercih sorusu | C |
|---|---|---|---|
| 29 | Seri haberleşme: satır tabanlı metin protokolü, request id, 115200 baud | "Neden binary değil de metin?", UART'ın log çıktısıyla paylaşılması | string parse etme, `snprintf`, **buffer taşması** |
| 30 | Katmanlı mimari ve güvenlik tasarımı | Firmware/PC ayrımı, passive varsayılan, araca yazma işlemi olmaması | ön işlemci anahtarları (`#if`, `OBD_VEHICLE_TEST_ENABLED`), makro tuzakları |
| 31 | **Başarısızlık analizi ve son prova** | "Ne yanlış gitti, ne öğrendin, bugün ne farklı yapardın?"; projeyi baştan sona anlatma | — |

## Gömülü C mülakat konuları – nerede işleniyor

| Konu | Gün |
|---|---|
| `stdint` tipleri, taşma | 4 |
| Struct padding ve hizalama | 4 |
| Bit işlemleri ve maskeler | 5, 24, 25 |
| `enum`, `#define`, ön işlemci | 6, 30 |
| Bellek düzeni (flash, `.data`, `.bss`, stack, heap) | 15 |
| `static` | 16, 17 |
| Pointer, function pointer, `volatile` | 18 |
| `malloc` / heap | 18 |
| `const` ve pointer | 19 |
| Hata kodları ve kaynak temizleme | 20 |
| Endianness, `union` | 24 |
| String, `char` dizisi, buffer taşması | 25, 29 |
| Durum makinesi | 26 |

## İlerleme

- [x] Gün 1 – Başlangıç
- [ ] Bölüm A – CAN'in temelleri (2–9) – Gün 2 ✅, sıradaki: Gün 3
- [ ] Bölüm B – Donanım ve tercihler (10–14)
- [ ] Bölüm C – ESP32 firmware'i (15–22)
- [ ] Bölüm D – OBD-II ve ISO-TP (23–28)
- [ ] Bölüm E – Mimari ve kapanış (29–31)
