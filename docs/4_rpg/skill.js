class Skill {
    ['energy'] = 0x0;
    ['pre_delay'] = 0x0;
    ['post_delay'] = 0x0;
    ['target'] = null;
    constructor(a) {
        this['energy'] = a;
    }
    ['tryStart'](c, d, e) {
        if (this['pre_delay'] > 0x0 || this['post_delay'] > 0x0)
            return ![];
        var f = c;
        var g = f['e0'];
        var h = f['ed0'];
        if (!g)
            return ![];
        if (g['life'] <= 0x0)
            return ![];
        if (f['life'] <= 0x0)
            return ![];
        if (g['color'] === f['color'])
            return ![];
        if (h > d['RANGE_SQUARE'])
            return ![];
        if (this['energy'] < d['ENERGY_SPEND'])
            return ![];
        this['energy'] -= d['ENERGY_SPEND'];
        this['target'] = g;
        this['pre_delay'] = d['PRE_DELAY'];
        this['post_delay'] = d['POST_DELAY'];
        return !![];
    }
    ['update'](a, b, c) {
        if (++this['energy'] > b['MAX_ENERGY'])
            this['energy'] = b['MAX_ENERGY'];
        if (this['pre_delay'] > 0x0) {
            this['pre_delay'] -= 0x1;
            if (this['pre_delay'] <= 0x0) {
                this['pre_delay'] = 0x0;
                this['act'](a, b, c);
            }
            return !![];
        } else if (this['post_delay'] > 0x0) {
            this['post_delay'] -= 0x1;
            if (this['post_delay'] <= 0x0) {
                this['post_delay'] = 0x0;
                this['finish'](a, b, c);
            }
            return !![];
        }
        return ![];
    }
    ['act'](c, d, e) {
        var f = c;
        var g = this['target'];
        if (!g || g['life'] <= 0x0 || f['life'] <= 0x0) {
            this['post_delay'] = 0x0;
            return;
        }
        if (g['life'] < f['attribute']['RUNAWAY_LIFE'])
            g['state'] = -0x1;
        g['life'] -= d['POWER'] * (0x1 - g['attribute']['ARMOR_DAMAGE_DECREMENT']);
        if (g['life'] < 0x0)
            g['life'] = 0x0;
        if (g['attribute']['STUN_PNT_DECREMENT'] * d['STUN_PNT'] < Math['random']())
            g['stun_dalay'] = (0x1 - g['attribute']['STUN_DELAY_DECREMENT']) * d['STUN'];
        e['push'](new Event(0x0, 0xa, f['x'], f['y'], g['x'], g['y']));
    }
    ['finish'](a, b, c) {
    }
}
class Skill_attack extends Skill {
}
class Skill_dash extends Skill {
    ['x'] = 0x0;
    ['y'] = 0x0;
    ['tryStart'](c, e, f) {
        var g = super['tryStart'](c, e, f);
        if (g === !![]) {
            var h = c;
            var i = h['e0'];
            var j = h['ed0'];
            var k = h['x'] - i['x'];
            var l = h['y'] - i['y'];
            if (j > EPSILON) {
                var m = Math['sqrt'](j);
                if (m > EPSILON) {
                    this['x'] = k / m;
                    this['y'] = l / m;
                }
            }
            h['dashState'] = 1.5;
        }
        return g;
    }
    ['update'](a, b, c) {
        if (++this['energy'] > b['MAX_ENERGY'])
            this['energy'] = b['MAX_ENERGY'];
        if (this['pre_delay'] > 0x0) {
            this['vx'] += this['x'];
            this['vy'] += this['y'];
            this['pre_delay'] -= 0x1;
            if (this['pre_delay'] <= 0x0) {
                this['pre_delay'] = 0x0;
                this['act'](a, b, c);
                a['dashState'] = 0x0;
            }
            return !![];
        } else if (this['post_delay'] > 0x0) {
            this['post_delay'] -= 0x1;
            if (this['post_delay'] <= 0x0) {
                this['post_delay'] = 0x0;
                this['finish'](a, b, c);
            }
            return !![];
        }
        return ![];
    }
}
class Skill_heal extends Skill {
    ['tryStart'](a, b, c) {
        if (this['state'] < 0x0 || this['pre_delay'] > 0x0 || this['post_delay'] > 0x0)
            return ![];
        if (this['_tryStart'](a, a['f0'], a['fd0'], b, c) === !![])
            return !![];
        if (this['_tryStart'](a, a['f1'], a['fd1'], b, c) === !![])
            return !![];
        if (this['_tryStart'](a, a['f2'], a['fd2'], b, c) === !![])
            return !![];
        return ![];
    }
    ['_tryStart'](c, d, e, f, g) {
        if (!d)
            return ![];
        if (d['life'] <= 0x0)
            return ![];
        if (c['life'] <= 0x0)
            return ![];
        if (d['color'] !== c['color'])
            return ![];
        if (e > f['RANGE_SQUARE'])
            return ![];
        if (this['energy'] < f['ENERGY_SPEND'])
            return ![];
        if (d['life'] >= d['attribute']['MAX_LIFE'])
            return ![];
        this['energy'] -= f['ENERGY_SPEND'];
        this['target'] = d;
        this['pre_delay'] = f['PRE_DELAY'];
        this['post_delay'] = f['POST_DELAY'];
        return !![];
    }
    ['act'](c, d, e) {
        var f = c;
        var g = this['target'];
        if (!g || g['life'] <= 0x0 || f['life'] <= 0x0) {
            this['post_delay'] = 0x0;
            return;
        }
        g['life'] += d['POWER'];
        if (g['life'] > g['attribute']['MAX_LIFE'])
            g['life'] = g['attribute']['MAX_LIFE'];
        e['push'](new Event(0x1, 0xa, f['x'], f['y'], g['x'], g['y']));
    }
}