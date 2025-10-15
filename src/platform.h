/*
 * platform.h
 * 
 * Platform configuration for ESP32 WebSocket Server
 * Based on mWebSockets library by Dawid Kurek
 * 
 * Simplified for ESP32 only (no multi-platform support needed)
 * Uses native ESP32 WiFi library directly
 */

#pragma once

#include <stdint.h>
#include "config.h"

// ESP32 native WiFi library
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

/** Specifies maximum number of clients connected to server. */
constexpr uint8_t kMaxConnections{8};

// Use ESP32's native WiFi types
using NetClient = WiFiClient;
using NetServer = WiFiServer;

