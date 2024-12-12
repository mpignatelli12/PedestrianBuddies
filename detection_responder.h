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


// Provides an interface to take an action based on the output from the gesture
// detection model.

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_GESTURE_DETECTION_DETECTION_RESPONDER_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_GESTURE_DETECTION_DETECTION_RESPONDER_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"

// Called every time the results of a detection run are available.
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        int8_t go_score, int8_t stop_score,
                        int8_t slow_score, int8_t disco_score);


#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_GESTURE_DETECTION_DETECTION_RESPONDER_H_
