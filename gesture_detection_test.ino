#include <TensorFlowLite.h>

// Include the following for a XIAO implementation
// # include <TensorFlowLite_ESP32.h>

#include "main_functions.h"
#include "detection_responder.h"
#include "image_provider.h"
#include "model_settings.h"
#include "gesture_detect_model_data.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

constexpr int kTensorArenaSize = 136 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

tflite::MicroErrorReporter error_reporter; // Error reporter for debugging
const tflite::Model* model;                // TensorFlow Lite model
tflite::MicroInterpreter* interpreter;     // TensorFlow Lite interpreter
TfLiteTensor* input_tensor;                // Pointer to input tensor

int8_t image_data[kNumCols * kNumRows * kNumChannels];  // Buffer for preprocessed image data

void setup() {
    // Load the TFLite model
    model = tflite::GetModel(g_gesture_detect_model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(&error_reporter,
            "Model schema version %d does not match runtime version %d.",
            model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    static tflite::MicroMutableOpResolver<5> micro_op_resolver;
    micro_op_resolver.AddConv2D();
    micro_op_resolver.AddMaxPool2D();
    micro_op_resolver.AddReshape();
    micro_op_resolver.AddFullyConnected();
    micro_op_resolver.AddSoftmax();

    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(&error_reporter, "AllocateTensors() failed");
        return;
    }

    // Get a pointer to the input tensor
    input_tensor = interpreter->input(0);
  
}

void loop() {
    // Capture and preprocess an image
    if (GetImage(&error_reporter, kNumCols, kNumRows, kNumChannels, image_data) != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(&error_reporter, "Image capture failed");
        return;
    }

    // Populate the input tensor with preprocessed image data
    int index = 0;
    for (int y = 0; y < kNumRows; y++) {
        for (int x = 0; x < kNumCols; x++) {
            for (int c = 0; c < kNumChannels; c++) {
                input_tensor->data.int8[index++] =
                    static_cast<int8_t>((image_data[(y * kNumCols + x) * kNumChannels + c] / 255.0 - 0.5) * 256);
            }
        }
    }

    // Run inference
    if (interpreter->Invoke() != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(&error_reporter, "Invoke() failed");
        return;
    }

    // Get the model output
    TfLiteTensor* output_tensor = interpreter->output(0);

    // Debug output tensor information
    for (int i = 0; i < output_tensor->dims->data[0]; i++) {
        TF_LITE_REPORT_ERROR(&error_reporter, "Output[%d]: %d", i, output_tensor->data.int8[i]);
    }

    // Process the inference results
    RespondToDetection(&error_reporter,
        output_tensor->data.int8[0],  // "GO" score
        output_tensor->data.int8[1],  // "STOP" score
        output_tensor->data.int8[2],  // "SLOW" score
        output_tensor->data.int8[3]); // "DISCO" score

}

