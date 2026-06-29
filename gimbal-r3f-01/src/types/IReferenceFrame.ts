import { ArrowHelper, Vector3 } from "three";

/**
 * https://discoverthreejs.com/book/first-steps/transformations/
 */
export interface IReferenceFrame {

    getOrigin(): Vector3;

    /**
     * get the normalized X-value of this frame, can also be thought of as "LEFT" when looking from origin
     */
    getVectorX(): Vector3;

    /**
     * get the normalized Y-value of this frame, can also be thought of as "UP" when looking from origin
     */
    getVectorY(): Vector3;

    /**
     * get the normalized Z-value of this frame, can also be thought of as "FORWARD" when looking from origin
     */
    getVectorZ(): Vector3;

    /**
     * project the vector into the plane normal to the z-vector, then calculate the angle of the projected vector with respect to x and y
     * @param vector
     */
    getAngleZ(vector: Vector3, debugPoints: Vector3[]): number;

    /**
     * apply this instance origin and x-vector to the given ArrowHelper
     * @param arrowHelper
     */
    applyToArrowHelperX(arrowHelper: ArrowHelper): void;

    /**
     * apply this instance origin and y-vector to the given ArrowHelper
     * @param arrowHelper
     */
    applyToArrowHelperY(arrowHelper: ArrowHelper): void;

    /**
     * apply this instance origin and z-vector to the given ArrowHelper
     * @param arrowHelper
     */
    applyToArrowHelperZ(arrowHelper: ArrowHelper): void;

    /**
     * adds a set of points in the plane normal to the z-vector to show an arc around origin at 1 degree steps
     * @param angle
     * @param debugPoints
     */
    drawAngleZ(angle: number, debugPoints: Vector3[]): void;

}