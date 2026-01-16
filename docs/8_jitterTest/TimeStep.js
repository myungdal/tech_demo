class TimeStepGenerator {
    ['mInterval'] = 0x0;
    ['mStartTime'] = 0x0;
    ['mPastStepCount'] = 0x0;
    constructor(a) {
        this['mInterval'] = a;
        this['mStartTime'] = Date['now']();
    }
    ['Generate']() {
        var a = Date['now']();
        if (a <= this['mStartTime'])
            return 0x0;
        var b = this['mStartTime'] + this['mPastStepCount'] * this['mInterval'];
        var c = a - b;
        if (c <= 0x0)
            return 0x0;
        var d = Math['floor'](c / this['mInterval']);
        this['mPastStepCount'] += d;
        return d;
    }
}
class TimeStepQueue {
    ['mInterval'] = 0x0;
    ['mStartTime'] = 0x0;
    ['mQueuedStepCount'] = 0x0;
    ['mPastStepCount'] = 0x0;
    constructor(a) {
        this['mInterval'] = a;
        this['mStartTime'] = Date['now']();
    }
    ['PushStep'](a) {
        this['mQueuedStepCount'] += a;
    }
    ['PopStep'](a) {
        var b = Date['now']();
        if (b <= this['startTime'])
            return 0x0;
        var c = this['mStartTime'] + this['mPastStepCount'] * this['mInterval'];
        var d = b - c;
        var e = Math['floor'](d / this['mInterval']);
        this['mPastStepCount'] += e;
        var f = d * a;
        var g = Math['floor'](f / this['mInterval']);
        if (g > this['mQueuedStepCount'])
            g = this['mQueuedStepCount'];
        this['mQueuedStepCount'] -= g;
        return g;
    }
    ['GetQueuedStepCount']() {
        return this['mQueuedStepCount'];
    }
}
class TimeStepTuner {
    static ['JITTER_COEFFICIENT'] = 0x3;
    static ['TIME_COEFFICIENT'] = 0x1 / 0x12c;
    static ['CATCH_UP_THRESHOLD'] = 0x12c;
    static ['MAX_SPEED'] = 1.5;
    static ['SPEED_COEFFICIENT'] = 0.1;
    ['mSpeed'] = 0x1;
    ['GetSpeed']() {
        return this['mSpeed'];
    }
    ['TuneSpeed'](a, b) {
        var c = b - a * TimeStepTuner['JITTER_COEFFICIENT'];
        var d = c * TimeStepTuner['TIME_COEFFICIENT'];
        var e = 0x1 + this['DoubleSigmoid'](d) * TimeStepTuner['SPEED_COEFFICIENT'];
        if (c > TimeStepTuner['CATCH_UP_THRESHOLD']) {
            var f = (c - TimeStepTuner['CATCH_UP_THRESHOLD']) / TimeStepTuner['CATCH_UP_THRESHOLD'] * TimeStepTuner['SPEED_COEFFICIENT'];
            e += f;
        }
        e = Math['min'](e, TimeStepTuner['MAX_SPEED']);
        this['mSpeed'] = this['mSpeed'] * 0.9 + e * 0.1;
    }
    ['DoubleSigmoid'](a) {
        a *= 0x5;
        var b = 0x1 / (0x1 + Math['exp'](-a));
        if (0x0 > a) {
            return (b * (0x1 - b) - 0.25) * 0x4;
        }
        return -(b * (0x1 - b) - 0.25) * 0x4;
    }
}
class TimeStepPlayer {
    ['mQueue'] = null;
    ['mTuner'] = null;
    ['mInterval'] = 0x0;
    ['mJitterTime'] = 0x0;
    ['mQueuedTime'] = 0x0;
    constructor(a) {
        this['mQueue'] = new TimeStepQueue(a);
        this['mTuner'] = new TimeStepTuner();
        this['mInterval'] = a;
    }
    ['PushStep'](a) {
        if (0x0 === a)
            return;
        this['mJitterTime'] = this['mJitterTime'] * 0.9 + a * this['mInterval'] * 0.1;
        this['mQueue']['PushStep'](a);
    }
    ['PlayStep']() {
        var a = this['mTuner']['GetSpeed']();
        var b = this['mQueue']['PopStep'](a);
        if (0x0 === b)
            return 0x0;
        var c = this['mQueue']['GetQueuedStepCount']();
        var d = b;
        while (0x0 < d--) {
            this['mQueuedTime'] = this['mQueuedTime'] * 0.9 + c * this['mInterval'] * 0.1;
            this['mTuner']['TuneSpeed'](this['mJitterTime'], this['mQueuedTime']);
        }
        return b;
    }
}