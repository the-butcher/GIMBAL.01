

export class SerialPortHandler {

    // private encoder: TextEncoder;
    // private decoder: TextDecoder;
    // private port: SerialPort | undefined;

    // private reader: ReadableStreamDefaultReader<Uint8Array<ArrayBufferLike>> | undefined
    // private isOpened: boolean;

    constructor() {
        // this.encoder = new TextEncoder();
        // this.decoder = new TextDecoder();
        // this.isOpened = false;
        // this.setupListeners();
    }

    onConnect() {
        console.log('Device connected.');
    }

    onDisconnect() {
        console.log('Device disconnected.');
        // TODO disconnect
    }

    async openPort(): Promise<SerialPort | undefined> {
        try {

            const port = await navigator.serial.requestPort();
            await port.open({ baudRate: 9600 });

            // this.port = port;
            // this.isOpened = true;

            return port;
        } catch (error) {
            console.error(error);
            throw error;
        }
    }

    // async close() {

    //     this.reader?.releaseLock()
    //     // this.port?.readable?.getReader().releaseLock();
    //     await this.port?.close();
    //     this.isOpened = false;
    // }

    // async write(data: string) {
    //     const writer = this.port?.writable?.getWriter();
    //     const encoded = this.encoder.encode(data);
    //     await writer?.write(encoded);
    //     writer?.releaseLock();
    // }

    // async read(): Promise<string> {

    //     while (this.port?.readable) {
    //         this.reader = this.port.readable.getReader();
    //         let chunks = '';

    //         try {
    //             while (true) {
    //                 const { value, done } = await this.reader.read();
    //                 const decoded = this.decoder.decode(value);

    //                 chunks += decoded;

    //                 if (done || decoded.includes(EOT)) {
    //                     console.log('Reading done.');
    //                     this.reader.releaseLock();
    //                     break;
    //                 }
    //             }
    //             return chunks;
    //         } catch (error) {
    //             console.error(error);
    //             throw error;
    //         } finally {
    //             this.reader.releaseLock();

    //         }
    //     }
    //     return '';
    // }

    setupListeners() {
        navigator.serial.addEventListener('connect', this.onConnect);
        navigator.serial.addEventListener('disconnect', this.onDisconnect);
    }
}