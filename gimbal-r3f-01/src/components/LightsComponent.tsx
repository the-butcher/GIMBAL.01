import { useThree } from '@react-three/fiber';
import { useEffect, useRef } from 'react';
import { DirectionalLight } from 'three';

const LightsComponent = () => {

  const { camera, gl, scene } = useThree();

  const lightRef = useRef<DirectionalLight>();

  useEffect(() => {

    console.log('✨ building lights component');

    // const lightHelper = new DirectionalLightHelper(lightRef.current!);
    // scene.add(lightHelper);

  }, []);

  useEffect(() => {

    console.log('⚙ updating lights component (camera, gl)', camera, gl);

  }, [camera, gl, scene]);


  return (
    <>
      < ambientLight
        intensity={0}
      />
      <directionalLight
        ref={lightRef}
        castShadow={true}
        intensity={2}
        position={[5, 5, 5]}
        lookAt={[0, 0, 0]}
      />
      <directionalLight
        ref={lightRef}
        castShadow={true}
        intensity={2}
        position={[-5, 5, -5]}
        lookAt={[0, 0, 0]}
      />
    </>
  );

};

export default LightsComponent;
