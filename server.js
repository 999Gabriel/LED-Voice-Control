const express = require('express');
const { SerialPort } = require('serialport');
const app = express();
const port = 5050;

app.use(express.json());
app.use(express.static('public'));

// Initialize serialPort as null
let serialPort = null;

// Function to list available ports
async function listPorts() {
    const ports = await SerialPort.list();
    console.log('Available ports:');
    ports.forEach(port => {
        console.log(port.path);
    });
    return ports;
}

// Initialize serial port connection
async function initializeSerial() {
    try {
        const ports = await listPorts();
        const arduinoPort = ports.find(p => 
            p.path.includes('usbserial') || 
            p.path.includes('usbmodem')
        );

        if (arduinoPort) {
            console.log('Versuche Verbindung mit Arduino...');
            
            // Warte kurz, falls andere Prozesse die Schnittstelle noch freigeben müssen
            await new Promise(resolve => setTimeout(resolve, 2000));

            serialPort = new SerialPort({
                path: arduinoPort.path,
                baudRate: 9600,
                autoOpen: false  // Verhindert automatisches Öffnen
            });

            // Öffne Port manuell mit Fehlerbehandlung
            serialPort.open((err) => {
                if (err) {
                    console.log('Fehler beim Öffnen des Ports:', err.message);
                    console.log('Bitte schließen Sie den Arduino Serial Monitor und starten Sie den Server neu.');
                    process.exit(1);
                }
                console.log(`Erfolgreich verbunden mit Arduino auf ${arduinoPort.path}`);
            });

            // Verbindungsfehler-Handler
            serialPort.on('error', (err) => {
                console.log('Verbindungsfehler:', err.message);
                if (err.message.includes('Resource busy')) {
                    console.log('Der Port wird von einem anderen Programm verwendet.');
                    console.log('Bitte schließen Sie den Arduino Serial Monitor.');
                }
            });
        } else {
            console.log('Kein Arduino gefunden. Bitte Verbindung prüfen.');
        }
    } catch (err) {
        console.error('Fehler bei der Initialisierung:', err);
    }
}

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/public/index.html');  // Updated path
});

app.post('/send-command', (req, res) => {
    const command = req.body.command.toLowerCase();
    console.log('Received voice command:', command);
    
    let arduinoCommand = '';
    
    // Translate German commands to English
    if (command.includes('einschalten') || command.includes('on')) {
        arduinoCommand = 'ON';
    } else if (command.includes('ausschalten') || command.includes('off')) {
        arduinoCommand = 'OFF';
    } else if (command.includes('rot') || command.includes('red')) {
        arduinoCommand = 'RED';
    } else if (command.includes('grün') || command.includes('gruen') || command.includes('green')) {
        arduinoCommand = 'GREEN';
    } else if (command.includes('blau') || command.includes('blue')) {
        arduinoCommand = 'BLUE';
    } else if (command.includes('weiß') || command.includes('weiss') || command.includes('white')) {
        arduinoCommand = 'WHITE';
    } else if (command.includes('regenbogen') || command.includes('rainbow')) {
        arduinoCommand = 'RAINBOW';
    }

    if (!serialPort) {
        console.log('Arduino not connected');
        return res.json({ success: false, error: 'Arduino not connected' });
    }

    if (arduinoCommand) {
        serialPort.write(arduinoCommand + '\n', (err) => {
            if (err) {
                console.error('Fehler:', err);
                res.json({ success: false });
            } else {
                res.json({ success: true });
            }
        });
    } else {
        res.json({ success: false, error: 'Unbekannter Befehl' });
    }
});

// Initialize serial connection before starting server
initializeSerial().then(() => {
    const os = require('os');
    const networkInterfaces = os.networkInterfaces();
    
    console.log('\n=== Network Interfaces ===');
    Object.keys(networkInterfaces).forEach((ifname) => {
        networkInterfaces[ifname].forEach((iface) => {
            if (iface.family === 'IPv4') {
                console.log(`Interface ${ifname}: ${iface.address}`);
            }
        });
    });

    let ipAddress = '';
    Object.keys(networkInterfaces).forEach((ifname) => {
        networkInterfaces[ifname].forEach((iface) => {
            if (iface.family === 'IPv4' && !iface.internal) {
                ipAddress = iface.address;
            }
        });
    });

    app.listen(port, '0.0.0.0', () => {
        console.log('\n=== Server Configuration ===');
        console.log(`Server running on port ${port}`);
        console.log('\nAccess URLs:');
        console.log(`Local: http://localhost:${port}`);
        console.log(`Network: http://${ipAddress}:${port}`);
        console.log('\nMake sure:');
        console.log('1. Your computer is connected to your phone\'s hotspot');
        console.log('2. Try accessing the local URL from this computer first');
        console.log('3. Then try the network URL from your phone');
        console.log('\nPress Ctrl+C to stop the server');
        console.log('===========================\n');
    });
});