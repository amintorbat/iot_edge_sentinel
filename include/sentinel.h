#ifndef SENTINEL_H
#define SENTINEL_H


typedef struct {
    float max_temp;
    float max_cpu;
} AppConfig;


int load_config(const char* filename, AppConfig* config);
void process_sensor_data(float temp, float cpu, const AppConfig* config);
void log_anomaly(const char* message);

#endif
