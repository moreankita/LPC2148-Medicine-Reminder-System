# 💊 LPC2148 Medicine Reminder System

## 📌 Project Overview
This project is a **User-Configurable Medicine Reminder System** built using the **LPC2148 microcontroller**.  
It reminds users to take medicines on time using RTC-based scheduling, LCD display, keypad input, and buzzer alerts.

---

## ⚙️ Hardware Components
- LPC2148 Microcontroller
- 16x2 LCD Display
- 4x4 Matrix Keypad
- Buzzer
- Push Buttons (Switch-1 & Switch-2)

---

## 💻 Software & Tools
- Embedded C
- Keil uVision
- Flash Magic

---

## 🚀 Features
- Real-Time Clock (RTC) based time tracking
- User-configurable medicine schedule
- Supports multiple medicine timings
- LCD displays time, date, and alerts
- Buzzer alert for medicine reminder
- Interrupt-based user interaction
- Acknowledgement using switch
- Auto buzzer OFF after 1 minute

---

## 🔄 System Working
1. RTC continuously updates current time  
2. User sets medicine schedule using keypad  
3. When RTC time matches stored schedule:
   - Buzzer turns ON  
   - LCD shows "Take Medicine Now"  
4. User presses switch to acknowledge  
5. If no response → buzzer turns OFF after 1 minute automatically  

---

## 📁 Project Files Description

### 🔹 Core Files
- `miniproject_main.c`  
  → Main program logic, RTC reading, comparison, alert handling  

- `miniproject.c`  
  → LCD, RTC, keypad, and alert function implementations  

- `miniproject.h`  
  → Function declarations for all modules  

---

### 🔹 Supporting Files
- `miniproject_defines.h`  
  → Pin configurations, macros, LCD commands, RTC settings  

- `struct.h`  
  → Structure to store medicine schedule  

- `types.h`  
  → Custom data types (u8, u32, etc.)  

---

### 🔹 Driver Files
- `lcd.c / lcd.h`  
  → LCD interfacing functions  

- `kpm.c / kpm.h`  
  → Keypad scanning functions  

- `delay.c / delay.h`  
  → Delay functions  

---

## 🧠 Key Concepts Used
- Embedded C Programming  
- GPIO Interfacing  
- LCD Communication (8-bit mode)  
- RTC (Real Time Clock)  
- External Interrupts (EINT0 & EINT1)  
- Structure & Arrays  
- Time Comparison Logic  

---

## 🎯 Applications
- Personal medicine reminder system  
- Elderly care systems  
- Embedded healthcare devices  

---

## 👩‍💻 Author
**Ankita More**

---

## 📌 Future Improvements
- Add IoT (SMS/Notification alerts)  
- Mobile app integration  
- Voice alert system  
- Battery backup system  
