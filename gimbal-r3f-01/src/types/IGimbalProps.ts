import { Quaternion } from "three";
import { ILeverProps } from "./ILeverProps";


export interface IGimbalProps {
  handle: Quaternion;
  lever0: ILeverProps; // first lever from handle to roll axis
  lever1: ILeverProps; // second lever roll axis to pitch axis
  lever2: ILeverProps; // third lever pitch axis to camera
}
