class BarrierSystem extends System {
    ['scale'] = 0x0;
    ['colCnt'] = 0x0;
    ['rowCnt'] = 0x0;
    ['barrier'] = null;
    ['_barrier'] = null;
    ['needUpdate'] = !![];
    ['completedUpdate'] = !![];
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](MovementComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['initGrid'](a) {
        this['scale'] = a;
        this['colCnt'] = ConstAttribute['WIDTH'] >> this['scale'];
        this['rowCnt'] = ConstAttribute['HEIGHT'] >> this['scale'];
        this['barrier'] = new Barrier();
        this['barrier']['initGrid'](this['colCnt'], this['rowCnt']);
        this['_barrier'] = new Barrier();
        this['_barrier']['initGrid'](this['colCnt'], this['rowCnt']);
    }
    ['setBarrier'](a, b, d) {
        this['needUpdate'] = !![];
        this['barrier']['setBarrier'](a, b, d);
        this['_barrier']['setBarrier'](a, b, d);
    }
    ['update'](a, b) {
        if (this['needUpdate']) {
            this['needUpdate'] = ![];
            this['completedUpdate'] = ![];
            this['_barrier']['reset']();
        }
        if (!this['completedUpdate']) {
            this['completedUpdate'] = this['_barrier']['update']();
            if (this['completedUpdate']) {
                var c = this['_barrier'];
                this['_barrier'] = this['barrier'];
                this['barrier'] = c;
            }
        }
        var d = this['getDataRowCnt'](a);
        for (var e = 0x0; e < d; ++e) {
            var f = this['getDataValue'](a, e, 0x1);
            var g = this['getDataValue'](a, e, 0x2);
            var h = this['pick'](f['x'], f['y']);
            if (!h['b'])
                continue;
            var j = this['pick'](g['px'], g['py']);
            if (!j['b']) {
                f['x'] = g['px'];
                f['y'] = g['py'];
                if (!j['detourDir7'] && !j['detourDir9'] && !j['detourDir1'] && !j['detourDir3'])
                    continue;
                let A = this['getDetourDir'](j, g['vx'], g['vy']);
                if (A === j['c4']) {
                    g['vx'] = -g['speed'] * ConstAttribute['BARRIER_NAVIGATION_POWER'];
                    g['vy'] = 0x0;
                } else if (A === j['c6']) {
                    g['vx'] = g['speed'] * ConstAttribute['BARRIER_NAVIGATION_POWER'];
                    g['vy'] = 0x0;
                } else if (A === j['c8']) {
                    g['vx'] = 0x0;
                    g['vy'] = -g['speed'] * ConstAttribute['BARRIER_NAVIGATION_POWER'];
                } else if (A === j['c2']) {
                    g['vx'] = 0x0;
                    g['vy'] = g['speed'] * ConstAttribute['BARRIER_NAVIGATION_POWER'];
                } else
                    continue;
                f['x'] += g['vx'];
                f['y'] += g['vy'];
                continue;
            }
            j = h;
            while (j['nextCell'] && j !== j['nextCell']) {
                j = j['nextCell'];
            }
            var k = (j['c'] << this['scale']) + ConstAttribute['BARRIER_REPOSITION_MARGIN'];
            var l = (j['r'] << this['scale']) + ConstAttribute['BARRIER_REPOSITION_MARGIN'];
            var m = (j['c'] << this['scale']) + (0x1 << this['scale']) - ConstAttribute['BARRIER_REPOSITION_MARGIN'];
            var q = (j['r'] << this['scale']) + (0x1 << this['scale']) - ConstAttribute['BARRIER_REPOSITION_MARGIN'];
            var r = this['getSqure'](f['x'], f['y'], k, l);
            var s = this['getSqure'](f['x'], f['y'], m, l);
            var t = this['getSqure'](f['x'], f['y'], k, q);
            var u = this['getSqure'](f['x'], f['y'], m, q);
            var v = k;
            var w = l;
            var z = r;
            if (z > s) {
                v = m;
                w = l;
                z = s;
            }
            if (z > t) {
                v = k;
                w = q;
                z = t;
            }
            if (z > u) {
                v = m;
                w = q;
            }
            g['px'] = f['x'] = v;
            g['py'] = f['y'] = w;
        }
    }
    ['getDetourDir'](a, b, c) {
        if (c < 0x0) {
            if (b < 0x0)
                return a['detourDir7'];
            else if (b > 0x0)
                return a['detourDir9'];
        } else if (c > 0x0) {
            if (b < 0x0)
                return a['detourDir1'];
            else if (b > 0x0)
                return a['detourDir3'];
        }
    }
    ['getSqure'](a, b, c, d) {
        var e = a - c;
        var f = b - d;
        return e * e + f * f;
    }
    ['pick'](a, b) {
        var d = Math['floor'](a) >> this['scale'];
        var e = Math['floor'](b) >> this['scale'];
        return this['getCell'](d, e);
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
        return this['barrier']['cells'][a][b];
    }
}