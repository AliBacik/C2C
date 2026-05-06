# C2C - Usermode Komple Kod Rehberi
## Sıfır Bilgiyle Başlayanlar İçin Adım Adım, Kelime Kelime, Hiç Bir Şey Atlanmadan Anlatım

---

# ÖNSÖZ: BU REHBERİ KİME YAZDIM?

Sevgili okuyucu, bu rehberi yazarken senin hakkında hiçbir şey bilmediğimi varsaydım. Belki de hayatında ilk defa kod görüyorsun. Belki bilgisayarın "RAM" denen şeyini ilk kez duyuyorsun. Belki de "matematik" deyince aklına lise yıllarında öğretmen tahtaya yazdığı zorlu denklemler geliyor. Hiç önemli değil. Bu rehber, hayatında programlama veya bilgisayar konusunda hiçbir şey bilmeyen birinin bile, sonuna kadar okuduğunda her şeyi anlayabileceği şekilde yazıldı.

Önce sana çok temel kavramlardan başlayacağım: bilgisayar nedir, bellek nedir, oyunlar nasıl çalışır. Sonra programlamanın ne demek olduğunu, C++ dilinin nasıl kullanıldığını anlatacağım. Daha sonra yavaş yavaş ESP, Radar, View Matrix gibi karmaşık görünen kavramlara geleceğiz. Hepsini sıfırdan, hiçbir şey atlamadan anlatacağım.

Hızlı okuma. Sabırlı ol. Bir şeyi anlamadıysan geri dön ve tekrar oku. Çünkü programlama bir defada öğrenilen bir şey değil, zamanla anlaşılan bir şeydir.

---

# BÖLÜM 0: HİÇ BİR ŞEY BİLMİYORUM, EN BAŞTAN ANLATIR MISIN?

## 0.1 Bilgisayar Nasıl Çalışır?

Bilgisayar dediğimiz şey, çok hızlı hesap yapan elektronik bir makinedir. Her saniye milyarlarca işlem yapabilir. Ama yapabildiği işlemler çok basittir: toplama, çıkarma, karşılaştırma, bir yerden başka bir yere veri taşıma. Bu basit işlemleri çok hızlı ve sırayla yaptığı için karmaşık şeyler ortaya çıkar (oyunlar, internet siteleri, video düzenleme programları vb.).

Bir bilgisayarın iki ana parçası vardır:

**İşlemci (CPU - Central Processing Unit):** Bilgisayarın beyni. Hesapları yapan ve kararları veren kısım. Çok hızlıdır ama az sayıda şey hatırlayabilir.

**Bellek (RAM - Random Access Memory):** Bilgisayarın hafızası. İşlemcinin üzerinde çalıştığı veriler burada saklanır. Çok şey hatırlayabilir ama işlemciye göre yavaştır.

Şöyle düşün: Sen bir matematik problemi çözüyorsun. Beynin işlemci, defterin de bellek. Beynin hızlı düşünür ama az sayıda rakamı aynı anda tutabilir. Defterine yazarsın, sonra ihtiyaç olunca bakarsın. Bilgisayar da aynen böyle çalışır.

Bellek aslında dev bir kasalar dizisi gibidir. Her kasanın bir numarası vardır (mesela 0, 1, 2, 3, ... şeklinde milyarlarca). Her kasanın içine 8 bit (yani 0 ile 255 arasında bir sayı) konabilir. İşlemci, "şu numaralı kasanın içinde ne var?" diye sorabilir veya "şu numaralı kasaya şunu yaz" diyebilir.

Bu kasa numaralarına **bellek adresi** (memory address) denir. Modern bilgisayarlarda adresler çok büyük sayılardır. Genelde **onaltılık (hexadecimal)** sayı sisteminde yazılırlar. Çünkü çok büyük sayılar normal sistemde yazınca uzun olur, onaltılıkta daha kısa olur.

**Onaltılık sistem nedir?** Normal sayılar 0-9 arasındadır (10 farklı rakam). Onaltılık sistemde ise 0-9 ve A-F arasındadır (16 farklı rakam). Yani:
- 10 yerine A
- 11 yerine B
- 12 yerine C
- 13 yerine D
- 14 yerine E
- 15 yerine F

16 sayısı ise "10" olarak yazılır (onaltılıkta). Onaltılık bir sayı yazılırken başına `0x` öneki konur, böylece "Bu onaltılık sayıdır" denmiş olur. Mesela `0x7FFF1234` bir bellek adresi. Bu sayı normal sistemde 2,147,488,820 gibi büyük bir sayıdır.

## 0.2 Oyunlar Nasıl Çalışır?

Bir bilgisayar oyunu da bir programdır. Çalıştığında bellekte yer kaplar. Oyunun bütün verileri bellekte saklanır:

- Senin karakterin nerede? (x, y, z koordinatları)
- Senin canın ne kadar? (mesela 100)
- Düşmanlar nerede? (her birinin x, y, z koordinatları)
- Düşmanların canı ne kadar?
- Hangi silahı kullanıyorsun?
- ... ve daha çok şey

Bu veriler bellekteki belirli adreslerde saklanır. Mesela senin karakterinin x koordinatı 0x7FFF1234 adresinde olabilir. Düşmanın canı 0x7FFF5678 adresinde olabilir.

Şimdi şunu düşün: Eğer biz bu adresleri bilirsek ve içlerini okuyabilirsek, oyunun her şeyini "görebiliriz". Yani düşman ekranda görünmese bile, biz duvarın arkasındaki düşmanın bellekte nerede olduğunu bilebiliriz.

İşte **ESP** ve **Radar** denen şeyler tam olarak bunu yapar:
- **ESP (Extra Sensory Perception, yani Ekstra Duyusal Algı):** Oyuncuların yerini, canını vb. ekranda gösterir. Hatta duvar arkasındakilerin bile.
- **Radar:** Bütün oyuncuları bir mini haritada gösterir.

## 0.3 Programlama Nedir?

Programlama, bilgisayara ne yapacağını söylemektir. Bilgisayar Türkçe veya İngilizce anlamaz. Sadece sayılarla ifade edilen makine kodunu anlar. Ama insanlar makine kodunu doğrudan yazamaz, çok zordur.

Bu yüzden **programlama dilleri** icat edilmiştir. C++, Python, JavaScript gibi diller. Bu dillerle insan tarafından okunabilir kod yazılır, sonra bir program (derleyici - compiler) bu kodu makine koduna çevirir.

Bu projede **C++** kullanılıyor. C++ güçlü ve hızlı bir dildir. Oyun programlamada, sürücü yazımında ve performans gerektiren her şeyde tercih edilir.

## 0.4 C++ Temel Bilgileri

Şimdi sana C++'ın temel kavramlarını anlatacağım. Hiç bilmiyorsan bile takip edebileceksin.

### 0.4.1 Değişkenler ve Veri Tipleri

Bir **değişken** (variable), bellekte bir yer ayırıp ona isim veren şeydir. Mesela:

```cpp
int yaş = 25;
```

Bu satır şunu yapıyor: Bellekte 4 byte'lık bir yer ayır, oraya 25 sayısını yaz, ve bu yere "yaş" adını ver. Artık biz "yaş" deyince bilgisayar o belleğe gider.

Buradaki `int` kelimesine **veri tipi** (data type) denir. Değişkenin ne tür veri tutacağını söyler. C++'ta birçok veri tipi vardır:

| Tip Adı | Boyut | Ne Tutar | Aralık |
|---------|-------|----------|--------|
| `bool` | 1 byte | Doğru/Yanlış | true veya false |
| `char` | 1 byte | Tek karakter | 'a', 'A', '5', '!' vb. |
| `int` | 4 byte | Tam sayı | -2,147,483,648 ile 2,147,483,647 |
| `unsigned int` | 4 byte | Negatif olmayan tam sayı | 0 ile 4,294,967,295 |
| `float` | 4 byte | Ondalıklı sayı | 3.14, -0.5 vb. |
| `double` | 8 byte | Daha hassas ondalıklı sayı | 3.14159265358979 vb. |

Windows programlamada karşına özel tipler de çıkar:

| Tip Adı | Aslında Ne | Kullanım |
|---------|-----------|----------|
| `BYTE` | unsigned char (1 byte) | Tek byte tutmak için |
| `WORD` | unsigned short (2 byte) | 16 bit sayılar |
| `DWORD` | unsigned int (4 byte) | 32 bit sayılar |
| `DWORD64` | unsigned long long (8 byte) | 64 bit sayılar (büyük adresler) |
| `HANDLE` | Pointer (8 byte) | Windows'a özel kaynak göstergeci |
| `BOOL` | int | true/false (Windows'un kendi versiyonu) |

`DWORD64`, bellek adreslerini tutmak için sık kullanılır. Çünkü modern bilgisayarlarda adresler 64-bit yani 8 byte'lık sayılardır.

### 0.4.2 Operatörler

Operatörler, değişkenler üzerinde işlem yaptıran işaretlerdir.

**Aritmetik operatörler:**
- `+` toplama
- `-` çıkarma
- `*` çarpma
- `/` bölme
- `%` mod (kalan, mesela `7 % 3 = 1`)

**Karşılaştırma operatörleri:**
- `==` eşit mi? (dikkat, tek `=` atama, çift `==` karşılaştırma)
- `!=` eşit değil mi?
- `<` küçük mü?
- `>` büyük mü?
- `<=` küçük veya eşit mi?
- `>=` büyük veya eşit mi?

**Mantıksal operatörler:**
- `&&` "VE" - ikisi de doğruysa doğru
- `||` "VEYA" - en az biri doğruysa doğru
- `!` "DEĞİL" - tersini al

**Atama operatörleri:**
- `=` atama
- `+=` üstüne ekle (örn: `x += 5` demek `x = x + 5` demek)
- `-=` üstünden çıkar
- `*=` çarp
- `/=` böl

**Bit operatörleri (ileri seviye, sonra göreceğiz):**
- `&` bit AND
- `|` bit OR
- `^` bit XOR
- `~` bit NOT
- `<<` sola kaydır
- `>>` sağa kaydır

### 0.4.3 Pointer (İşaretçi) - ÇOK ÖNEMLİ

Bu kavram başlangıçta kafa karıştırıcı gelebilir, ama çok önemli olduğu için iyice anlamamız lazım.

Bir **pointer** (işaretçi), bir bellek adresini tutan değişkendir. Yani değerin kendisini değil, değerin **nerede olduğunu** tutar.

Şöyle düşün: Sen bir kütüphanedesin. Bir kitabın içeriğini hatırlamak yerine, kitabın hangi rafın hangi numarasında olduğunu hatırlıyorsun. İşte pointer böyle bir şey - kitabın yeri. İçeriğine bakmak istediğinde, o yere gidip bakıyorsun.

C++'ta bir pointer şöyle yazılır:

```cpp
int x = 42;        // x adında bir tamsayı, değeri 42
int* p = &x;       // p adında bir pointer, x'in adresini tutuyor
```

Burada:
- `int*` "tamsayıya işaret eden pointer" demek. Yıldız (`*`) "pointer" demek.
- `&x` "x'in adresi" demek. Ampersand (`&`) "adresini al" demek.

Yani biz `p`'nin içine `x`'in bellekteki adresini koyduk. `p`'nin değeri mesela `0x7FFE1234` gibi bir şey olur.

Pointer'ın işaret ettiği değere ulaşmak için yine `*` kullanırız:

```cpp
int değer = *p;    // değer = 42 (p'nin işaret ettiği yerdeki şey)
*p = 100;          // p'nin işaret ettiği yere 100 yaz (yani x = 100)
```

Burada dikkat: `int*` (tip tanımında) ile `*p` (kullanım sırasında) farklı şeyler. Tip tanımındaki `*` "pointer" demek, kullanımdaki `*` ise "işaret ettiği yere git" demek.

**Neden pointer'ları kullanırız?**

1. **Büyük veriyi hızlı geçirmek için:** Bir fonksiyona büyük bir veri vereceksen, kopyalamak yavaştır. Onun yerine adresini verirsin, fonksiyon o adresten okur.

2. **Belleği yönetmek için:** Çalışma zamanında bellek ayırmak için.

3. **Aynı veriyi birden fazla yerden değiştirmek için:** Eğer birinin değişkenini değiştirmek istiyorsan, ona pointer verirsin.

### 0.4.4 Referans (Reference)

Referans, pointer'a benzer ama biraz farklıdır. Bir değişkenin **takma adı** (alias) gibidir. Bir kez atandıktan sonra başka bir şeye işaret edemez.

```cpp
int x = 42;
int& r = x;      // r artık x'in başka adı
r = 100;         // x de 100 oldu (çünkü r aslında x)
```

`int&` "tamsayıya referans" demek. Pointer'la farkı: Referansa erişmek için `*` kullanmazsın, doğrudan değişken gibi kullanırsın.

Referanslar genelde fonksiyon parametrelerinde kullanılır:

```cpp
void Topla5(int& x) {
    x += 5;      // x'i 5 artır (gerçek değişkeni etkiler)
}

int sayı = 10;
Topla5(sayı);    // sayı artık 15
```

### 0.4.5 Fonksiyon

Bir **fonksiyon**, belirli bir işi yapan kod parçasıdır. Bir kere yazılır, çok kere çağrılır.

```cpp
int Topla(int a, int b) {
    int sonuç = a + b;
    return sonuç;
}
```

Bu fonksiyonun parçaları:
- `int`: Dönüş değerinin tipi (bu fonksiyon bir tamsayı döndürecek)
- `Topla`: Fonksiyonun adı
- `(int a, int b)`: Parametreler. İki tane tamsayı alıyor.
- `{ ... }`: Fonksiyonun gövdesi
- `return sonuç`: Sonucu döndür

Bu fonksiyonu çağırmak için:

```cpp
int x = Topla(3, 5);    // x artık 8
```

### 0.4.6 Struct (Yapı)

Birden fazla değişkeni bir araya getiren tiptir.

```cpp
struct Oyuncu {
    int can;
    int kalkan;
    float x;
    float y;
    float z;
};

Oyuncu ben;
ben.can = 100;
ben.kalkan = 50;
ben.x = 10.5f;
```

Burada `Oyuncu` adında bir tip oluşturduk. İçinde 5 alan var. Sonra `ben` adında bir `Oyuncu` oluşturduk ve alanlarına değer atadık. Erişmek için `.` kullanıyoruz.

`f` harfi ondalık sayıların sonuna konur, "bu sayı float tipinde" demek. Yoksa double olarak kabul edilir.

### 0.4.7 Class (Sınıf)

Class, struct'a çok benzer ama daha güçlüdür. Hem veri hem de fonksiyon (yani **metod**) içerebilir.

```cpp
class Oyuncu {
public:
    int can;
    
    void Hasar_Al(int miktar) {
        can -= miktar;
        if (can < 0) can = 0;
    }
    
    bool Hayatta_Mi() {
        return can > 0;
    }
};

Oyuncu ben;
ben.can = 100;
ben.Hasar_Al(30);          // ben.can artık 70
if (ben.Hayatta_Mi()) {     // true
    // ...
}
```

`public:` kelimesi, "bu kısımdakiler dışarıdan erişilebilir" demek. Bir de `private:` var, "sadece içeriden erişilebilir" demek.

### 0.4.8 Template (Şablon)

Şablon, "herhangi bir tip için çalışan" kod yazmamızı sağlar.

```cpp
template <typename T>
T En_Buyuk(T a, T b) {
    if (a > b) return a;
    return b;
}

int x = En_Buyuk<int>(5, 10);          // x = 10
float y = En_Buyuk<float>(3.5f, 2.1f); // y = 3.5
```

`template <typename T>` demek, "T adında bir tip parametresi var, kullanılırken belirtilecek." Çağırırken `<int>` veya `<float>` diyerek T'nin ne olacağını söylüyoruz.

### 0.4.9 Namespace (İsim Alanı)

Kodları gruplamak ve isim çakışmalarını önlemek için kullanılır.

```cpp
namespace ESPConfig {
    bool ESPenabled = true;
    int FontSize = 14;
}

// Erişim:
ESPConfig::ESPenabled = false;
```

`::` operatörü "scope resolution operator" denir, "şunun içindeki şu" demek.

### 0.4.10 const Anahtar Kelimesi

`const` "değişmez" demektir.

```cpp
const int MAKS_OYUNCU = 64;
MAKS_OYUNCU = 100;  // HATA! const olduğu için değiştirilemez
```

Fonksiyon parametrelerinde de kullanılır:

```cpp
void Yazdır(const std::string& metin) {
    // metin'i değiştiremem
    std::cout << metin;
}
```

Burada `const std::string&` demek, "bir string referansı al, ama değiştirme."

---

# BÖLÜM 1: PROJENİN GENEL YAPISI

## 1.1 Usermode Nedir?

Daha önce öğrendiğimiz gibi, bilgisayarda iki çalışma modu var: kernel modu (Ring 0) ve kullanıcı modu (Ring 3). Bu projedeki **usermode** klasörü, kullanıcı modunda çalışan programdır.

Yani ana mantık şöyle:
- Kernel klasöründeki driver (sürücü), Ring 0'da çalışır ve oyunun belleğini okuyabilir.
- Usermode klasöründeki program, Ring 3'te çalışır. Driver ile konuşarak oyunun belleğini okur, ekranda ESP/Radar gibi şeyler çizer.

## 1.2 Klasör Yapısı

Usermode klasörünün içinde şu klasörler var:

```
usermode/
├── main.cpp                       → Programın başladığı yer
├── Core/                          → Çekirdek sistem dosyaları
│   ├── Cheats.cpp/.h              → Ana hile mantığı
│   ├── Config.h                   → Tüm ayar değişkenleri
│   ├── GUI.h                      → Menü arayüzü
│   ├── Init.h                     → Başlangıç fonksiyonları
│   ├── MemoryMgr.cpp/.h           → Bellek okuma yöneticisi
│   └── Render.h                   → Ekrana çizim fonksiyonları
├── Features/                      → Özellikler
│   ├── ESP.h                      → ESP çizimi
│   ├── Radar.cpp/.h               → Radar çizimi
│   └── Misc.cpp/.h                → Diğer özellikler (Bypass, Anti-Record)
├── Game/                          → Oyun ile ilgili
│   ├── Bone.cpp/.h                → Kemik (skeleton) sistemi
│   ├── Entity.cpp/.h              → Oyuncu (varlık) sınıfı
│   ├── Game.cpp/.h                → Oyun yönetimi
│   └── View.h                     → Kamera ve view matrix
├── Helpers/                       → Yardımcılar
│   ├── KeyManager.h               → Klavye dinleme
│   ├── Mouse.cpp/.h               → Fare kontrolü
│   ├── StorageMgr.cpp/.h          → Disk işlemleri
│   └── WebApi.h                   → İnternet istekleri
├── Config/                        → Ayar kaydetme/yükleme
│   ├── ConfigSaver.cpp/.h         → JSON kaydet/yükle
│   └── ConfigMenu.cpp/.h          → Menüde gösterme
├── Offsets/                       → Bellek offset'leri
│   └── Offsets.cpp/.h             → Oyun verilerinin adresleri
├── OS-ImGui/                      → Çizim kütüphanesi (ImGui)
└── Resources/                     → İkonlar, fontlar, dil dosyaları
```

## 1.3 Programın Akışı

Program çalıştığında şunlar olur:

1. **main.cpp** çalışır.
2. **Init.h**'taki başlangıç fonksiyonları çağrılır: pencere oluşturulur, ImGui başlatılır, kernel driver'a bağlanılır.
3. Sonsuz bir döngü başlar: her saniye 60-144 kez:
   - Oyuncu listesi güncellenir (kim nerede?)
   - View matrix okunur (kamera nereye bakıyor?)
   - ESP ve Radar çizilir
   - Menü görüntülenir (eğer açıksa)

Şimdi bu adımları detaylı görelim.

---

# BÖLÜM 2: BELLEK OKUMA SİSTEMİ - MemoryMgr

## 2.1 Genel Amaç

Programımızın temel görevi, oyunun belleğinden veri okumaktır. Bunu yapmak için kernel modundaki driver'ımızla konuşuruz. Bu konuşma **IOCTL** (Input/Output Control) denen bir Windows mekanizması ile yapılır.

Şöyle bir senaryo düşün: Sen kullanıcı modusunda bir programsın. Oyunun belleğine erişimin yok (çünkü Windows izin vermiyor). Ama kernel modunda bir driver'ın var, onun erişimi var. Sen driver'a "şu adresteki şu kadar byte'ı bana ver" diye mesaj gönderiyorsun. Driver gidip okuyor, sana geri yolluyor. İşte bu kadar.

## 2.2 IOCTL Kodları - MemoryMgr.h

Dosyamızın başında şu satırlar var:

```cpp
#define C2C_DEVICE 0x8000
#define IOCTL_ATTACH         CTL_CODE(C2C_DEVICE, 0x4452, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_READ           CTL_CODE(C2C_DEVICE, 0x4453, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_MODULE_BASE CTL_CODE(C2C_DEVICE, 0x4454, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_PID        CTL_CODE(C2C_DEVICE, 0x4455, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_BATCH_READ     CTL_CODE(C2C_DEVICE, 0x4456, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
```

Bu satırlar ne demek? Açıklayayım.

`#define` bir **makro** (macro) tanımlar. Makro, derleme zamanında metin değişimi yapar. Yani derleyici programı derlerken, kodda her `IOCTL_READ` gördüğünde onun yerine `CTL_CODE(C2C_DEVICE, 0x4453, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)` koyar.

`CTL_CODE` Windows'un sunduğu bir makrodur. Bir IOCTL kodu üretir. Parametreleri:
1. **Cihaz tipi** (`C2C_DEVICE = 0x8000`): Bu komutun hangi cihaz için olduğunu söyler. 0x8000 ve sonrası "kullanıcı tanımlı" cihazlar için kullanılır.
2. **Fonksiyon kodu** (`0x4453` vb.): Bu cihaz için hangi alt-komut. Her komut için farklı.
3. **Yöntem** (`METHOD_BUFFERED`): Veri nasıl aktarılacak. BUFFERED, "ara bellek kullanılacak" demek. Yani veri kullanıcı modundan kernel'e tampon üzerinden geçecek.
4. **Yetki** (`FILE_SPECIAL_ACCESS`): Hangi yetki gerekli.

Yani her IOCTL bir benzersiz numaradır. Driver'a "şu numaralı komutu gönderiyorum" deyince, driver hangi işi yapacağını bilir.

Listemizdeki komutlar:
- **IOCTL_ATTACH**: Driver'a "şu işleme bağlan" der. Hangi oyunu okuyacağımızı söyler.
- **IOCTL_READ**: "Şu adresteki şu kadar byte'ı oku" der.
- **IOCTL_GET_MODULE_BASE**: "Şu modülün başlangıç adresini bul ve bana ver" der.
- **IOCTL_GET_PID**: "Şu işlemin ID'sini bul" der.
- **IOCTL_BATCH_READ**: "Birden fazla okumayı aynı anda yap" der (hızlı toplu okuma).

## 2.3 MemoryMgr Sınıfı

```cpp
class MemoryMgr
{
public:
    MemoryMgr();
    ~MemoryMgr();
    bool ConnectDriver(const LPCWSTR name);
    bool DisconnectDriver();
    bool Attach(const DWORD pid);
    DWORD64 GetModuleBase(const wchar_t* moduleName);
    DWORD GetProcessID(const wchar_t* processName);
    bool BatchReadMemory(const std::vector<std::pair<DWORD64, SIZE_T>>& requests, void* output_buffer);
    
    template <typename ReadType>
    bool ReadMemory(DWORD64 address, ReadType& value, SIZE_T size = sizeof(ReadType));
    
private:
    DWORD ProcessID = 0;
    HANDLE kernelDriver = nullptr;
    // ... struct'lar
};
```

Bu sınıfın yaptığı işleri tek tek anlatayım.

`MemoryMgr()` ve `~MemoryMgr()`: Bunlar **constructor** (yapıcı) ve **destructor** (yıkıcı) fonksiyonlardır. Constructor, sınıftan bir nesne oluşturulduğunda otomatik çağrılır. Destructor, nesne yok edildiğinde otomatik çağrılır. Genelde kaynak ayırma ve serbest bırakma için kullanılırlar.

`bool ConnectDriver(const LPCWSTR name)`: Driver'a bağlanır. `LPCWSTR` daha önce gördüğümüz tip - değişmez bir Unicode metin pointer'ı. Yani driver'ın adını alır. Başarılıysa true, değilse false döndürür.

`bool DisconnectDriver()`: Driver bağlantısını koparır.

`bool Attach(const DWORD pid)`: Driver'a "şu işleme bağlan" der. `DWORD pid` parametresi process ID, yani işlem numarası. Her çalışan program bir PID'ye sahiptir.

`DWORD64 GetModuleBase(const wchar_t* moduleName)`: Bir modülün (DLL veya EXE) başlangıç adresini bulur. Mesela "client.dll"in oyun belleğinde 0x7FFE0000'da olduğunu bize söyler.

`DWORD GetProcessID(const wchar_t* processName)`: Bir işlemin adından PID'sini bulur. Mesela "cs2.exe"nin PID'sini almak için.

`private` kısmındaki değişkenler:
- `DWORD ProcessID = 0`: Bağlandığımız işlemin ID'si.
- `HANDLE kernelDriver = nullptr`: Driver'a bağlantı handle'ı. `nullptr` "boş pointer" demek.

## 2.4 ReadMemory() - Asıl Bellek Okuma

Şimdi en önemli fonksiyona bakalım. Bu **template** fonksiyon.

```cpp
template <typename ReadType>
bool ReadMemory(DWORD64 address, ReadType& value, SIZE_T size = sizeof(ReadType))
{
    if (kernelDriver != nullptr && ProcessID != 0)
    {
        if (address == 0 || address >= 0x7FFFFFFFFFFF || size == 0 || size > 0x1000) {
            return false;
        }

        if (address + size < address) {
            return false;
        }

        Request readRequest;
        readRequest.process_id = ULongToHandle(ProcessID);
        readRequest.target = reinterpret_cast<PVOID>(address);
        readRequest.buffer = &value;
        readRequest.size = size;

        BOOL result = DeviceIoControl(kernelDriver,
            IOCTL_READ,
            &readRequest,
            sizeof(readRequest),
            &readRequest,
            sizeof(readRequest),
            nullptr,
            nullptr);
        return result == TRUE;
    }
    return false;
}
```

Adım adım açıklayalım.

**`template <typename ReadType>`**

Bu fonksiyon herhangi bir tip için çalışır. Hangi tipte okumak istediğimizi söyleriz, fonksiyon ona göre çalışır. Mesela `ReadMemory<int>(...)` int okur, `ReadMemory<Vec3>(...)` Vec3 okur.

**`bool ReadMemory(DWORD64 address, ReadType& value, SIZE_T size = sizeof(ReadType))`**

Parametreler:
- `DWORD64 address`: Okunacak bellek adresi. 64-bit tamsayı.
- `ReadType& value`: Okunan veri buraya yazılacak. Referans olarak alıyoruz, böylece doğrudan değiştirebiliyoruz.
- `SIZE_T size = sizeof(ReadType)`: Kaç byte okunacak. Varsayılan olarak `sizeof(ReadType)`. `sizeof` operatörü bir tipin kaç byte olduğunu söyler. Mesela `sizeof(int)` = 4.

`= sizeof(ReadType)` ifadesi **varsayılan parametre** (default parameter) demek. Eğer kullanan size parametresini vermezse, otomatik olarak tipin boyutu kullanılır.

**İlk kontrol: `if (kernelDriver != nullptr && ProcessID != 0)`**

Driver'a bağlandık mı ve bir işleme attach olduk mu? İkisi de OK ise içeri gir.

**Güvenlik kontrolleri:**

```cpp
if (address == 0 || address >= 0x7FFFFFFFFFFF || size == 0 || size > 0x1000) {
    return false;
}
```

Bu satır şunları kontrol ediyor:
- `address == 0`: Adres 0 mı? Çoğu sistemde 0 geçersiz adres.
- `address >= 0x7FFFFFFFFFFF`: Adres çok büyük mü? Bu sayıdan büyük adresler kullanıcı modu için geçersiz.
- `size == 0`: Boyut 0 mı? Anlamsız.
- `size > 0x1000`: 0x1000 = 4096 byte = 4KB. Bundan fazla okuma istemiyoruz. Çok büyük okumalar şüpheli.

Eğer herhangi biri doğruysa false döndür ve fonksiyon biter.

```cpp
if (address + size < address) {
    return false;
}
```

Bu **integer overflow** (taşma) kontrolü. Eğer `address + size` çok büyürse, taşıp negatif veya küçük bir sayıya dönebilir. O zaman `address + size < address` olur. Bu kontrol böyle bir durumu engeller.

**Request struct'ını doldur:**

```cpp
Request readRequest;
readRequest.process_id = ULongToHandle(ProcessID);
readRequest.target = reinterpret_cast<PVOID>(address);
readRequest.buffer = &value;
readRequest.size = size;
```

`Request` adında bir struct oluşturduk. Bu struct daha önce private kısmında tanımlandı:

```cpp
typedef struct _Request
{
    HANDLE process_id;
    PVOID target;
    PVOID buffer;
    SIZE_T size;
} Request, *PRequest;
```

Açıklamalar:
- `HANDLE process_id`: Hangi işleme bağlanacağız (PID).
- `PVOID target`: Okunacak hedef adres. `PVOID` "Pointer to VOID", yani herhangi bir şeye işaret eden pointer.
- `PVOID buffer`: Okunan verinin yazılacağı yer.
- `SIZE_T size`: Boyut.

Şimdi doldurma:

`readRequest.process_id = ULongToHandle(ProcessID);`: PID'yi HANDLE'a çevir. `ULongToHandle` Windows'un sağladığı dönüşüm fonksiyonu. Çünkü PID bir `DWORD` (32-bit) ama HANDLE 64-bit olabilir.

`readRequest.target = reinterpret_cast<PVOID>(address);`: Adresi PVOID'e çevir. `reinterpret_cast` C++'ta tip dönüşümünün katı yöntemi. "Bu byte'ları farklı bir tip olarak yorumla" demek.

`readRequest.buffer = &value;`: `value`'nun adresini ver. Driver okuduğunu buraya yazacak.

`readRequest.size = size;`: Boyutu kopyala.

**DeviceIoControl çağrısı:**

```cpp
BOOL result = DeviceIoControl(kernelDriver,
    IOCTL_READ,
    &readRequest,
    sizeof(readRequest),
    &readRequest,
    sizeof(readRequest),
    nullptr,
    nullptr);
```

`DeviceIoControl` Windows'un IOCTL gönderme fonksiyonu. Parametreleri:
1. `kernelDriver`: Driver'a açık handle.
2. `IOCTL_READ`: Hangi komut.
3. `&readRequest`: Girdi tampon adresi.
4. `sizeof(readRequest)`: Girdi boyutu.
5. `&readRequest`: Çıktı tampon adresi (aynı şeyi kullanıyoruz, çünkü driver buraya yazacak).
6. `sizeof(readRequest)`: Çıktı boyutu.
7. `nullptr`: Geri dönen byte sayısı (kullanmıyoruz).
8. `nullptr`: Asenkron işlem yok (senkron yapacağız).

`return result == TRUE;`: Sonuç TRUE ise true döndür.

**Kullanım Örneği:**

```cpp
int can = 0;
if (memoryManager.ReadMemory<int>(0x7FFF1234, can)) {
    std::cout << "Düşmanın canı: " << can << std::endl;
}
```

Burada `0x7FFF1234` adresinden bir int okuduk. Eğer başarılıysa `can` değişkeni o değere eşit oldu.

Bu kadar! ReadMemory'nin tüm sırrı bu. Adres, hedef değişken, IOCTL gönderme.

## 2.5 BatchReadMemory() - Toplu Okuma

```cpp
template<typename T>
bool BatchReadStructured(const std::vector<DWORD64>& addresses, std::vector<T>& results) {
    if (addresses.empty()) return false;

    std::vector<std::pair<DWORD64, SIZE_T>> requests;
    requests.reserve(addresses.size());

    for (DWORD64 addr : addresses) {
        requests.emplace_back(addr, sizeof(T));
    }

    results.resize(addresses.size());
    return BatchReadMemory(requests, results.data());
}
```

Bu fonksiyon birden fazla adresi tek seferde okur. Niye? Çünkü her IOCTL çağrısı yavaş. Eğer 64 oyuncunun bilgisini okumak istiyorsan, 64 ayrı çağrı yerine bir tane yaparsan çok daha hızlı olur.

`std::vector<DWORD64>& addresses`: Okunacak adreslerin listesi. `std::vector` C++'ın dinamik dizisi.

`std::vector<T>& results`: Sonuçların yazılacağı liste.

`requests.reserve(addresses.size())`: Belleği önceden ayır. Performans için önemli.

`for (DWORD64 addr : addresses)`: Her adres için döngü.

`requests.emplace_back(addr, sizeof(T))`: Adres ve boyut çiftini listeye ekle. `emplace_back` `push_back`'e benzer ama biraz daha hızlı.

`results.resize(addresses.size())`: Sonuç listesinin boyutunu ayarla.

`BatchReadMemory(requests, results.data())`: Asıl toplu okuma. `.data()` vektörün içindeki dizinin pointer'ını verir.

---

# BÖLÜM 3: VEKTÖR MATEMATİĞİ - Vec2, Vec3

3D oyunlarda her şey vektörlerle ifade edilir. Pozisyon, hız, yön - hepsi vektör. Bu yüzden vektör matematiğini iyi anlamamız lazım.

## 3.1 Vec2 Nedir?

Vec2 = "Vector 2" yani iki boyutlu vektör. İki bileşeni var: x ve y.

```cpp
struct Vec2 {
    float x, y;
};
```

İki boyutlu vektör nedir? Aslında düzlemde bir nokta ya da yön. Mesela `(3, 4)` koordinatları, ya da "3 birim sağa, 4 birim yukarı" yönü.

## 3.2 Vec3 Nedir?

Vec3 = "Vector 3" yani üç boyutlu vektör. Üç bileşeni var: x, y, z.

```cpp
struct Vec3 {
    float x, y, z;
};
```

3D dünyada bir nokta veya yön. Mesela oyunda bir karakterin pozisyonu Vec3'tür.

- **x**: Yatay konum (sağa-sola)
- **y**: Derinlik (ileri-geri) ya da yukarı-aşağı (oyuna göre değişir)
- **z**: Dikey konum (yukarı-aşağı) ya da derinlik

## 3.3 Vektör İşlemleri

### Toplama ve Çıkarma

İki vektörü toplamak/çıkarmak, her bileşeni ayrı ayrı toplamak/çıkarmaktır:

```cpp
Vec3 a = {1, 2, 3};
Vec3 b = {4, 5, 6};

Vec3 toplam;
toplam.x = a.x + b.x;  // 5
toplam.y = a.y + b.y;  // 7
toplam.z = a.z + b.z;  // 9

Vec3 fark;
fark.x = b.x - a.x;    // 3
fark.y = b.y - a.y;    // 3
fark.z = b.z - a.z;    // 3
```

### Uzunluk (Length)

Bir vektörün **uzunluğu** veya **büyüklüğü**, başlangıçtan o noktaya olan mesafedir. **Pisagor teoremi** ile hesaplanır:

```
uzunluk = √(x² + y² + z²)
```

```cpp
float Length() const {
    return std::sqrt(x*x + y*y + z*z);
}
```

`std::sqrt` karekök fonksiyonu. `x*x` aslında `x²`. C++'ta üs alma operatörü yok, çarpıyoruz.

### Mesafe (Distance)

İki nokta arasındaki mesafe, fark vektörünün uzunluğudur:

```cpp
float DistanceTo(const Vec3& other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    float dz = z - other.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}
```

Düşman ile aramızdaki mesafeyi bulmak için bunu kullanırız.

### Normalize Etme

Bir vektörü **normalize etmek**, uzunluğunu 1 yapmaktır. Yani sadece yön bilgisi kalır. Bunu yön gösteren vektörlerle çalışırken yaparız.

```cpp
Vec3 Normalized() const {
    float len = Length();
    if (len == 0) return {0, 0, 0};
    return {x / len, y / len, z / len};
}
```

Her bileşeni uzunluğa böleriz. Eğer uzunluk 0 ise, sıfır vektörü döndürürüz (sıfıra bölünmeyi engellemek için).

### Skaler Çarpım (Dot Product)

İki vektörün skaler çarpımı:
```
a · b = a.x*b.x + a.y*b.y + a.z*b.z
```

Sonuç bir sayıdır (skaler), vektör değil. Şu özelliklere sahiptir:
- İki vektör arasındaki **açı** ile ilişkili.
- Eğer vektörler dik ise sonuç 0.
- Eğer aynı yöne bakıyorlarsa sonuç pozitif.
- Eğer ters yöne bakıyorlarsa sonuç negatif.

```cpp
float DotProduct(const Vec3& other) const {
    return x*other.x + y*other.y + z*other.z;
}
```

### Açı Hesaplama

İki vektör arasındaki açıyı bulmak için skaler çarpımı kullanırız:
```
cos(θ) = (a · b) / (|a| * |b|)
θ = acos((a · b) / (|a| * |b|))
```

`acos` ters kosinüs fonksiyonu. Kosinüs değerinden açıyı verir.

```cpp
float AngleTo(const Vec3& other) const {
    float dot = DotProduct(other);
    float lenA = Length();
    float lenB = other.Length();
    if (lenA == 0 || lenB == 0) return 0;
    float cos_angle = dot / (lenA * lenB);
    return std::acos(std::clamp(cos_angle, -1.0f, 1.0f)) * (180.0f / M_PI);
}
```

`std::clamp(cos_angle, -1.0f, 1.0f)`: Değeri -1 ile 1 arasına sınırla. Çünkü kayan nokta hatalarından dolayı bazen çok küçük taşmalar olabilir, ve `acos` -1 ile 1 dışında değer almaz.

`* (180.0f / M_PI)`: Radyan'dan dereceye çevir. `M_PI` π sayısı yaklaşık 3.14159.

### Radyan ve Derece

Açıları iki şekilde ifade edebiliriz:
- **Derece**: 0 ile 360 arası. İnsanlar için kolay.
- **Radyan**: 0 ile 2π arası. Matematik için kolay.

Dönüşümler:
- Radyan → Derece: `derece = radyan * (180/π)`
- Derece → Radyan: `radyan = derece * (π/180)`

Çoğu C++ matematik fonksiyonu radyan kullanır (cos, sin, tan vb.). Bu yüzden dönüşüm yaparız.

---

# BÖLÜM 4: VIEW MATRIX VE 3D'DEN 2D'YE DÖNÜŞÜM

## 4.1 Problem: 3D Dünyayı 2D Ekrana Çizmek

Oyunlar 3D dünyada geçer. Her şeyin x, y, z koordinatları var. Ama bizim ekranımız 2D - sadece x ve y. Peki nasıl 3D'den 2D'ye geçeceğiz?

İşte burada **View Matrix** (görüş matrisi) devreye girer.

Şöyle düşün: Sen bir kameraydın. Kamera bir yerden dünyaya bakıyor. Kameranın gördüğü her şeyi bir resim haline getiriyor (yani ekrana). Bu işleme **projeksiyon** denir.

## 4.2 Matris Nedir?

Bir **matris** (matrix), sayılardan oluşan dikdörtgen bir tablodur. Mesela 4x4 matris:

```
┌                      ┐
│ m00  m01  m02  m03  │
│ m10  m11  m12  m13  │
│ m20  m21  m22  m23  │
│ m30  m31  m32  m33  │
└                      ┘
```

16 sayı var. C++'ta bunu bir dizide tutabiliriz:

```cpp
float viewMatrix[16];
```

veya 2 boyutlu dizi:

```cpp
float viewMatrix[4][4];
```

View matrix, kameranın 3D dünyayı 2D ekrana nasıl yansıttığını anlatan bilgidir. Oyun motorları bu matrisi her frame'de hesaplar ve bellekte saklar. Biz onu okuyup kullanırız.

## 4.3 3D'den 2D'ye Dönüşüm

Bir 3D nokta `(x, y, z)`'yi 2D ekran noktası `(screen_x, screen_y)`'ye dönüştürmek için:

**Adım 1: Homojen Koordinata Çevir**

3D noktaya 4. bileşen ekleyip homojen koordinat yapıyoruz: `(x, y, z, 1)`.

**Adım 2: View Matrix ile Çarp**

Matrisi vektörle çarpıyoruz:

```
clip_x = m[0]*x + m[1]*y + m[2]*z + m[3]
clip_y = m[4]*x + m[5]*y + m[6]*z + m[7]
clip_z = m[8]*x + m[9]*y + m[10]*z + m[11]
clip_w = m[12]*x + m[13]*y + m[14]*z + m[15]
```

Bunu kodda yazarsak:

```cpp
float w = x * viewMatrix[12] + 
          y * viewMatrix[13] + 
          z * viewMatrix[14] + 
          viewMatrix[15];
```

**Adım 3: Kamera Arkasında mı Kontrol Et**

Eğer `w <= 0` ise, nokta kameranın arkasında. Yani göremeyiz, ekranda gösteremeyiz.

```cpp
if (w < 0.01f)
    return Vec2{-1, -1};  // Geçersiz
```

`0.01f` çok küçük bir değer. 0 değilse de yakınında olabilir, bu yüzden 0.01'den küçük olanları reddediyoruz.

**Adım 4: Perspektif Bölme**

Bu en sihirli kısım. clip_x'i w'ye, clip_y'yi w'ye böleriz. Bu, "uzaktaki şeyler küçük görünür" mantığını sağlar.

```cpp
float screenX = (x * viewMatrix[0] + 
                y * viewMatrix[1] + 
                z * viewMatrix[2] + 
                viewMatrix[3]) / w;

float screenY = (x * viewMatrix[4] + 
                y * viewMatrix[5] + 
                z * viewMatrix[6] + 
                viewMatrix[7]) / w;
```

Şimdi `screenX` ve `screenY` -1 ile 1 arasında değerlerdir (eğer ekran içindeyse). Bu **NDC** (Normalized Device Coordinates) denir.

**Adım 5: Piksel Koordinatına Çevir**

NDC'yi piksel koordinatlarına çeviriyoruz:

```cpp
float pixelX = (screenX + 1.0f) * windowWidth * 0.5f;
float pixelY = (1.0f - screenY) * windowHeight * 0.5f;
```

Niye `+1` ve sonra `*0.5`?
- screenX -1 ile 1 arasında.
- `screenX + 1` ile 0 ile 2 arasına geliyor.
- `* windowWidth * 0.5` ile 0 ile windowWidth arasına geliyor.

Niye `1.0f - screenY` (yani ters)?
- Ekran koordinatlarında y aşağı doğru artar (ters).
- 3D'de y yukarı doğru artar.
- Bu yüzden ters çevirmek gerekiyor.

## 4.4 Tam ToScreen Fonksiyonu

```cpp
Vec2 ToScreen(float viewMatrix[16]) const {
    // Adım 1-2: w hesapla
    float w = x * viewMatrix[12] + 
              y * viewMatrix[13] + 
              z * viewMatrix[14] + 
              viewMatrix[15];
    
    // Adım 3: Kamera arkasında mı?
    if (w < 0.01f)
        return Vec2{-1, -1};
    
    // Adım 4: Perspektif bölme
    float screenX = (x * viewMatrix[0] + 
                    y * viewMatrix[1] + 
                    z * viewMatrix[2] + 
                    viewMatrix[3]) / w;
    
    float screenY = (x * viewMatrix[4] + 
                    y * viewMatrix[5] + 
                    z * viewMatrix[6] + 
                    viewMatrix[7]) / w;
    
    // Adım 5: Piksel koordinatına çevir
    float pixelX = (screenX + 1.0f) * windowWidth * 0.5f;
    float pixelY = (1.0f - screenY) * windowHeight * 0.5f;
    
    return Vec2{pixelX, pixelY};
}
```

Bu fonksiyon, 3D bir noktayı ekrandaki 2D pozisyonuna çevirir. ESP'nin temeli budur.

---

# BÖLÜM 5: KEMİK SİSTEMİ - Bone.h

## 5.1 Kemik (Bone) Nedir?

Bilgisayar oyunlarında, karakterler **iskelet sistemi** ile animasyon yapar. Tıpkı insan iskeleti gibi. Karakterin vücudunda kemikler (bones) vardır:

- Kafa (head)
- Boyun (neck)
- Omurga (spine)
- Pelvis (kalça)
- Sol omuz, sol dirsek, sol bilek (left arm)
- Sağ omuz, sağ dirsek, sağ bilek (right arm)
- Sol uyluk, sol diz, sol ayak bileği (left leg)
- Sağ uyluk, sağ diz, sağ ayak bileği (right leg)

Her kemiğin 3D dünyada bir pozisyonu vardır. Karakter hareket ettikçe, kemikler de hareket eder.

ESP'nin amacı, bu kemiklerin pozisyonlarını okumak, ekranda 2D'ye çevirmek ve aralarına çizgi çekmektir. Böylece karakterin "iskeletini" ekranda görürüz.

## 5.2 BONEINDEX Enum

```cpp
enum BONEINDEX : DWORD
{
    head = 6,
    neck_0 = 5,
    spine_0 = 1,
    spine_1 = 2,
    spine_2 = 3,
    spine_3 = 4,
    pelvis = 0,
    arm_upper_L = 8,
    arm_lower_L = 9,
    hand_L = 10,
    arm_upper_R = 13,
    arm_lower_R = 14,
    hand_R = 15,
    leg_upper_L = 22,
    leg_lower_L = 23,
    ankle_L = 24,
    leg_upper_R = 25,
    leg_lower_R = 26,
    ankle_R = 27,
};
```

`enum` (enumeration), bir grup adlandırılmış sabit oluşturur. Burada `BONEINDEX` enum'u, her kemik için bir indeks numarası tanımlıyor. Mesela `head = 6` demek, oyunda kemik dizisinin 6. elemanı kafadır.

`enum BONEINDEX : DWORD` notasyonu, "bu enum'un altta yatan tipi DWORD olsun" diyor. Yani her enum değeri 4 byte'lık bir tamsayıdır.

Bu sayılar oyunun kendi kemik düzeninden geliyor. Her oyun farklı sayılar kullanır. Counter-Strike 2'de mesela bunlardır.

## 5.3 Quaternion Nedir?

Bir kemiğin sadece pozisyonu yetmez, **yönü** de lazım. Yani el yukarı mı bakıyor, aşağı mı? Quaternion bunu söyler.

**Quaternion**, 3D dönüşleri ifade etmenin matematiksel bir yoludur. 4 sayıdan oluşur: x, y, z, w.

```cpp
class Quaternion_t
{
public:
    float x, y, z, w;
};
```

Quaternion'ları **Euler açıları** (pitch, yaw, roll) yerine kullanırız çünkü:
- **Gimbal lock** sorununa neden olmaz (Euler'de bazı durumlarda dönme bilgisi kaybolur)
- İki dönüş arasında **interpolasyon** kolaydır (slerp)
- Çarpma yoluyla iki dönüş birleştirilebilir

Detayına girmeyeceğim, çünkü bizim için sadece okuyup kullanmak yeterli. Ama bilmen gereken şey: kemiklerin dönüşü quaternion ile ifade ediliyor.

## 5.4 BoneJointPos Struct'ı

```cpp
struct BoneJointPos {
    Vec3 Pos;
    Vec2 ScreenPos;
    bool IsVisible = false;
};
```

Bir kemik eklemi (bone joint) hakkında bilgi:
- `Vec3 Pos`: 3D dünyadaki pozisyon.
- `Vec2 ScreenPos`: 2D ekrandaki pozisyon (ToScreen ile hesaplanmış).
- `bool IsVisible = false`: Ekranda görünür mü? Varsayılan false.

`= false` syntax'ı struct alanına varsayılan değer verir. Yeni bir BoneJointPos oluşturulduğunda, IsVisible otomatik false olur.

## 5.5 CBone Sınıfı

```cpp
class CBone {
public:
    std::vector<BoneJointPos> BonePosList;
    std::vector<CBoneData> IBoneData;

    DWORD64 EntityPawnAddress = 0;
    DWORD64 GameSceneNode = 0;

    bool UpdateAllBoneData(const DWORD64& EntityPawnAddress);
    bool UpdateAllBoneDataBatch(const DWORD64& EntityPawnAddress);
};
```

Bu sınıf bir karakterin iskeletini temsil eder.

`std::vector<BoneJointPos> BonePosList`: Tüm kemik pozisyonlarının listesi.
`std::vector<CBoneData> IBoneData`: Tüm kemik verilerinin listesi (pozisyon + dönüş).

`DWORD64 EntityPawnAddress = 0`: Bu karakterin oyun belleğindeki adresi.
`DWORD64 GameSceneNode = 0`: Karakterin sahne grafiği düğümünün adresi (kemik bilgisinin saklandığı yapı).

`UpdateAllBoneData()`: Karakterin tüm kemiklerini günceller (her kemik için ayrı okuma).
`UpdateAllBoneDataBatch()`: Aynı işi toplu okuma ile yapar (daha hızlı).

## 5.6 BoneJointList Namespace

```cpp
namespace BoneJointList {
    inline std::list<DWORD> Trunk = { neck_0, spine_3, spine_2, spine_1, spine_0, pelvis };
    inline std::list<DWORD> LeftArm = { neck_0, arm_upper_L, arm_lower_L, hand_L };
    inline std::list<DWORD> RightArm = { neck_0, arm_upper_R, arm_lower_R, hand_R };
    inline std::list<DWORD> LeftLeg = { pelvis, leg_upper_L, leg_lower_L, ankle_L };
    inline std::list<DWORD> RightLeg = { pelvis, leg_upper_R, leg_lower_R, ankle_R };
    inline std::vector<std::list<DWORD>> List = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg };
}
```

Bu, iskeletin nasıl çizileceğini söyler. Her liste, sırayla bağlanacak kemiklerdir:

- **Trunk** (Gövde): Boyun → Omurga 3 → Omurga 2 → Omurga 1 → Omurga 0 → Pelvis. Bu sırayla çizgi çekilir.
- **LeftArm** (Sol Kol): Boyun → Sol Üst Kol → Sol Alt Kol → Sol El.
- **RightArm** (Sağ Kol): Boyun → Sağ Üst Kol → Sağ Alt Kol → Sağ El.
- **LeftLeg** (Sol Bacak): Pelvis → Sol Uyluk → Sol Diz → Sol Ayak Bileği.
- **RightLeg** (Sağ Bacak): Pelvis → Sağ Uyluk → Sağ Diz → Sağ Ayak Bileği.

`inline` anahtar kelimesi, "bu değişken birden fazla yerde tanımlandığında çakışma çıkmasın" demek.

`std::list<DWORD>` C++'ın bağlı liste yapısı. Vektöre benzer ama elemanlara erişimi farklıdır.

ESP çizimi yapılırken bu listeler dolaşılır. Her listede ardışık iki eleman alınır, aralarına çizgi çekilir. Mesela LeftArm:
- Çizgi 1: neck_0 → arm_upper_L
- Çizgi 2: arm_upper_L → arm_lower_L
- Çizgi 3: arm_lower_L → hand_L

3 çizgi ile sol kol çizilmiş olur. Tüm vücut için toplam ~17 çizgi vardır.

---

# BÖLÜM 6: ESP SİSTEMİ - ESP.h

## 6.1 ESP Nedir?

ESP = "Extra Sensory Perception" yani Ekstra Duyusal Algı. Oyunda göremediğimiz şeyleri (duvar arkasındaki düşmanlar gibi) bize gösteren özellik.

ESP genelde şunları gösterir:
- Kemik (Bone) çizgileri - oyuncunun iskeleti
- Kafa kutucuğu (Head Box) - kafanın etrafında daire/kare
- Vücut kutusu (Box) - oyuncunun etrafında kutu
- Mesafe yazısı
- Sağlık çubuğu
- Silah ikonu

Bu projede, sadece **kemik** ve **kafa dairesi** gösteriliyor.

## 6.2 ESP Namespace

```cpp
namespace ESP
{
    void RenderPlayerESP(const CEntity& LocalEntity, const CEntity& Entity, ImVec4 Rect, int LocalPlayerControllerIndex, int Index)
    {
        // ...
    }
    
    void RenderPreview(ImVec2 windowSize)
    {
        // ...
    }
    
    void RenderOutOfFOVArrow(const CEntity& LocalEntity, const CEntity& Entity) {}
}
```

Üç tane fonksiyon var:
- `RenderPlayerESP`: Bir oyuncunun ESP'sini çizer.
- `RenderPreview`: Menüde ön izleme çizer (gerçek oyuncu olmadan, ayarları görmek için).
- `RenderOutOfFOVArrow`: Görüş alanı dışındaki düşmanlar için ok çizer (boş bırakılmış).

## 6.3 RenderPlayerESP() Detayı

```cpp
void RenderPlayerESP(const CEntity& LocalEntity, const CEntity& Entity, ImVec4 Rect, int LocalPlayerControllerIndex, int Index)
{
    if (Entity.Pawn.Pos == Vec3(0, 0, 0))
        return;

    Render::DrawBone(Entity, ESPConfig::BoneColor, 1.3f);
    Render::DrawHeadCircle(Entity, ESPConfig::HeadBoxColor);
}
```

Adım adım:

**`const CEntity& LocalEntity`**: Bizim karakterimiz. `CEntity` oyuncu sınıfı, sonra göreceğiz.

**`const CEntity& Entity`**: ESP çizilecek hedef oyuncu (düşman).

**`ImVec4 Rect`**: ESP'nin çizileceği kutu. ImVec4 dört float'lı vektör (x, y, width, height).

**`int LocalPlayerControllerIndex`**: Bizim karakterimizin ID'si.

**`int Index`**: Hedefin ID'si.

**`if (Entity.Pawn.Pos == Vec3(0, 0, 0)) return;`**

Eğer hedefin pozisyonu (0,0,0) ise, geçersiz veri demek (oyuncu yüklenmemiş). Hiçbir şey çizme, fonksiyondan çık.

`Vec3(0, 0, 0)` bir Vec3 oluşturuyor. `==` operatörü iki Vec3'ün eşit olup olmadığını kontrol ediyor (Vec3 sınıfında bu operatör tanımlanmış olmalı).

**`Render::DrawBone(Entity, ESPConfig::BoneColor, 1.3f);`**

`Render` namespace'indeki `DrawBone` fonksiyonunu çağırıyor. Parametreler:
- `Entity`: Hedef oyuncu.
- `ESPConfig::BoneColor`: Kemiklerin rengi (kullanıcı tarafından ayarlanmış).
- `1.3f`: Çizgi kalınlığı.

Bu fonksiyon, oyuncunun tüm kemiklerini ekranda çizer.

**`Render::DrawHeadCircle(Entity, ESPConfig::HeadBoxColor);`**

Kafanın etrafına daire çizer.

## 6.4 RenderPreview() Detayı - Menüdeki Ön İzleme

```cpp
void RenderPreview(ImVec2 windowSize)
{
    if (!ESPConfig::ShowPreview)
        return;

    auto drawList = ImGui::GetWindowDrawList();
    auto& io = ImGui::GetIO();

    const ImVec2 rectSize(100, 150);
    const ImVec2 rectPos((windowSize.x - rectSize.x) * 0.45f, (windowSize.y - rectSize.y) * 0.3f);
    ImVec2 centerPos = ImGui::GetCursorScreenPos();
    centerPos.x += rectPos.x;
    centerPos.y += rectPos.y * -1.20f;
```

Bu fonksiyon menüde küçük bir önizleme çizer. Böylece kullanıcı renk ve ayarları değiştirdiğinde, gerçek oyuncu olmadan görsel değişiklik görür.

**`if (!ESPConfig::ShowPreview) return;`**: Önizleme kapalıysa hiç çizme.

**`auto drawList = ImGui::GetWindowDrawList();`**: ImGui'nin çizim listesini al. `auto` tipini otomatik bul (burada bir pointer olur). Bu liste üzerine çizgi, daire vb. ekleyebiliriz.

**`auto& io = ImGui::GetIO();`**: ImGui IO yapısı (input/output - giriş/çıkış). Klavye, fare durumu gibi şeyleri içerir. `&` referans yapıyoruz, kopyalamadan erişiyoruz.

**`const ImVec2 rectSize(100, 150);`**: Önizleme kutusunun boyutu - 100 piksel genişlik, 150 piksel yükseklik. `const` çünkü bu değer değişmeyecek.

**`const ImVec2 rectPos(...)`**: Kutunun pozisyonu, pencere boyutuna oranlı.

**`(windowSize.x - rectSize.x) * 0.45f`**: Pencere genişliğinden kutu genişliğini çıkar, 0.45 ile çarp. Yani biraz sağa kaydırılmış pozisyon.

**`ImVec2 centerPos = ImGui::GetCursorScreenPos();`**: ImGui'nin "imleç" konumunu al. ImGui'de yazdırma sırasında bir imleç vardır, bir sonraki şey orada çizilir.

```cpp
if (ESPConfig::ShowBoneESP) {
    const ImU32 boneColor = ESPConfig::BoneColor;
    struct BoneSegment { ImVec2 startOffset, endOffset; };
    const BoneSegment segments[] = {
        { {50, 25}, {60, 55} },
        { {60, 55}, {62, 65} },
        // ... daha çok segment
    };
    for (const auto& seg : segments) {
        ImVec2 start(centerPos.x + seg.startOffset.x, centerPos.y + seg.startOffset.y);
        ImVec2 end(centerPos.x + seg.endOffset.x, centerPos.y + seg.endOffset.y);
        drawList->AddLine(start, end, boneColor, 1.8f);
    }
}
```

Eğer kemik ESP'si açıksa, sabit bir iskelet çiziyoruz.

**`const ImU32 boneColor = ESPConfig::BoneColor;`**: Renk. `ImU32` 32-bit unsigned integer, ImGui'nin renk tipi (ARGB formatında: alfa, kırmızı, yeşil, mavi).

**`struct BoneSegment { ImVec2 startOffset, endOffset; };`**: Lokal struct. Her kemik segmenti bir başlangıç ve bitiş ofsetinden oluşuyor.

**`const BoneSegment segments[] = { ... }`**: Segmentlerin sabit dizisi. Her satır bir kemik. Mesela `{ {50, 25}, {60, 55} }` demek "(50,25)'den (60,55)'e çiz" demek.

**`for (const auto& seg : segments)`**: Her segment için döngü.

**`drawList->AddLine(start, end, boneColor, 1.8f);`**: Çizgi çiz! Parametreler:
- `start`: Başlangıç noktası
- `end`: Bitiş noktası
- `boneColor`: Renk
- `1.8f`: Kalınlık

```cpp
if (ESPConfig::ShowHeadBox) {
    drawList->AddCircle({ centerPos.x + 44, centerPos.y + 17 }, 12.0f, ESPConfig::HeadBoxColor, 0, 1.8f);
}
```

Eğer kafa kutusu açıksa, daire çiz:

**`drawList->AddCircle(...)`**: Daire çiz. Parametreler:
1. Merkez
2. Yarıçap (12 piksel)
3. Renk
4. Segment sayısı (0 = otomatik)
5. Çizgi kalınlığı

## 6.5 Gerçek Oyuncuda ESP Çizimi (Render::DrawBone)

ESP.h'da `RenderPlayerESP` fonksiyonu `Render::DrawBone`'u çağırıyordu. Bu fonksiyon ne yapıyor anlatayım (Render.h'da olmalı):

```cpp
namespace Render {
    void DrawBone(const CEntity& entity, ImColor color, float thickness) {
        // 1. Tüm kemiklerin ekran pozisyonlarını al
        // (entity.Bone.BonePosList'te zaten hesaplanmış)
        
        // 2. Her kemik segmenti için
        for (auto& boneList : BoneJointList::List) {
            // boneList şunlardan biri: Trunk, LeftArm, RightArm, LeftLeg, RightLeg
            
            auto it = boneList.begin();
            DWORD prev = *it;
            ++it;
            
            for (; it != boneList.end(); ++it) {
                DWORD curr = *it;
                
                // prev ve curr arasında çizgi çek
                Vec2 start = entity.Bone.BonePosList[prev].ScreenPos;
                Vec2 end = entity.Bone.BonePosList[curr].ScreenPos;
                
                // Eğer ikisi de görünür ise çiz
                if (entity.Bone.BonePosList[prev].IsVisible &&
                    entity.Bone.BonePosList[curr].IsVisible) {
                    ImGui::GetWindowDrawList()->AddLine(
                        ImVec2(start.x, start.y),
                        ImVec2(end.x, end.y),
                        color,
                        thickness
                    );
                }
                
                prev = curr;
            }
        }
    }
}
```

İşte bu kadar - kemik listelerinde dolaşıyoruz, her ardışık çift için aralarında çizgi çekiyoruz.

`auto it = boneList.begin();`: İterator (yineleyici) oluştur. Listenin başlangıcına işaret eder.

`*it`: İterator'ın işaret ettiği değeri al.

`++it`: Bir sonrakine git.

`it != boneList.end()`: Listenin sonuna gelmedik mi kontrol et.

---

# BÖLÜM 7: RADAR SİSTEMİ - Radar.cpp

## 7.1 Radar Nedir?

Radar, oyun haritasının kuşbakışı (top-down) bir mini görünümüdür. Tüm oyuncuların minik noktalar olarak gösterildiği bir minimap.

Bizim radar şöyle çalışır:
- Bizim karakterimiz radar'ın merkezinde
- Düşmanlar etrafımızda dağılmış noktalar
- Radar bizim baktığımız yöne göre döner (yani biz hep yukarı bakıyor gibi görünürüz)

## 7.2 Koordinat Dönüştürme: 2D Rotasyon

```cpp
Vec2 RevolveCoordinatesSystem(float RevolveAngle, Vec2 OriginPos, Vec2 DestPos)
```

Bu fonksiyon, bir noktayı bir merkez etrafında döndürür.

**Parametreler:**
- `float RevolveAngle`: Dönüş açısı (derece cinsinden).
- `Vec2 OriginPos`: Dönüş merkezi.
- `Vec2 DestPos`: Döndürülecek nokta.

**Dönüş:**
- `Vec2`: Yeni pozisyon.

### Matematik

Bir noktayı merkez etrafında döndürmek için **rotasyon matrisi** kullanılır:

```
┌                ┐   ┌    ┐   ┌      ┐
│ cos(θ)  -sin(θ)│ * │ x  │ = │ x_new│
│ sin(θ)   cos(θ)│   │ y  │   │ y_new│
└                ┘   └    ┘   └      ┘
```

Yani:
```
x_new = x * cos(θ) - y * sin(θ)
y_new = x * sin(θ) + y * cos(θ)
```

Ama biz bir merkez etrafında döndürmek istediğimiz için önce noktayı merkezden kaydırıyoruz, döndürüyoruz, sonra geri kaydırıyoruz:

1. Kaydır: `(p - origin)`
2. Döndür: `Rotation * (p - origin)`
3. Geri kaydır: `Rotation * (p - origin) + origin`

### Kod

```cpp
Vec2 RevolveCoordinatesSystem(float RevolveAngle, Vec2 OriginPos, Vec2 DestPos)
{
    if (RevolveAngle == 0)
        return DestPos;
    
    // Açıyı dereceden radyana çevir
    float rad = RevolveAngle * (M_PI / 180.0f);
    
    // cos ve sin hesapla
    float cosVal = cos(rad);
    float sinVal = sin(rad);
    
    // Merkezden olan farkı hesapla
    float dx = DestPos.x - OriginPos.x;
    float dy = DestPos.y - OriginPos.y;
    
    // Rotasyon uygula ve geri kaydır
    Vec2 ResultPos;
    ResultPos.x = OriginPos.x + dx * cosVal + dy * sinVal;
    ResultPos.y = OriginPos.y - dx * sinVal + dy * cosVal;
    
    return ResultPos;
}
```

**`if (RevolveAngle == 0) return DestPos;`**

0 derece dönüş = hiçbir şey yapma. Hesap yapmadan hızlıca dön.

**`float rad = RevolveAngle * (M_PI / 180.0f);`**

C++'taki `cos` ve `sin` fonksiyonları radyan ister, derece istemez. Bu yüzden çevirme yapıyoruz.

**`float cosVal = cos(rad); float sinVal = sin(rad);`**

Trigonometrik fonksiyonları bir kere hesaplayıp saklıyoruz, çünkü iki kez kullanacağız.

**`float dx = DestPos.x - OriginPos.x;` ve `float dy = DestPos.y - OriginPos.y;`**

Merkezden olan farkı hesapla. Bu, "merkezden çıkardığımız" adım.

**`ResultPos.x = OriginPos.x + dx * cosVal + dy * sinVal;`**

Standart rotasyon formülünden biraz farklı. Burada saat yönüne dönme için işaretler farklı (orijinde de görüntü ters olduğu için ekran koordinatlarını hesaba katıyor).

**`ResultPos.y = OriginPos.y - dx * sinVal + dy * cosVal;`**

Y bileşeni de benzer şekilde.

## 7.3 Radar Çiziminin Mantığı

Radar çizimi şöyle yapılır:

1. **Düşmanın gerçek dünyadaki konumunu al** (Vec3 olarak).
2. **Bizim karakterimizden olan farkı hesapla** (vektör).
3. **Bu farkı 2D'ye düşür** (z'yi at, sadece x ve y kullan).
4. **Radar boyutuna göre ölçeklendir** (zoom seviyesi).
5. **Bizim baktığımız yöne göre döndür** (rotasyon).
6. **Radar penceresinin içine çiz** (daire veya ok).

### Adım 1-3: Mesafe Hesapla

```cpp
float distX = enemyPos.x - playerPos.x;
float distY = enemyPos.y - playerPos.y;
```

Bu basit çıkarma. X farkı ve Y farkı.

### Adım 4: Ölçeklendir

```cpp
float radarX = distX / Proportion * RadarRange;
float radarY = distY / Proportion * RadarRange;
```

`Proportion` zoom seviyesi (mesela 2700 birim oyun mesafesi 1 birim radar mesafesi). `RadarRange` radar görünüm aralığı.

### Adım 5: Döndür

```cpp
Vec2 rotatedPos = RevolveCoordinatesSystem(
    playerAngle,
    {0, 0},
    {radarX, radarY}
);
```

Radar her zaman bizim baktığımız yöne göre döner.

### Adım 6: Çiz

```cpp
drawList->AddCircle(
    ImVec2(radarCenterX + rotatedPos.x, radarCenterY + rotatedPos.y),
    CircleSize,
    color,
    12,
    1.5f
);
```

Radar merkezine göre düşmanın yerine bir daire çizdik.

`AddCircle` parametreleri:
- Merkez
- Yarıçap
- Renk
- Segment sayısı (12 = oldukça yumuşak daire)
- Kalınlık

---

# BÖLÜM 8: ENTITY (OYUNCU) SİSTEMİ - Entity.h

## 8.1 Bir Oyuncuyu Temsil Etmek

Oyundaki her oyuncu (kendiniz dahil), bizim programımızda bir `CEntity` nesnesi ile temsil edilir.

Bir oyuncunun verisi oyunda iki yerde saklanır:
- **Controller** (Kontrolcü): Oyuncuyla ilgili genel bilgiler. İsim, takım, ID vb.
- **Pawn** (Karakter): Oyuncunun karakteriyle ilgili. Pozisyon, can, açı, kemikler vb.

Bunun nedeni şu: Oyuncu öldüğünde Pawn yok olur ama Controller kalır. Yeniden doğunca yeni bir Pawn oluşur.

## 8.2 CEntity Sınıfı

```cpp
class CEntity {
public:
    struct ControllerData {
        DWORD64 Address;
        int TeamID;
        char Name[64];
        // ... diğer alanlar
    } Controller;
    
    struct PawnData {
        DWORD64 Address;
        Vec3 Pos;
        Vec3 ViewAngle;
        float Health;
        bool IsAlive;
        // ... diğer alanlar
    } Pawn;
    
    CBone Bone;
    
    bool UpdateController(DWORD64 controllerAddress);
    bool UpdatePawn(DWORD64 pawnAddress);
    bool IsAlive() const;
    bool IsInScreen() const;
};
```

İç içe struct'lar görüyoruz. `ControllerData` ve `PawnData` `CEntity` sınıfının içinde tanımlı.

`CBone Bone`: Daha önce gördüğümüz kemik sınıfı.

`UpdateController()`: Controller verisini bellek okuma ile günceller.
`UpdatePawn()`: Pawn verisini günceller.
`IsAlive()`: Hayatta mı?
`IsInScreen()`: Ekranda görünür mü?

## 8.3 Bellek Offsetleri

Oyun verileri, oyun belleğinde belirli **offset**'lerde saklanır. Mesela bir Pawn struct'ının başlangıcından 0x1340 byte sonra Pos saklanıyor olabilir.

Offsetler oyun versiyonuna göre değişir. Yeni bir oyun güncellemesi geldiğinde offsetler değişebilir.

```cpp
bool UpdatePawn(DWORD64 pawnAddress) {
    this->Pawn.Address = pawnAddress;
    
    // Pozisyon oku
    memoryManager.ReadMemory<Vec3>(pawnAddress + 0x1340, this->Pawn.Pos);
    
    // Sağlık oku
    memoryManager.ReadMemory<float>(pawnAddress + 0x0344, this->Pawn.Health);
    
    // Bakış açısı oku
    memoryManager.ReadMemory<Vec3>(pawnAddress + 0x0B00, this->Pawn.ViewAngle);
    
    return true;
}
```

`this->` ifadesi "bu nesnenin" demek. Sınıfın içindeki üyeye erişirken kullanılır. `this` pointer'ı, mevcut nesneyi gösterir.

`pawnAddress + 0x1340`: Pawn başlangıcı + offset = Pos'un olduğu adres.

`memoryManager.ReadMemory<Vec3>(...)`: O adresten Vec3 oku, `this->Pawn.Pos`'a yaz.

---

# BÖLÜM 9: ANA RENDER LOOP - Cheats::Run()

## 9.1 Ana Döngü

Programın kalbi `Cheats::Run()` fonksiyonu. Her frame'de çağrılır (saniyede 60-144 kez).

```cpp
void Cheats::Run() {
    // 1. Menüyü göster
    Menu();
    
    // 2. Oyun aktif değilse erken çık
    if (!Init::Client::isGameWindowActive() && !MenuConfig::ShowMenu) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return;
    }
    
    // 3. View matrix oku
    if (!memoryManager.ReadMemory(
        gGame.GetMatrixAddress(),
        gGame.View.Matrix,
        64
    )) {
        return;
    }
    
    // 4. Entity listesini güncelle
    gGame.UpdateEntityListEntry();
    
    // 5. Local oyuncu verilerini al
    DWORD64 localControllerAddr = 0;
    DWORD64 localPawnAddr = 0;
    memoryManager.ReadMemory(gGame.GetLocalControllerAddress(), localControllerAddr);
    memoryManager.ReadMemory(gGame.GetLocalPawnAddress(), localPawnAddr);
    
    if (localPawnAddr == 0 || localControllerAddr == 0) {
        return;
    }
    
    CEntity localEntity;
    localEntity.UpdateController(localControllerAddr);
    localEntity.UpdatePawn(localPawnAddr);
    
    // 6. Radar'ı ayarla
    Base_Radar GameRadar;
    if (RadarCFG::ShowRadar && localEntity.Controller.TeamID != 0) {
        RadarSetting(GameRadar);
    }
    
    // 7. Tüm düşmanları işle
    auto entityResults = ProcessEntities(localEntity, localPlayerControllerIndex);
    
    // 8. Düşmanları çiz
    HandleEnts(entityResults, localEntity, localPlayerControllerIndex, GameRadar);
    
    // 9. Radar'ı çiz
    Radar(GameRadar, localEntity);
    
    // 10. FPS sınırla
    int currentFPS = static_cast<int>(ImGui::GetIO().Framerate);
    if (currentFPS > MenuConfig::RenderFPS) {
        int frameWait = 1000 / MenuConfig::RenderFPS;
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWait));
    }
}
```

Adım adım açıklayayım.

### Adım 1: Menü Göster

`Menu()` fonksiyonu, kullanıcının ayarlarını değiştirebileceği menüyü çizer. Kullanıcı F2'ye basarsa veya başka bir tuşa basarsa, menü açılır.

### Adım 2: Erken Çıkış Kontrolü

```cpp
if (!Init::Client::isGameWindowActive() && !MenuConfig::ShowMenu) {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    return;
}
```

Eğer oyun penceresi aktif DEĞİL VE menü kapalı ise, hiçbir şey yapma. 20 milisaniye uyu, sonra çık. Bu CPU kullanımını düşürür.

`std::this_thread::sleep_for` mevcut thread'i belli bir süre uyutur. `std::chrono::milliseconds(20)` "20 milisaniye" demek.

### Adım 3: View Matrix Oku

```cpp
if (!memoryManager.ReadMemory(gGame.GetMatrixAddress(), gGame.View.Matrix, 64)) {
    return;
}
```

View matrix 16 float = 64 byte. Bunu okuyamadıysak, hiçbir şey çizemeyiz, çünkü 3D'den 2D'ye dönüşüm yapamayız.

### Adım 4: Entity Listesini Güncelle

```cpp
gGame.UpdateEntityListEntry();
```

Oyundaki tüm oyuncu listesinin başlangıcını günceller. Bu liste oyun belleğinde dinamik bir yerde, her seferinde okunması gerekir.

### Adım 5: Bizim Karakterimiz

```cpp
DWORD64 localControllerAddr = 0;
DWORD64 localPawnAddr = 0;
memoryManager.ReadMemory(gGame.GetLocalControllerAddress(), localControllerAddr);
memoryManager.ReadMemory(gGame.GetLocalPawnAddress(), localPawnAddr);
```

Bizim controller ve pawn adresimizi alıyoruz. `GetLocalControllerAddress()` bunun saklandığı bellek adresini verir, `ReadMemory` o adresten okur.

```cpp
if (localPawnAddr == 0 || localControllerAddr == 0) {
    return;
}
```

Eğer ikisi de 0 ise, oyun yüklenmemiş veya biz öldük. Çıkış yap.

```cpp
CEntity localEntity;
localEntity.UpdateController(localControllerAddr);
localEntity.UpdatePawn(localPawnAddr);
```

Bizim CEntity'mizi oluştur ve güncelle.

### Adım 6: Radar Hazırla

```cpp
Base_Radar GameRadar;
if (RadarCFG::ShowRadar && localEntity.Controller.TeamID != 0) {
    RadarSetting(GameRadar);
}
```

Yeni bir radar nesnesi oluştur. Eğer radar açıksa ve takım ID'si geçerli ise, ayarları uygula.

### Adım 7: Düşmanları İşle

```cpp
auto entityResults = ProcessEntities(localEntity, localPlayerControllerIndex);
```

Tüm oyuncu listesinde dolaş, her birini güncelle, filtrele.

### Adım 8: Çiz

```cpp
HandleEnts(entityResults, localEntity, localPlayerControllerIndex, GameRadar);
```

Her oyuncu için ESP çiz, radar'a nokta ekle.

### Adım 9: Radar'ı Çiz

```cpp
Radar(GameRadar, localEntity);
```

Hazır radar'ı ekrana çiz.

### Adım 10: FPS Sınırlama

```cpp
int currentFPS = static_cast<int>(ImGui::GetIO().Framerate);
if (currentFPS > MenuConfig::RenderFPS) {
    int frameWait = 1000 / MenuConfig::RenderFPS;
    std::this_thread::sleep_for(std::chrono::milliseconds(frameWait));
}
```

Eğer FPS çok yüksekse (mesela 1000 FPS), CPU'yu boşa harcıyor demektir. Limit koyup uyutuyoruz.

`1000 / RenderFPS`: Bir frame için ayrılan milisaniye. Mesela 60 FPS için 16.6 ms.

## 9.2 ProcessEntities() Detayı

```cpp
std::vector<EntityResult> Cheats::ProcessEntities(CEntity& localEntity, int& localPlayerControllerIndex) {
    std::vector<EntityResult> results;
    results.reserve(64);
    
    for (int entityIndex = 0; entityIndex < 64; entityIndex++) {
        DWORD64 entityAddress = 0;
        if (!memoryManager.ReadMemory(
            gGame.GetEntityListEntry() + (entityIndex + 1) * 0x70,
            entityAddress
        )) continue;
        
        if (entityAddress == localEntity.Controller.Address) {
            localPlayerControllerIndex = entityIndex;
            continue;
        }
        
        CEntity entity;
        entity.Controller.Address = entityAddress;
        entity.UpdateController(entityAddress);
        DWORD64 pawnAddr = entity.Controller.GetPlayerPawnAddress();
        if (pawnAddr == 0) continue;
        entity.UpdatePawn(pawnAddr);
        
        if (!entity.IsAlive()) continue;
        if (MenuConfig::TeamCheck && entity.Controller.TeamID == localEntity.Controller.TeamID)
            continue;
        
        EntityResult result;
        result.entityIndex = entityIndex;
        result.entity = entity;
        result.isInScreen = entity.IsInScreen();
        result.distance = static_cast<int>(
            entity.Pawn.Pos.DistanceTo(localEntity.Pawn.Pos) / 100
        );
        result.isValid = true;
        
        results.push_back(result);
    }
    
    return results;
}
```

**`results.reserve(64);`**: Vektör için yer ayır. CS:GO/CS2'de maksimum 64 oyuncu olabilir.

**`for (int entityIndex = 0; entityIndex < 64; entityIndex++)`**: 64 oyuncu üzerinde döngü.

**`gGame.GetEntityListEntry() + (entityIndex + 1) * 0x70`**: Entity listesinin başlangıcı + (i+1) * 0x70. 0x70 her entity'nin liste içindeki boyutu (=112 byte).

`entityIndex + 1`: 1'den başlıyor çünkü 0'ıncı entity worldspawn (dünya kendisi).

**`if (entityAddress == localEntity.Controller.Address)`**: Bu biz miyiz? Bizimle ilgili işlem yapma, `localPlayerControllerIndex`'i kaydet.

**`if (!entity.IsAlive()) continue;`**: Ölmüşse atla.

**`if (MenuConfig::TeamCheck && entity.Controller.TeamID == localEntity.Controller.TeamID) continue;`**: Eğer takım kontrolü açıksa ve bizim takımımızdansa atla. `&&` "ve" operatörü.

**Sonuç oluştur:**

```cpp
EntityResult result;
result.entityIndex = entityIndex;
result.entity = entity;
result.isInScreen = entity.IsInScreen();
result.distance = static_cast<int>(
    entity.Pawn.Pos.DistanceTo(localEntity.Pawn.Pos) / 100
);
result.isValid = true;
```

Her geçerli oyuncu için bir sonuç oluşturuyoruz. Mesafeyi 100'e bölüyoruz çünkü oyun mesafe birimi metre'nin 1/100'ü gibi (yaklaşık).

**`results.push_back(result);`**: Sonucu listeye ekle.

---

# BÖLÜM 10: KONFİGÜRASYON - JSON İLE AYARLARI KAYDET/YÜKLE

## 10.1 JSON Nedir?

JSON = "JavaScript Object Notation". İnsanlar tarafından okunabilir, bilgisayarlar tarafından parse edilebilir bir veri formatı.

Örnek:
```json
{
    "name": "Ali",
    "age": 25,
    "isStudent": true,
    "hobbies": ["coding", "reading"]
}
```

Süslü parantezler obje, köşeli parantezler dizi, çift tırnaklar string.

## 10.2 Konfigürasyon Kaydet

```cpp
void MyConfigSaver::SaveConfig(const std::string& filename) {
    json config;
    
    config["0"]["Name"] = "C2C";
    config["0"]["Version"] = "3.7.10.4";
    
    config["ESP"]["Enable"] = ESPConfig::ESPenabled;
    config["ESP"]["BoneColor"]["r"] = ESPConfig::BoneColor.x;
    config["ESP"]["BoneColor"]["g"] = ESPConfig::BoneColor.y;
    config["ESP"]["BoneColor"]["b"] = ESPConfig::BoneColor.z;
    config["ESP"]["BoneColor"]["a"] = ESPConfig::BoneColor.w;
    
    config["Radar"]["ShowRadar"] = RadarCFG::ShowRadar;
    config["Radar"]["Range"] = RadarCFG::RadarRange;
    
    std::ofstream configFile(filename);
    configFile << config.dump(4);
    configFile.close();
}
```

**`json config;`**: Boş bir JSON nesnesi oluştur.

**`config["ESP"]["Enable"] = ESPConfig::ESPenabled;`**: JSON'da iç içe yapılar oluşturuyor. `config` nesnesinin içinde `ESP` adında bir nesne, onun içinde `Enable` adında bir alan.

**`std::ofstream configFile(filename);`**: Yazma için dosya aç. `ofstream` "output file stream" demek.

**`configFile << config.dump(4);`**: JSON'u stringe çevir ve dosyaya yaz. `dump(4)` "4 boşluk girintiyle güzel format" demek.

**`configFile.close();`**: Dosyayı kapat.

## 10.3 Konfigürasyon Yükle

```cpp
void MyConfigSaver::LoadConfig(const std::string& filename) {
    std::ifstream configFile(filename);
    if (!configFile.is_open())
        return;
    
    json config;
    configFile >> config;
    configFile.close();
    
    if (config.contains("ESP")) {
        ESPConfig::ESPenabled = config["ESP"]["Enable"];
        if (config["ESP"].contains("BoneColor")) {
            ESPConfig::BoneColor = ImColor(
                config["ESP"]["BoneColor"]["r"],
                config["ESP"]["BoneColor"]["g"],
                config["ESP"]["BoneColor"]["b"],
                config["ESP"]["BoneColor"]["a"]
            );
        }
    }
}
```

**`std::ifstream configFile(filename);`**: Okuma için dosya aç.

**`if (!configFile.is_open()) return;`**: Açılamadıysa çık.

**`configFile >> config;`**: Dosyadan JSON oku.

**`if (config.contains("ESP"))`**: Eğer JSON'da ESP varsa.

---

# BÖLÜM 11: ANTI-RECORD (KAYIT KORUMASI) - Misc.cpp

## 11.1 OBS Bypass

OBS gibi ekran kayıt programları, bizim cheats'imizi görüp kaydedebilir. Bunu engellemek için bazı teknikler kullanırız.

```cpp
std::string windowTitle = "SystemProcess_" + std::to_string(rand());
SetWindowTextA(GetConsoleWindow(), windowTitle.c_str());
```

Pencere adını rastgele bir isime değiştir. OBS belli pencerelerin üstüne gelmek için isimle arar. Eğer her seferinde isim farklıysa, OBS bizi bulamaz.

`std::to_string(rand())`: Rastgele sayıyı stringe çevir.

`+`: String birleştirme.

`SetWindowTextA(...)`: Pencere başlığını değiştir.

`GetConsoleWindow()`: Console penceremizin handle'ını al.

`.c_str()`: std::string'i C tarzı pointer'a çevir.

## 11.2 Bellek İzlerini Temizleme

```cpp
void Misc::CleanTraces() {
    std::filesystem::remove_all(MenuConfig::path + "\\Data");
    std::filesystem::rename(
        MenuConfig::docPath + "\\C2C",
        MenuConfig::docPath + "\\Adobe Software Data"
    );
}
```

`std::filesystem::remove_all(...)`: Bir klasörü ve içindeki her şeyi sil.

`std::filesystem::rename(...)`: Dosyayı/klasörü yeniden adlandır. C2C klasörünü "Adobe Software Data" gibi normal bir isme çevir.

---

# SONSÖZ

Bu rehberi okuyarak C2C usermode programının nasıl çalıştığını anladın. Önemli kavramlar:

1. **Bellek Okuma**: Kernel driver ile IOCTL kullanarak oyun belleğinden veri okuma.
2. **Vektör Matematiği**: Vec2, Vec3, mesafe, açı, dot product.
3. **View Matrix**: 3D dünya koordinatlarını 2D ekran koordinatlarına çevirme.
4. **Kemik Sistemi**: Karakter iskeletinin pozisyonlarını okuma ve çizgilerle birleştirme.
5. **ESP**: Düşmanların ekranda görünmesini sağlama.
6. **Radar**: Mini harita ile düşmanları gösterme.
7. **Entity Sistemi**: Her oyuncuyu CEntity nesnesi olarak temsil etme.
8. **JSON**: Ayarları kaydetme/yükleme.
9. **Anti-Record**: Kayıt programlarından gizlenme.

Tüm bu bileşenler `Cheats::Run()` fonksiyonunda her frame'de bir araya geliyor. Saniyede 60-144 kez tekrarlanan bir döngü:
- Belleği oku
- View matrix güncelle
- Oyuncuları işle
- ESP çiz
- Radar çiz

Eğer bir kavramı tam anlamadıysan, ilgili bölümü tekrar oku. Programlama bir defada öğrenilen bir şey değil, anlamak için bazen birkaç kez aynı şeyi okumak lazım. Sabırlı ol, devam et.

Başarılar! 🚀
