const { Client, LocalAuth } = require('whatsapp-web.js');
const readline = require('readline');
const qrcodeTerminal = require('qrcode-terminal');

const client = new Client({
    authStrategy: new LocalAuth(),
    puppeteer: {
        executablePath: process.env.CHROME_PATH || '/usr/bin/chromium',
        headless: true,
        args: [
            '--no-sandbox',
            '--disable-setuid-sandbox',
            '--disable-dev-shm-usage',
            '--disable-gpu',
            '--disable-accelerated-2d-canvas'
        ]
    }
});

async function enviarEmergencia() {
    try {
        const numero = '34654777350';
        const chatId = `${numero}@c.us`;
        const missatge = "Emergencia de l'avia";
        await client.sendMessage(chatId, missatge);
        console.log('EMERGENCY_SENT');
        console.log('Missatge d\'emergència enviat correctament');
    } catch (error) {
        console.log('EMERGENCY_ERROR');
        console.error('Error enviant l\'emergència:', error.message || error);
    }
}

client.on('qr', (qr) => {
    qrcodeTerminal.generate(qr, { small: true }, (qrAscii) => {
        console.log('QR_ASCII_START');
        console.log(qrAscii);
        console.log('QR_ASCII_END');
    });
});

client.on('authenticated', () => {
    console.log('APP_AUTHENTICATED');
    console.log('Autenticat correctament');
});

client.on('ready', () => {
    console.log('APP_READY');
    console.log('Bot preparat. La Raspberry Pi està connectada a WhatsApp');
});

client.on('message', async (msg) => {
    if (msg.isStatus || msg.from === 'status@broadcast') return;

    try {
        const contact = await msg.getContact();
        let nomMostrar = contact.name || contact.pushname || 'Desconegut';
        const hora = new Date().toLocaleTimeString('ca-ES', { hour: '2-digit', minute: '2-digit' });

        if (msg.hasMedia) {
            const media = await msg.downloadMedia();
            if (media && media.mimetype && media.mimetype.startsWith('image/')) {
                // Guardar la foto al disc
                const fs = require('fs');
                const ext = media.mimetype.split('/')[1] || 'jpg';
                const rutaFoto = `/home/paudomenech/TerraLink/foto_rebuda.${ext}`;
                fs.writeFileSync(rutaFoto, Buffer.from(media.data, 'base64'));
                console.log(`INCOMING:${hora} - ${nomMostrar}: [FOTO]${rutaFoto}`);
            } else {
                console.log(`INCOMING:${hora} - ${nomMostrar}: [Arxiu rebut]`);
            }
        } else {
            const cos = msg.body || '[Missatge sense text]';
            console.log(`INCOMING:${hora} - ${nomMostrar}: ${cos}`);
        }

    } catch (error) {
        console.error('Error en obtenir el contacte:', error);
    }
});

client.on('disconnected', (reason) => {
    console.log('APP_DISCONNECTED');
    console.log('Desconnectat:', reason);
});

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
});

rl.on('line', async (line) => {
    const cmd = line.trim();

    if (cmd === 'EMERGENCY') {
        await enviarEmergencia();
    }
    else if (cmd.startsWith('SEND:')) {
        // Format: SEND:numero:text
        const parts = cmd.substring(5).split(':');
        const numero = parts[0];
        const text = parts.slice(1).join(':'); // per si el text té ":"
        try {
            const chatId = `${numero}@c.us`;
            await client.sendMessage(chatId, text);
            console.log('SEND_OK');
        } catch (error) {
            console.log('SEND_ERROR');
            console.error('Error enviant:', error.message || error);
        }
    }
    else if (cmd === 'GET_CONTACTS') {
        try {
            const contacts = await client.getContacts();
            const cleaned = contacts
                .filter(c => c && (c.isMyContact || c.isWAContact) && c.number)
                .map(c => ({
                    number: c.number,
                    name: c.name || c.pushname || c.number
                }));
            console.log('CONTACTS_START');
            console.log(JSON.stringify(cleaned));
            console.log('CONTACTS_END');
        } catch (error) {
            console.log('CONTACTS_ERROR');
        }
    }
});

client.initialize();