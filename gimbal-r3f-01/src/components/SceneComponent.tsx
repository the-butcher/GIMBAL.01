import { Canvas } from '@react-three/fiber';
import { createRef, useEffect } from 'react';
import { PCFSoftShadowMap } from 'three';

import { ISceneProps } from '../types/ISceneProps';
import ControlsComponent from './ControlsComponent';
import GimbalComponent from './GimbalComponent';
import LightsComponent from './LightsComponent';
import { Stats } from '@react-three/drei';


export const ID_CANVAS = 'dashcanvas';

const SceneComponent = (props: ISceneProps) => {

  const { gimbal } = { ...props };
  const canvasRef = createRef<HTMLCanvasElement>();

  useEffect(() => {

    console.debug('✨ building scene component', props, canvasRef.current);

    if (canvasRef.current) {
      canvasRef.current.id = ID_CANVAS;
    }

    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);


  return (
    <Canvas
      ref={canvasRef}
      style={{ position: 'absolute', height: '100%', width: '100%' }}
      gl={{
        preserveDrawingBuffer: true,
        /**
         * https://github.com/pmndrs/postprocessing
         */
        powerPreference: "high-performance",
        antialias: true,
        stencil: false,
        depth: true
      }}
      frameloop="demand" // demand
      shadows={{ type: PCFSoftShadowMap, enabled: true, autoUpdate: true }}
      // orthographic
      camera={{
        near: 0.5,
        far: 100,
        fov: 30
        // isOrthographicCamera: true
      }}
    >

      <ControlsComponent />
      <LightsComponent />

      <gridHelper scale={0.25} args={[10, 10, 0x444444, 0x333333]} />
      {/* <axesHelper scale={1} /> */}
      {/* <Stats /> */}
      <GimbalComponent {...gimbal} />

    </Canvas>
  );
};

export default SceneComponent;
