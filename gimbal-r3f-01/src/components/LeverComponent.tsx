import { createRef, useEffect, useState } from 'react';
import { Group } from 'three';

import { ILeverProps } from '../types/ILeverProps';


const LeverComponent = (props: ILeverProps) => {

    const { name, points, matrix } = { ...props };
    const groupRef = createRef<Group>();
    // const { scene } = useThree();

    const [cylinders, setCylinders] = useState<JSX.Element[]>([]);

    useEffect(() => {

        console.debug('✨ building lever component', props);

        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, []);

    useEffect(() => {

        console.log('⚙ updating lever component (points)', points);

        const groupMatrix = groupRef.current?.matrixWorld;
        console.log(name, 'inserted at: ', groupMatrix)

        const _cylinders: JSX.Element[] = [];
        for (let i = 1; i < points.length; i++) {

            const pointA = points[i - 1];
            const pointB = points[i];

            const diffAB = pointB.clone().sub(pointA.clone());
            const length = diffAB.length();
            const angle = Math.atan2(diffAB.y, diffAB.x);

            const mesh: JSX.Element = <group
                rotation={[Math.PI / 2, 0, 0]}
            >
                <group
                    position={[-pointA.y, pointA.x, 0]}
                    rotation={[0, 0, angle]}
                >
                    <group
                        position={[0, length / 2, 0]} // move to starting point of cylinder
                    >
                        <mesh>
                            <cylinderGeometry args={[0.01, 0.01, length, 32, 1, true]} />
                            <meshPhysicalMaterial
                                color={0xffff00}
                                wireframe={false}
                                roughness={0.5}
                                specularIntensity={1}
                                reflectivity={1}
                                opacity={0.75}
                                transparent={true}
                            />
                        </mesh>
                    </group>
                </group>
            </group>

            _cylinders.push(mesh);

        }

        setCylinders(_cylinders);

        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, [points]);

    return (
        <group
            ref={groupRef}
            castShadow={true}
            receiveShadow={true}
            matrix={matrix}
        >
            {cylinders}
        </group>
    );
};

export default LeverComponent;
