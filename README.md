# 📡 Amateur Radio Satellite Tracking System

A budget-friendly solution for amateur radio enthusiasts to **track and listen to satellite data and communications** by automatically adjusting an antenna rotator to match a satellite's azimuth and elevation in real time.

---

## 🚀 How It Works

1. **Tracking Data Input**
   - Uses [Orbitron](https://www.stoff.pl/) to output azimuth (bearing) and elevation (height) data via serial.

2. **Microcontroller & Motor Control**
   - An **ESP32 microcontroller** reads target values from Orbitron.
   - Stepper motor drivers convert these values into precise movements.

3. **Position Feedback & Adjustment**
   - **Magnetometer** measures the current azimuth.
   - **Potentiometer** measures the current elevation.
   - Stepper motors adjust the antenna **until the real position matches the target values**.

4. **User Interaction & Control**
   - Users can enter target values via **serial input** or fine-tune with **rotary encoders**.
   - An **LCD screen** displays the current and target azimuth/elevation values in real time.

---

## 🔧 Features  

✔️ **Automatic satellite tracking** with Orbitron  
✔️ **Real-time position correction** with sensor feedback  
✔️ **Manual fine-tuning** via rotary encoders  
✔️ **Live tracking display** on an LCD screen  
✔️ **ESP32-based system** for affordability and flexibility  

---

## 🛠 Hardware Requirements

- **ESP32** microcontroller  
- **Stepper motors & drivers**  
- **Magnetometer** (for azimuth sensing)  
- **Potentiometer** (for elevation sensing)  
- **Rotary encoders** (for manual control)  
- **LCD screen** (for real-time data display)  
- **Antenna & mounting system**  

---

## 📜 Getting Started  

1. Install **Orbitron** and configure it to output azimuth/elevation data over serial.  
2. Flash the **ESP32 firmware** from this repository.  
3. Connect the hardware as per the wiring diagram.  
4. Use the **serial interface or rotary encoders** to control and monitor the system.  

---

## 📖 License  

This project is open-source and available under the **MIT License**.
