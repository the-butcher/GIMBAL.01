import { createRef, useEffect, useRef } from 'react';
import { ArrowHelper, BufferGeometry, DoubleSide, Group, Line, LineBasicMaterial, Matrix4, Mesh, MeshBasicMaterial, Quaternion, Vector2, Vector3 } from 'three';

import { invalidate, useFrame, useThree } from '@react-three/fiber';
import { IAxisAngles } from '../types/IAxisAngles';
import { IGimbalProps } from '../types/IGimbalProps';
import { IReferenceFrame } from '../types/IReferenceFrame';
import { ReferenceFrame } from '../types/ReferenceFrame';
import { easeOut } from "motion";


const GimbalComponent = (props: IGimbalProps) => {

    const { handle, lever0, lever1, lever2 } = { ...props };

    const groupRef = createRef<Group>();

    /**
     * plane vertical through camera (desired) direction
     */
    const planeRefCV = createRef<Group>();

    /**
     * helper cone to describe possible roll axis positions
     */
    const coneRefYD = createRef<Group>();

    const arrowYDRef = useRef<ArrowHelper>(); // yaw direction
    const arrowYURef = useRef<ArrowHelper>(); // yaw up

    const arrow0DRef = useRef<ArrowHelper>();
    const arrow0URef = useRef<ArrowHelper>();
    const arrow0RRef = useRef<ArrowHelper>();

    const arrow1DRef = useRef<ArrowHelper>();
    const arrow1URef = useRef<ArrowHelper>();
    const arrow1RRef = useRef<ArrowHelper>();

    const arrow2FRef = useRef<ArrowHelper>(); // camera direction as set by gimbal angles

    const arrowCDRef = useRef<ArrowHelper>(); // camera direction as desired

    const anglesDestRef = useRef<IAxisAngles>({
        y: 0,
        r: 0,
        p: 0
    });
    const anglesCurrRef = useRef<IAxisAngles>({
        y: 0,
        r: 0,
        p: 0
    });

    const handleDestRef = useRef<Quaternion>(new Quaternion());
    const handleOrigRef = useRef<Quaternion>(new Quaternion());
    const handleCurrRef = useRef<Quaternion>(new Quaternion());

    /**
     * beginning millis of animation
     */
    const tsAnimOrig = useRef<number>(-1);
    /**
     * ending millis of animation
     */
    const tsAnimDest = useRef<number>(-1);

    const { scene } = useThree();

    const leverPos0Ref = useRef(new Float32Array(1000 * 3));
    const leverGeom0Ref = useRef<BufferGeometry>(new BufferGeometry());
    const leverPos1Ref = useRef(new Float32Array(1000 * 3));
    const leverGeom1Ref = useRef<BufferGeometry>(new BufferGeometry());
    const leverPos2Ref = useRef(new Float32Array(1000 * 3));
    const leverGeom2Ref = useRef<BufferGeometry>(new BufferGeometry());

    /**
     * desired camera direction
     */
    const vectorCD: Vector3 = new Vector3(1, 0, 1).normalize();
    const leverHColor = 0xffff00;

    const setArrowHelperOpacity = (arrowHelper: ArrowHelper, opacity: number) => {
        const arrowLine = arrowHelper.children[0] as Line;
        (arrowLine.material as LineBasicMaterial).transparent = true;
        (arrowLine.material as LineBasicMaterial).opacity = opacity;
        const arrowMesh = arrowHelper.children[1] as Mesh;
        (arrowMesh.material as MeshBasicMaterial).transparent = true;
        (arrowMesh.material as MeshBasicMaterial).opacity = opacity;
    }

    useEffect(() => {

        console.debug('✨ building handle component', props);

        const opacity = 0.5;

        arrowYDRef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 1.0, 0xffff00, 0.1);
        scene.add(arrowYDRef.current);
        setArrowHelperOpacity(arrowYDRef.current, opacity);
        arrowYURef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 0.3, 0xff00ff, 0.1);
        scene.add(arrowYURef.current);
        setArrowHelperOpacity(arrowYURef.current, opacity);

        arrow0DRef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 0.15, 0xffff00, 0.08);
        scene.add(arrow0DRef.current);
        setArrowHelperOpacity(arrow0DRef.current, opacity);
        arrow0URef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 0.12, 0xff00ff, 0.07);
        scene.add(arrow0URef.current);
        setArrowHelperOpacity(arrow0URef.current, opacity);
        arrow0RRef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 0.12, 0x00ffff, 0.07);
        scene.add(arrow0RRef.current);
        setArrowHelperOpacity(arrow0RRef.current, opacity);

        arrow1DRef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 0.15, 0xffff00, 0.08);
        scene.add(arrow1DRef.current);
        setArrowHelperOpacity(arrow1DRef.current, opacity);
        arrow1URef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 0.12, 0xff00ff, 0.07);
        scene.add(arrow1URef.current);
        setArrowHelperOpacity(arrow1URef.current, opacity);
        arrow1RRef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 0.12, 0x00ffff, 0.07);
        scene.add(arrow1RRef.current);
        setArrowHelperOpacity(arrow1RRef.current, opacity);

        arrow2FRef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 0.3, 0xcccccc, 0.1);
        scene.add(arrow2FRef.current);

        arrowCDRef.current = new ArrowHelper(new Vector3(1, 0, 0), new Vector3(0, 0, 0), 1.0, 0xcccccc, 0.1);
        arrowCDRef.current?.setDirection(vectorCD);
        scene.add(arrowCDRef.current);

        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, []);

    // TODO :: move to util function
    const getAngle = (vectorA: Vector3, vectorB: Vector3, normal: Vector3): number => {

        const angle = vectorA.clone().angleTo(vectorB.clone());
        const cross = new Vector3().crossVectors(vectorA.clone().normalize(), vectorB.clone().normalize());
        if (normal.clone().dot(cross) < 0) {
            return angle; // It is already "clockwise" relative to the normal
        } else {
            return -angle;
        }
    }

    const normalizeAngle = (angle: number): number => {
        let _angle = angle;
        while (_angle < 0) {
            _angle += (Math.PI * 2);
        }
        while (_angle > Math.PI * 2) {
            _angle -= (Math.PI * 2);
        }
        return _angle;
    }

    const RAD_TO_GRAD = 180 / Math.PI;
    const GRAD_TO_RAD = Math.PI / 180;

    const createReferenceFrameY = (): IReferenceFrame => {

        /**
         * yaw direction transformed by handle quaternion
         */
        const arrowDirYD = new Vector3(1, 0, 0);
        arrowDirYD.applyQuaternion(handleCurrRef.current);
        arrowDirYD.applyAxisAngle(new Vector3(1, 0, 0), -Math.PI / 2);
        arrowDirYD.set(arrowDirYD.x, -arrowDirYD.y, arrowDirYD.z);
        arrowDirYD.normalize();

        /**
         * yaw zero angle (up)
         */
        const arrowDirYU = new Vector3(0, 0, -1);
        arrowDirYU.applyQuaternion(handleCurrRef.current);
        arrowDirYU.applyAxisAngle(new Vector3(1, 0, 0), -Math.PI / 2);
        arrowDirYU.set(arrowDirYU.x, -arrowDirYU.y, arrowDirYU.z)
        arrowDirYU.normalize();

        const referenceFrameY = new ReferenceFrame({
            id: 'axis-yaw',
            origin: new Vector3(),
            vectorD: arrowDirYD,
            vectorU: arrowDirYU
        });

        // point the helper cone in the correct direction
        coneRefYD.current?.lookAt(referenceFrameY.getVectorZ())

        // set direction of yaw helper arrows
        referenceFrameY.applyToArrowHelperY(arrowYURef.current!);
        referenceFrameY.applyToArrowHelperZ(arrowYDRef.current!);

        return referenceFrameY;

    }

    const createReferenceFrameR = (referenceFrameY: IReferenceFrame, angleYaw: number, debugPoints: Vector3[]): IReferenceFrame => {

        let leverPoint2A: Vector2 | undefined = undefined;
        let leverPoint2B: Vector2 | undefined = undefined;
        let leverPoint3A: Vector3;
        let leverPoint3B: Vector3;

        const leverMatrix0: Matrix4 = new Matrix4();
        leverMatrix0.lookAt(new Vector3(), referenceFrameY.getVectorZ(), referenceFrameY.getVectorY()); // just look down the yaw-axis
        leverMatrix0.multiply(new Matrix4().makeRotationZ(angleYaw)) // apply yaw-angle

        // build lever-0
        // -- hinged at [0, 0, 0] and arrowDirA
        for (let i = 1; i < lever0.points.length; i++) {
            leverPoint2A = lever0.points[i - 1];
            leverPoint2B = lever0.points[i];
            leverPoint3A = new Vector3(0, leverPoint2A.x, leverPoint2A.y).applyMatrix4(leverMatrix0);
            leverPoint3B = new Vector3(0, leverPoint2B.x, leverPoint2B.y).applyMatrix4(leverMatrix0);
            debugPoints.push(leverPoint3A);
            debugPoints.push(leverPoint3B);
        }
        const leverAxis0 = leverPoint2B!.clone().sub(leverPoint2A!.clone()).normalize();

        // lever-0 continuing axis angles 90 degrees with respect to the last lever segment
        leverPoint3A = new Vector3(0, leverPoint2B!.x, leverPoint2B!.y).applyMatrix4(leverMatrix0);
        leverPoint3B = new Vector3(0, leverPoint2B!.x + leverAxis0.y, leverPoint2B!.y - leverAxis0.x).applyMatrix4(leverMatrix0);

        // roll axis after yaw rotation
        const arrowDir0D = leverPoint3B.clone().sub(leverPoint3A.clone());

        leverPoint3B = new Vector3(0, leverPoint2B!.x - leverAxis0.x, leverPoint2B!.y - leverAxis0.y).applyMatrix4(leverMatrix0);

        // up the lever (=zero rotation)
        const arrowDir0U = leverPoint3B.clone().sub(leverPoint3A.clone());

        const referenceFrameR = new ReferenceFrame({
            id: 'axis-roll',
            origin: leverPoint3A,
            vectorD: arrowDir0D,
            vectorU: arrowDir0U
        });

        referenceFrameR.applyToArrowHelperX(arrow0RRef.current!);
        referenceFrameR.applyToArrowHelperY(arrow0URef.current!);
        referenceFrameR.applyToArrowHelperZ(arrow0DRef.current!);

        return referenceFrameR;

    }

    const createReferenceFrameP = (referenceFrameR: IReferenceFrame, angleRoll: number, debugPoints: Vector3[]): IReferenceFrame => {

        let leverPoint2A: Vector2 | undefined = undefined;
        let leverPoint2B: Vector2 | undefined = undefined;
        let leverPoint3A: Vector3;
        let leverPoint3B: Vector3;

        const leverMatrix1 = new Matrix4().makeTranslation(referenceFrameR.getOrigin());
        leverMatrix1.lookAt(new Vector3(), referenceFrameR.getVectorZ(), referenceFrameR.getVectorY()); // just look down the roll-axis
        leverMatrix1.multiply(new Matrix4().makeRotationZ(angleRoll)) // apply roll-angle

        // build lever-1
        // -- hinged at leverPoint3A and arrowDir0D
        for (let i = 1; i < lever1.points.length; i++) {
            leverPoint2A = lever1.points[i - 1];
            leverPoint2B = lever1.points[i];
            leverPoint3A = new Vector3(0, leverPoint2A.x, leverPoint2A.y).applyMatrix4(leverMatrix1);
            leverPoint3B = new Vector3(0, leverPoint2B.x, leverPoint2B.y).applyMatrix4(leverMatrix1);
            debugPoints.push(leverPoint3A);
            debugPoints.push(leverPoint3B);
        }
        // last segment of lever-1
        const leverAxis1 = leverPoint2B!.clone().sub(leverPoint2A!.clone()).normalize();

        // normal to last lever-1 segment
        leverPoint3A = new Vector3(0, leverPoint2B!.x, leverPoint2B!.y).applyMatrix4(leverMatrix1);
        leverPoint3B = new Vector3(0, leverPoint2B!.x + leverAxis1.y, leverPoint2B!.y - leverAxis1.x).applyMatrix4(leverMatrix1);

        const arrowDir1D = leverPoint3B.clone().sub(leverPoint3A.clone());

        leverPoint3B = new Vector3(0, leverPoint2B!.x - leverAxis1.x, leverPoint2B!.y - leverAxis1.y).applyMatrix4(leverMatrix1);

        const arrowDir1U = leverPoint3B.clone().sub(leverPoint3A.clone());

        const referenceFrameP = new ReferenceFrame({
            id: 'axis-pitch',
            origin: leverPoint3A,
            vectorD: arrowDir1D,
            vectorU: arrowDir1U
        });

        referenceFrameP.applyToArrowHelperX(arrow1RRef.current!);
        referenceFrameP.applyToArrowHelperY(arrow1URef.current!);
        referenceFrameP.applyToArrowHelperZ(arrow1DRef.current!);

        return referenceFrameP;

    }

    const createLever2 = (referenceFrameP: IReferenceFrame, anglePitch: number, debugPoints: Vector3[]): void => {

        let leverPoint2A: Vector2 | undefined = undefined;
        let leverPoint2B: Vector2 | undefined = undefined;
        let leverPoint3A: Vector3;
        let leverPoint3B: Vector3;

        const leverMatrix2 = new Matrix4().makeTranslation(referenceFrameP.getOrigin());
        leverMatrix2.lookAt(new Vector3(), referenceFrameP.getVectorZ(), referenceFrameP.getVectorY());
        leverMatrix2.multiply(new Matrix4().makeRotationZ(anglePitch)) // pitch motor angle

        // build lever-2
        // -- hinged at leverPoint3A and arrowDir0D
        for (let i = 1; i < lever2.points.length; i++) {
            leverPoint2A = lever2.points[i - 1];
            leverPoint2B = lever2.points[i];
            leverPoint3A = new Vector3(0, leverPoint2A.x, leverPoint2A.y).applyMatrix4(leverMatrix2);
            leverPoint3B = new Vector3(0, leverPoint2B.x, leverPoint2B.y).applyMatrix4(leverMatrix2);
            debugPoints.push(leverPoint3A);
            debugPoints.push(leverPoint3B);
        }

        leverPoint3A = new Vector3(0, leverPoint2B!.x, leverPoint2B!.y).applyMatrix4(leverMatrix2);
        leverPoint3B = new Vector3(1, leverPoint2B!.x, leverPoint2B!.y).applyMatrix4(leverMatrix2);

        const arrowDir2F = leverPoint3B.clone().sub(leverPoint3A.clone());
        arrow2FRef.current?.position.set(leverPoint3A.x, leverPoint3A.y, leverPoint3A.z);
        arrow2FRef.current?.setDirection(arrowDir2F);

    }

    const handleHandle = () => {

        const leverPoints0: Vector3[] = [];
        const leverPoints1: Vector3[] = [];
        const leverPoints2: Vector3[] = [];

        const referenceFrameY = createReferenceFrameY();

        // place the 3d representation of the handle
        if (groupRef.current) {
            groupRef.current.setRotationFromQuaternion(handleCurrRef.current);
        }

        // ==== YAW ANGLE ==============================================================================================

        /**
         * the plane normal that we want the roll axis to be parallel to after yaw rotation, vertical through camera axis
         * Camera Vertival Normal
         */
        const vectorCVV = new Vector3(0, 1, 0).applyAxisAngle(vectorCD, 0); // camera roll angle here

        const vectorCVN = vectorCD.clone().normalize().cross(vectorCVV);

        // put the helper plane through camera axis
        planeRefCV.current?.lookAt(vectorCVN)

        /**
         * direction of intersection beween yaw plane and camera vertical plane
         * Camera Vertical Yaw
         */
        const vectorCVY = vectorCVN.clone().normalize().cross(referenceFrameY.getVectorZ());

        const angleYawBase = getAngle(referenceFrameY.getVectorY(), vectorCVY, referenceFrameY.getVectorZ());
        console.log('angleYawBase', angleYawBase * RAD_TO_GRAD);

        // project handle axis onto camera normal, then calculate lengthCBN from it
        // this angle is in the "vertical", between handle axis and camera vertical plane
        const angleCBN = Math.asin(vectorCVN.dot(referenceFrameY.getVectorZ()));
        const lengthCBN = Math.sin(angleCBN) / Math.cos(angleCBN);

        // limiting angle is where lengthCBN > 1
        const angleYawSemi = Math.abs(lengthCBN) < 1 ? Math.asin(lengthCBN) : Math.PI / 2 * Math.sign(lengthCBN);

        const angleYawS = angleYawSemi + angleYawBase; // yaw-lever hangs
        const angleYawA = Math.PI - angleYawSemi + angleYawBase;

        // console.log('angleYawBase', angleYawBase, 'angleYawSemi', angleYawSemi, 'angleYawS', angleYawS * RAD_TO_GRAD, 'angleYawA', angleYawA * RAD_TO_GRAD);

        const angleYaw = normalizeAngle(angleYawS + Math.PI);

        referenceFrameY.drawAngleZ(angleYaw, leverPoints0);

        console.log('angleYaw', angleYaw * RAD_TO_GRAD);
        const isAngleCalc = true;

        // ==== YAW ANGLE ==============================================================================================

        /**
         * intersection of planes (normal to yaw / camera vertical)
         * In the yaw plane, normal to camera vector ()
         */
        // const vectorCBN = vectorCVY.clone().normalize().cross(referenceFrameY.getVectorZ())
        // if (vectorCBN) {

        //     const intersectCBN = referenceFrameY.getVectorZ().clone().add(vectorCBN.clone().multiplyScalar(lengthCBN));

        //     // leverPoints2.push(new Vector3());
        //     // leverPoints2.push(referenceFrameY.getVectorY());
        //     // leverPoints2.push(new Vector3());
        //     // leverPoints2.push(vectorCVY.clone().normalize());

        //     const lengthCFC = Math.sqrt(1 - lengthCBN * lengthCBN);

        //     // normal between handle axis and camera vertical plane
        //     leverPointsH.push(referenceFrameY.getVectorZ().clone());
        //     leverPointsH.push(intersectCBN.clone());

        //     const pointCFCS = intersectCBN.clone().sub(vectorCVY.clone().normalize().multiplyScalar(lengthCFC));
        //     const pointCFCA = intersectCBN.clone().add(vectorCVY.clone().normalize().multiplyScalar(lengthCFC));

        //     leverPointsH.push(new Vector3());
        //     leverPointsH.push(pointCFCS.clone());
        //     leverPointsH.push(new Vector3());
        //     leverPointsH.push(pointCFCA.clone());

        //     leverPointsH.push(pointCFCS.clone());
        //     leverPointsH.push(pointCFCA.clone());

        //     leverPointsH.push(referenceFrameY.getVectorZ().clone());
        //     leverPointsH.push(pointCFCS.clone());

        //     leverPointsH.push(referenceFrameY.getVectorZ().clone());
        //     leverPointsH.push(pointCFCA.clone());

        // }

        const referenceFrameR = createReferenceFrameR(referenceFrameY, angleYaw, leverPoints0);

        // ==== ROLL ANGLE =============================================================================================

        let angleRoll = referenceFrameR.getAngleZ(vectorCD, leverPoints0);
        console.log('angleRoll 1', angleRoll * RAD_TO_GRAD, Math.cos(angleRoll));
        // angleRoll = isAngleCalc ? normalizeAngle(angleRoll + Math.sign(Math.cos(angleRoll)) * Math.PI / 2) : 0;
        angleRoll = isAngleCalc ? normalizeAngle(angleRoll - Math.PI / 2) : 0;
        console.log('angleRoll 2', angleRoll * RAD_TO_GRAD);
        // angleRoll = isAngleCalc ? angleRoll - Math.PI / 2 : 0;
        // console.log('angleRoll', angleRoll * RAD_TO_GRAD);

        referenceFrameR.drawAngleZ(angleRoll, leverPoints1);

        // ==== ROLL ANGLE =============================================================================================

        const referenceFrameP = createReferenceFrameP(referenceFrameR, angleRoll, leverPoints1);

        // ==== PITCH ANGLE ============================================================================================

        let anglePitch = referenceFrameP.getAngleZ(vectorCD.clone(), leverPoints1);
        console.log('anglePitch 1', anglePitch * RAD_TO_GRAD);
        anglePitch = isAngleCalc ? normalizeAngle(anglePitch + Math.PI / 2) : 0;
        console.log('anglePitch 2', anglePitch * RAD_TO_GRAD);

        referenceFrameP.drawAngleZ(anglePitch, leverPoints2);

        // ==== PITCH ANGLE ============================================================================================

        createLever2(referenceFrameP, anglePitch, leverPoints2);

        anglesDestRef.current = {
            y: angleYaw,
            r: angleRoll,
            p: anglePitch
        };

    }

    useEffect(() => {

        console.debug('⚙ updating gimbal component (handle)', handle);

        // set the handle destination position
        handleOrigRef.current = handleCurrRef.current;
        handleDestRef.current = handle;

        tsAnimOrig.current = Date.now();
        tsAnimDest.current = tsAnimOrig.current + 350; // assume new positions are coming in every ~250ms

        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, [handle]);

    const getAngleDifference = (startAngle: number, endAngle: number): number => {
        const diff = (endAngle - startAngle + Math.PI) % (Math.PI * 2) - Math.PI;
        return diff < -Math.PI ? diff + Math.PI * 2 : diff;
    }

    const checkAngles = (): boolean => {

        const incr = 3;
        const yDiff = getAngleDifference(anglesCurrRef.current.y, anglesDestRef.current.y);
        const rDiff = getAngleDifference(anglesCurrRef.current.r, anglesDestRef.current.r);
        const pDiff = getAngleDifference(anglesCurrRef.current.p, anglesDestRef.current.p);
        // console.log(anglesDest.y * RAD_TO_GRAD, anglesCurr.y * RAD_TO_GRAD, yDiff * RAD_TO_GRAD);
        if (yDiff != 0) {
            if (Math.abs(yDiff) > incr * GRAD_TO_RAD) {
                anglesCurrRef.current.y += (incr * GRAD_TO_RAD) * Math.sign(yDiff);
            } else {
                anglesCurrRef.current.y = anglesDestRef.current.y;
            }
        }
        if (rDiff != 0) {
            if (Math.abs(rDiff) > incr * GRAD_TO_RAD) {
                anglesCurrRef.current.r += (incr * GRAD_TO_RAD) * Math.sign(rDiff);
            } else {
                anglesCurrRef.current.r = anglesDestRef.current.r;
            }
        }
        if (pDiff != 0) {
            if (Math.abs(pDiff) > incr * GRAD_TO_RAD) {
                anglesCurrRef.current.p += (incr * GRAD_TO_RAD) * Math.sign(pDiff);
            } else {
                anglesCurrRef.current.p = anglesDestRef.current.p;
            }
        }
        return yDiff != 0 || rDiff != 0 || pDiff != 0;

    }

    useFrame(() => { // { gl, scene, camera }

        const tsAnimN = Date.now();
        if (tsAnimN < tsAnimDest.current) {

            const fraction = (tsAnimN - tsAnimOrig.current) / (tsAnimDest.current - tsAnimOrig.current);
            // console.log('fraction', fraction);
            handleCurrRef.current = handleOrigRef.current.clone().slerp(handleDestRef.current, easeOut(fraction));
            handleHandle();

            invalidate();

        } else if (tsAnimDest.current > 0) {

            // console.log('fraction', 1);
            handleCurrRef.current = handleDestRef.current
            handleOrigRef.current = handleDestRef.current;
            handleHandle();

            tsAnimOrig.current = -1;
            tsAnimDest.current = -1;

        }

        if (checkAngles()) {

            console.log('useFrame (gimbal)');

            const leverPoints0: Vector3[] = [];
            const leverPoints1: Vector3[] = [];
            const leverPoints2: Vector3[] = [];

            const referenceFrameY = createReferenceFrameY();
            referenceFrameY.drawAngleZ(normalizeAngle(anglesCurrRef.current.y), leverPoints0);

            const referenceFrameR = createReferenceFrameR(referenceFrameY, normalizeAngle(anglesCurrRef.current.y), leverPoints0);
            referenceFrameR.drawAngleZ(normalizeAngle(anglesCurrRef.current.r), leverPoints1);

            const referenceFrameP = createReferenceFrameP(referenceFrameR, normalizeAngle(anglesCurrRef.current.r), leverPoints1);
            referenceFrameP.drawAngleZ(normalizeAngle(anglesCurrRef.current.p), leverPoints2);

            createLever2(referenceFrameP, normalizeAngle(anglesCurrRef.current.p), leverPoints2);

            const _leverPos0: number[] = [];
            leverPoints0.forEach(p => _leverPos0.push(p.x, p.y, p.z));
            for (let i = 0; i < leverPoints0.length; i++) {
                leverPos0Ref.current[i * 3 + 0] = leverPoints0[i].x;
                leverPos0Ref.current[i * 3 + 1] = leverPoints0[i].y;
                leverPos0Ref.current[i * 3 + 2] = leverPoints0[i].z;
            }
            for (let i = leverPoints0.length * 3; i < leverPos0Ref.current.length; i++) {
                leverPos0Ref.current[i] = 0;
            }
            leverGeom0Ref.current.attributes.position.needsUpdate = true;

            const _leverPos1: number[] = [];
            leverPoints1.forEach(p => _leverPos1.push(p.x, p.y, p.z));
            for (let i = 0; i < leverPoints1.length; i++) {
                leverPos1Ref.current[i * 3 + 0] = leverPoints1[i].x;
                leverPos1Ref.current[i * 3 + 1] = leverPoints1[i].y;
                leverPos1Ref.current[i * 3 + 2] = leverPoints1[i].z;
            }
            for (let i = leverPoints1.length * 3; i < leverPos1Ref.current.length; i++) {
                leverPos1Ref.current[i] = 0;
            }
            leverGeom1Ref.current.attributes.position.needsUpdate = true;

            const _leverPos2: number[] = [];
            leverPoints2.forEach(p => _leverPos2.push(p.x, p.y, p.z));
            for (let i = 0; i < leverPoints2.length; i++) {
                leverPos2Ref.current[i * 3 + 0] = leverPoints2[i].x;
                leverPos2Ref.current[i * 3 + 1] = leverPoints2[i].y;
                leverPos2Ref.current[i * 3 + 2] = leverPoints2[i].z;
            }
            for (let i = leverPoints2.length * 3; i < leverPos2Ref.current.length; i++) {
                leverPos2Ref.current[i] = 0;
            }
            leverGeom2Ref.current.attributes.position.needsUpdate = true;

            invalidate();

        }


    }, 4);

    return (

        <group>
            <lineSegments>
                <bufferGeometry ref={leverGeom0Ref}>
                    <bufferAttribute
                        attach={'attributes-position'}
                        count={leverPos0Ref.current.length / 3}
                        array={leverPos0Ref.current}
                        args={[leverPos0Ref.current, 3]}
                        itemSize={3}
                    />
                </bufferGeometry>
                <lineBasicMaterial
                    color={lever0.color}
                    transparent={true}
                    opacity={1}
                    depthTest={true}
                    depthWrite={false}
                />
            </lineSegments>
            <lineSegments>
                <bufferGeometry ref={leverGeom1Ref}>
                    <bufferAttribute
                        attach={'attributes-position'}
                        count={leverPos1Ref.current.length / 3}
                        array={leverPos1Ref.current}
                        args={[leverPos1Ref.current, 3]}
                        itemSize={3}
                    />
                </bufferGeometry>
                <lineBasicMaterial
                    color={lever1.color}
                    transparent={true}
                    opacity={1}
                    depthTest={true}
                    depthWrite={false}
                />
            </lineSegments>
            <lineSegments>
                <bufferGeometry ref={leverGeom2Ref}>
                    <bufferAttribute
                        attach={'attributes-position'}
                        count={leverPos2Ref.current.length / 3}
                        array={leverPos2Ref.current}
                        args={[leverPos2Ref.current, 3]}
                        itemSize={3}
                    />
                </bufferGeometry>
                <lineBasicMaterial
                    color={lever2.color}
                    transparent={true}
                    opacity={1}
                    depthTest={true}
                    depthWrite={false}
                />
            </lineSegments>
            <group
                ref={planeRefCV}
                renderOrder={1}
            >
                <mesh>
                    <meshPhysicalMaterial
                        transparent={true}
                        color={0xcccc00}
                        opacity={0.2}
                        side={DoubleSide}
                        visible={true}
                        emissive={0xcccc00}
                        emissiveIntensity={1}
                        polygonOffset={true}
                        polygonOffsetFactor={0.25}
                    />
                    <planeGeometry args={[0.5, 0.5]} />
                    {/* <circleGeometry args={[0.5, 32]} /> */}
                </mesh>
                <lineSegments>
                    <bufferGeometry>
                        <bufferAttribute attach={'attributes-position'} args={[new Float32Array([
                            -0.25, -0.25, 0,
                            0.25, -0.25, 0,
                            0.25, -0.25, 0,
                            0.25, 0.25, 0,
                            0.25, 0.25, 0,
                            -0.25, 0.25, 0,
                            -0.25, 0.25, 0,
                            -0.25, -0.25, 0,
                        ]), 3]} />
                    </bufferGeometry>
                    <lineBasicMaterial
                        transparent={true}
                        color={0xcccc00}
                        opacity={0.75}
                    />
                </lineSegments>
            </group>
            <group
                ref={coneRefYD}
                renderOrder={1}
            >
                <group
                    rotation={[-Math.PI / 2, 0, 0]}
                >
                    <mesh
                        position={[0, -0.50, 0]}
                    >
                        <meshPhysicalMaterial
                            transparent={true}
                            color={0xffff00}
                            opacity={0.2}
                            side={DoubleSide}
                            visible={false}
                            emissive={0xffff00}
                            emissiveIntensity={1}
                        />
                        <coneGeometry args={[1, 1, 32, 1, true]} />
                    </mesh>
                </group>
            </group>
            <group
                rotation={[Math.PI / 2, Math.PI, 0]}
            >
                <group
                    ref={groupRef}
                    castShadow={true}
                    receiveShadow={true}
                    scale={[-1, 1, 1]}
                >
                    <mesh
                        rotation={[0, 0, Math.PI / 2]}
                        position={[-0.5, 0, 0]}
                    >
                        <meshPhysicalMaterial
                            color={leverHColor}
                            wireframe={false}
                            roughness={0.5}
                            specularIntensity={1}
                            reflectivity={1}
                            opacity={0.5}
                            transparent={true}
                            side={DoubleSide}
                        />
                        <cylinderGeometry args={[0.05, 0.10, 1, 32, 1, false]} />
                    </mesh>
                </group>
            </group>
        </group>


    );
};

export default GimbalComponent;
