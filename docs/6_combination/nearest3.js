class Nearest3 {
    ['odd'] = 0x0;
    ['grid'] = null;
    ['entities'] = [];
    ['indices'] = [];
    ['scores'] = [
        0x0,
        0x0,
        0x0,
        0x0
    ];
    ['events'] = [];
    ['cnt'] = 0x0;
    constructor(a, b, c) {
        this['grid'] = new Grid(a, b, c);
    }
    ['add'](a) {
        var b = a['length'];
        var c = new Array(b);
        var d = new Attribute0();
        var e = new Attribute1();
        var f = new Attribute2();
        for (var g = b; g--;) {
            if (a[g][0x3] === 0x0)
                c[g] = new Entity0(a[g][0x0], a[g][0x1], a[g][0x2], a[g][0x3], d);
            else if (a[g][0x3] === 0x1)
                c[g] = new Entity1(a[g][0x0], a[g][0x1], a[g][0x2], a[g][0x3], e);
            else if (a[g][0x3] === 0x2)
                c[g] = new Entity2(a[g][0x0], a[g][0x1], a[g][0x2], a[g][0x3], f);
        }
        this['entities'] = this['entities']['concat'](c);
        this['indices'] = new Array(this['entities']['length']);
    }
    ['getSquare'](c, e) {
        var f = e - c;
        return f * f;
    }
    ['updateConnections']() {
        var a = this['entities'];
        var b = this['indices'];
        var c = a['length'];
        var d = 0x0, e = 0x0;
        for (d = c; d--;)
            a[d]['cutConnections']();
        for (d = c; d--;)
            b[d] = d;
        if (this['odd'] === 0x0) {
            b['sort'](function (m, o) {
                var p = a[o]['x'] - a[m]['x'];
                return p !== 0x0 ? p : m - o;
            });
        } else if (this['odd'] === 0x1) {
            b['sort'](function (m, o) {
                var p = a[m]['x'] - a[o]['x'];
                return p !== 0x0 ? p : o - m;
            });
        } else if (this['odd'] === 0x2) {
            b['sort'](function (m, o) {
                var p = a[o]['y'] - a[m]['y'];
                return p !== 0x0 ? p : m - o;
            });
        } else if (this['odd'] === 0x3) {
            b['sort'](function (m, o) {
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
            for (d = c; d--;) {
                f = a[b[d]];
                for (e = d - 0x1; e >= 0x0; --e) {
                    g = a[b[e]];
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
                for (e = d + 0x1; e < c; ++e) {
                    g = a[b[e]];
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
            for (d = c; d--;) {
                f = a[b[d]];
                for (e = d - 0x1; e >= 0x0; --e) {
                    g = a[b[e]];
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
                for (e = d + 0x1; e < c; ++e) {
                    g = a[b[e]];
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
        ++this['odd'];
        if (this['odd'] === 0x4)
            this['odd'] = 0x0;
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
    ['update'](a, b, c, d, e) {
        var f = this['entities']['length'];
        var g = 0x0;
        for (g = f; g--;)
            this['entities'][g]['preMove'](this['grid']);
        for (g = f; g--;)
            this['entities'][g]['resolveOverlap']();
        for (g = f; g--;)
            this['entities'][g]['move'](this['grid']);
        this['updateConnections']();
        for (g = f; g--;)
            this['entities'][g]['behavior'](this['events']);
        this['processDestroyEntities']();
        this['calcScore']();
        this['grid']['force'](a, b, c, d, e);
        this['grid']['update'](this['entities']);
        ++this['cnt'];
        return {
            'entities': this['entities'],
            'grid': this['grid']
        };
    }
}