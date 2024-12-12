import os

# Define the paths
tflite_model_path = "quantized_model5pm.tflite"  # Your TFLite model file
output_header_path = "quantized_model5pm.cpp"      # Output header file

# Read the .tflite model as binary data
with open(tflite_model_path, "rb") as f:
    tflite_model = f.read()

# Write the .h file
with open(output_header_path, "w") as f:
    f.write('#ifndef TRAFFIC_LIGHT_GESTURE_MODEL_H_\n')
    f.write('#define TRAFFIC_LIGHT_GESTURE_MODEL_H_\n\n')
    f.write('// TFLite model converted to a C data array\n\n')
    f.write('const unsigned char g_traffic_light_gesture_model_tflite[] = {\n')

    # Write the model data in C array format
    for i, byte in enumerate(tflite_model):
        if i % 12 == 0:  # Add line breaks for readability
            f.write("\n  ")
        f.write(f"0x{byte:02x}, ")

    f.write('\n};\n')
    f.write(f'const int g_traffic_light_gesture_model_tflite_len = {len(tflite_model)};\n\n')
    f.write('#endif  // TRAFFIC_LIGHT_GESTURE_MODEL_H_\n')

print(f"Header file saved as {output_header_path}")
