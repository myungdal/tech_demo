class Geometry {
    ['indices'] = [];
    ['odd'] = -0x1;
    ['getSquare'](c, e) {
        var f = e - c;
        return f * f;
    }
    ['updateConnections'](a) {
        ++this['odd'];
        if (this['odd'] === 0x4)
            this['odd'] = 0x0;
        var b = a['length'];
        var c = this['indices'];
        c['length'] = a['length'];
        var d = 0x0, e = 0x0;
        for (d = b; d--;)
            a[d]['cutConnections']();
        for (d = b; d--;)
            c[d] = d;
        if (this['odd'] === 0x0) {
            c['sort'](function (m, o) {
                var p = a[o]['x'] - a[m]['x'];
                return p !== 0x0 ? p : m - o;
            });
        } else if (this['odd'] === 0x1) {
            c['sort'](function (m, o) {
                var p = a[m]['x'] - a[o]['x'];
                return p !== 0x0 ? p : o - m;
            });
        } else if (this['odd'] === 0x2) {
            c['sort'](function (m, o) {
                var p = a[o]['y'] - a[m]['y'];
                return p !== 0x0 ? p : m - o;
            });
        } else if (this['odd'] === 0x3) {
            c['sort'](function (m, o) {
                var p = a[m]['y'] - a[o]['y'];
                return p !== 0x0 ? p : o - m;
            });
        }
        var f = 0x0;
        var g = 0x0;
        var h = 0x0;
        var k = 0x0;
        var l = 0x0;
        if (this['odd'] === 0x0 || this['odd'] === 0x1) {
            for (d = b; d--;) {
                f = a[c[d]];
                for (e = d - 0x1; e >= 0x0; --e) {
                    g = a[c[e]];
                    h = this['getSquare'](f['x'], g['x']);
                    if (f['isNearestCompleted'](h) === !![])
                        break;
                    k = this['getSquare'](f['y'], g['y']);
                    l = h + k;
                    if (f['team'] === g['team']) {
                        f['tryConnectToFriend'](g, l);
                        g['tryConnectToFriend'](f, l);
                    } else {
                        f['tryConnectToEnemy'](g, l);
                        g['tryConnectToEnemy'](f, l);
                    }
                }
                for (e = d + 0x1; e < b; ++e) {
                    g = a[c[e]];
                    h = this['getSquare'](f['x'], g['x']);
                    if (f['isNearestCompleted'](h) === !![])
                        break;
                    k = this['getSquare'](f['y'], g['y']);
                    l = h + k;
                    if (f['team'] === g['team']) {
                        f['tryConnectToFriend'](g, l);
                        g['tryConnectToFriend'](f, l);
                    } else {
                        f['tryConnectToEnemy'](g, l);
                        g['tryConnectToEnemy'](f, l);
                    }
                }
            }
        } else {
            for (d = b; d--;) {
                f = a[c[d]];
                for (e = d - 0x1; e >= 0x0; --e) {
                    g = a[c[e]];
                    h = this['getSquare'](f['y'], g['y']);
                    if (f['isNearestCompleted'](h) === !![])
                        break;
                    k = this['getSquare'](f['x'], g['x']);
                    l = h + k;
                    if (f['team'] === g['team']) {
                        f['tryConnectToFriend'](g, l);
                        g['tryConnectToFriend'](f, l);
                    } else {
                        f['tryConnectToEnemy'](g, l);
                        g['tryConnectToEnemy'](f, l);
                    }
                }
                for (e = d + 0x1; e < b; ++e) {
                    g = a[c[e]];
                    h = this['getSquare'](f['y'], g['y']);
                    if (f['isNearestCompleted'](h) === !![])
                        break;
                    k = this['getSquare'](f['x'], g['x']);
                    l = h + k;
                    if (f['team'] === g['team']) {
                        f['tryConnectToFriend'](g, l);
                        g['tryConnectToFriend'](f, l);
                    } else {
                        f['tryConnectToEnemy'](g, l);
                        g['tryConnectToEnemy'](f, l);
                    }
                }
            }
        }
    }
    ['chainHull_isLeft'](a, b, c) {
        return (b['x'] - a['x']) * (c['y'] - a['y']) - (c['x'] - a['x']) * (b['y'] - a['y']);
    }
    ['chainHull'](a, b) {
        var c = a['length'];
        if (c <= 0x0)
            return 0x0;
        a['sort'](function (o, p) {
            return o['y'] - p['y'];
        });
        a['sort'](function (o, p) {
            return o['x'] - p['x'];
        });
        var d = 0x0;
        var e = -0x1;
        var f;
        var g = 0x0;
        var h;
        var j = a[0x0]['x'];
        for (f = 0x1; f < c; f++)
            if (a[f]['x'] !== j)
                break;
        h = f - 0x1;
        if (h === c - 0x1) {
            b[++e] = a[g];
            if (a[h]['y'] !== a[g]['y'])
                b[++e] = a[h];
            b[++e] = a[g];
            return e + 0x1;
        }
        var k;
        var l = c - 0x1;
        var m = a[c - 0x1]['x'];
        for (f = c - 0x2; f >= 0x0; f--)
            if (a[f]['x'] !== m)
                break;
        k = f + 0x1;
        b[++e] = a[g];
        f = h;
        while (++f <= k) {
            if (this['chainHull_isLeft'](a[g], a[k], a[f]) >= 0x0 && f < k)
                continue;
            while (e > 0x0) {
                if (this['chainHull_isLeft'](b[e - 0x1], b[e], a[f]) > 0x0)
                    break;
                --e;
            }
            b[++e] = a[f];
        }
        if (l !== k)
            b[++e] = a[l];
        d = e;
        f = k;
        while (--f >= h) {
            if (this['chainHull_isLeft'](a[l], a[h], a[f]) >= 0x0 && f > h)
                continue;
            while (e > d) {
                if (this['chainHull_isLeft'](b[e - 0x1], b[e], a[f]) > 0x0)
                    break;
                --e;
            }
            if (a[f]['x'] === b[0x0]['x'] && a[f]['y'] === b[0x0]['y'])
                return e + 0x1;
            b[++e] = a[f];
        }
        if (h !== g)
            b[++e] = a[g];
        return e + 0x1;
    }
}