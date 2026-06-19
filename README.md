# C2C — CS2 External Kernel Cheat

External, read-only, kernel-mode CS2 hilesi. VAC ve user-mode anticheat sistemlerine karşı tasarlanmıştır.

---

## Nasıl Çalışır?

C2C iki parçadan oluşur:

**1. Kernel Mapper (`C2C-kernel.exe`)**
- Intel'in imzalı `iqvw64e.sys` driver'ını kullanarak kernel'a erişim sağlar
- Asıl C2C driver'ını (`C2C-kmd`) manual mapping ile kernel'a yükler
- Windows'un driver listelerine kayıt olmaz, iz bırakmaz

**2. Usermode Cheat (`C2C.exe`)**
- Kernel driver'a bağlanır
- CS2'nin belleğini kernel üzerinden **sadece okur**, yazmaz
- Overlay olarak CS2 penceresine yapışır, DirectX 11 ile çizim yapar

**Çalıştırma sırası:**
1. `C2C-kernel.exe` → driver'ı kernel'a map eder
2. `C2C.exe` → driver'a bağlanır, CS2'yi bekler
3. CS2 açıldığında otomatik bağlanır

> İki dosya aynı klasörde olmalıdır. Usermode, kernel'ı bulamazsa `C2C-kernel.exe`'yi otomatik çalıştırır.

---

## VAC Neden Göremez?

| Katman | Açıklama |
|---|---|
| **Ring 0** | VAC user-mode (Ring 3) çalışır. Driver kernel-mode (Ring 0) yaşar. VAC ulaşamaz. |
| **Manual Mapping** | `NtLoadDriver()` çağrılmaz. Windows driver listesine girilmez. |
| **Cache Temizleme** | `PiDDB Cache`, `KernelHashBucketList`, `MmUnloadedDrivers` temizlenir. |
| **Rastgele Driver Adı** | Her çalıştırmada farklı servis adı. İsim bazlı kara listeler işe yaramaz. |
| **XOR Şifreli Binary** | Driver diskte şifreli saklanır. Statik imza taraması eşleşme bulamaz. |
| **PE Header Yıkımı** | Kernel bellekte "MZ/PE" imzası yok. Taramalar driver'ı bulamaz. |
| **Dağıtık Bellek** | `--securemode` ile driver bağımsız sayfalara dağıtılır. Bitişik tarama işe yaramaz. |
| **Sadece Okuma** | CS2 belleğine yazılmıyor. Oyun davranışı değişmiyor, tespit riski düşük. |

---

## Özellikler

### ESP
- Skeleton (iskelet) çizimi — renk ayarlanabilir
- Head box (kafa çemberi) — renk ayarlanabilir
- Team check — takım arkadaşlarını atla
- Visible check — sadece görünen düşmanları göster
- Flash check — kör olunca ESP devre dışı

### Radar
- Mini harita üzerinde düşman noktaları
- 3 farklı nokta tipi: daire, ok, ok+daire
- Boyut, mesafe, oran, saydamlık ayarlanabilir
- Özel radar modu

### Trigger Bot
- Crosshair düşmana gelince otomatik ateş
- ALT tuşu basılı tutulurken aktif
- Ayarlanabilir gecikme (10–500ms), ±15ms random jitter
- Scope check — AWP/SSG/SCAR/G3SG1 için scope şartı
- Team check ile takım arkadaşlarını atla

### WallHack Helper
- Düşmanlar ESP'de başta kırmızı görünür — vurman riskli
- Görünür olduktan belirli süre sonra yeşile döner — vurman safe
- **Cold Peek** (default 250ms) — düşman aniden çıktıysa, insan reaksiyon süresi baz alındı
- **Pre-Aim Peek** (default 90ms) — zaten o yöne nişan alıyorduysan daha kısa süre
- Düşmanın üzerinde kalan süre gösterir (örn. `134ms`)
- Smooth kırmızı → yeşil renk geçişi
- Ayarlanabilir süreler (50–800ms)

### Behavior Meter (Analytics)
- Oyuncunun davranışını analiz eder, şüpheli pattern'leri uyarır
- **Wall Tracking** — duvar arkasındaki düşmanı crosshair ile takip edince tespit eder
- **Peek Reaction** — düşman görününce reaksiyon süresini ölçer, ortalaması gösterilir
- **Headshot Oranı** — çok yüksekse uyarır
- **TriggerBot Tutarlılığı** — ateş pattern'i çok düzenli olursa uyarır
- Yeşil (Legit) / Sarı (Caution) / Kırmızı (Obvious!) bar
- Ekran ortasında büyük fade uyarı mesajı

### Diğer
- Watermark — FPS ve hız göstergesi
- Anti-Record (OBS bypass) — overlay ekran kaydında görünmez
- Config sistemi — ayarları kaydet/yükle
- Clear Traces — config klasörünü gizler, iz temizler

---

## Gereksinimler

- Windows 10/11
- HVCI kapalı
- Secure Boot kapalı
- FACEIT / Vanguard kapalı (otomatik durdurulur)
- CS2 (Steam)

---

## Derleme

Visual Studio 2022, x64 Release

1. `C2C.sln` aç
2. `kernel` projesini derle → `C2C-kernel.exe`
3. `user` projesini derle → `C2C.exe`
4. İkisini aynı klasöre koy

Driver binary'sini `Crypto` aracıyla şifreleyip `cfg.h`'e gömmek gerekir:
```
crypto gen key.bin
crypto crypt key.bin driver.sys
# çıktıyı cfg::image array'ine yapıştır
```
