#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#ifndef ARDUINO_EXCLUDE_CODE

#include "detection_responder.h"
#include "Arduino.h"

// Flash the blue LED after each inference
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        int8_t go_score, int8_t stop_score,
                        int8_t slow_score, int8_t disco_score) {
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    is_initialized = true;
  }

  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  // This is meant to output to the control signals of a traffic light
  if (go_score > stop_score && go_score > slow_score && go_score > disco_score) {
    digitalWrite(LEDG, LOW); // Green LED for "go"
  } else if (stop_score > slow_score && stop_score > disco_score) {
    digitalWrite(LEDR, LOW); // Red LED for "stop"
  } else if (slow_score > disco_score) {
    digitalWrite(LEDB, LOW); // Blue LED for "slow"
  } else {
    // Combine LEDs for "disco"
    for (int i = 0; i < 3; ++i) {
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDB, LOW);
      delay(100);
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);
      delay(100);
    }
  }

  TF_LITE_REPORT_ERROR(error_reporter,
                       "Scores - Go: %d, Stop: %d, Slow: %d, Disco: %d",
                       go_score, stop_score, slow_score, disco_score);
}


#endif  // ARDUINO_EXCLUDE_CODE
