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
    ['processDestroyEntities']() {
        var a = this['entities']['length'];
        for (var b = a; b--;)
            if (this['entities'][b]['life'] <= 0x0)
                this['entities']['splice'](b, 0x1);
        this['indices']['length'] = this['entities']['length'];
    }
    ['calcScore']() {
        this['scores'][0x0] = this['scores'][0x1] = this['scores'][0x2] = this['scores'][0x3] = 0x0;
        var a = this['entities']['length'];
        for (var b = a; b--;) {
            ++this['scores'][this['entities'][b]['team']];
        }
    }
    ['update'](a, b, c, d) {
        var e = this['entities']['length'];
        var f = 0x0;
        for (f = e; f--;)
            this['entities'][f]['preMove'](this);
        for (f = e; f--;)
            this['entities'][f]['resolveOverlap']();
        for (f = e; f--;)
            this['entities'][f]['move'](this);
        this['updateConnections']();
        for (f = e; f--;)
            this['entities'][f]['behavior'](this['events']);
        this['processDestroyEntities']();
        this['calcScore']();
    }
}