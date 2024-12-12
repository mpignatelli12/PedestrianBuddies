
import os
import cv2
import tensorflow as tf
import numpy as np

print(os.getcwd())
# Load the trained Keras model
model = tf.keras.models.load_model("./traffic_light_gesture_model_new5pm.keras")
for input_spec in model.inputs:
    print(input_spec)

# Path to the dataset
dataset_path = "./GrayscaleProcessedDataset/"
image_size = (32, 32)

# Define a representative dataset generator function
def representative_dataset_gen():
    for file in os.listdir(dataset_path):
        img = cv2.imread(os.path.join(dataset_path, file))

        if img is None:
            print(f"Warning: Unable to read image ''. Skipping.")
            continue  # Skip invalid or unreadable files

        img = cv2.resize(img, image_size)
        # Normalize and scale the image


        img = (img / 255.0 - 0.5) * 256
        # Explicitly cast to float32
        img = img.astype(np.float32)
        # Add batch dimension
        yield [np.expand_dims(img, axis=0)]

# Set up the TFLite converter
converter = tf.lite.TFLiteConverter.from_keras_model(model)

# Apply optimizations and specify quantization settings
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]  # Use only integer operations
converter.representative_dataset = representative_dataset_gen

# Ensure input/output tensors are also quantized to int8
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8

# Convert the model
quantized_model = converter.convert()

# Save the quantized model
with open("quantized_model5pm.tflite", "wb") as f:
    f.write(quantized_model)

# print("Quantized model saved as quantized_model.tflite")
