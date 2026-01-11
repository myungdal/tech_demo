class Entity {
    ['attribute'] = {};
    ['life'] = 0x0;
    ['state'] = 0x1;
    ['color'] = 0x0;
    ['type'] = 0x0;
    ['x'] = 0x0;
    ['y'] = 0x0;
    ['px'] = 0x0;
    ['py'] = 0x0;
    ['e0'] = null;
    ['ed0'] = 0x0;
    ['f0'] = null;
    ['f1'] = null;
    ['f2'] = null;
    ['fd0'] = 0x0;
    ['fd1'] = 0x0;
    ['fd2'] = 0x0;
    ['vx'] = 0x0;
    ['vy'] = 0x0;
    ['skill0'] = null;
    ['skill1'] = null;
    ['stun_dalay'] = 0x0;
    ['dashState'] = 0x0;
    constructor(a, b, c, d, e) {
        this['x'] = a;
        this['y'] = b;
        this['color'] = c;
        this['type'] = d;
        this['attribute'] = e;
        this['life'] = this['attribute']['MAX_LIFE'];
        this['setupSkill']();
    }
    ['setupSkill']() {
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
    ['preMove'](a) {
        this['px'] = this['x'] + this['vx'];
        this['py'] = this['y'] + this['vy'];
        if (this['px'] < 0x0) {
            this['px'] = 0x0;
            this['vx'] += 0x1;
        } else if (this['px'] >= a['width']) {
            this['px'] = a['width'];
            this['vx'] -= 0x1;
        }
        if (this['py'] < 0x0) {
            this['py'] = 0x0;
            this['vy'] += 0x1;
        } else if (this['py'] >= a['height']) {
            this['py'] = a['height'];
            this['vy'] -= 0x1;
        }
        this['vx'] *= MOVE_RESISTANCE;
        this['vy'] *= MOVE_RESISTANCE;
    }
    ['follow'](a) {
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
    ['resolveOverlap']() {
        if (this['life'] <= 0x0)
            return;
        this['forceByOverlap'](this['f0'], this['fd0']);
        this['forceByOverlap'](this['f1'], this['fd1']);
        this['forceByOverlap'](this['f2'], this['fd2']);
        this['forceByOverlap'](this['e0'], this['ed0']);
    }
    ['forceByOverlap'](c, e) {
        var f = this;
        if (!c)
            return;
        if (c['life'] <= 0x0)
            return;
        if (f['life'] <= 0x0)
            return;
        var g = f['x'] - c['x'];
        var h = f['y'] - c['y'];
        var i = 0x0;
        if (e < 0x5 * 0x5) {
            if (e < EPSILON) {
                this['vx'] += 0.001 * (Math['random']() - 0.5);
                this['vy'] += 0.001 * (Math['random']() - 0.5);
            } else {
                i = Math['sqrt'](e);
                if (i < EPSILON) {
                    this['vx'] += 0.001 * (Math['random']() - 0.5);
                    this['vy'] += 0.001 * (Math['random']() - 0.5);
                } else {
                    this['vx'] += 0.03 * g / i;
                    this['vy'] += 0.03 * h / i;
                }
            }
        }
    }
    ['behavior'](a) {
        if (this['life'] <= 0x0)
            return;
        if (this['stun_dalay'] <= 0x0) {
            this['updateForce']();
            if (this['skill1']['tryStart'](this, this['attribute']['skill1'], a) === ![]) {
                this['skill0']['tryStart'](this, this['attribute']['skill0'], a);
            }
            this['skill1']['update'](this, this['attribute']['skill1'], a);
            this['skill0']['update'](this, this['attribute']['skill0'], a);
        }
        if (this['stun_dalay'] > 0x0)
            this['stun_dalay'] -= 0x1;
        if (this['state'] < 0x0 && (!this['f0'] || this['f0']['life'] > this['f0']['attribute']['RUNAWAY_LIFE']) && (!this['f1'] || this['f1']['life'] > this['f1']['attribute']['RUNAWAY_LIFE']) && (!this['f2'] || this['f2']['life'] > this['f2']['attribute']['RUNAWAY_LIFE']) && this['life'] >= this['attribute']['MAX_LIFE']) {
            this['state'] = 0x1;
        } else {
            this['state'] = this['state'] * 0.998 + 0x1 * 0.002;
        }
    }
    ['updateForce']() {
        var a = {
            'x': 0x0,
            'y': 0x0,
            'type': ''
        };
        var b = {
            'x': 0x0,
            'y': 0x0,
            'type': ''
        };
        var c = {
            'x': 0x0,
            'y': 0x0,
            'type': ''
        };
        var d = {
            'x': 0x0,
            'y': 0x0,
            'type': ''
        };
        this['forceForFriend'](a, this['f0'], this['fd0'], this['attribute']['SPEED1']);
        this['forceForFriend'](b, this['f1'], this['fd1'], this['attribute']['SPEED1']);
        this['forceForFriend'](c, this['f2'], this['fd2'], this['attribute']['SPEED1']);
        this['forceForEnemy'](d, this['e0'], this['ed0'], this['attribute']['SPEED0'], this['attribute']['SPEED0'] * 1.5);
        this['applyForce']({
            'x': a['x'] + b['x'] + c['x'],
            'y': a['y'] + b['y'] + c['y']
        });
        this['applyForce'](d);
    }
    ['applyForce'](a) {
        var b = 0x0;
        var c = 0x0;
        c = a['x'] * a['x'] + a['y'] * a['y'];
        if (c > EPSILON) {
            b = Math['sqrt'](c);
            if (b > EPSILON && b > this['attribute']['VELLOCITY'] + this['dashState']) {
                a['x'] /= b;
                a['y'] /= b;
            }
        }
        this['vx'] += a['x'] * (this['attribute']['VELLOCITY'] + this['dashState']);
        this['vy'] += a['y'] * (this['attribute']['VELLOCITY'] + this['dashState']);
    }
    ['forceForEnemy'](c, e, f, g, h) {
        var i = this;
        if (!e)
            return;
        if (e['life'] <= 0x0)
            return;
        if (i['life'] <= 0x0)
            return;
        var j = i['x'] - e['x'];
        var k = i['y'] - e['y'];
        var l = 0x0;
        if (f < TOO_NEAR_ENEMY) {
            if (f < EPSILON) {
                c['type'] = 'EPSILON';
            } else {
                l = Math['sqrt'](f);
                if (l < EPSILON) {
                    c['type'] = 'EPSILON';
                } else {
                    c['x'] += g * j / l;
                    c['y'] += g * k / l;
                    c['type'] = 'TOO_NEAR';
                }
            }
        } else {
            l = Math['sqrt'](f);
            if (l < EPSILON) {
                c['type'] = 'EPSILON';
            } else if (i['state'] < 0x0) {
                c['x'] += h * (0.5 * j / l + 0.5 * j / f);
                c['y'] += h * (0.5 * k / l + 0.5 * k / f);
                c['type'] = 'RUN_AWAY';
            } else if (f > this['attribute']['ENEMY_RANGE_SQUARE']) {
                c['x'] -= g * j / l;
                c['y'] -= g * k / l;
                c['type'] = 'GO_AHEAD';
            } else {
                c['x'] += g * (0.5 * j / l + 0.5 * j / f);
                c['y'] += g * (0.5 * k / l + 0.5 * k / f);
                c['type'] = 'GO_BACK';
            }
        }
    }
    ['forceForFriend'](c, e, f, g) {
        var h = this;
        if (!e)
            return;
        if (e['life'] <= 0x0)
            return;
        if (h['life'] <= 0x0)
            return;
        var i = h['x'] - e['x'];
        var j = h['y'] - e['y'];
        var k = 0x0;
        if (f < TOO_NEAR_FRIEND) {
            if (f < EPSILON) {
                c['type'] = 'EPSILON';
            } else {
                k = Math['sqrt'](f);
                if (k < EPSILON) {
                    c['type'] = 'EPSILON';
                } else {
                    c['x'] += g * i / k;
                    c['y'] += g * j / k;
                    c['type'] = 'TOO_NEAR';
                }
            }
        } else {
            k = Math['sqrt'](f);
            if (k < EPSILON) {
                c['type'] = 'EPSILON';
            } else if (f > this['attribute']['FRIEND_RANGE_SQUARE']) {
                c['x'] -= g * i / k;
                c['y'] -= g * j / k;
                c['type'] = 'GO_AHEAD';
            }
        }
    }
    ['cutConnections']() {
        this['e0'] = null;
        this['ed0'] = Number['MAX_VALUE'];
        this['f0'] = this['f1'] = this['f2'] = null;
        this['fd0'] = this['fd1'] = this['fd2'] = Number['MAX_VALUE'];
    }
    ['isNearestCompleted'](a) {
        return a > this['fd0'] && a > this['fd1'] && a > this['fd2'] && a > this['ed0'];
    }
    ['tryConnectToFriend'](c, d) {
        var e = this;
        if (d >= e['fd2'])
            return;
        if (d >= e['fd1']) {
            e['fd2'] = d;
            e['f2'] = c;
            return;
        }
        if (d >= e['fd0']) {
            e['fd2'] = e['fd1'];
            e['f2'] = e['f1'];
            e['fd1'] = d;
            e['f1'] = c;
            return;
        }
        e['fd2'] = e['fd1'];
        e['f2'] = e['f1'];
        e['fd1'] = e['fd0'];
        e['f1'] = e['f0'];
        e['fd0'] = d;
        e['f0'] = c;
    }
    ['tryConnectToEnemy'](c, d) {
        var e = this;
        if (d < e['ed0']) {
            e['ed0'] = d;
            e['e0'] = c;
        }
    }
}
class Entity0 extends Entity {
    ['setupSkill']() {
        this['skill0'] = new Skill_attack(this['attribute']['skill0']['INIT_ENERGY']);
        this['skill1'] = new Skill_dash(this['attribute']['skill1']['INIT_ENERGY']);
    }
}
class Entity1 extends Entity {
    ['setupSkill']() {
        this['skill0'] = new Skill_attack(this['attribute']['skill0']['INIT_ENERGY']);
        this['skill1'] = new Skill_attack(this['attribute']['skill1']['INIT_ENERGY']);
    }
}
class Entity2 extends Entity {
    ['setupSkill']() {
        this['skill0'] = new Skill_attack(this['attribute']['skill0']['INIT_ENERGY']);
        this['skill1'] = new Skill_heal(this['attribute']['skill1']['INIT_ENERGY']);
    }
    ['tryConnectToFriend'](c, d) {
        var e = this;
        if (d >= e['fd2'])
            return;
        if (d >= e['fd1']) {
            if (c['type'] === 0x0) {
                e['fd2'] = d;
                e['f2'] = c;
            }
            return;
        }
        if (d >= e['fd0']) {
            if (e['f1'] && e['f1']['type'] === 0x0) {
                e['fd2'] = e['fd1'];
                e['f2'] = e['f1'];
            }
            e['fd1'] = d;
            e['f1'] = c;
            return;
        }
        if (e['f1'] && e['f1']['type'] === 0x0) {
            e['fd2'] = e['fd1'];
            e['f2'] = e['f1'];
        }
        e['fd1'] = e['fd0'];
        e['f1'] = e['f0'];
        e['fd0'] = d;
        e['f0'] = c;
    }
}