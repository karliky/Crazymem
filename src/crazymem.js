"use strict";
exports.__esModule = true;
exports.Crazymem = void 0;
var crazy = require("./../build/Release/binding.node").process;
var Crazymem = /** @class */ (function () {
    function Crazymem(process_name) {
        this.pid = crazy.findByProcessName(process_name);
    }
    Crazymem.prototype.GetProcessInfo = function () {
        return crazy.getInfo(this.pid);
    };
    return Crazymem;
}());
exports.Crazymem = Crazymem;
