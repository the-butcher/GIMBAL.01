import CcApi from './CcApi';
import type { ICcApiProps } from './ICcApiProps';
import { ObjectUtil } from './utils/ObjectUtil';


// apache reverse proxy must be active

// const ccApiBase = 'http://192.168.1.2:8080/ccapi/ver100';
const ccApiBase = 'http://localhost:8081/ccapi/ver100';
const ccApiMode: ICcApiProps = {
  uuid: ObjectUtil.createId(),
  title: 'mode-dial',
  ccApi: `${ccApiBase}/shooting/settings/shootingmodedial`,
  methods: [
    {
      method: 'GET',
      allow: true
    },
    {
      method: 'POST',
      allow: false
    }
  ]
};
const ccApiShut: ICcApiProps = {
  uuid: ObjectUtil.createId(),
  title: 'shutter',
  ccApi: `${ccApiBase}/shooting/control/shutterbutton`,
  methods: [
    {
      method: 'GET',
      allow: false
    },
    {
      method: 'POST',
      allow: true,
      body: {
        af: true
      }
    }
  ]
};

function App() {

  return (
    <div>
      <CcApi {...ccApiMode}></CcApi>
      <CcApi {...ccApiShut}></CcApi>
    </div>
  )

}

export default App
