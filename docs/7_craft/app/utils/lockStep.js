class LockStep {
    ['startingTime'] = 0x0;
    ['updateDelta'] = 0x0;
    ['updateDeltaLimit'] = 0x0;
    ['stepCnt'] = 0x0;
    constructor(a, b, c) {
        this['startingTime'] = a;
        this['updateDelta'] = b;
        this['updateDeltaLimit'] = c;
    }
    ['getStepCntForNow']() {
        var a = Date['now']() - this['startingTime'];
        if (a < 0x0)
            return 0x0;
        a -= this['stepCnt'] * this['updateDelta'];
        if (a < 0x0)
            return 0x0;
        if (a > this['updateDeltaLimit'])
            return -0x1;
        var b = Math['floor'](a / this['updateDelta']);
        this['stepCnt'] += b;
        return b;
    }
}