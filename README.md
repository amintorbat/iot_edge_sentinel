# IoT Edge Sentinel 🛡️

**IoT Edge Sentinel** is a lightweight, high-performance edge computing monitoring tool written entirely in Standard C. It is designed to process telemetry data locally on resource-constrained devices (such as Raspberry Pi or industrial embedded systems) without requiring cloud connectivity or external databases.

## 🚀 Why This Project Exists (Use Cases)
In the world of Industrial IoT (IIoT) and remote sensing, devices often operate in environments with unstable or zero internet connectivity. Sending every piece of data to the cloud is expensive and introduces latency. 

**IoT Edge Sentinel solves this by:**
1. **Processing at the Edge:** Analyzes CPU and Temperature data locally in real-time ($O(N)$ time complexity).
2. **Offline Auditing:** Persistently logs critical anomalies to a local file (`anomalies.log`) for later auditing.
3. **Resource Efficiency:** Consumes minimal CPU and Memory ($O(1)$ space complexity), making it perfect for battery-powered or low-spec hardware.
4. **Config-Driven:** Thresholds and data paths can be dynamically updated without recompiling the C code.

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
3. (Optional) To clean the compiled binaries:
bash
make clean

## ⚙️ Configuration (`config.txt`)
Before running the engine, you can define your system thresholds and data source. Create or edit the `config.txt` file in the root directory:

text
# Monitoring Thresholds
MAX_TEMP=75.0
MAX_CPU=85.0

# Dynamic Data Path
DATA_PATH=sensor_data.csv
*Note: If this file is missing, the engine will safely fallback to default limits and look for `sensor_data.csv` in the current directory.*

## 📊 Data Ingestion (`sensor_data.csv`)
The engine reads telemetry data from your specified `DATA_PATH`. Ensure you have a sample CSV file with the following format:
csv
timestamp,temperature,cpu_usage
2026-04-19T10:00:00Z,45.5,30.2
2026-04-19T10:00:05Z,80.1,90.5

## ▶️ Usage & Interactive Shell

Run the compiled executable:
bash
./sentinel

Once initialized, the engine will process the CSV data and drop you into a **Hybrid Interactive Shell**. You can type the following commands while the engine is idling:

- `/status` : Displays the currently loaded thresholds and data path.
- `/search <keyword>` : Instantly searches the offline `anomalies.log` for specific data (e.g., `/search CPU`).
- `/help` : Lists all available commands.
- `/exit` : Safely terminates the process and releases memory (Graceful Shutdown).

## 📄 License
MIT License
