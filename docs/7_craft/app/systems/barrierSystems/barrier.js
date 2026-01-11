class BarrierCell {
    ['b'] = ![];
    ['c'] = 0x0;
    ['r'] = 0x0;
    ['c8'] = null;
    ['c4'] = null;
    ['c6'] = null;
    ['c2'] = null;
    ['nextCell'] = null;
    ['working'] = ![];
    ['cost'] = 0x0;
    ['needDetour0'] = ![];
    ['needDetour1'] = ![];
    ['detourDir7'] = null;
    ['detourDir9'] = null;
    ['detourDir1'] = null;
    ['detourDir3'] = null;
}
class Barrier {
    ['colCnt'] = 0x0;
    ['rowCnt'] = 0x0;
    ['cells'] = null;
    ['currentCol'] = 0x0;
    ['initGrid'](a, b) {
        this['colCnt'] = a;
        this['rowCnt'] = b;
        this['cells'] = new Array(this['colCnt']);
        var d = 0x0;
        var e = 0x0;
        for (d = 0x0; d < this['colCnt']; ++d) {
            this['cells'][d] = new Array(this['rowCnt']);
            for (e = 0x0; e < this['rowCnt']; ++e)
                this['cells'][d][e] = new BarrierCell();
        }
        for (d = 0x0; d < this['colCnt']; ++d) {
            for (e = 0x0; e < this['rowCnt']; ++e) {
                var f = this['cells'][d][e];
                f['c'] = d;
                f['r'] = e;
                f['c8'] = this['getCell'](d, e - 0x1);
                f['c4'] = this['getCell'](d - 0x1, e);
                f['c6'] = this['getCell'](d + 0x1, e);
                f['c2'] = this['getCell'](d, e + 0x1);
            }
        }
    }
    ['setBarrier'](a, b, d) {
        var e = this['getCell'](a, b);
        e['b'] = !d ? ![] : !![];
    }
    ['getCell'](a, b) {
        if (a < 0x0)
            a = 0x0;
        else if (a >= this['colCnt'])
            a = this['colCnt'] - 0x1;
        if (b < 0x0)
            b = 0x0;
        else if (b >= this['rowCnt'])
            b = this['rowCnt'] - 0x1;
        return this['cells'][a][b];
    }
    ['reset']() {
        this['currentCol'] = 0x0;
        var a = 0x0;
        var b = 0x0;
        var d = null;
        for (a = 0x0; a < this['colCnt']; ++a) {
            for (b = 0x0; b < this['rowCnt']; ++b) {
                d = this['cells'][a][b];
                d['nextCell'] = null;
                d['working'] = ![];
                d['cost'] = 0x0;
                if (d['c8'] === d || d['c4'] === d || d['c6'] === d || d['c2'] === d) {
                    d['b'] = !![];
                }
                d['needDetuor0'] = ![];
                d['needDetuor1'] = ![];
                d['detourDir7'] = null;
                d['detourDir9'] = null;
                d['detourDir1'] = null;
                d['detourDir3'] = null;
            }
        }
        for (a = 0x0; a < this['colCnt']; ++a) {
            for (b = 0x0; b < this['rowCnt']; ++b) {
                d = this['cells'][a][b];
                if (!d['b'])
                    continue;
                if (d['nextCell'])
                    continue;
                d['working'] = !![];
                this['processNextCell'](d);
                d['working'] = ![];
            }
        }
        for (a = 0x0; a < this['colCnt']; ++a) {
            for (b = 0x0; b < this['rowCnt']; ++b) {
                d = this['cells'][a][b];
                if (d['b'])
                    continue;
                if (!d['c8']['b'] && !d['c4']['b'] && !d['c6']['b'] && !d['c2']['b'])
                    continue;
                d['needDetuor0'] = !![];
            }
        }
        for (a = 0x0; a < this['colCnt']; ++a) {
            for (b = 0x0; b < this['rowCnt']; ++b) {
                d = this['cells'][a][b];
                if (d['b'])
                    continue;
                var e = 0x0;
                if (d['c8']['needDetuor0'])
                    ++e;
                if (d['c4']['needDetuor0'])
                    ++e;
                if (d['c6']['needDetuor0'])
                    ++e;
                if (d['c2']['needDetuor0'])
                    ++e;
                if (e < 0x2)
                    continue;
                d['needDetuor1'] = !![];
            }
        }
    }
    ['update']() {
        var a = 0x0;
        var b = 0x0;
        var d = null;
        for (a = this['currentCol']; a < this['currentCol'] + ConstAttribute['BARRIER_UPDATE_UNIT']; ++a) {
            for (b = 0x0; b < this['rowCnt']; ++b) {
                d = this['cells'][a][b];
                if (!d['needDetuor0'] && !d['needDetuor1'])
                    continue;
                d['detourDir7'] = this['processDetourDir'](d, function (e, f) {
                    return e < a && f < b ? !![] : ![];
                });
                d['detourDir9'] = this['processDetourDir'](d, function (e, f) {
                    return e > a && f < b ? !![] : ![];
                });
                d['detourDir1'] = this['processDetourDir'](d, function (e, f) {
                    return e < a && f > b ? !![] : ![];
                });
                d['detourDir3'] = this['processDetourDir'](d, function (e, f) {
                    return e > a && f > b ? !![] : ![];
                });
            }
        }
        this['currentCol'] += ConstAttribute['BARRIER_UPDATE_UNIT'];
        return this['currentCol'] === this['colCnt'] ? !![] : ![];
    }
    ['processDetourDir'](a, b) {
        var c = [];
        var d = [];
        if (!a['c8']['b'])
            c['push']([
                a['c8'],
                a['c8']
            ]);
        if (!a['c4']['b'])
            c['push']([
                a['c4'],
                a['c4']
            ]);
        if (!a['c6']['b'])
            c['push']([
                a['c6'],
                a['c6']
            ]);
        if (!a['c2']['b'])
            c['push']([
                a['c2'],
                a['c2']
            ]);
        var e = 0x0;
        while (c['length'] > 0x0) {
            var f = c['shift']();
            var g = f[0x0];
            var h = f[0x1];
            if (++e > 0x3e8)
                return null;
            if (b(g['c'], g['r'])) {
                return h;
            }
            d['push'](g);
            if (!g['c8']['b'] && !d['includes'](g['c8']))
                c['push']([
                    g['c8'],
                    h
                ]);
            if (!g['c4']['b'] && !d['includes'](g['c4']))
                c['push']([
                    g['c4'],
                    h
                ]);
            if (!g['c6']['b'] && !d['includes'](g['c6']))
                c['push']([
                    g['c6'],
                    h
                ]);
            if (!g['c2']['b'] && !d['includes'](g['c2']))
                c['push']([
                    g['c2'],
                    h
                ]);
        }
        return null;
    }
    ['processNextCell'](a) {
        this['subProcessNextCell'](a, a['c8']);
        this['subProcessNextCell'](a, a['c4']);
        this['subProcessNextCell'](a, a['c6']);
        this['subProcessNextCell'](a, a['c2']);
    }
    ['subProcessNextCell'](a, b) {
        if (!b['b']) {
            a['nextCell'] = b;
            a['cost'] = 0x1;
            return;
        }
        if (b['working'])
            return;
        if (b === a)
            return;
        if (!b['nextCell']) {
            b['working'] = !![];
            this['processNextCell'](b);
            b['working'] = ![];
        }
        if (a['cost'] === 0x0 || b['cost'] !== 0x0 && b['cost'] < a['cost']) {
            a['nextCell'] = b;
            a['cost'] = b['cost'] + 0x1;
        }
        if (b['cost'] === 0x0 || a['cost'] !== 0x0 && a['cost'] < b['cost']) {
            b['nextCell'] = a;
            b['cost'] = a['cost'] + 0x1;
        }
    }
}