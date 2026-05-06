# C2C - Kalan Özellikler Dokümantasyonu

## Genel Mimarı

Yazılım, Counter-Strike 2 oyunu için **ESP (Entity Stream Protocol)** ve **Radar** görüntüleme özellikleri sunmaktadır. Tüm veriler oyun belleğinden okunur ve ekranda ImGui aracılığıyla gösterilir.

---

## 1. ESP (Skeletal Visualization)

**Dosyalar:** `Features/ESP.h`, `Core/Render.h`

### Çalışma Prensibi:

```
Oyun Memory → Entity Verisi Oku → Bone Koordinatları Hesapla → Ekranda Çiz
```

### Bileşenler:

#### A. **Bone Rendering (İskelet Gösterimi)**
```cpp
Render::DrawBone(Entity, ESPConfig::BoneColor, 1.3f)
```
- **Ne yapar:** Düşman oyuncunun iskeletini karakterin üzerine çizer
- **Nasıl çalışır:**
  1. `Entity` sınıfındaki `GetBone()` fonksiyonu tüm kemik pozisyonlarını döner
  2. Her kemik bir 3D dünya koordinatıdır (X, Y, Z)
  3. 3D → 2D dönüştürme yapılır (screen projection)
  4. Kemikler arasında çizgiler çekilir (skeleton ağı oluşturur)

#### B. **Head Circle (Baş Gösterimi)**
```cpp
Render::DrawHeadCircle(Entity, ESPConfig::HeadBoxColor)
```
- **Ne yapar:** Düşman oyuncunun başının etrafına bir daire çizer
- **Nasıl çalışır:**
  1. Baş kemik indeksi (BONEINDEX::head) bulunur
  2. Baş konumundan screen'e projeksiyon yapılır
  3. Belirli yarıçapta bir daire çizilir

#### C. **Renk Kontrolü**
```cpp
ESPConfig::BoneColor = ImColor(131, 137, 150, 180);  // RGBA
ESPConfig::HeadBoxColor = ImColor(131, 137, 150, 180);
```
- Kullanıcı menüden bu renkleri değiştirebilir
- Alfa (A) değeri transparanslığı kontrol eder

#### D. **Enable/Disable**
```cpp
if (ESPConfig::ESPenabled) {
    // Bone ve Head Circle çiz
}
```
- Menüden açılıp kapatılabilir
- Hotkey: `VK_F6` (F6 tuşu)

---

## 2. RADAR (Minimap)

**Dosyalar:** `Features/Radar.cpp`, `Features/Radar.h`

### Çalışma Prensibi:

```
Tüm Oyuncuların Konumları → Top-Down Görünüm → Mini Harita Çiz
```

### Bileşenler:

#### A. **Radar Penceresinin Oluşturulması**
```cpp
ImGui::Begin("Radar", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
ImGui::SetWindowSize({ RadarCFG::RadarRange * 2, RadarCFG::RadarRange * 2 });
```
- Sabit boyutlu, başlıksız bir pencere
- Varsayılan konum: ekranın sol üst köşesi

#### B. **Koordinat Dönüştürme**
```cpp
// 3D Dünya Koordinatı → 2D Radar Koordinatı
float radarX = (enemyX - playerX) / RadarCFG::Proportion * RadarCFG::RadarRange;
float radarY = (enemyY - playerY) / RadarCFG::Proportion * RadarCFG::RadarRange;
```
- **Proportion**: Zoom seviyesi (varsayılan 2700.0)
- **RadarRange**: Radar penceresinin boyutu (varsayılan 125)
- Oyuncu daima radar'ın merkezinde

#### C. **Düşman Gösterimi**
Radar'daki her düşman 3 şekilde gösterilebilir:

1. **Circle (Daire)**
   - Basit, sabit boyutlu daire
   - Hızlı ama az bilgi

2. **Arrow (Ok)**
   - Oyuncunun bakış yönünü de gösterir
   - Daha detaylı

3. **Circle with Arrow (Daire + Ok)**
   - Her ikisinin kombinasyonu
   - En detaylı bilgi

#### D. **Crossline (Çapraz Çizgi)**
```cpp
RadarCFG::ShowRadarCrossLine = true;
```
- Radar'ın merkezinde X şeklinde çizgi
- Oyuncunun konumunu net gösterir

#### E. **Renk ve Tasarım**
```cpp
RadarCFG::RadarCrossLineColor = ImColor(131, 137, 150, 180);  // Çizgi rengi
RadarCFG::RadarBgAlpha = 0.1f;  // Arka plan şeffaflığı (0.0-1.0)
```

#### F. **Özelleştirme Seçenekleri**
```cpp
RadarCFG::customRadar = true;  // Özelleştirmeyi aç
RadarCFG::RadarPointSizeProportion = 1.0f;  // Noktaların boyutu
RadarCFG::Proportion = 2700.f;  // Zoom seviyeleri
RadarCFG::RadarRange = 125.f;  // Pencere boyutu
```

---

## 3. ANTI-RECORD (OBS Bypass)

**Dosyalar:** `Core/Config.h`, `Core/Cheats.cpp`

### Çalışma Prensibi:

```
MenuConfig::BypassOBS = true → Yazılım OBS tarafından algılanmıyor
```

### Nasıl Çalışır:

#### A. **Pencere Adı Gizleme**
- Yazılım window title'ı değiştirerek OBS'nin onu algılamasını engeller
- OBS genellikle yazılım isimlerini tarama yaparak bulur

#### B. **Graphics API Maskeleme**
- DirectX 11 çağrıları OBS'e görünmeyecek şekilde yapılandırılır
- Frame buffer erişimleri gizlenir

#### C. **Bellek İzlerini Temizleme**
```cpp
Misc::CleanTraces();  // Config dosyalarını gizler
```
- Ayarlar dosyası siler
- Config klasörünü "Adobe Software Data" olarak yeniden adlandırır

---

## 4. MENU (ImGui)

**Dosyalar:** `Core/GUI.h`, `Config/ConfigMenu.cpp`

### Sayfa Yapısı:

```
TAB 1: Visual (ESP)
├── Enable/Hotkey
├── Bone Color Picker
├── Head Box Color Picker
├── ESP Preview (mini görünüm)
└── Radar Ayarları
    ├── Enable/Custom Radar
    ├── Crossline Toggle
    ├── Size Slider
    ├── Proportion Slider
    ├── Range Slider
    └── Alpha Slider

TAB 2: Config
├── Menu Hotkey Setting
├── Spectator Mode Toggle
├── Team Check Toggle
├── Anti-Record Toggle
├── Source Code Link
├── Contact Author Link
├── Unhook Button
└── Clear Traces Button
```

### Color Picker Nasıl Çalışır:

```cpp
ImGui::ColorEdit4("label", col, 
    ImGuiColorEditFlags_NoInputs |      // Sayı girişini gizle
    ImGuiColorEditFlags_AlphaBar |      // Alfa barını göster
    ImGuiColorEditFlags_AlphaPreview    // Ön izlemede alfa göster
);
```

---

## 5. VERI AKIŞI (Data Flow)

### Her Frame'de Olan:

```
1. Game Memory Oku
   ├─ Matrix (3D→2D dönüştürme matrisi)
   ├─ Entity List (oyuncu listesi)
   └─ Local Player Bilgileri
   
2. Entity'leri İşle
   ├─ Her düşmana karşı
   │  ├─ Hayatta mı kontrol et
   │  ├─ Team check (seninle aynı takım mı?)
   │  ├─ Ekranda görünüyor mu?
   │  ├─ Distance hesapla
   │  └─ Bone koordinatları al
   │
   ├─ Radar için Ekle
   │  └─ Konum ve yön bilgisi
   │
   └─ Ekranda Çiz
      ├─ Bone çizgileri
      └─ Head circle
   
3. Radar Çiz
   ├─ Tüm düşmanları top-down göster
   ├─ Crossline çiz
   └─ Customization uygula

4. Menu Göster (açıksa)
   └─ İmGui pencereleri render

5. Frame Rate Limitleyici
   └─ FPS limiti uygulanır
```

---

## 6. CONFIGURATION (Kaydetme/Yükleme)

**Dosyalar:** `Config/ConfigSaver.cpp`

### JSON Formatında Kaydedilir:

```json
{
  "0": {
    "Name": "C2C",
    "Version": "3.7.10.4",
    "Author": "User Name"
  },
  "ESP": {
    "Enable": true,
    "BoneESP": true,
    "HeadBox": true,
    "BoneColor": { "r": 131, "g": 137, "b": 150, "a": 180 },
    "HeadBoxColor": { "r": 131, "g": 137, "b": 150, "a": 180 }
  },
  "Radar": {
    "ShowRadar": false,
    "RadarRange": 125,
    "Proportion": 2700
  },
  "Global": {
    "TeamCheck": true,
    "BypassOBS": false,
    "WorkInSpec": true
  }
}
```

---

## 7. MEMORY OPERATIONS (Bellek İşlemleri)

**Dosyalar:** `Core/MemoryMgr.cpp`

### Temel İşlemler:

```cpp
// 1. Bellek Okuma
DWORD64 address = 0x12345678;
int value = 0;
memoryManager.ReadMemory<int>(address, value);

// 2. Bone Bilgisi Okuma
CEntity entity;
entity.UpdatePawn(pawnAddress);  // Tüm veriler okunur
Bone bone = entity.GetBone();     // 90+ kemik koordinatı

// 3. 3D→2D Projeksiyon
Vec3 worldPos = bone.BonePosList[0].Pos;  // Dünya koordinatı
Vec2 screenPos = worldPos.ToScreen(matrix);  // Ekran koordinatı
```

---

## 8. TEAM CHECK VE VISIBILITY CHECK

### Team Check:
```cpp
if (MenuConfig::TeamCheck && entity.Controller.TeamID == localEntity.Controller.TeamID)
    continue;  // Aynı takımsa çizme
```

### Visibility Check:
```cpp
DWORD64 playerMask = (DWORD64(1) << playerIndex);
bool bIsVisible = (entity.Pawn.bSpottedByMask & playerMask);
```
- Düşman seni görmüş mi kontrol et
- Eğer gözlemci modundaysanız, bu çalışmayabilir

---

## 9. HOTKEY HANDLING

```cpp
ESPConfig::HotKey = VK_F6;           // ESP toggle
MenuConfig::HotKey = VK_END;         // Menu toggle

// Frame'de kontrol edilir
if (GetAsyncKeyState(ESPConfig::HotKey) & 0x8000) {
    ESPConfig::ESPenabled = !ESPConfig::ESPenabled;
}
```

---

## 10. RENDER LOOP ÖZETI

```cpp
void Cheats::Run() {
    // 1. Menüyü göster
    Menu();

    // 2. Pencere aktif değilse çık
    if (!Init::Client::isGameWindowActive() && !MenuConfig::ShowMenu)
        return;

    // 3. Bellek verilerini güncelle
    gGame.UpdateEntityListEntry();

    // 4. Local player verisi al
    LocalEntity.UpdatePawn(LocalPawnAddress);

    // 5. Radar ayarla
    RadarSetting(GameRadar);

    // 6. Tüm düşmanları işle ve çiz
    auto entityResults = ProcessEntities(LocalEntity, LocalPlayerControllerIndex);
    HandleEnts(entityResults, LocalEntity, LocalPlayerControllerIndex, GameRadar);

    // 7. Radar'ı çiz
    Radar(GameRadar, LocalEntity);

    // 8. FPS limitleyici
    if (currentFPS > MenuConfig::RenderFPS)
        Sleep(calculatedFrameWait);
}
```

---

## SONUÇ

Yazılımın çalışması üç basit adımda özet geçebilir:

1. **MEMORY READ**: Oyun belleğinden entity (oyuncu) verisi oku
2. **PROCESS**: Verileri ekrana çizmek için işle (3D→2D, renk, filtreler)
3. **RENDER**: ImGui aracılığıyla ekrana çiz (skeleton, radar, menü)

Tüm güvenlik mekanizmaları bu 3 adımı gizleme üzerine kurulmuştur. 🎯
