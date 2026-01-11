class FlockSystem extends System {
    ['odd'] = 0x0;
    ['indices'] = [];
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](FlockComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        ++this['odd'];
        if (this['odd'] === 0x4)
            this['odd'] = 0x0;
        var c = this['getDataRowCnt'](a);
        var d = 0x0;
        var e = 0x0;
        this['indices']['length'] = c;
        for (d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x3);
            f['dd0'] = Number['MAX_VALUE'];
            f['e0'] = null;
            f['c0'] = null;
            f['p0'] = null;
            f['k0'] = null;
            f['dd1'] = Number['MAX_VALUE'];
            f['e1'] = null;
            f['c1'] = null;
            f['p1'] = null;
            f['k1'] = null;
            f['dd2'] = Number['MAX_VALUE'];
            f['e2'] = null;
            f['c2'] = null;
            f['p2'] = null;
            f['k2'] = null;
            f['idx'] = -0x1;
        }
        for (d = 0x0; d < c; ++d)
            this['indices'][d] = d;
        if (this['odd'] === 0x0) {
            this['indices']['sort'](function (r, s) {
                var t = this['getDataValue'](a, r, 0x2);
                var u = this['getDataValue'](a, s, 0x2);
                var v = t['x'] - u['x'];
                return v !== 0x0 ? v : r - s;
            }['bind'](this));
        } else if (this['odd'] === 0x1) {
            this['indices']['sort'](function (r, s) {
                var t = this['getDataValue'](a, s, 0x2);
                var u = this['getDataValue'](a, r, 0x2);
                var v = t['x'] - u['x'];
                return v !== 0x0 ? v : s - r;
            }['bind'](this));
        } else if (this['odd'] === 0x2) {
            this['indices']['sort'](function (r, s) {
                var t = this['getDataValue'](a, r, 0x2);
                var u = this['getDataValue'](a, s, 0x2);
                var v = t['y'] - u['y'];
                return v !== 0x0 ? v : r - s;
            }['bind'](this));
        } else if (this['odd'] === 0x3) {
            this['indices']['sort'](function (r, s) {
                var t = this['getDataValue'](a, s, 0x2);
                var u = this['getDataValue'](a, r, 0x2);
                var v = t['y'] - u['y'];
                return v !== 0x0 ? v : s - r;
            }['bind'](this));
        }
        var g = null;
        var h = null;
        if (this['odd'] === 0x0 || this['odd'] === 0x1) {
            g = this['getSquareX'];
            h = this['getSquareY'];
        } else {
            g = this['getSquareY'];
            h = this['getSquareX'];
        }
        for (d = 0x0; d < c; ++d) {
            var l = this['indices'][d];
            var m = this['getDataValue'](a, l, 0x0);
            var o = this['getDataValue'](a, l, 0x1);
            var p = this['getDataValue'](a, l, 0x2);
            var q = this['getDataValue'](a, l, 0x3);
            q['idx'] = d;
            for (e = d - 0x1; e >= 0x0; --e) {
                if (!this['processFlock'](g, h, e, a, m, o, p, q))
                    break;
            }
            for (e = d + 0x1; e < c; ++e) {
                if (!this['processFlock'](g, h, e, a, m, o, p, q))
                    break;
            }
        }
    }
    ['processFlock'](a, b, c, d, e, f, g, h) {
        var i = this['indices'][c];
        var k = this['getDataValue'](d, i, 0x1);
        if (f['team'] !== k['team'])
            return !![];
        var l = this['getDataValue'](d, i, 0x2);
        var m = a(g, l);
        if (m > h['dd0'])
            return ![];
        var n = b(g, l);
        var o = m + n;
        var p = this['getDataValue'](d, i, 0x0);
        var q = this['getDataValue'](d, i, 0x3);
        this['tryConnectToFlock'](e, f, g, h, p, k, l, q, o);
        this['tryConnectToFlock'](p, k, l, q, e, f, g, h, o);
        return !![];
    }
    ['getSquareX'](c, e) {
        var f = e['x'] - c['x'];
        return f * f;
    }
    ['getSquareY'](c, e) {
        var f = e['y'] - c['y'];
        return f * f;
    }
    ['tryConnectToFlock'](a, b, c, d, e, f, g, h, i) {
        if (h['e0'] === a || h['e1'] === a || h['e2'] === a)
            return;
        if (d['dd2'] < i)
            return;
        if (d['dd1'] < i) {
            d['dd2'] = i;
            d['e2'] = e;
            d['c2'] = f;
            d['p2'] = g;
            d['k2'] = h;
            return;
        }
        if (d['dd0'] < i) {
            d['dd2'] = d['dd1'];
            d['e2'] = d['e1'];
            d['c2'] = d['c1'];
            d['p2'] = d['p1'];
            d['k2'] = d['k1'];
            d['dd1'] = i;
            d['e1'] = e;
            d['c1'] = f;
            d['p1'] = g;
            d['k1'] = h;
            return;
        }
        d['dd2'] = d['dd1'];
        d['e2'] = d['e1'];
        d['c2'] = d['c1'];
        d['p2'] = d['p1'];
        d['k2'] = d['k1'];
        d['dd1'] = d['dd0'];
        d['e1'] = d['e0'];
        d['c1'] = d['c0'];
        d['p1'] = d['p0'];
        d['k1'] = d['k0'];
        d['dd0'] = i;
        d['e0'] = e;
        d['c0'] = f;
        d['p0'] = g;
        d['k0'] = h;
    }
}