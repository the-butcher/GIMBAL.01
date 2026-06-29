import { Button, CssBaseline, ThemeProvider } from '@mui/material';
import { useEffect, useRef, useState } from 'react';
import { Quaternion, Vector2 } from 'three';
import SceneComponent from './components/SceneComponent';
import "./styles.css";
import { ISceneProps } from './types/ISceneProps';
import { SerialPortHandler } from './utils/SerialPortHandler';
import { ThemeUtil } from './utils/ThemeUtil';


/**
 * TODO :: remove appState and replace with appropriate setMapProps (may need to set a ref, then state)
 * TODO :: require a concept that can animate the lights along with color and height (maybe include lights into the hexagons component)
 */

const theme = ThemeUtil.getTheme();

function App() {

  const [serialPort, setSerialPort] = useState<SerialPort>();

  const readTo = useRef<number>(-1);
  const decoder = new TextDecoder();

  const handleSerialConnect = () => {

    console.debug(`📞 handling serial connect`);

    const _serialPortHandler = new SerialPortHandler();
    _serialPortHandler.openPort().then(_serialPort => {
      console.log('got port', _serialPort?.getInfo());
      setSerialPort(_serialPort);
    });

  }

  const handleHandle = (handle: Quaternion) => {

    console.log(`📞 handling handle (handle)`, handle);

    scenePropsRef.current = {
      ...scenePropsRef.current,
      gimbal: {
        ...scenePropsRef.current.gimbal,
        handle
      }
    };
    setSceneProps(scenePropsRef.current);

  }

  // const handleAngles = (angleYaw: number, angleRoll: number, anglePitch: number): void => {

  //   console.log(`📞 handling angle (angleYaw, angleRoll, anglePitch)`, angleYaw, angleRoll, anglePitch);

  // }

  useEffect(() => {

    console.log('✨ building app component');

  }, []);

  const readFromReader = async (reader: ReadableStreamDefaultReader<Uint8Array<ArrayBufferLike>>) => {

    try {
      while (true) {

        const { value, done } = await reader.read();
        if (value && value[0] == 123 && value[value.length - 1] == 10) { // 123 = '{'

          const decoded = decoder.decode(new Uint8Array(value));
          // decoded = decoded.replace(/([a-z][^:]*)(?=\s*:)/g, '"$1"');
          console.log('decoded', decoded);

          const handleRaw = JSON.parse(decoded);
          const handle = new Quaternion(handleRaw.x, handleRaw.y, handleRaw.z, handleRaw.w);
          handleHandle(handle);


        }

        if (done) {
          console.log('Reading done.');
          reader.releaseLock();
          break;
        }

      }

    } catch (error) {
      console.error(error);
      throw error;
    } finally {
      reader.releaseLock();
    }

  }

  useEffect(() => {

    console.log('⚙ updating app component (serialPort)', serialPort);

    if (serialPort) {

      const reader = serialPort.readable?.getReader();
      if (reader) {

        window.clearInterval(readTo.current);
        window.setInterval(() => {

          readFromReader(reader);

        }, 100)

      }

    }

    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [serialPort]);

  const scenePropsRef = useRef<ISceneProps>({
    gimbal: {
      handle: new Quaternion(0.074585, 0.415710, -0.451782, 0.785828),
      lever0: {
        name: 'lever-0',
        points: [
          new Vector2(0, 0), new Vector2(0, -0.05), new Vector2(0.30, -0.05), new Vector2(0.40, -0.15)
        ],
        color: 0xff0000
      },
      lever1: {
        name: 'lever-1',
        points: [
          new Vector2(0, 0), new Vector2(0, -0.05), new Vector2(0.15, -0.05), new Vector2(0.40, -0.30), new Vector2(0.40, -0.65)
        ],
        color: 0x00ff00,
      },
      lever2: {
        name: 'lever-2',
        points: [
          new Vector2(0, 0), new Vector2(0, -0.05), new Vector2(0.20, -0.05), new Vector2(0.20, -0.30)
        ],
        color: 0x0000ff
      }


    }
  });
  const [sceneProps, setSceneProps] = useState<ISceneProps>(scenePropsRef.current);


  return (
    <ThemeProvider theme={theme}>
      <CssBaseline />
      <div style={{ position: 'absolute', width: '100%', height: '100%' }}>
        <SceneComponent {...sceneProps} />
      </div>
      <div style={{ position: 'absolute', width: '100%' }}>
        <Button
          onClick={handleSerialConnect}
        >connect</Button>

      </div>
    </ThemeProvider>
  );

};

export default App;