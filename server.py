from flask import Flask, render_template, request, jsonify
import serial
import time

app = Flask(__name__)

# Configure this to match your Arduino's serial port
SERIAL_PORT = '/dev/cu.usbserial-10' # Adjust this for your setup
BAUD_RATE = 9600

def send_to_arduino(command):
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
            time.sleep(2)  # Wait for Arduino to reset
            ser.write(f"{command}\n".encode())
            return True
    except Exception as e:
        print(f"Error: {e}")
        return False

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/send-command', methods=['POST'])
def send_command():
    command = request.json.get('command', '').lower()
    success = send_to_arduino(command)
    return jsonify({'success': success})

if __name__ == '__main__':
    print("\n=== Server Configuration ===")
    print("Server starting on port 5050...")
    print("\nAccess URLs:")
    print("Local: http://localhost:5050")
    print("Network: http://10.10.216.168:5050")
    print("\nMake sure:")
    print("1. Your phone is on the same WiFi network")
    print("2. Try accessing the local URL from this computer first")
    print("3. Then try the network URL from your phone")
    print("\nPress Ctrl+C to stop the server")
    print("===========================\n")
    
    app.run(host='0.0.0.0', port=5050, debug=True, threaded=True)