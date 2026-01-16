class Cell {
    ['vx0'] = 0x0;
    ['vy0'] = 0x0;
    ['vx1'] = 0x0;
    ['vy1'] = 0x0;
    ['vx2'] = 0x0;
    ['vy2'] = 0x0;
    ['vx3'] = 0x0;
    ['vy3'] = 0x0;
    ['pressure0'] = 0x0;
    ['pressure1'] = 0x0;
    ['pressure2'] = 0x0;
    ['pressure3'] = 0x0;
    ['c7'] = null;
    ['c8'] = null;
    ['c9'] = null;
    ['c4'] = null;
    ['c6'] = null;
    ['c1'] = null;
    ['c2'] = null;
    ['c3'] = null;
    ['updatePressure']() {
        var a = this['c7']['vx0'] * 0.5 + this['c4']['vx0'] + this['c1']['vx0'] * 0.5 - this['c9']['vx0'] * 0.5 - this['c6']['vx0'] - this['c3']['vx0'] * 0.5;
        var b = this['c7']['vy0'] * 0.5 + this['c8']['vy0'] + this['c9']['vy0'] * 0.5 - this['c1']['vy0'] * 0.5 - this['c2']['vy0'] - this['c3']['vy0'] * 0.5;
        this['pressure0'] = (a + b) * 0.25;
        var c = this['c7']['vx1'] * 0.5 + this['c4']['vx1'] + this['c1']['vx1'] * 0.5 - this['c9']['vx1'] * 0.5 - this['c6']['vx1'] - this['c3']['vx1'] * 0.5;
        var d = this['c7']['vy1'] * 0.5 + this['c8']['vy1'] + this['c9']['vy1'] * 0.5 - this['c1']['vy1'] * 0.5 - this['c2']['vy1'] - this['c3']['vy1'] * 0.5;
        this['pressure1'] = (c + d) * 0.25;
        var e = this['c7']['vx2'] * 0.5 + this['c4']['vx2'] + this['c1']['vx2'] * 0.5 - this['c9']['vx2'] * 0.5 - this['c6']['vx2'] - this['c3']['vx2'] * 0.5;
        var f = this['c7']['vy2'] * 0.5 + this['c8']['vy2'] + this['c9']['vy2'] * 0.5 - this['c1']['vy2'] * 0.5 - this['c2']['vy2'] - this['c3']['vy2'] * 0.5;
        this['pressure2'] = (e + f) * 0.25;
        var g = this['c7']['vx3'] * 0.5 + this['c4']['vx3'] + this['c1']['vx3'] * 0.5 - this['c9']['vx3'] * 0.5 - this['c6']['vx3'] - this['c3']['vx3'] * 0.5;
        var h = this['c7']['vy3'] * 0.5 + this['c8']['vy3'] + this['c9']['vy3'] * 0.5 - this['c1']['vy3'] * 0.5 - this['c2']['vy3'] - this['c3']['vy3'] * 0.5;
        this['pressure3'] = (g + h) * 0.25;
    }
    ['adaptVelocity']() {
        this['vx0'] += (this['c7']['pressure0'] * 0.5 + this['c4']['pressure0'] + this['c1']['pressure0'] * 0.5 - this['c9']['pressure0'] * 0.5 - this['c6']['pressure0'] - this['c3']['pressure0'] * 0.5) * 0.25;
        this['vy0'] += (this['c7']['pressure0'] * 0.5 + this['c8']['pressure0'] + this['c9']['pressure0'] * 0.5 - this['c1']['pressure0'] * 0.5 - this['c2']['pressure0'] - this['c3']['pressure0'] * 0.5) * 0.25;
        this['vx0'] *= CELL_RESISTANCE;
        this['vy0'] *= CELL_RESISTANCE;
        this['vx1'] += (this['c7']['pressure1'] * 0.5 + this['c4']['pressure1'] + this['c1']['pressure1'] * 0.5 - this['c9']['pressure1'] * 0.5 - this['c6']['pressure1'] - this['c3']['pressure1'] * 0.5) * 0.25;
        this['vy1'] += (this['c7']['pressure1'] * 0.5 + this['c8']['pressure1'] + this['c9']['pressure1'] * 0.5 - this['c1']['pressure1'] * 0.5 - this['c2']['pressure1'] - this['c3']['pressure1'] * 0.5) * 0.25;
        this['vx1'] *= CELL_RESISTANCE;
        this['vy1'] *= CELL_RESISTANCE;
        this['vx2'] += (this['c7']['pressure2'] * 0.5 + this['c4']['pressure2'] + this['c1']['pressure2'] * 0.5 - this['c9']['pressure2'] * 0.5 - this['c6']['pressure2'] - this['c3']['pressure2'] * 0.5) * 0.25;
        this['vy2'] += (this['c7']['pressure2'] * 0.5 + this['c8']['pressure2'] + this['c9']['pressure2'] * 0.5 - this['c1']['pressure2'] * 0.5 - this['c2']['pressure2'] - this['c3']['pressure2'] * 0.5) * 0.25;
        this['vx2'] *= CELL_RESISTANCE;
        this['vy2'] *= CELL_RESISTANCE;
        this['vx3'] += (this['c7']['pressure3'] * 0.5 + this['c4']['pressure3'] + this['c1']['pressure3'] * 0.5 - this['c9']['pressure3'] * 0.5 - this['c6']['pressure3'] - this['c3']['pressure3'] * 0.5) * 0.25;
        this['vy3'] += (this['c7']['pressure3'] * 0.5 + this['c8']['pressure3'] + this['c9']['pressure3'] * 0.5 - this['c1']['pressure3'] * 0.5 - this['c2']['pressure3'] - this['c3']['pressure3'] * 0.5) * 0.25;
        this['vx3'] *= CELL_RESISTANCE;
        this['vy3'] *= CELL_RESISTANCE;
    }
}
class Grid {
    ['width'] = 0x0;
    ['height'] = 0x0;
    ['cellSize'] = 0x0;
    ['cols'] = 0x0;
    ['rows'] = 0x0;
    ['cells'] = null;
    constructor(a, b, d) {
        this['width'] = a;
        this['height'] = b;
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
                i[e][f]['c7'] = this['getCell'](e - 0x1, f - 0x1);
                i[e][f]['c8'] = this['getCell'](e, f - 0x1);
                i[e][f]['c9'] = this['getCell'](e + 0x1, f - 0x1);
                i[e][f]['c4'] = this['getCell'](e - 0x1, f);
                i[e][f]['c6'] = this['getCell'](e + 0x1, f);
                i[e][f]['c1'] = this['getCell'](e - 0x1, f + 0x1);
                i[e][f]['c2'] = this['getCell'](e, f + 0x1);
                i[e][f]['c3'] = this['getCell'](e + 0x1, f + 0x1);
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
    ['force'](a, b, e, f, g, h, i, j, k, l, m) {
        var n = Math['floor'](a / this['cellSize']);
        var o = Math['floor'](b / this['cellSize']);
        for (var p = this['cols']; p--;) {
            for (var q = this['rows']; q--;) {
                var s = p - n;
                var t = q - o;
                var u = s * s + t * t;
                this['cells'][p][q]['vx0'] += u < EPSILON ? e * f : e * f / u;
                this['cells'][p][q]['vy0'] += u < EPSILON ? e * g : e * g / u;
                this['cells'][p][q]['vx1'] += u < EPSILON ? e * h : e * h / u;
                this['cells'][p][q]['vy1'] += u < EPSILON ? e * i : e * i / u;
                this['cells'][p][q]['vx2'] += u < EPSILON ? e * j : e * j / u;
                this['cells'][p][q]['vy2'] += u < EPSILON ? e * k : e * k / u;
                this['cells'][p][q]['vx3'] += u < EPSILON ? e * l : e * l / u;
                this['cells'][p][q]['vy3'] += u < EPSILON ? e * m : e * m / u;
            }
        }
    }
    ['update'](a) {
        var b = 0x0;
        var d = 0x0;
        for (b = this['cols']; b--;)
            for (d = this['rows']; d--;)
                this['cells'][b][d]['updatePressure']();
        for (b = this['cols']; b--;)
            for (d = this['rows']; d--;)
                this['cells'][b][d]['adaptVelocity']();
    }
}