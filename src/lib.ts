import { Crazymem } from './crazymem';
import { ProcessName } from './types';

const Lib = (process_name: ProcessName) => new Crazymem(process_name);

export { Lib as Crazymem }
