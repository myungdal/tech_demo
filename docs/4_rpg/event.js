class Event {
    ['type'] = 0x0;
    ['ttl'] = 0x0;
    ['maxTtl'] = 0x0;
    ['sx'] = 0x0;
    ['sy'] = 0x0;
    ['dx'] = 0x0;
    ['dy'] = 0x0;
    constructor(a, b, c, d, e, f) {
        this['type'] = a;
        this['maxTtl'] = this['ttl'] = b;
        this['sx'] = c;
        this['sy'] = d;
        this['dx'] = e;
        this['dy'] = f;
    }
}