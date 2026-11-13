# CAN + C Çalışma Planı (60 gün)

Amaç: Isuzu projesindeki `firmware/esp32-gateway` kodunu (saf C, ESP-IDF) satır satır anlayan, C'yi kendi
başına yazabilen ve CAN/OBD/ISO-TP'yi açıklayabilen biri olmak. C# masaüstü uygulaması kapsam dışıdır.

İncelenen kod, ayrı bir repo olan `../isuzu-diagnostic-system` içinde, `main` = `b98ccd5` (Codex öncesi):
`firmware/esp32-gateway/main/`. O repoda yalnızca okuma yapılır; bütün çalışmalar bu repoya yazılır.

## learnesp32 ile birlikte

Bu planla aynı dönemde **learnesp32** kursuna da çalışılıyor. Oturumun başında kursta hangi derste
olduğunuzu söyleyin; o günün konusunu kursla bağlarız. Kurs alıştırmalarınızı bu repoda `learnesp32/`
klasörüne koyabilirsiniz. ESP-IDF ve FreeRTOS konularını (Hafta 6) kursta önden görürseniz o hafta kısalır,
CAN haftalarına daha çok zaman kalır.

## Nasıl çalışıyoruz

* **Günde 30 dakika**, en fazla 1 saat. Bitmeyen gün ertesi güne taşınır; plan yarışma değil.
* Haftada 5 gün. Her haftanın 6. günü **tekrar** günüdür (kısa sınav + mini program).
* Her oturum aynı düzende:
  1. **5 dk** – dünün tekrarı (ben 2-3 soru sorarım, siz cevaplarsınız)
  2. **15 dk** – konu + projedeki gerçek kodu birlikte okuma
  3. **10 dk** – alıştırma: **C kodunu siz yazarsınız**, ben inceler, hataları açıklarım
* Çözümü önce siz denersiniz. Takılırsanız önce ipucu, sonra çözüm (Learning Mode).
* Alıştırmalar PC'de, kartsız derlenir:
  ```bash
  gcc -std=c11 -Wall -Wextra -o gunler/gun03/gun03.exe gunler/gun03/main.c
  ```
  Dosyalar bu repoda `gunler/gunXX/` klasörüne konur; her günün sonunda yerelde commit edilir
  (GitHub'a yalnızca siz açıkça isterseniz gönderilir).
* 🔧 işaretli adımlar karta/donanıma dokunur. Bunlar **o gün ayrıca sizin izninizle** yapılır;
  araca bağlanmak ve araca aktif CAN mesajı göndermek bu planın parçası değildir.

## Hafta 1 – Başlangıç ve C'nin temelleri

| Gün | Konu | Projedeki kod | Alıştırma (siz yazarsınız) |
|---|---|---|---|
| 1 | Proje haritası: araç → CAN → transceiver → ESP32 → UART → PC. Derleme adımları (ön işlemci, derleyici, linker) | `firmware/esp32-gateway/main/` 5 modül | İlk C programı; `gcc` ile derleyip çalıştırma |
| 2 | Tipler, `stdint.h` (`uint8_t`, `uint32_t`, `int64_t`), neden sabit genişlik | `can_bus.h` → `can_bus_frame_t` | `sizeof` tablosu; `uint8_t` taşma deneyi (255 + 1) |
| 3 | Hex/binary, `printf` biçimleri (`%u %X %02X`), `inttypes.h` | `0x7DF`, `0x7E8`, `CAN_STANDARD_MAX_ID 0x7FF` | Bir sayıyı onlu/hex/ikili yazan fonksiyon |
| 4 | Kontrol akışı: `if`, `switch`, `for`, `while`, `continue` | `esp32_gateway.c` → `process_serial_input` | String sabitinden `'\n'` ile satır ayıran döngü |
| 5 | Fonksiyonlar, prototip, dönüş değeri, `static` fonksiyon | `esp32_gateway.c` static fonksiyonlar | RPM formülü fonksiyonu `(A*256+B)/4` |
| 6 | **Tekrar** | — | Mini program: 3 PID değerini hesaplayıp tablo yazdır |

## Hafta 2 – Bit işlemleri, diziler, struct, enum, modüller

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 7 | Bit işlemleri `& \| ^ ~ << >>`, maske, nibble | `obd_protocol.c` → `data[0] >> 4`, `data[0] & 0x0F` | Bit set/clear/test fonksiyonları; nibble ayırma |
| 8 | Diziler, indeks sınırları, `sizeof(a)/sizeof(a[0])` | `uint8_t data[8]` | 8 byte'ı `04 41 0C 1A F8 00 00 00` biçiminde yazdır |
| 9 | `struct`, `typedef`, designated initializer (`.baud_rate = …`) | `can_bus_frame_t`, `uart_config_t`, `node_config` | Kendi frame struct'ınızı doldurup yazdırın |
| 10 | `enum`, enum üzerinde `switch` | `can_bus_status_t`, `can_bus_mode_t`, `can_bus_bitrate_t` | `status_to_string()` |
| 11 | `.h`/`.c` ayrımı, include guard, derleme birimi, `static` vs `extern` | `can_bus.h` / `can_bus.c` | Alıştırmayı `frame.h`, `frame.c`, `main.c` dosyalarına bölüp birlikte derleyin |
| 12 | **Tekrar** | — | Mini program: frame oluştur → yazdır → alanları doğrula |

## Hafta 3 – Pointer'lar ve hata yönetimi

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 13 | Pointer: adres, `&`, `*`, `NULL` | — | `swap(int *a, int *b)` |
| 14 | Struct pointer'ı, `->`, çıkış parametresi, `const` pointer | `obd_build_pid_request(const … *request, can_bus_frame_t *out_frame)` | Out-param ile frame dolduran fonksiyon |
| 15 | Pointer ve dizi, pointer aritmetiği, fonksiyona dizi + uzunluk geçme | `memcpy`, `data` kopyalama | Buffer üzerinde toplam/arama fonksiyonu |
| 16 | Hata döndürme: `bool`, `esp_err_t`, erken `return`, hata yolunda kaynak temizleme | `can_bus_init` hata yolları | Üç adımlı init; her hata yolu kendinden öncekileri geri alsın |
| 17 | Bellek bölgeleri: stack, static, heap. Modül durumu (`s_` değişkenleri), static yerel değişken | `s_twai_node`, `s_rx_queue`, `process_serial_input` içindeki static buffer | Çağrı sayacı; stack'teki adresi döndürme hatasını görme |
| 18 | **Tekrar** | — | Mini proje: "frame yazıcı" modülü (`.h` + `.c` + test) |

## Hafta 4 – Stringler ve buffer güvenliği

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 19 | C string'i: `char` dizisi, `'\0'`, `strlen`, string literal | `request_line[request_length] = '\0'` | Kendi `my_strlen` fonksiyonunuz |
| 20 | `snprintf`, buffer boyutu, kesilme, taşma | `gateway_protocol.c` → `write_response` | Cevap satırı üret; küçük buffer'la kesilmeyi test et |
| 21 | Parse: `strcmp`, `strncmp`, `strtol`, girdi doğrulama | `parse_request_id`, `remove_line_ending` | `"REQ\|42\|PING"` satırını parçala |
| 22 | `gateway_protocol.c`'nin tamamını okuma; tanımsız davranış örnekleri | `gateway_protocol_handle_line` | Kodda bulduğunuz riskleri listeleyin |
| 23 | `assert` ile test yazma | — | Mini parser + 10 test vakası |
| 24 | **Tekrar** | — | Satır tabanlı protokolün PC'de çalışan sürümü |

## Hafta 5 – CAN teorisi

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 25 | CAN nedir: bus, düğüm, OBD konnektörü (pin 6 CANH, pin 14 CANL) | — | Sistemi kağıda çizin |
| 26 | Fiziksel katman: diferansiyel sinyal, dominant/recessive, gerilimler, 2 × 120 Ω = 60 Ω | — | Ölçüm tablosu: hangi durumda hangi gerilim |
| 27 | Transceiver SN65HVD230: D, R, Rs, Vref pinleri; GPIO echo testi | `can_bus_run_transceiver_test` | 🔧 (isteğe bağlı) Multimetre ölçümleri: masadaki "CRX takılı" arızası |
| 28 | Frame: SOF, ID, RTR, IDE, DLC, data, CRC, ACK, EOF; standart/extended ID; arbitration | `is_extended`, `is_remote`, `dlc` | İki ID'den arbitration'ı hangisinin kazandığını bit bit hesaplayan fonksiyon |
| 29 | Bit timing, bitrate, sample point; yanlış bitrate'te ne olur | `probe_can_bitrate` | Bit süresi ve frame süresi hesaplayıcı |
| 30 | Hatalar: error frame, TEC/REC, error-active/passive, bus-off | `can_bus_print_diagnostics` | TEC/REC değerinden durum hesaplayan fonksiyon |

## Hafta 6 – ESP-IDF ve FreeRTOS

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 31 | ESP-IDF proje yapısı: `CMakeLists.txt`, `main` component'i, `sdkconfig`, `idf.py` | `firmware/esp32-gateway/` | 🔧 Yalnızca `idf.py build` (flash yok) |
| 32 | `app_main`, task, tick, `vTaskDelay`, `pdMS_TO_TICKS` | `esp32_gateway.c` → `app_main` | Ana döngünün akış şemasını çizin |
| 33 | Kuyruk: `xQueueCreate/Send/Receive`, `FromISR` sürümleri, kopyalayarak taşıma | `s_rx_queue` | PC'de dairesel kuyruk (ring buffer) yazın |
| 34 | Kesme ve callback kuralları: kısa, bloklamayan; `user_ctx`, `volatile` | `can_bus_rx_callback` | Callback'te yapılmaması gerekenleri listeleyin |
| 35 | UART: `uart_config_t`, `uart_read_bytes`, `printf` ile aynı UART | `initialize_serial_input` | Seri girdiyi taklit eden PC programı |
| 36 | **Tekrar** | — | Ring buffer + satır ayırıcıyı birleştirin |

## Hafta 7 – `can_bus.c` derin okuma

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 37 | Init satır satır: modlar, listen-only, `tx_gpio = -1` tasarımı | `can_bus_init` | Passive/diagnostic farkını kendi cümlelerinizle yazın |
| 38 | Durum makinesi: start/stop/deinit | `can_bus_start/stop/deinit`, `s_can_status` | Aynı durum makinesini PC'de stub'larla kurun |
| 39 | RX yolu: callback → kuyruk → `can_bus_receive`; kuyruk dolarsa ne olur | `can_bus_rx_callback`, `can_bus_receive` | Kayıp frame sayacı ekleme tasarımı |
| 40 | TX yolu: ID/DLC doğrulama, zaman aşımı; passive modda TX neden yasak | `can_bus_transmit`, `test_obd_passive_tx_guard` | `validate_frame()` fonksiyonu + testler |
| 41 | Loopback ve transceiver testleri; loopback neden yalnızca denetleyicinin içini test eder | `can_bus_run_loopback_test` | Test sonuçlarını yorumlayan karar tablosu |
| 42 | **Tekrar + kod inceleme** | `can_bus.c/.h` | Bulduğunuz hata ve iyileştirme listesi |

## Hafta 8 – `can_stats` ve OBD-II

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 43 | Sabit boyutlu tablo, lineer arama, periyot hesabı | `can_stats.c` | Aynı modülü PC'de yazıp test edin |
| 44 | OBD-II: modlar, `0x7DF` fonksiyonel / `0x7E0` fiziksel istek, `0x7E8` cevap, `+0x40`, NRC `0x7F` | `obd_protocol.c` sabitleri | İstek/cevap frame'lerini elle yazın |
| 45 | Mode 01 PID formülleri | `obd_decode_pid_response` | 4 PID decoder'ı + test vektörleri |
| 46 | Mode 03 DTC kodlaması: 2 byte → `P0301` | `obd_decode_dtc` | Decoder'ı kendiniz yazın + testler |
| 47 | İstek/bekleme, zaman aşımı, yanlış cevabı ayıklama, birden çok ECU | `obd_request_pid`, `obd_wait_pid_response` | Riskleri listeleyin |
| 48 | **Tekrar** | — | Ham frame alıp decode eden mini OBD kütüphanesi |

## Hafta 9 – ISO-TP (sizin yazacağınız modül)

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 49 | ISO-TP: Single/First/Consecutive Frame, Flow Control, sıra no, BS, STmin | `OBD_MAX_DTCS_SINGLE_FRAME 3` (bugünkü sınır) | Örnek çok frame'li cevabı elle çözün |
| 50 | Alıcı tasarımı: durum makinesi, buffer, hata durumları | — | Tasarımı kağıda yazın; ben inceleyeyim |
| 51 | Uygulama 1: SF + FF | — | `isotp.h` / `isotp.c` |
| 52 | Uygulama 2: CF + sıra kontrolü + zaman aşımı | — | Devamı |
| 53 | Test: 10 DTC'lik çok frame'li cevap, kayıp frame, yanlış sıra | — | Test dosyası |
| 54 | **Tekrar** | — | Tasarım değerlendirmesi |

## Hafta 10 – Bütünleşme ve kapanış

| Gün | Konu | Projedeki kod | Alıştırma |
|---|---|---|---|
| 55 | Ana döngünün tamamı | `process_serial_input`, `process_can_input` | Uçtan uca akış şeması |
| 56 | Ön işlemci: `#define`, `#if`, makro tuzakları, derleme zamanı anahtarları | `OBD_VEHICLE_TEST_ENABLED`, `CAN_AUTO_BITRATE_PROBE_ENABLED` | Güvenli bir makro yazın |
| 57 | Güvenlik analizi: kod hangi koşulda hatta aktif mesaj gönderebilir? | Tüm firmware | Risk listesi |
| 58 | ISO-TP'yi firmware'e entegre etme tasarımı | `obd_protocol.c` | 🔧 Yalnızca build |
| 59 | Bulunan sorunları önceliklendirme | — | Sorun listesi |
| 60 | Kapanış: aynı modülün C++ sürümüne bakış, sonraki yol | — | — |

## İlerleme

Bitirdiğiniz günü `[x]` yapın; bir sonraki oturumda kaldığımız yeri buradan buluruz.

- [ ] Hafta 1 (gün 1–6)
- [ ] Hafta 2 (gün 7–12)
- [ ] Hafta 3 (gün 13–18)
- [ ] Hafta 4 (gün 19–24)
- [ ] Hafta 5 (gün 25–30)
- [ ] Hafta 6 (gün 31–36)
- [ ] Hafta 7 (gün 37–42)
- [ ] Hafta 8 (gün 43–48)
- [ ] Hafta 9 (gün 49–54)
- [ ] Hafta 10 (gün 55–60)
