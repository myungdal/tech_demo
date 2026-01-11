const EPSILON = 0x1 / 0x100000;
const TOO_NEAR_ENEMY = 0x3 * 0x3;
const TOO_NEAR_FRIEND = 0x4 * 0x4;
const MOVE_RESISTANCE = 0.8;
const CELL_RESISTANCE = 0.85;
const WAVE = 0.2;
const FOLLOW = 0.3;
const FOLLOW_OTHER = 0.2;
const MAX_LIFE = 0x1f4;
const MAX_ENERGY = 0x32;
const DAMAGE = 0x14;
const HEAL = 0x1e;
const ENERGY_ATTACK = 0x14;
const ENERGY_HEAL = 0x1e;
const ATTACK_RANGE_SQUARE = 0x5 * 0x5;
const HEAL_RANGE_SQUARE = 0x9 * 0x9;
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
    ['applyForce'](a, b, e, f, g, h, i, j, k, l, m) {
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
class Attribute {
}
class Entity {
    ['life'] = MAX_LIFE;
    ['energy'] = MAX_ENERGY;
    ['state'] = 0x1;
    ['color'] = 0x0;
    ['x'] = 0x0;
    ['y'] = 0x0;
    ['vx'] = 0x0;
    ['vy'] = 0x0;
    ['e0'] = null;
    ['ed0'] = 0x0;
    ['f0'] = null;
    ['f1'] = null;
    ['f2'] = null;
    ['fd0'] = 0x0;
    ['fd1'] = 0x0;
    ['fd2'] = 0x0;
    constructor(a, b, c) {
        this['x'] = a;
        this['y'] = b;
        this['color'] = c;
    }
    ['clearConnections']() {
        this['e0'] = null;
        this['f0'] = this['f1'] = this['f2'] = null;
        this['ed0'] = Number['MAX_VALUE'];
        this['fd0'] = this['fd1'] = this['fd2'] = Number['MAX_VALUE'];
    }
    ['move'](a) {
        this['x'] += this['vx'];
        this['y'] += this['vy'];
        if (this['x'] < 0x0) {
            this['x'] = 0x0;
            this['vx'] += 0x1;
        } else if (this['x'] >= a['width']) {
            this['x'] = a['width'];
            this['vx'] -= 0x1;
        }
        if (this['y'] < 0x0) {
            this['y'] = 0x0;
            this['vy'] += 0x1;
        } else if (this['y'] >= a['height']) {
            this['y'] = a['height'];
            this['vy'] -= 0x1;
        }
        this['vx'] *= MOVE_RESISTANCE;
        this['vy'] *= MOVE_RESISTANCE;
    }
    ['behavior'](a) {
        if (this['life'] <= 0x0)
            return;
        this['applyForceToEnemy'](this, this['e0'], this['ed0'], 0.16, 0.2, a);
        this['applyForceToFriend'](this, this['f0'], this['fd0'], 0.05, 0.09);
        this['applyForceToFriend'](this, this['f1'], this['fd1'], 0.06, 0.1);
        this['applyForceToFriend'](this, this['f2'], this['fd2'], 0.07, 0.11);
        this['performHeal'](this, this['f0'], this['fd0']);
        this['performAttack'](this, this['e0'], this['ed0']);
        ++this['energy'];
        if (this['energy'] > MAX_ENERGY) {
            this['energy'] = MAX_ENERGY;
        }
        if ((!this['f0'] || this['state'] > 0x0 || this['f0']['life'] >= MAX_LIFE || this['df0'] > HEAL_RANGE_SQUARE * 0x3) && (!this['f1'] || this['state'] > 0x0 || this['f1']['life'] >= MAX_LIFE || this['df1'] > HEAL_RANGE_SQUARE * 0x3) && (!this['f2'] || this['state'] > 0x0 || this['f2']['life'] >= MAX_LIFE || this['df2'] > HEAL_RANGE_SQUARE * 0x3) && this['life'] >= MAX_LIFE) {
            this['state'] = this['state'] * 0.99 + 0x1 * 0.01;
        }
    }
    ['followWave'](a) {
        if (this['life'] <= 0x0)
            return;
        var b = a['pick'](this['x'], this['y']);
        if (this['color'] === 0x0) {
            this['vx'] += b['vx0'] * FOLLOW;
            this['vy'] += b['vy0'] * FOLLOW;
            this['vx'] += b['vx1'] * FOLLOW_OTHER;
            this['vy'] += b['vy1'] * FOLLOW_OTHER;
            this['vx'] += b['vx2'] * FOLLOW_OTHER;
            this['vy'] += b['vy2'] * FOLLOW_OTHER;
            this['vx'] += b['vx3'] * FOLLOW_OTHER;
            this['vy'] += b['vy3'] * FOLLOW_OTHER;
        } else if (this['color'] === 0x1) {
            this['vx'] += b['vx0'] * FOLLOW_OTHER;
            this['vy'] += b['vy0'] * FOLLOW_OTHER;
            this['vx'] += b['vx1'] * FOLLOW;
            this['vy'] += b['vy1'] * FOLLOW;
            this['vx'] += b['vx2'] * FOLLOW_OTHER;
            this['vy'] += b['vy2'] * FOLLOW_OTHER;
            this['vx'] += b['vx3'] * FOLLOW_OTHER;
            this['vy'] += b['vy3'] * FOLLOW_OTHER;
        } else if (this['color'] === 0x2) {
            this['vx'] += b['vx0'] * FOLLOW_OTHER;
            this['vy'] += b['vy0'] * FOLLOW_OTHER;
            this['vx'] += b['vx1'] * FOLLOW_OTHER;
            this['vy'] += b['vy1'] * FOLLOW_OTHER;
            this['vx'] += b['vx2'] * FOLLOW;
            this['vy'] += b['vy2'] * FOLLOW;
            this['vx'] += b['vx3'] * FOLLOW_OTHER;
            this['vy'] += b['vy3'] * FOLLOW_OTHER;
        } else if (this['color'] === 0x3) {
            this['vx'] += b['vx0'] * FOLLOW_OTHER;
            this['vy'] += b['vy0'] * FOLLOW_OTHER;
            this['vx'] += b['vx1'] * FOLLOW_OTHER;
            this['vy'] += b['vy1'] * FOLLOW_OTHER;
            this['vx'] += b['vx2'] * FOLLOW_OTHER;
            this['vy'] += b['vy2'] * FOLLOW_OTHER;
            this['vx'] += b['vx3'] * FOLLOW;
            this['vy'] += b['vy3'] * FOLLOW;
        }
    }
    ['applyForceToEnemy'](c, e, f, g, h, i) {
        if (!e)
            return;
        if (e['life'] <= 0x0)
            return;
        if (c['life'] <= 0x0)
            return;
        var j = c['x'] - e['x'];
        var k = c['y'] - e['y'];
        var l = 0x0;
        if (f < TOO_NEAR_ENEMY) {
            if (f < EPSILON) {
                c['vx'] += 0.01 * (Math['random']() - 0.5);
                c['vy'] += 0.01 * (Math['random']() - 0.5);
            } else {
                l = Math['sqrt'](f);
                if (l < EPSILON) {
                    c['vx'] += 0.01 * (Math['random']() - 0.5);
                    c['vy'] += 0.01 * (Math['random']() - 0.5);
                } else {
                    c['vx'] += h * j / l;
                    c['vy'] += h * k / l;
                }
            }
        } else {
            l = Math['sqrt'](f);
            if (l < EPSILON) {
                c['vx'] += 0.01 * (Math['random']() - 0.5);
                c['vy'] += 0.01 * (Math['random']() - 0.5);
            } else if (c['state'] < 0x0) {
                c['vx'] += -c['state'] / 0x1 * h * (0.5 * j / l + 0.5 * j / f);
                c['vy'] += -c['state'] / 0x1 * h * (0.5 * k / l + 0.5 * k / f);
            } else {
                var m = 0x1;
                if (e['state'] > 0x0) {
                    m = c['state'] > e['state'] ? c['state'] / 0x1 : c['state'] / e['state'];
                }
                c['vx'] -= g * j / l * m;
                c['vy'] -= g * k / l * m;
            }
        }
    }
    ['applyForceToFriend'](c, e, f, g, h) {
        if (!e)
            return;
        if (e['life'] <= 0x0)
            return;
        if (c['life'] <= 0x0)
            return;
        var i = c['x'] - e['x'];
        var j = c['y'] - e['y'];
        var k = 0x0;
        if (f < TOO_NEAR_FRIEND) {
            if (f < EPSILON) {
                c['vx'] += 0.01 * (Math['random']() - 0.5);
                c['vy'] += 0.01 * (Math['random']() - 0.5);
            } else {
                k = Math['sqrt'](f);
                if (k < EPSILON) {
                    c['vx'] += 0.01 * (Math['random']() - 0.5);
                    c['vy'] += 0.01 * (Math['random']() - 0.5);
                } else {
                    c['vx'] += h * i / k;
                    c['vy'] += h * j / k;
                }
            }
        } else {
            k = Math['sqrt'](f);
            if (k < EPSILON) {
                c['vx'] += 0.01 * (Math['random']() - 0.5);
                c['vy'] += 0.01 * (Math['random']() - 0.5);
            } else {
                if (c['state'] < 0x0) {
                    c['vx'] -= h * i / k;
                    c['vy'] -= h * j / k;
                } else {
                    c['vx'] -= g * i / k;
                    c['vy'] -= g * j / k;
                }
            }
        }
    }
    ['performHeal'](c, d, e) {
        if (!d)
            return;
        if (d['life'] <= 0x0)
            return;
        if (c['life'] <= 0x0)
            return;
        if (d['color'] === c['color'] && e < HEAL_RANGE_SQUARE && c['energy'] >= ENERGY_HEAL && d['life'] < MAX_LIFE) {
            c['energy'] -= ENERGY_HEAL;
            d['life'] += HEAL;
            if (d['life'] > MAX_LIFE)
                d['life'] = MAX_LIFE;
        }
    }
    ['performAttack'](c, d, e) {
        if (!d)
            return;
        if (d['life'] <= 0x0)
            return;
        if (c['life'] <= 0x0)
            return;
        if (d['color'] !== c['color'] && e < ATTACK_RANGE_SQUARE && c['energy'] >= ENERGY_ATTACK && d['life'] > 0x0 && c['state'] > 0x0) {
            c['energy'] -= ENERGY_ATTACK;
            if (d['life'] < MAX_LIFE * (0x3 + d['color'] * 0x3) / 0xc) {
                d['state'] = -0x1;
            }
            d['life'] -= DAMAGE;
            if (d['life'] < 0x0)
                d['life'] = 0x0;
        }
    }
}
class Nearest3 {
    ['odd'] = 0x0;
    ['grid'] = null;
    ['entities'] = null;
    ['indices'] = null;
    ['scores'] = [
        0x0,
        0x0,
        0x0,
        0x0
    ];
    constructor(a, b, c, d) {
        this['grid'] = new Grid(a, b, c);
        var e = 0x0;
        var f = d['length'];
        this['entities'] = new Array(f);
        for (e = f; e--;)
            this['entities'][e] = new Entity(d[e][0x0], d[e][0x1], d[e][0x2]);
        this['indices'] = new Array(f);
    }
    ['add'](a) {
        var b = a['length'];
        var c = new Array(b);
        for (i = b; i--;)
            c[i] = new Entity(a[i][0x0], a[i][0x1], a[i][0x2]);
        this['entities'] = this['entities']['concat'](c);
        this['indices'] = new Array(this['entities']['length']);
    }
    ['getSquaredDistance'](c, e) {
        var f = e - c;
        return f * f;
    }
    ['tryConnectToFriend'](c, d, e) {
        if (d['f0'] === c || d['f1'] === c || d['f2'] === c)
            return;
        if (e < c['fd2']) {
            if (e < c['fd1']) {
                if (e < c['fd0']) {
                    c['fd2'] = c['fd1'];
                    c['f2'] = c['f1'];
                    c['fd1'] = c['fd0'];
                    c['f1'] = c['f0'];
                    c['fd0'] = e;
                    c['f0'] = d;
                } else {
                    c['fd2'] = c['fd1'];
                    c['f2'] = c['f1'];
                    c['fd1'] = e;
                    c['f1'] = d;
                }
            } else {
                c['fd2'] = e;
                c['f2'] = d;
            }
        }
    }
    ['tryConnectToEnemy'](c, d, e) {
        if (d['e0'] === c)
            return;
        if (e < c['ed0']) {
            c['ed0'] = e;
            c['e0'] = d;
        }
    }
    ['updateConnections']() {
        var a = this['entities'];
        var b = this['indices'];
        var c = a['length'];
        var d = 0x0, e = 0x0;
        for (d = c; d--;)
            a[d]['clearConnections']();
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
                    h = this['getSquaredDistance'](f['x'], g['x']);
                    if (h > f['fd0'] && h > f['fd1'] && h > f['fd2'] && h > f['ed0'])
                        break;
                    k = this['getSquaredDistance'](f['y'], g['y']);
                    l = h + k;
                    if (f['color'] === g['color']) {
                        this['tryConnectToFriend'](f, g, l);
                        this['tryConnectToFriend'](g, f, l);
                    } else {
                        this['tryConnectToEnemy'](f, g, l);
                        this['tryConnectToEnemy'](g, f, l);
                    }
                }
                for (e = d + 0x1; e < c; ++e) {
                    g = a[b[e]];
                    h = this['getSquaredDistance'](f['x'], g['x']);
                    if (h > f['fd0'] && h > f['fd1'] && h > f['fd2'] && h > f['ed0'])
                        break;
                    k = this['getSquaredDistance'](f['y'], g['y']);
                    l = h + k;
                    if (f['color'] === g['color']) {
                        this['tryConnectToFriend'](f, g, l);
                        this['tryConnectToFriend'](g, f, l);
                    } else {
                        this['tryConnectToEnemy'](f, g, l);
                        this['tryConnectToEnemy'](g, f, l);
                    }
                }
            }
        } else {
            for (d = c; d--;) {
                f = a[b[d]];
                for (e = d - 0x1; e >= 0x0; --e) {
                    g = a[b[e]];
                    h = this['getSquaredDistance'](f['y'], g['y']);
                    if (h > f['fd0'] && h > f['fd1'] && h > f['fd2'] && h > f['ed0'])
                        break;
                    k = this['getSquaredDistance'](f['x'], g['x']);
                    l = h + k;
                    if (f['color'] === g['color']) {
                        this['tryConnectToFriend'](f, g, l);
                        this['tryConnectToFriend'](g, f, l);
                    } else {
                        this['tryConnectToEnemy'](f, g, l);
                        this['tryConnectToEnemy'](g, f, l);
                    }
                }
                for (e = d + 0x1; e < c; ++e) {
                    g = a[b[e]];
                    h = this['getSquaredDistance'](f['y'], g['y']);
                    if (h > f['fd0'] && h > f['fd1'] && h > f['fd2'] && h > f['ed0'])
                        break;
                    k = this['getSquaredDistance'](f['x'], g['x']);
                    l = h + k;
                    if (f['color'] === g['color']) {
                        this['tryConnectToFriend'](f, g, l);
                        this['tryConnectToFriend'](g, f, l);
                    } else {
                        this['tryConnectToEnemy'](f, g, l);
                        this['tryConnectToEnemy'](g, f, l);
                    }
                }
            }
        }
        ++this['odd'];
        if (this['odd'] === 0x4)
            this['odd'] = 0x0;
    }
    ['removeDeadEntities']() {
        var a = this['entities']['length'];
        for (i = a; i--;)
            if (this['entities'][i]['life'] <= 0x0)
                this['entities']['splice'](i, 0x1);
        this['indices']['length'] = this['entities']['length'];
    }
    ['calculateScores']() {
        this['scores'][0x0] = this['scores'][0x1] = this['scores'][0x2] = this['scores'][0x3] = 0x0;
        var a = this['entities']['length'];
        for (i = a; i--;) {
            ++this['scores'][this['entities'][i]['color']];
        }
    }
    ['update'](a, b, c, d, e) {
        var f = this['entities']['length'];
        var g = 0x0;
        for (g = f; g--;)
            this['entities'][g]['followWave'](this['grid']);
        for (g = f; g--;)
            this['entities'][g]['move'](this['grid']);
        this['updateConnections']();
        for (g = f; g--;)
            this['entities'][g]['behavior'](this['grid']);
        var h = [
            this['scores'][0x0] === 0x0 ? 0x0 : WAVE / this['scores'][0x0],
            this['scores'][0x1] === 0x0 ? 0x0 : WAVE / this['scores'][0x1],
            this['scores'][0x2] === 0x0 ? 0x0 : WAVE / this['scores'][0x2],
            this['scores'][0x3] === 0x0 ? 0x0 : WAVE / this['scores'][0x3]
        ];
        for (g = f; g--;) {
            if (this['entities'][g]['color'] === 0x0)
                this['grid']['wave0'](this['entities'][g]['x'], this['entities'][g]['y'], this['entities'][g]['vx'], this['entities'][g]['vy'], h[0x0]);
            else if (this['entities'][g]['color'] === 0x1)
                this['grid']['wave1'](this['entities'][g]['x'], this['entities'][g]['y'], this['entities'][g]['vx'], this['entities'][g]['vy'], h[0x1]);
            else if (this['entities'][g]['color'] === 0x2)
                this['grid']['wave2'](this['entities'][g]['x'], this['entities'][g]['y'], this['entities'][g]['vx'], this['entities'][g]['vy'], h[0x2]);
            else if (this['entities'][g]['color'] === 0x3)
                this['grid']['wave3'](this['entities'][g]['x'], this['entities'][g]['y'], this['entities'][g]['vx'], this['entities'][g]['vy'], h[0x3]);
        }
        this['removeDeadEntities']();
        this['calculateScores']();
        this['grid']['applyForce'](a, b, c, d, e, d, e, d, e, d, e);
        this['grid']['update'](this['entities']);
        return {
            'entities': this['entities'],
            'grid': this['grid']
        };
    }
}