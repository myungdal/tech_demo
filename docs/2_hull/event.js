class Event {
    ['type'] = 0x0;
    ['ttl'] = 0x0;
    ['maxTtl'] = 0x0;
    ['a'] = null;
    ['b'] = null;
    ['ax'] = 0x0;
    ['ay'] = 0x0;
    ['bx'] = 0x0;
    ['by'] = 0x0;
    constructor(c, d, e, f, g, h, i, j) {
        this['type'] = c;
        this['maxTtl'] = this['ttl'] = d;
        this['a'] = e;
        this['b'] = f;
        this['ax'] = g;
        this['ay'] = h;
        this['bx'] = i;
        this['by'] = j;
    }
}