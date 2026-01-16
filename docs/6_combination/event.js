class Event {
    ['type'] = 0x0;
    ['ttl'] = 0x0;
    ['maxTtl'] = 0x0;
    ['a'] = null;
    ['b'] = null;
    ['sx'] = 0x0;
    ['sy'] = 0x0;
    ['dx'] = 0x0;
    ['dy'] = 0x0;
    constructor(c, d, e, f, g, h, i, j) {
        this['type'] = c;
        this['maxTtl'] = this['ttl'] = d;
        this['a'] = e;
        this['b'] = f;
        this['sx'] = g;
        this['sy'] = h;
        this['dx'] = i;
        this['dy'] = j;
    }
}