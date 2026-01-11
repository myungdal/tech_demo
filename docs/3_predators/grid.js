class Grid {
    ['cellSize'] = 0x0;
    ['cols'] = 0x0;
    ['rows'] = 0x0;
    ['cells'] = null;
    constructor(a, b, d) {
        this['cellSize'] = d;
        var e = 0x0;
        var f = 0x0;
        var g = this['cols'] = Math['floor'](a / d);
        var h = this['rows'] = Math['floor'](b / d);
        var i = this['cells'] = new Array(g);
        for (e = g; e--;) {
            i[e] = new Array(h);
            for (f = h; f--;)
                i[e][f] = new Cell();
        }
        for (e = g; e--;) {
            for (f = h; f--;) {
                var j = i[e][f];
                j['c7'] = this['getCell'](e - 0x1, f - 0x1);
                j['c8'] = this['getCell'](e, f - 0x1);
                j['c9'] = this['getCell'](e + 0x1, f - 0x1);
                j['c4'] = this['getCell'](e - 0x1, f);
                j['c6'] = this['getCell'](e + 0x1, f);
                j['c1'] = this['getCell'](e - 0x1, f + 0x1);
                j['c2'] = this['getCell'](e, f + 0x1);
                j['c3'] = this['getCell'](e + 0x1, f + 0x1);
            }
        }
    }
    ['pick'](a, b) {
        var d = Math['floor'](a / this['cellSize']);
        var e = Math['floor'](b / this['cellSize']);
        return this['getCell'](d, e);
    }
    ['getCell'](a, b) {
        if (a < 0x0)
            a = 0x0;
        else if (a >= this['cols'])
            a = this['cols'] - 0x1;
        if (b < 0x0)
            b = 0x0;
        else if (b >= this['rows'])
            b = this['rows'] - 0x1;
        return this['cells'][a][b];
    }
    ['wave0'](a, b, c, d, e) {
        var f = this['pick'](a, b);
        f['vx0'] += c * e;
        f['vy0'] += d * e;
    }
    ['wave1'](a, b, c, d, e) {
        var f = this['pick'](a, b);
        f['vx1'] += c * e;
        f['vy1'] += d * e;
    }
    ['wave2'](a, b, c, d, e) {
        var f = this['pick'](a, b);
        f['vx2'] += c * e;
        f['vy2'] += d * e;
    }
    ['wave3'](a, b, c, d, e) {
        var f = this['pick'](a, b);
        f['vx3'] += c * e;
        f['vy3'] += d * e;
    }
    ['force'](a, b, e, f, g, h, i, j, k, l, m) {
        var n = Math['floor'](a / this['cellSize']);
        var o = Math['floor'](b / this['cellSize']);
        for (var p = this['cols']; p--;) {
            for (var q = this['rows']; q--;) {
                var s = p - n;
                var t = q - o;
                var u = s * s + t * t;
                var v = this['cells'][p][q];
                v['vx0'] += u < EPSILON ? e * f : e * f / u;
                v['vy0'] += u < EPSILON ? e * g : e * g / u;
                v['vx1'] += u < EPSILON ? e * h : e * h / u;
                v['vy1'] += u < EPSILON ? e * i : e * i / u;
                v['vx2'] += u < EPSILON ? e * j : e * j / u;
                v['vy2'] += u < EPSILON ? e * k : e * k / u;
                v['vx3'] += u < EPSILON ? e * l : e * l / u;
                v['vy3'] += u < EPSILON ? e * m : e * m / u;
            }
        }
    }
    ['follow'](a) {
        var b = a['length'];
        var c = 0x0;
        for (c = b; c--;) {
            var d = a[c];
            var e = this['pick'](d['x'], d['y']);
            if (d['team'] === 0x0) {
                d['vx'] += e['vx0'] * FOLLOW;
                d['vy'] += e['vy0'] * FOLLOW;
                d['vx'] += e['vx1'] * FOLLOW_OTHER;
                d['vy'] += e['vy1'] * FOLLOW_OTHER;
                d['vx'] += e['vx2'] * FOLLOW_OTHER;
                d['vy'] += e['vy2'] * FOLLOW_OTHER;
                d['vx'] += e['vx3'] * FOLLOW_OTHER;
                d['vy'] += e['vy3'] * FOLLOW_OTHER;
            } else if (d['team'] === 0x1) {
                d['vx'] += e['vx0'] * FOLLOW_OTHER;
                d['vy'] += e['vy0'] * FOLLOW_OTHER;
                d['vx'] += e['vx1'] * FOLLOW;
                d['vy'] += e['vy1'] * FOLLOW;
                d['vx'] += e['vx2'] * FOLLOW_OTHER;
                d['vy'] += e['vy2'] * FOLLOW_OTHER;
                d['vx'] += e['vx3'] * FOLLOW_OTHER;
                d['vy'] += e['vy3'] * FOLLOW_OTHER;
            } else if (d['team'] === 0x2) {
                d['vx'] += e['vx0'] * FOLLOW_OTHER;
                d['vy'] += e['vy0'] * FOLLOW_OTHER;
                d['vx'] += e['vx1'] * FOLLOW_OTHER;
                d['vy'] += e['vy1'] * FOLLOW_OTHER;
                d['vx'] += e['vx2'] * FOLLOW;
                d['vy'] += e['vy2'] * FOLLOW;
                d['vx'] += e['vx3'] * FOLLOW_OTHER;
                d['vy'] += e['vy3'] * FOLLOW_OTHER;
            } else if (d['team'] === 0x3) {
                d['vx'] += e['vx0'] * FOLLOW_OTHER;
                d['vy'] += e['vy0'] * FOLLOW_OTHER;
                d['vx'] += e['vx1'] * FOLLOW_OTHER;
                d['vy'] += e['vy1'] * FOLLOW_OTHER;
                d['vx'] += e['vx2'] * FOLLOW_OTHER;
                d['vy'] += e['vy2'] * FOLLOW_OTHER;
                d['vx'] += e['vx3'] * FOLLOW;
                d['vy'] += e['vy3'] * FOLLOW;
            }
        }
    }
    ['wave'](a, b) {
        var c = [
            0x0,
            0x0,
            0x0,
            0x0
        ];
        var d = b[0x0] + b[0x1] + b[0x2] + b[0x3];
        if (d > 0x0) {
            c[0x0] = WAVE * b[0x0] / d;
            c[0x1] = WAVE * b[0x1] / d;
            c[0x2] = WAVE * b[0x2] / d;
            c[0x3] = WAVE * b[0x3] / d;
        }
        var e = a['length'];
        var f = 0x0;
        for (f = e; f--;) {
            var g = a[f];
            if (g['team'] === 0x0)
                this['wave0'](g['x'], g['y'], g['vx'], g['vy'], c[0x0]);
            else if (g['team'] === 0x1)
                this['wave1'](g['x'], g['y'], g['vx'], g['vy'], c[0x1]);
            else if (g['team'] === 0x2)
                this['wave2'](g['x'], g['y'], g['vx'], g['vy'], c[0x2]);
            else if (g['team'] === 0x3)
                this['wave3'](g['x'], g['y'], g['vx'], g['vy'], c[0x3]);
        }
    }
    ['update']() {
        var a = 0x0;
        var b = 0x0;
        for (a = this['cols']; a--;)
            for (b = this['rows']; b--;)
                this['cells'][a][b]['updatePressure']();
        for (a = this['cols']; a--;)
            for (b = this['rows']; b--;)
                this['cells'][a][b]['adaptVelocity']();
    }
}