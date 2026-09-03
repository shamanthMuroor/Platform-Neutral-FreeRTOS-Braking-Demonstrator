# Platform-Neutral Braking Demonstrator

**Automotive software running on open-source FreeRTOS and ARM Cortex-M boards**

---

## 💡 What is this?

The **driving logic** (for example, *"if a wheel is slipping, ease off the brake"*) is written once, completely free of any hardware details. The hardware-specific parts are added **automatically at build time** using a technique called *aspect-oriented programming* (AspectC++). The result: the same logic can run on a big, powerful chip or a tiny, cheap one without being rewritten or reconfigured.

To prove it works on real hardware, the project builds a small **two-wheel anti-lock braking (ABS) demonstrator**:

- A **powerful board** (STM32 H7) reads the driver inputs and drives a display.
- A **tiny, cheap board** (STM32 C0) controls a second wheel.
- They talk over a simple serial link and cooperate in real time (a decision every **10 milliseconds**) to stop safely.

> **Why it matters:** it points toward cheaper, faster, more flexible car software that isn't locked to a single vendor, the same idea can scale from one signal to hundreds, and from a serial cable to a full CAN bus.

---


## 🧩 How it's built

```text
 Simulink model  ──▶  clean C++ logic (no hardware!)
                              │
                              ▼
         AspectC++ weaves in the platform details
                              │
                              ▼
            GCC compiles & links  ──▶  firmware.elf  ──▶  flash to board
```

The driving logic never mentions the hardware. A small **aspect file** supplies the platform behaviour, and the build "weaves" the two together just before compiling. Everything else is a normal embedded build.

---

## 🛠️ Tech at a glance

| Area | Choice |
|------|--------|
| Hardware | NUCLEO-H7S3L8 (Cortex-M7) + NUCLEO-C031C6 (Cortex-M0+) |
| RTOS | FreeRTOS (open-source) |
| Language | C++ application logic, C platform services |
| Platform binding | AspectC++ (`.ah` aspects, woven at build time) |
| Logic source | MATLAB / Simulink → generated C++ |
| Communication | UART, CAN-ready by design |

---

## 👤 Author

**Shamanth Muroor**.

🔗 **LinkedIn:** [Shamanth Muroor](https://www.linkedin.com/in/shamanthmuroor/)
---

## 📚 Background

This work builds on the **PLATO** research paper - *"Platform-Neutral Automotive Software Development Leveraging Aspect-Oriented Programming"* (IEEE/ACM ICSE 2026), taking the idea from a controlled setting onto **real, heterogeneous hardware**.

