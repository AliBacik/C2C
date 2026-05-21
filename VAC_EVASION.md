# VAC Neden C2C'yi Göremez? — Tam Teknik Analiz

> Bu belge, C2C yazılımının Valve Anti-Cheat (VAC) sisteminden nasıl gizlendiğini,
> hem teknik hem de kavramsal düzeyde, sıfır bilgiden başlayarak anlatır.

---

## İçindekiler

1. [VAC Nedir ve Nasıl Çalışır?](#1-vac-nedir-ve-nasıl-çalışır)
2. [Ring 0 vs Ring 3: Temel Güç Farkı](#2-ring-0-vs-ring-3-temel-güç-farkı)
3. [Manual Mapping: İz Bırakmayan Yükleme](#3-manual-mapping-i̇z-bırakmayan-yükleme)
4. [Kernel Cache Temizleme: Üç Katmanlı Silme](#4-kernel-cache-temizleme-üç-katmanlı-silme)
5. [Rastgele Driver Adı: Her Seferinde Yeni Kimlik](#5-rastgele-driver-adı-her-seferinde-yeni-kimlik)
6. [XOR Şifreli Driver: Statik Analize Kapalı Kapı](#6-xor-şifreli-driver-statik-analize-kapalı-kapı)
7. [IndependentPages Modu: Dağıtık Bellek](#7-independentpages-modu-dağıtık-bellek)
8. [Anti-Cheat Servislerini Durdurma](#8-anti-cheat-servislerini-durdurma)
9. [PE Header Yıkımı: Kernel'da İz Bırakmama](#9-pe-header-yıkımı-kernelda-i̇z-bırakmama)
10. [Pencere Başlığı Gizleme: OBS ve Ekran Yakalama](#10-pencere-başlığı-gizleme-obs-ve-ekran-yakalama)
11. [Genel Tablo: Hangi Yöntem Neyi Engelliyor?](#11-genel-tablo-hangi-yöntem-neyi-engelliyor)

---

## 1. VAC Nedir ve Nasıl Çalışır?

### VAC'ın Kısaca Tarihçesi

Valve Anti-Cheat, Valve Corporation tarafından 2002 yılında geliştirilmiş, CS:GO, CS2, TF2 gibi Steam oyunlarında kullanılan bir hile karşıtı sistemdir. VAC, **kullanıcı modunda (User Mode / Ring 3)** çalışır.

### VAC Ne Yapar?

VAC oyunun süreçlerine (process) bağlanır ve periyodik olarak şu kontrolleri yapar:

```
1. Bellek Tarama (Memory Scanning)
   → Bilinen hile imzalarını (signature) bellekte arar
   → Değiştirilmiş oyun fonksiyonlarını tespit etmeye çalışır

2. Modül Listesi Kontrolü
   → Oyunun process'ine yüklenmiş DLL'leri listeler
   → Tanımadığı modülleri şüpheli sayar

3. API Hook Tespiti
   → Windows sistem fonksiyonlarının değiştirilip değiştirilmediğini kontrol eder

4. Cheat Database Karşılaştırması
   → Tespit edilen hileler kara listeye alınır
   → Sonraki taramalar bu bilgiyle güncellenir
```

### VAC'ın Temel Sınırlaması

VAC, **kullanıcı modunda (Ring 3)** çalışır. Bu, VAC'ın yalnızca şunları görebildiği anlamına gelir:

- Kullanıcı modunda çalışan process'ler
- Kullanıcı moduna yüklenmiş DLL'ler  
- Kullanıcı modundaki bellek bölgeleri

Kernel modundaki (Ring 0) kod ve bellek, VAC'ın **hiçbir zaman ulaşamayacağı** bir bölgededir.

---

## 2. Ring 0 vs Ring 3: Temel Güç Farkı

### CPU'nun Halka Sistemi

Modern Intel ve AMD işlemciler, kodun ne kadar yetkiyle çalışacağını belirlemek için **"Privilege Ring"** (Yetki Halkası) sistemi kullanır:

```
┌──────────────────────────────────────────────────────────┐
│                    CPU Privilege Rings                   │
│                                                          │
│   ┌────────────────────────────────────────────────┐     │
│   │  Ring 0 — Kernel Mode (Çekirdek Modu)          │     │
│   │  • İşletim sistemi çekirdeği burada çalışır    │     │
│   │  • Her RAM adresine erişebilir                 │     │
│   │  • Tüm donanıma doğrudan erişebilir            │     │
│   │  • Hiçbir kısıtlama yok                        │     │
│   └────────────────────────────────────────────────┘     │
│                         ↑                                │
│            Buraya erişmek çok zor!                       │
│                         ↓                                │
│   ┌────────────────────────────────────────────────┐     │
│   │  Ring 3 — User Mode (Kullanıcı Modu)           │     │
│   │  • Tüm uygulamalar burada çalışır              │     │
│   │  • VAC, CS2, Chrome, VS Code — hepsi burada   │     │
│   │  • Kendi bellek alanıyla sınırlı               │     │
│   │  • Kernel belleğini okuyamaz                   │     │
│   └────────────────────────────────────────────────┘     │
└──────────────────────────────────────────────────────────┘
```

### Bu Fark Ne Anlama Geliyor?

Şöyle düşün: Ring 3, bir binanın zemin katı. Ring 0 ise binanın çatı katı, güvenlikli bölge. VAC zemin katta geziyor. C2C'nin kernel driver'ı çatı katta yaşıyor. VAC çatıya çıkamaz, o yüzden C2C'yi göremez.

```
VAC (Ring 3)         C2C Kernel Driver (Ring 0)
     ↓                           ↑
 CS2 process'ini tarar      Kernel bellekte yaşar
 DLL listesine bakar        CS2'nin belleğini kernel'dan okur
 Hook kontrol eder          VAC'ın ulaşamadığı bölgede
     ↓                           ↑
 BURADA HİÇBİR ŞEY GÖREMEZ ─────┘
```

### C2C'nin Kernel'dan Bellek Okuması

C2C'nin kernel driver'ı, CS2'nin belleğini doğrudan kernel katmanından okur. IOCTL sistemi üzerinden user-mode tarafa iletir:

```
CS2 Belleği (fiziksel RAM)
        ↓
C2C Kernel Driver (Ring 0) ← MmCopyMemory() kullanır
        ↓  IOCTL üzerinden
C2C User Mode (Ring 3) ← ReadMemory() çağrısı
        ↓
ESP, Radar çizimi
```

VAC bu zincirin hiçbir halkasını göremez çünkü kritik olan ilk iki halka Ring 0'da.

---

## 3. Manual Mapping: İz Bırakmayan Yükleme

### Normal Driver Yükleme Nasıl Çalışır?

Normalde Windows'a bir driver (sürücü) yüklemek için şu adımlar gerekir:

```
1. Driver'ın Microsoft tarafından dijital imzalanmış olması lazım
   → Windows, imzasız driver'ları reddeder (Kernel-mode Code Signing)

2. Driver, Windows Service Manager'a kayıt edilmeli
   → Registry'ye yazılır: HKLM\SYSTEM\CurrentControlSet\Services\

3. NtLoadDriver() ile yüklenir
   → Windows, yüklenen driver'ı kendi listelerine ekler
   → PiDDB cache, MmUnloadedDrivers, KernelHashBucketList güncellenir

4. VAC veya başka anticheat bu listeleri tarayabilir
   → Tanımadığı driver'ı tespit eder → BAN
```

### Manual Mapping Nedir?

Manual Mapping (Manuel Haritalama), bir driver'ı Windows'un standart mekanizmaları **kullanmadan** kernel belleğine yüklemektir. kdmapper.cpp bunu yapar:

```cpp
// kdmapper.cpp — MapDriver() fonksiyonu
ULONG64 kdmapper::MapDriver(BYTE* data, ...) {
    
    // 1. PE formatını doğrula
    const PIMAGE_NT_HEADERS64 nt_headers = portable_executable::GetNtHeaders(data);
    
    // 2. Kernel'da bellek ayır (Windows'un listelerine GEÇMİYOR)
    kernel_image_base = intel_driver::AllocatePool(
        nt::POOL_TYPE::NonPagedPool, 
        image_size
    );
    
    // 3. Relocation table'ı düzelt (adres farkını hesapla)
    RelocateImageByDelta(relocs, kernel_image_base - image_base);
    
    // 4. Import Address Table'ı çöz (bağımlı fonksiyonları bul)
    ResolveImports(imports);
    
    // 5. Stack cookie'yi düzelt
    FixSecurityCookie(local_image_base, kernel_image_base);
    
    // 6. Hazırlanmış kodu kernel belleğine yaz
    intel_driver::WriteMemory(kernel_image_base, fixed_image, size);
    
    // 7. Driver'ın giriş noktasını çağır
    intel_driver::CallKernelFunction(&status, entry_point, ...);
}
```

### Neden Windows Bunu Farketmiyor?

Çünkü:

- `NtLoadDriver()` hiç çağrılmıyor → Registry'ye kayıt yok
- Windows Driver Manager listesine eklenmedi
- Dijital imza doğrulaması atlandı
- Bellekmiş gibi görünüyor, driver değil

Anticheat sistemleri `PsLoadedModuleList` gibi kernel listelerini tarar. C2C bu listeye hiç girmez.

---

## 4. Kernel Cache Temizleme: Üç Katmanlı Silme

Kernel, yüklenen driver'ları takip etmek için birden fazla iç yapı (internal structure) kullanır. Manual mapping yapılsa bile bazı izler kalabilir. C2C üç farklı cache yapısını temizler:

### 4.1 PiDDB Cache Temizleme

**PiDDB Cache nedir?**

Windows'un `PiDDBCacheTable` adlı bir hash tablosu vardır. Her yüklenen driver'ın hash değerini ve zaman damgasını (timestamp) buraya yazar. Bu sayede daha önce yüklenen bilinen kötü driver'ları hatırlar.

**C2C bu tablodan silinir:**

```cpp
// kdmapper, PiDDBCacheTable'dan driver kaydını temizler
// intel_driver içinde bu yapı bulunur ve entry silinir
// Böylece kernel "bu driver yüklenmiş" diye düşünemez
```

**Eğer temizlenmeseydi ne olurdu?**

Driver'ın zaman damgası ve ismi tabloda kalırdı. İleriki bir taramada (örn. başka bir anticheat) bu tabloyu okursa, imzasız bir driver yüklendiğini anlayabilirdi.

### 4.2 KernelHashBucketList Temizleme

**Bu yapı nedir?**

Windows, yüklenen modüllerin (driver'lar dahil) hash değerlerini `KernelHashBucketList` adlı bir linked list'te tutar. Bu liste, kernel integrity (bütünlük) kontrolleri için kullanılır.

**C2C temizleme:**

```cpp
// kdmapper bu listeden de driver'ın hash kaydını siler
// Liste boşalmış gibi görünür, driver yüklenmemiş izlenimi verir
```

### 4.3 MmUnloadedDrivers Temizleme

**Bu yapı nedir?**

`MmUnloadedDrivers`, son 50 driver unload (boşaltma) işleminin kaydını tutar. Normal akışta: driver yükle → driver boşalt → buraya kayıt gir.

**C2C temizleme:**

```cpp
// Eğer driver bir şekilde listeye girdiyse
// MmUnloadedDrivers'dan da silinir
// "Bu driver hiç var olmadı" gibi görünür
```

### Üç Katmanın Önemi

```
Anticheat Sistem       Ne Kontrol Eder?          C2C'nin Cevabı
─────────────────    ──────────────────────    ──────────────────────
Kernel Integrity  →  PiDDB Cache            →  Temizlendi ✓
Module Scanning   →  KernelHashBucketList   →  Temizlendi ✓
Unload History    →  MmUnloadedDrivers      →  Temizlendi ✓
```

---

## 5. Rastgele Driver Adı: Her Seferinde Yeni Kimlik

### Sorun: Bilinen İsimler Kara Listeye Alınır

Bir anticheat sistemi, bilinen hile driver adlarını (örn. "c2c_driver", "cheathax") kara listesine alabilir. Bir sonraki çalıştırmada aynı ismi görünce hemen engeller.

### Çözüm: Her Çalıştırmada Farklı Rastgele İsim

`intel_driver.cpp`'deki `GetDriverNameW()` fonksiyonu:

```cpp
// intel_driver.cpp
std::wstring intel_driver::GetDriverNameW() {
    // Rastgele karakterlerden oluşan driver adı üretir
    // Örnek: "xk7mPqZ2v" — her çalıştırmada farklı
    std::wstring name = L"";
    static const wchar_t alphabet[] = L"abcdefghijklmnopqrstuvwxyz0123456789";
    
    srand((unsigned)time(0));
    int len = 6 + rand() % 8; // 6 ile 14 karakter arası
    
    for (int i = 0; i < len; i++)
        name += alphabet[rand() % (sizeof(alphabet)/sizeof(wchar_t) - 1)];
    
    return name;
}
```

**Sonuç:** Her çalıştırmada farklı bir servis adı oluşturulur. Anticheat sistemleri, kara listedeki isimlerle eşleştiremez.

```
1. Çalıştırma:  servis adı = "mq7xk2p"   → anticheat görmez
2. Çalıştırma:  servis adı = "bz9rt1wv"  → anticheat görmez  
3. Çalıştırma:  servis adı = "nf3als8"   → anticheat görmez
...
Hiçbiri kara listede değil!
```

---

## 6. XOR Şifreli Driver: Statik Analize Kapalı Kapı

### Statik Analiz Nedir?

Anticheat sistemleri, şüpheli programları iki şekilde analiz edebilir:

1. **Dinamik Analiz:** Program çalışırken belleği taramak
2. **Statik Analiz:** Program çalışmadan dosyayı analiz etmek (byte imzaları aramak)

### XOR Şifreleme Nedir?

XOR (Exclusive OR), iki bit arasındaki temel mantık işlemi:

```
0 XOR 0 = 0
0 XOR 1 = 1
1 XOR 0 = 1
1 XOR 1 = 0

Özelliği: A XOR KEY XOR KEY = A  (iki kez XOR yaparsan başa dönersin)
```

Bu özellik şifreleme için mükemmeldir:

```
Şifrele:  VERI XOR ANAHTAR = ŞİFRELİ
Çöz:      ŞİFRELİ XOR ANAHTAR = VERI
```

### C2C'nin Kullanımı: cfg.h + RollingVectorProcedure

`cfg.h` dosyasında driver binary'si XOR şifreli olarak saklanır:

```cpp
// cfg.h (basitleştirilmiş)
namespace cfg {
    // Driver binary'si XOR şifreli byte dizisi olarak
    inline std::vector<BYTE> image = {
        0xAB, 0x7F, 0x3C, 0x91, ...  // şifreli bytes
    };
    
    // XOR anahtarı
    inline std::vector<BYTE> key = {
        0x4D, 0x2A, 0x8E, ...
    };
}
```

`main.cpp`'de yükleme sırasında şifre çözülür:

```cpp
// main.cpp
RollingVectorProcedure(cfg::image, cfg::key);
// Bu fonksiyon image üzerinde XOR işlemi yapar
// Şifreli bytes → Gerçek driver bytes
img = cfg::image.data();
```

### Statik Analize Karşı Neden Etkili?

```
Disk üzerinde (dosya olarak):
  C2C.exe içinde → şifreli bytes: 0xAB 0x7F 0x3C ...
  → Anticheat imza taraması yapar → HİÇBİR TANIMI EŞLEŞMİYOR ✓

RAM'de (çalışma sırasında):
  RollingVectorProcedure çözüyor → gerçek driver bytes
  → Bu nokta SADECE kısa bir süre bellekte var
  → Hemen kernel'a yüklenip orijinal bellek temizleniyor
  → Anticheat yakalarsa görebilir AMA pencere çok dar
```

---

## 7. IndependentPages Modu: Dağıtık Bellek

### Normal Bellek Tahsisi

Normalde kernel'da büyük bellek bloğu tahsis edildiğinde (`AllocatePool`), bellek **bitişik** (contiguous) bir bölgede konumlanır:

```
Kernel Belleği (RAM):
┌──────┬──────┬──────┬──────┬──────┬──────┐
│Sayfa │Sayfa │Sayfa │Sayfa │Sayfa │Sayfa │
│  1   │  2   │  3   │  4   │  5   │  6   │
└──────┴──────┴──────┴──────┴──────┴──────┘
 ↑_________________________↑
   Tüm bu alan bir blok: driver buraya yerleşir
   → Anticheat bitişik bölge taraması yapabilir
```

### IndependentPages Modu

`--securemode` parametresiyle (`indPagesMode = true`) aktif olur:

```cpp
// main.cpp
bool indPagesMode = CheckArg(argc, argv, L"securemode");

// kdmapper.cpp
if (indPagesMode)
    mode = kdmapper::AllocationMode::AllocateIndependentPages;

// kernel_image_base tahsisi:
kernel_image_base = intel_driver::MmAllocateIndependentPagesEx(image_size);
```

`MmAllocateIndependentPages`, her sayfayı (page = 4KB'lık bellek birimi) **bağımsız olarak** tahsis eder:

```
Kernel Belleği (RAM):
┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
│Sayfa │ BOŞ  │Sayfa │ SİS  │Sayfa │ BOŞ  │Sayfa │ SİS  │
│  1   │      │  2   │      │  3   │      │  4   │      │
└──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
 ↑       ↑       ↑
 Farklı yerlere dağıtılmış — bitişik değil
```

**Neden daha güvenli?**

Anticheat sistemleri büyük bitişik kernel bellek bloklarını tarayabilir. Dağıtık sayfalarda imza aramak çok daha zordur; her sayfayı ayrı ayrı bulmak gerekir.

Ayrıca `IndependentPages` ile her sayfaya **ayrı ayrı bellek koruma ayarı** yapılabilir:

```cpp
// kdmapper.cpp — IndependentPages modunda section korumaları
for (int i = 0; i < sections; i++) {
    if (sec->Characteristics & IMAGE_SCN_MEM_EXECUTE)
        prot = PAGE_EXECUTE_READ;      // Kod bölgesi: çalıştırılabilir
    else if (sec->Characteristics & IMAGE_SCN_MEM_WRITE)
        prot = PAGE_READWRITE;         // Veri bölgesi: yazılabilir
    else
        prot = PAGE_READONLY;          // Diğer: sadece okunabilir
    
    intel_driver::MmSetPageProtection(secAddr, secSize, prot);
}
```

Bu, gerçek bir signed driver gibi davranır ve anormallikleri azaltır.

---

## 8. Anti-Cheat Servislerini Durdurma

### Rakip Anticheat Sistemleri

CS2 yalnızca VAC kullanmaz. Bazı durumlarda FACEIT, ESEA gibi üçüncü parti anticheat sistemleri de yüklü olabilir. Ayrıca Riot'un Vanguard'ı (Valorant için) da kernel seviyesinde çalışır.

C2C yüklenirken bu servislerin durdurulması gerekir:

```cpp
// main.cpp
system("sc stop faceit >nul 2>&1");  // FACEIT anticheat durdur
system("sc stop vgc >nul 2>&1");     // Vanguard client durdur
system("sc stop vgk >nul 2>&1");     // Vanguard kernel driver durdur
```

### Ayrıca Kernel Güvenlik Özellikleri Devre Dışı

```cpp
// main.cpp — CheckWindowsKernelPrefs() başarısız olursa uygulanan ayarlar
system("reg add HKLM\\SYSTEM\\...\\HypervisorEnforcedCodeIntegrity /v Enabled /d 0");
// HVCI: Kernel modüllerinin imzasını zorla kontrol eder → KAPATILMALI

system("reg add HKLM\\SYSTEM\\...\\Lsa /v RunAsPPL /d 0");
// PPL: Protected Process Light → kernel erişimini kısıtlar → KAPATILMALI

system("bcdedit /set hypervisorlaunchtype off");
// Hyper-V: Sanallaştırma tabanlı güvenlik → KAPATILMALI

system("reg add ...\\CI\\Config /v VulnerableDriverBlocklistEnable /d 0");
// Bilinen kötü driver'ların yüklenmesini engelleyen liste → KAPATILMALI
```

**Neden bu ayarlar önemli?**

- **HVCI (Hypervisor Protected Code Integrity):** Kernel'a yüklenen her kod modülünün imzalı olmasını zorunlu kılar. Açıksa manual mapping başarısız olur.
- **Vulnerable Driver Blocklist:** Intel'in istismar edilen `iqvw64e.sys` driver'ı bu listede! Liste aktifse Intel driver yüklenemez, C2C çalışamaz.

---

## 9. PE Header Yıkımı: Kernel'da İz Bırakmama

### PE Header Nedir?

Her Windows çalıştırılabilir dosyasının başında **PE Header** (Portable Executable Header) bulunur. Bu header, dosya hakkında meta bilgi içerir:

```
PE Header İçeriği:
├── DOS Header (MZ imzası)
├── PE Signature ("PE\0\0")
├── COFF File Header (mimari, bölüm sayısı)
└── Optional Header (giriş noktası, boyutlar, import tablosu, ...)
```

Anticheat sistemleri, kernel belleğinde "MZ" ve "PE" imzalarını arayarak manual-mapped modülleri bulabilir.

### C2C'nin Header Yıkımı

`destroyHeader = true` (varsayılan) ile:

```cpp
// kdmapper.cpp
DWORD TotalVirtualHeaderSize = (IMAGE_FIRST_SECTION(nt_headers))->VirtualAddress;
image_size = image_size - (destroyHeader ? TotalVirtualHeaderSize : 0);

// Header atlanarakkernel'a yazılmıyor
if (!intel_driver::WriteMemory(realBase, 
    (PVOID)((uintptr_t)local_image_base + (destroyHeader ? TotalVirtualHeaderSize : 0)), 
    image_size))
```

Kernel bellekteki görünüm:

```
Header Yıkımı YOK:          Header Yıkımı VAR:
┌──────────────────┐        ┌──────────────────┐
│ MZ...PE...Header │        │ .text section    │ ← Kod doğrudan başlar
│ .text section    │        │ .data section    │
│ .data section    │        │ .rdata section   │
└──────────────────┘        └──────────────────┘
  ↑ Anticheat "MZ"            ↑ Anticheat "MZ"
    bulabilir                   BULAMAZ
```

---

## 10. Pencere Başlığı Gizleme: OBS ve Ekran Yakalama

### Tehdit: Ekran Kaydı

OBS (Open Broadcaster Software) gibi araçlar ve ekran yakalama yazılımları, **pencere adlarına göre** hangi pencerenin kaydedileceğini seçebilir. "C2C", "cheat", "hack" gibi isimler içeren pencereler şüphe uyandırabilir.

### Çözüm: RandTitle() — Rastgele Başlık

`Init.h` içindeki (yorum satırında görülen) fonksiyon:

```cpp
// usermode/Core/Init.h
static void RandTitle()
{
    srand(time(0));
    constexpr int length = 25;
    const auto characters = TEXT("0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
    TCHAR title[length + 1]{};

    for (int j = 0; j < length; j++)
    {
        title[j] += characters[rand() % 63];
    }

    SetConsoleTitle(title);  // Konsol başlığını rastgele yap
}
```

Bu fonksiyon çalıştırıldığında konsol penceresi şu şekilde görünür:

```
Başlık OLMADAN:    "C2C - External Cheat"
                   → OBS bunu yakalayabilir, stream'de görünebilir

Başlık İLE:        "mK7xpQ2nBs4vRtW8jL3d"
                   → Anlamsız karakter dizisi, kimse ne olduğunu anlamaz
```

---

## 11. Genel Tablo: Hangi Yöntem Neyi Engelliyor?

| Güvenlik Yöntemi | Engellenen Tehdit | Nasıl Çalışır? |
|---|---|---|
| **Ring 0 Kernel Driver** | VAC ve tüm user-mode anticheat | Driver kernel'da yaşar, VAC ulaşamaz |
| **Manual Mapping** | Windows Driver Manager tespiti | NtLoadDriver() kullanılmaz, liste yok |
| **PiDDB Cache Temizleme** | Kernel hash tablosu tespiti | Kernel'ın driver hatıra listesinden silindi |
| **KernelHashBucketList Temizleme** | Kernel bütünlük kontrolleri | Hash listesinden silindi |
| **MmUnloadedDrivers Temizleme** | Driver geçmiş taraması | Unload geçmişinden silindi |
| **Rastgele Driver Adı** | İsim bazlı kara liste | Her çalıştırmada farklı isim |
| **XOR Şifreli Binary** | Statik imza taraması | Disk'te şifreli görünür, imza eşleşmez |
| **IndependentPages Modu** | Bitişik bellek taraması | Dağıtık sayfalar, bulmak zor |
| **Anti-Cheat Servisleri Durdurma** | FACEIT, Vanguard tespiti | Rakip sistemler kapatılır |
| **HVCI/Hypervisor Devre Dışı** | Kernel imza zorlaması | Manual mapping bloğu kaldırılır |
| **PE Header Yıkımı** | "MZ/PE" imzası arama | Kernel'da başlık yok, tanınamaz |
| **Pencere Başlığı Gizleme** | OBS/ekran yakalama | Rastgele başlık, tanımlanamaz |

---

## Özet: Neden VAC Göremez?

C2C, **çok katmanlı bir gizlilik mimarisi** kullanır:

```
1. MİMARİ KATMAN
   VAC Ring 3'te çalışır.
   C2C Ring 0'da çalışır.
   Aralarında aşılmaz bir kısıtlama var.

2. YÜKLEME KATMANI
   Normal driver yükleme: Windows'a kayıt, imza kontrol, liste girişi.
   Manual mapping: Hiçbiri yok. Adeta yokmuş gibi kernel'da var.

3. KERNELİZLEME KATMANI
   Kernel'ın kendi takip yapıları (PiDDB, HashBucket, UnloadedDrivers)
   temizlenir. Kernel bile "bu driver geldi mi?" diye sorsa cevap: "hayır."

4. KİMLİK KATMANI
   Her çalıştırmada yeni rastgele isim.
   Kara listeler anlamsız kalır.

5. DOSYA KATMANI
   Driver binary'si XOR şifreli.
   Disk'te hiçbir bilinen imzayla eşleşmez.

6. BELLEK KATMANI
   IndependentPages ile dağıtık yerleşim.
   Bitişik tarama işe yaramaz.

7. ORTAM KATMANI
   Rakip anticheat servisleri, HVCI, Hypervisor kapatılır.
   C2C'ye karşı çıkacak sistem yok.
```

Bu katmanların hepsi birlikte çalıştığında, VAC ve benzeri kullanıcı modu anticheat sistemleri C2C'yi tespit edemez.

---

*Belgeler: [COMPLETE_CODE_GUIDE.md](kernel/COMPLETE_CODE_GUIDE.md) — [CODE_IMPLEMENTATION.md](usermode/CODE_IMPLEMENTATION.md)*
