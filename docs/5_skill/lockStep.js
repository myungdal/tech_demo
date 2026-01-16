class LockStep {
    ['startingTime'] = 0x0;
    ['stepCnt'] = 0x0;
    constructor(a) {
        this['startingTime'] = a;
    }
    ['getStepCntForNow']() {
        var a = Date['now']() - this['startingTime'];
        if (a < 0x0)
            return 0x0;
        a -= this['stepCnt'] * LOCKSTEP_DELTA;
        if (a < 0x0)
            return 0x0;
        if (a > LOCKSTEP_MAX_DELTA)
            return -0x1;
        var b = Math['floor'](a / LOCKSTEP_DELTA);
        this['stepCnt'] += b;
        return b;
    }
}