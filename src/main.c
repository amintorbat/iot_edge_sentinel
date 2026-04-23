#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "sentinel.h"

// Persistent logging for critical system anomalies
void log_anomaly(const char* message) {
    FILE* log_file = fopen("anomalies.log", "a");
    if (log_file != NULL) {
        time_t now;
        time(&now);
        char* date_time = ctime(&now);
        date_time[strcspn(date_time, "\n")] = 0; // Remove trailing newline

        fprintf(log_file, "[%s] CRITICAL: %s\n", date_time, message);
        fclose(log_file);
    }
}

void search_logs(const char* keyword) {
    FILE* log_file = fopen("anomalies.log", "r");
    if (log_file == NULL) {
        printf("Error: Cannot open anomalies.log! No logs found.\n");
        return;
    }

    char line[512];
    int match_count = 0;

    printf("\n--- Searching Logs for: '%s' ---\n", keyword);
    while (fgets(line, sizeof(line), log_file) != NULL) {
        if (strstr(line, keyword) != NULL) {
            printf("%s", line);
            match_count++;
        }
    }

    fclose(log_file);
    printf("--- Search Complete: %d matches found ---\n\n", match_count);
}

int load_config(const char* filename, AppConfig* config) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return -1;

    char line[256];
    char key[50];
    char value_str[200];
    
    // Robust parser: handles "KEY VALUE" or "KEY=VALUE"
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip comments and empty lines
        if (line[0] == '\n' || line[0] == '#') continue;

        char *eq = strchr(line, '=');
        if (eq) *eq = ' ';

        if (sscanf(line, "%49s %199s", key, value_str) == 2) {
            if (strcmp(key, "MAX_TEMP") == 0) config->max_temp = atof(value_str);
            else if (strcmp(key, "MAX_CPU") == 0) config->max_cpu = atof(value_str);
            else if (strcmp(key, "DATA_PATH") == 0) {
                strncpy(config->data_path, value_str, sizeof(config->data_path) - 1);
                config->data_path[sizeof(config->data_path) - 1] = '\0';
            }
        }
    }
    fclose(file);
    return 0;
}

void process_sensor_data(float temp, float cpu, const AppConfig* config) {
    printf("  -> Temp: %.2f C, CPU: %.2f%%\n", temp, cpu);
    
    char alert_msg[256];
    int anomaly_detected = 0;

    // Temperature threshold validation
    if (temp > config->max_temp) {
        printf("  [ALERT] Temperature exceeds threshold (%.2f > %.2f)!\n", temp, config->max_temp);
        sprintf(alert_msg, "High Temperature Detected: %.2f C", temp);
        log_anomaly(alert_msg);
        anomaly_detected = 1;
    }
    
    // CPU threshold validation
    if (cpu > config->max_cpu) {
        printf("  [ALERT] CPU usage exceeds threshold (%.2f > %.2f)!\n", cpu, config->max_cpu);
        sprintf(alert_msg, "High CPU Usage Detected: %.2f%%", cpu);
        log_anomaly(alert_msg);
        anomaly_detected = 1;
    }

    if (!anomaly_detected) {
        printf("  [OK] Systems normal.\n");
    }
}

void show_help() {
    printf("\n--- Sentinel Interactive Commands ---\n");
    printf("  /search <keyword> : Search in anomaly logs\n");
    printf("  /status           : Show current config\n");
    printf("  /exit             : Stop the sentinel\n");
    printf("  /help             : Show this menu\n");
    printf("------------------------------------\n");
}

// Main entry point for the Edge Sentinel Agent
int main(int argc, char *argv[]) {
    // 1. CLI Mode: Quick search from outside
    if (argc >= 3 && strcmp(argv[1], "--search") == 0) {
        search_logs(argv[2]);
        return 0;
    }

    printf("Starting IoT Edge Sentinel (Professional Hybrid Mode)...\n");
    
    // Default values
    AppConfig config = {75.0, 85.0, "sensor_data.csv"}; 
    
    if (load_config("config.txt", &config) == 0) {
        printf("[INFO] Loaded configuration from config.txt\n");
    } else {
        printf("[WARN] config.txt not found. Using default thresholds and path.\n");
    }

    FILE *file = fopen(config.data_path, "r");
    if (file == NULL) {
        printf("[ERROR] Data file '%s' not found! Please check DATA_PATH in config.txt.\n", config.data_path);
        return 1;
    }

    char line[256];
    char command[100];
    
    // Skip CSV header
    fgets(line, sizeof(line), file);
    show_help();

    // 2. Continuous Processing + Interactive Loop
    while (1) {
        // A. Try to read a line from CSV (Simulation of data stream)
        if (fgets(line, sizeof(line), file) != NULL) {
            float t, c;
            // Skip the timestamp string (up to the first comma) and read the two floats
            if (sscanf(line, "%*[^,],%f,%f", &t, &c) == 2) {
                process_sensor_data(t, c, &config);
            }
        } else {
            // Reached end of file, let's wait for more data or user input
            printf("\n[IDLE] Waiting for data or command (type /help)... \n> ");
            
            // B. Simple Command Interface
            if (fgets(command, sizeof(command), stdin) != NULL) {
                command[strcspn(command, "\n")] = 0;

                if (strncmp(command, "/search ", 8) == 0) {
                    search_logs(command + 8);
                } 
                else if (strcmp(command, "/status") == 0) {
                    printf("Current Limits: Temp > %.2f, CPU > %.2f\nTarget Data Path: %s\n", 
                           config.max_temp, config.max_cpu, config.data_path);
                }
                else if (strcmp(command, "/exit") == 0) {
                    printf("Shutting down Sentinel...\n");
                    break;
                }
                else if (strcmp(command, "/help") == 0) {
                    show_help();
                }
                else if (strlen(command) > 0) {
                    printf("Unknown command. Type /help for options.\n");
                }
            }
            
            clearerr(file); 
        }
        
        usleep(500000);
    }

    fclose(file);
    return 0;
}
