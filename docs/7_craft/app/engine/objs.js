class Objs {
    ['objs'] = [];
    ['objCnt'] = 0x0;
    constructor() {
    }
    ['newObj'](a) {
        if (this['objs']['length'] <= this['objCnt']) {
            var b = this['objs'];
            var c = [];
            var d = b['length'];
            var e = d < 0x2 ? 0x2 : d * 0x2;
            c['length'] = e;
            var f = 0x0;
            for (f = 0x0; f < d; ++f) {
                c[f] = this['objs'][f];
            }
            for (f = d; f < e; ++f) {
                c[f] = new a();
            }
            this['objs'] = c;
            b['length'] = 0x0;
        }
        var g = this['objCnt'];
        ++this['objCnt'];
        return g;
    }
    ['deleteObj'](a) {
        --this['objCnt'];
        var b = this['objCnt'];
        if (b === a) {
            return;
        }
        var c = this['objs'][b];
        var d = this['objs'][a];
        for (var e in c) {
            if (!c['hasOwnProperty'](e))
                continue;
            d[e] = c[e];
        }
    }
    ['deleteObjCustom'](a, b) {
        --this['objCnt'];
        var c = this['objCnt'];
        if (c === a) {
            return;
        }
        var d = this['objs'][c];
        var e = this['objs'][a];
        b(d, e);
    }
}