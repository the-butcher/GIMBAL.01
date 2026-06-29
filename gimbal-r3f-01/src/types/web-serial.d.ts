declare global {
    interface Navigator {
        serial: Serial;
    }

    interface SerialPortInfo {
        usbVendorId?: number;
        usbProductId?: number;
    }

    interface SerialPort {
        open(options?: SerialOptions): Promise<void>;
        close(): Promise<void>;
        readable: ReadableStream<Uint8Array> | null;
        writable: WritableStream<Uint8Array> | null;
        getInfo(): SerialPortInfo;
    }

    interface Serial {
        requestPort(options?: SerialPortRequestOptions): Promise<SerialPort>;
        getPorts(): Promise<SerialPort[]>;
        addEventListener(type: "connect" | "disconnect", listener: (e: Event | undefined) => void): void;
    }


}



export { };