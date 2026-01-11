class FastId {
    ['idxes'] = [];
    ['freeIdxes'] = [];
    constructor() {
    }
    ['setIdx'](a, b) {
        this['idxes'][a] = b;
    }
    ['pop']() {
        this['alloc']();
        var a = this['freeIdxes']['shift']();
        return a;
    }
    ['push'](a) {
        this['alloc']();
        this['idxes'][a] = -0x1;
        this['freeIdxes']['push'](a);
    }
    ['alloc']() {
        var a = this['idxes']['length'];
        var b = this['freeIdxes']['length'];
        if (b < 0x400) {
            var c = this['idxes']['slice']();
            c['length'] = a + 0x800 - b;
            var d = 0x0;
            for (d = a; d < a + 0x800 - b; ++d)
                c[d] = -0x1;
            for (d = a; d < a + 0x800 - b; ++d)
                this['freeIdxes']['push'](d);
            delete this['idxes'];
            this['idxes'] = c;
        }
    }
}