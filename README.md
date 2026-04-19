# IoT Edge Sentinel 🛡️

**IoT Edge Sentinel** is a lightweight, high-performance edge computing monitoring tool written entirely in Standard C. It is designed to process telemetry data locally on resource-constrained devices (such as Raspberry Pi or industrial embedded systems) without requiring cloud connectivity or external databases.

## 🚀 Why This Project Exists (Use Cases)
In the world of Industrial IoT (IIoT) and remote sensing, devices often operate in environments with unstable or zero internet connectivity. Sending every piece of data to the cloud is expensive and introduces latency. 

**IoT Edge Sentinel solves this by:**
1. **Processing at the Edge:** Analyzes CPU and Temperature data locally in real-time ($\mathcal{O}(N)$ time complexity).
2. **Offline Auditing:** Persistently logs critical anomalies to a local file (`anomalies.log`) for later auditing.
3. **Resource Efficiency:** Consumes minimal CPU and Memory ($\mathcal{O}(1)$ space complexity), making it perfect for battery-powered or low-spec hardware.
4. **Config-Driven:** Thresholds can be updated without recompiling the C code.

## 💻 Cross-Platform Compatibility
This tool is built with standard C POSIX libraries. It runs natively on:
- **macOS**
- **Linux** (Ubuntu, Debian, Alpine, etc.)
- **Windows** (via WSL, MinGW, or Cygwin)

## 🛠️ Installation & Build Instructions

1. **Clone the repository** and navigate to the project directory.
2. **Compile the project** using the provided Makefile:
```bash
   make
```
3. (Optional) To clean the compiled binaries:

```bash
       make clean
```

## Configuration (config.txt)
Before running the engine, you can define your system thresholds. Create or edit the config.txt file in the root directory:

MAX_TEMP=75.0
MAX_CPU=85.0
*If this file is missing, the engine will safely fallback to default limits.*

## 📊 Data Ingestion (`sensor_data.csv`)
The engine reads telemetry data from `sensor_data.csv`. Ensure you have a sample file with the following format:
csv
timestamp,temperature,cpu_usage
2026-04-19T10:00:00Z,45.5,30.2
2026-04-19T10:00:05Z,80.1,90.5

## ▶️ Usage & Interactive Shell

Run the compiled executable:
bash
./sentinel

Once initialized, the engine will process the CSV data and drop you into a **Hybrid Interactive Shell**. You can type the following commands while the engine is idling:

- `/status` : Displays the currently loaded thresholds from your config file.
- `/search <keyword>` : Instantly searches the offline `anomalies.log` for specific data (e.g., `/search CPU`).
- `/help` : Lists all available commands.
- `/exit` : Safely terminates the process and releases memory (Graceful Shutdown).

## 📄 License
MIT License
