import { Crazymem } from './crazymem';
import { PROCESS_NAME } from './types';

const Lib = (process_name: PROCESS_NAME) => new Crazymem(process_name);

export { Lib as Crazymem }
