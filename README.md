# Smart Clock (ESP32-S3)

โปรเจกต์นาฬิกาอัจฉริยะ (Smart Clock) สำหรับบอร์ดพัฒนา ESP32-S3 พัฒนาด้วย **ESP-IDF** ในรูปแบบ **Native C++** โดยเน้นการออกแบบโครงสร้างซอฟต์แวร์แบบ Modular Design เพื่อให้ง่ายต่อการดูแลรักษาและขยายผล

## 🛠 ฮาร์ดแวร์ (Hardware Specifications)

โปรเจกต์นี้รองรับฮาร์ดแวร์ดังต่อไปนี้:

*   **MCU:** ESP32-S3 (เปิดใช้งาน PSRAM 8MB)
*   **หน้าจอ (Display):** 3.5 นิ้ว Controller ST7796 (อินเทอร์เฟซ SPI)
*   **ทัชสกรีน (Touch Screen):** FT6336 (อินเทอร์เฟซ I2C)
*   **การจัดการพลังงาน (PMU):** AXP2101
*   **นาฬิกาจริง (RTC):** PCF85063 (เชื่อมต่อ I2C ผ่านขา IO7/IO8)
*   **IO Expander (EXIO):** ใช้สำหรับควบคุมขา Reset ของจอและรับสัญญาณ Interrupt ต่างๆ

## 📂 โครงสร้างโปรเจกต์ (Project Structure)

โปรเจกต์ถูกจัดเก็บในโฟลเดอร์ `main/` โดยแบ่งหน้าที่การทำงานออกเป็นคลาสต่างๆ (Object-Oriented):

*   **`main.cpp`**: จุดเริ่มต้นของโปรแกรม (App Main) ทำหน้าที่ตั้งค่าฮาร์ดแวร์ (Hardware Init), ควบคุม IO Expander, และรัน Main Loop
*   **`ui.cpp / .h`**: จัดการส่วนติดต่อผู้ใช้ (User Interface) และ State Machine
    *   **States:** CLOCK, SETTINGS, WIFI_SCAN, KEYBOARD
    *   จัดการความสว่างหน้าจอ (Brightness) และ Status Bar
*   **`network.cpp / .h`**: จัดการการเชื่อมต่อเครือข่าย
    *   เชื่อมต่อ Wi-Fi แบบ Non-blocking (ไม่ทำให้หน้าจอค้าง)
    *   ระบบ Async Scan สำหรับค้นหาเครือข่าย
    *   ซิงค์เวลาผ่าน NTP
*   **`settings.cpp / .h`**: จัดการข้อมูลการตั้งค่าถาวร (Persistent Storage)
    *   บันทึก SSID, Password, และค่าความสว่าง ลงใน NVS (Non-volatile Storage)
*   **`globals.h`**: เก็บตัวแปร Global, Pin Definitions และ Enum ของ State ต่างๆ

## ⚙️ การตั้งค่าและใช้งาน (Configuration & Usage)

### รหัสผ่าน Admin
*   รหัสเริ่มต้นสำหรับเข้าเมนูตั้งค่าคือ: **`88888888`**

### การปรับแต่ง Pin (Pin Definitions)
สามารถแก้ไขการกำหนดขา GPIO ได้ที่ไฟล์ `main/globals.h`

```cpp
#define PIN_I2C_SDA     GPIO_NUM_7
#define PIN_I2C_SCL     GPIO_NUM_8
#define PIN_LCD_MOSI    GPIO_NUM_11
...
```

## 🚀 การติดตั้งและ Build (Build Instructions)

1.  **ติดตั้ง ESP-IDF:** ตรวจสอบให้แน่ใจว่าเครื่องของคุณได้ติดตั้ง ESP-IDF Environment เรียบร้อยแล้ว
2.  **ตั้งค่า Target:**
    ```bash
    idf.py set-target esp32s3
    ```
3.  **ตั้งค่า Menuconfig:**
    ```bash
    idf.py menuconfig
    ```
    *   ไปที่ `Component config` -> `ESP32S3-Specific` -> เปิดใช้งาน **Support for external, SPI-connected RAM** (PSRAM)
4.  **Build และ Flash:**
    ```bash
    idf.py build flash monitor
    ```

## 📝 หมายเหตุ
*   **IO Expander:** ระบบมีการใช้งาน IO Expander ผ่าน I2C เพื่อ Reset หน้าจอ หากบอร์ดของคุณไม่มีชิปนี้ อาจต้องแก้ไขโค้ดใน `main.cpp` ฟังก์ชัน `init_hardware`
*   **LVGL:** โค้ดนี้เป็นโครงสร้างพื้นฐาน (Skeleton) แนะนำให้ติดตั้งไลบรารี **LVGL** เพิ่มเติมเพื่อความสวยงามและความง่ายในการสร้าง UI
