# Opera Installer Shield

A lightweight, silent Windows utility that monitors the **Downloads** folder in real-time and automatically deletes Opera browser installers (`OperaSetup.exe`, `OperaGXSetup.exe`, etc.) the moment they appear.



## 🚀 Features

* **Real-time Monitoring**: Uses the native Windows API to listen for file system events without polling, ensuring near-zero CPU impact.
* **Targeted Detection**: Automatically identifies installers for:
    * Standard Opera (`operasetup.exe`)
    * Opera GX (`operagxsetup.exe`)
    * Opera Air (`operaairsetup.exe`)
---

## 💻 Technical Requirements

* **OS**: Windows 7 / 8 / 10 / 11
* **Compiler**: MSVC (Microsoft Visual C++) or MinGW-w64
* **Windows Subsystem**: Should be compiled as a Windowed application.

## 🔨 Building from Source

### Using MinGW/g++:
```bash
g++ -o OperaShield.exe main.cpp -luser32 -lshell32 -lole32
