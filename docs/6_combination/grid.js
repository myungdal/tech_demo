class Cell {
    ['vx'] = 0x0;
    ['vy'] = 0x0;
    ['pressure'] = 0x0;
    ['c7'] = null;
    ['c8'] = null;
    ['c9'] = null;
    ['c4'] = null;
    ['c6'] = null;
    ['c1'] = null;
    ['c2'] = null;
    ['c3'] = null;
    ['updatePressure']() {
        var a = this['c7']['vx'] * 0.5 + this['c4']['vx'] + this['c1']['vx'] * 0.5 - this['c9']['vx'] * 0.5 - this['c6']['vx'] - this['c3']['vx'] * 0.5;
        var b = this['c7']['vy'] * 0.5 + this['c8']['vy'] + this['c9']['vy'] * 0.5 - this['c1']['vy'] * 0.5 - this['c2']['vy'] - this['c3']['vy'] * 0.5;
        this['pressure'] = (a + b) * 0.25;
    }
    ['adaptVelocity']() {
        this['vx'] += (this['c7']['pressure'] * 0.5 + this['c4']['pressure'] + this['c1']['pressure'] * 0.5 - this['c9']['pressure'] * 0.5 - this['c6']['pressure'] - this['c3']['pressure'] * 0.5) * 0.25;
        this['vy'] += (this['c7']['pressure'] * 0.5 + this['c8']['pressure'] + this['c9']['pressure'] * 0.5 - this['c1']['pressure'] * 0.5 - this['c2']['pressure'] - this['c3']['pressure'] * 0.5) * 0.25;
        this['vx'] *= CELL_RESISTANCE;
        this['vy'] *= CELL_RESISTANCE;
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
    ['force'](a, b, e, f, g) {
        var h = Math['floor'](a / this['cellSize']);
        var i = Math['floor'](b / this['cellSize']);
        for (var j = this['cols']; j--;) {
            for (var k = this['rows']; k--;) {
                var l = j - h;
                var m = k - i;
                var n = l * l + m * m;
                this['cells'][j][k]['vx'] += n < EPSILON ? e * f : e * f / n;
                this['cells'][j][k]['vy'] += n < EPSILON ? e * g : e * g / n;
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