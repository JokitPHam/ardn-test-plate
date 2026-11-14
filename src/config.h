#ifndef CONFIG_H
#define CONFIG_H

// --- Display ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// --- Sensoren ---
#define DHTPIN 2
#define DHTTYPE DHT22

// --- Relais (SSR) ---
#define SSR_HEAT 4
#define SSR_COOL 5
#define SSR_HUMID 6
#define SSR_LIGHT 7

// --- RS-485 ---
#define MAX485_DE 8

// --- Regelungsparameter ---
#define TEMP_SETPOINT 22.0
#define TEMP_HYSTERESIS 1.0
#define HUM_SETPOINT 50.0
#define HUM_HYSTERESIS 5.0

#endif
