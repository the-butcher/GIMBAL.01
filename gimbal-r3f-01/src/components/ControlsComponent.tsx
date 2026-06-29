import { OrbitControls, OrbitControlsChangeEvent } from '@react-three/drei';
import { useFrame, useThree } from '@react-three/fiber';
import { createRef, useEffect } from 'react';
import { OrbitControls as OrbitControlsX } from 'three/examples/jsm/controls/OrbitControls';
import { ScreenshotUtil } from '../utils/ScreenshotUtil';
// // @ts-expect - error no declaration
// import { N8AOPostPass } from "n8ao";

const ControlsComponent = () => {

  const { camera, gl, scene, invalidate } = useThree();

  const controlsRef = createRef();

  const takeScreenshot = () => {
    ScreenshotUtil.getInstance().renderToFrame(gl); // , scene, camera, 0
    if (ScreenshotUtil.getInstance().getFrameCount() === 1) {
      ScreenshotUtil.getInstance().exportToPng();
    }
    ScreenshotUtil.getInstance().removeFrame(0);
  };

  const handleKeyUp = (e: KeyboardEvent) => {

    if (e.key === 'p') {

      handleKeyUpP();

    } else if (e.key === 's') {

      takeScreenshot();

    }

  };

  const handleKeyUpP = () => {

    console.debug('⚙ handle key up "p"');

    takeScreenshot();

    setTimeout(() => {
      handleKeyUpP();
    }, 2500); // 2500

  }

  useEffect(() => {

    console.log('✨ building controls component');

    window.addEventListener('keyup', handleKeyUp);

    // const controls = controlsRef.current as OrbitControlsX;
    // controls.addEventListener('change', () => {

    //   // console.log('polar angle', controls.getPolarAngle(), camera.position, controls.target, controls.getPolarAngle(), controls.getAzimuthalAngle());
    //   // console.log(`camera.position.set(${camera.position.x}, ${camera.position.y}, ${camera.position.z});`);

    // });
    // controls.minDistance = 0.1;

    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  useEffect(() => {

    console.log('⚙ updating controls component (camera, gl)', camera, gl);

    camera.position.set(-2.252266569955268, 2.082737577327945, 3.9483540218485977);
    camera.zoom = 470;
    setTimeout(() => {
      invalidate();
    }, 500)

    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [camera, gl, scene]);

  const handleControlsChange = (e: OrbitControlsChangeEvent) => {

    e.target.rotateSpeed = camera.position.length() / 15;

    // console.log(`camera.position.set(${camera.position.x}, ${camera.position.y}, ${camera.position.z});`);
    // console.log('camera', camera)
    invalidate();

  }

  useFrame(() => { // { gl, scene, camera }

    // gl.clearColor();
    gl.render(scene, camera);


  }, 2);

  return (
    <OrbitControls ref={controlsRef} enableRotate={true} enablePan={true} dampingFactor={1.05} rotateSpeed={0.01} zoomSpeed={2} onChange={handleControlsChange} minDistance={1.05} />
  );

};

export default ControlsComponent;
