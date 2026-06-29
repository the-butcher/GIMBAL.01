import { ArrowHelper, Vector3 } from "three";
import { IReferenceFrame } from "./IReferenceFrame";

/**
 * D :: direction
 * U :: up
 */
export interface IRerenceFrameParamsDU {
    id: string;
    origin: Vector3;
    vectorD: Vector3;
    vectorU: Vector3;
}

export class ReferenceFrame implements IReferenceFrame {

    private id: string;
    private origin: Vector3;
    private vectorX: Vector3;
    private vectorY: Vector3;
    private vectorZ: Vector3;

    constructor(paramsDU: IRerenceFrameParamsDU) {
        this.id = paramsDU.id;
        this.origin = paramsDU.origin;
        this.vectorZ = paramsDU.vectorD.clone().normalize();
        this.vectorY = paramsDU.vectorU.clone().normalize();
        // const control = this.vectorZ.angleTo(this.vectorY);
        // console.log(`control (${this.id})`, control * 180 / Math.PI);
        this.vectorX = this.vectorY.clone().cross(this.vectorZ.clone());
    }

    drawAngleZ(angle: number, debugPoints: Vector3[]): void {

        let radius = 0.12;
        const numPoints = Math.round(Math.abs((angle * 180 / Math.PI) / 1));

        const angleStep = angle / numPoints;
        // console.log(`num-points (${this.id})`, numPoints)

        const getPointAtTheta = (theta: number) => {
            const direction = this.getVectorY().clone().multiplyScalar(Math.cos(theta)).add(this.getVectorX().clone().multiplyScalar(Math.sin(theta)));
            return this.origin.clone().add(direction.multiplyScalar(radius));
        }

        for (let i = 1; i <= numPoints; i++) {
            const thetaA = angleStep * (i - 1);
            const thetaB = angleStep * (i);
            debugPoints.push(getPointAtTheta(thetaA));
            debugPoints.push(getPointAtTheta(thetaB));
            if (i == numPoints) {
                radius = 0.11;
                debugPoints.push(getPointAtTheta(thetaB));
                radius = 0.14;
                debugPoints.push(getPointAtTheta(thetaB));
            }
        }

    }

    getOrigin(): Vector3 {
        return this.origin.clone();
    }


    getVectorX(): Vector3 {
        return this.vectorX.clone();
    }

    getVectorY(): Vector3 {
        return this.vectorY.clone();
    }

    getVectorZ(): Vector3 {
        return this.vectorZ.clone();
    }

    getAngleZ(vector: Vector3, debugPoints: Vector3[]): number {

        const vectorP = vector.clone().normalize().projectOnPlane(this.getVectorZ());

        const scalarRU = this.getVectorY().dot(vectorP.clone());
        const scalarRR = this.getVectorX().dot(vectorP.clone());

        const scale = 0.00;
        const markl = 0.03;

        debugPoints.push(this.origin.clone());
        debugPoints.push(this.origin.clone().add(vectorP.clone().multiplyScalar(scale)));

        debugPoints.push(this.origin.clone().add(vectorP.clone().multiplyScalar(scale)));
        debugPoints.push(this.origin.clone().add(vectorP.clone().multiplyScalar(scale)).add(this.getVectorX().multiplyScalar(-markl * Math.sign(scalarRR) * scale)));
        debugPoints.push(this.origin.clone().add(vectorP.clone().multiplyScalar(scale)));
        debugPoints.push(this.origin.clone().add(vectorP.clone().multiplyScalar(scale)).add(this.getVectorY().multiplyScalar(-markl * Math.sign(scalarRU) * scale)));

        debugPoints.push(this.origin.clone());
        debugPoints.push(this.origin.clone().add(this.getVectorY().multiplyScalar(scalarRU * scale)));
        debugPoints.push(this.origin.clone().add(this.getVectorY().multiplyScalar(scalarRU * scale)));
        debugPoints.push(this.origin.clone().add(this.getVectorY().multiplyScalar(scalarRU * scale).add(this.getVectorX().multiplyScalar(markl * Math.sign(scalarRR) * scale))));

        debugPoints.push(this.origin.clone());
        debugPoints.push(this.origin.clone().add(this.getVectorX().multiplyScalar(scalarRR * scale)));
        debugPoints.push(this.origin.clone().add(this.getVectorX().multiplyScalar(scalarRR * scale)));
        debugPoints.push(this.origin.clone().add(this.getVectorX().multiplyScalar(scalarRR * scale)).add(this.getVectorY().multiplyScalar(markl * Math.sign(scalarRU) * scale)));

        return Math.atan2(scalarRR, scalarRU);

    }

    applyToArrowHelperX(arrowHelper: ArrowHelper): void {
        arrowHelper.position.copy(this.origin);
        arrowHelper.setDirection(this.vectorX.clone().normalize());
    }

    applyToArrowHelperY(arrowHelper: ArrowHelper): void {
        arrowHelper.position.copy(this.origin);
        arrowHelper.setDirection(this.vectorY.clone().normalize());
    }

    applyToArrowHelperZ(arrowHelper: ArrowHelper): void {
        arrowHelper.position.copy(this.origin);
        arrowHelper.setDirection(this.vectorZ.clone().normalize());
    }

}