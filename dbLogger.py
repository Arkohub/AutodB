import serial
import time

# Set the correct port (find the correct COM port or tty for your Arduino)
arduino_port = 'COM7'  # Replace with the correct COM port (Windows)
# For macOS/Linux use something like '/dev/tty.usbmodemXXXX'
baud_rate = 115200
output_file = "sound_data.csv"

# Open the serial port
ser = serial.Serial(arduino_port, baud_rate, timeout=1)
time.sleep(2)  # Wait for the Arduino to reset

# Open the output file for writing
try:
    with open(output_file, "w") as f:
        f.write("Timestamp (ms), Decibel Reading (dBA)\n")  # Write header
        print(f"File '{output_file}' opened successfully.")  # Debugging line
        
        # Read from Arduino and log to file
        while True:
            if ser.in_waiting > 0:
                data = ser.readline().decode('utf-8').strip()  # Read line and decode it
                if data:
                    print(f"Writing data: {data}")  # Debugging line
                    f.write(data + "\n")  # Write the data to file
                    f.flush()  # Flush the buffer to ensure data is written immediately
                else:
                    print("No data received from Arduino.")
            time.sleep(0.1)  # Slight delay to prevent overwhelming the serial port
except Exception as e:
    print(f"Error writing to file: {e}")
