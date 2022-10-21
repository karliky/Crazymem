import { Crazymem } from '../src/lib';
import 'should';

describe('Crazymem - Integration tests', () => {
  it('should return a Crazymem constructor', () => {
    Crazymem.should.be.a.Function();
  });
});