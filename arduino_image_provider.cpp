/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Adapted for use by Michael Pignatelli, Zeno Dancanet 2024

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "image_provider.h"

#ifndef ARDUINO_EXCLUDE_CODE

#include "Arduino.h"
#include <TinyMLShield.h>

// Get an image from the camera module
TfLiteStatus GetImage(tflite::ErrorReporter* error_reporter, int image_width,
                      int image_height, int channels, int8_t* image_data) {

  byte data[176 * 144]; // Receiving QCIF grayscale from camera = 176 * 144 * 1

  if (!Camera.isInitialized()) {
      TF_LITE_REPORT_ERROR(error_reporter, "Camera not initialized!");
      return kTfLiteError;
  }

  static bool g_is_camera_initialized = false;
  static bool serial_is_initialized = false;

  // Initialize camera if necessary
  if (!g_is_camera_initialized) {
    if (!Camera.begin(QCIF, GRAYSCALE, 5, OV7675)) {
      TF_LITE_REPORT_ERROR(error_reporter, "Failed to initialize camera!");
      return kTfLiteError;
    }
    g_is_camera_initialized = true;
  }

  // Read camera data
  Camera.readFrame(data);

  int min_x = (176 - 32) / 2;
  int min_y = (144 - 32) / 2;
  int index = 0;

  // Crop 32x32 image. This lowers FOV, ideally we would downsample but this is simpler. 
  for (int y = min_y; y < min_y + 32; y++) {
    for (int x = min_x; x < min_x + 32; x++) {
      image_data[index++] = static_cast<int8_t>(((data[(y * 176) + x] / 255.0) - 0.5) * 256);
    }
  }

  return kTfLiteOk;
}

#endif  // ARDUINO_EXCLUDE_CODE