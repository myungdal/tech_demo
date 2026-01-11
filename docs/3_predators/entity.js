class Entity {
    ['attribute'] = {};
    ['life'] = 0x0;
    ['team'] = 0x0;
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
    ['state_runaway'] = 0x0;
    ['state_stun'] = 0x0;
    ['state_electricShock'] = 0x0;
    ['state_dash'] = 0x0;
    ['effects'] = [];
    constructor(a, b, c, d, e) {
        this['x'] = a;
        this['y'] = b;
        this['team'] = c;
        this['type'] = d;
        this['attribute'] = e;
        this['life'] = this['attribute']['MAX_LIFE'];
        this['setupSkill']();
    }
    ['setupSkill']() {
    }
    ['move'](a, b) {
        this['x'] += this['vx'];
        this['y'] += this['vy'];
        if (this['x'] < 0x0) {
            this['x'] = 0x0;
            this['vx'] += 0x1;
        } else if (this['x'] >= a) {
            this['x'] = a;
            this['vx'] -= 0x1;
        }
        if (this['y'] < 0x0) {
            this['y'] = 0x0;
            this['vy'] += 0x1;
        } else if (this['y'] >= b) {
            this['y'] = b;
            this['vy'] -= 0x1;
        }
        this['vx'] *= MOVE_RESISTANCE;
        this['vy'] *= MOVE_RESISTANCE;
    }
    ['resolveEpsilon']() {
        this['vx'] += 0.01 * (Math['random']() - 0.5);
        this['vy'] += 0.01 * (Math['random']() - 0.5);
    }
    ['behavior'](a) {
        if (this['life'] <= 0x0)
            return;
        if (this['state_stun'] === 0x0 && this['state_electricShock'] === 0x0) {
            this['updateForce']();
            this['skill0']['tryStart'](this, this['attribute']['skill0'], a);
            this['skill0']['update'](this, this['attribute']['skill0'], a);
        }
        var b = this['effects']['length'];
        for (var c = b; c--;) {
            if (this['effects'][c]['update'](this, a) === ![])
                this['effects']['splice'](c, 0x1);
        }
        ++this['life'];
        if (this['life'] > this['attribute']['MAX_LIFE'])
            this['life'] = this['attribute']['MAX_LIFE'];
    }
    ['updateForce']() {
        var a = {
            'x': 0x0,
            'y': 0x0
        };
        var b = {
            'x': 0x0,
            'y': 0x0
        };
        var c = {
            'x': 0x0,
            'y': 0x0
        };
        var d = {
            'x': 0x0,
            'y': 0x0
        };
        this['forceForFriend'](a, this['f0'], this['fd0'], this['attribute']['FORCE_FOR_FRIEND0']);
        this['forceForFriend'](b, this['f1'], this['fd1'], this['attribute']['FORCE_FOR_FRIEND1']);
        this['forceForFriend'](c, this['f2'], this['fd2'], this['attribute']['FORCE_FOR_FRIEND2']);
        this['forceForEnemy'](d, this['e0'], this['ed0'], this['attribute']['FORCE_FOR_ENEMY']);
        this['applyForce']({
            'x': a['x'] + b['x'] + c['x'] + d['x'],
            'y': a['y'] + b['y'] + c['y'] + d['y']
        });
    }
    ['applyForce'](a) {
        var b = 0x0;
        var c = 0x0;
        c = a['x'] * a['x'] + a['y'] * a['y'];
        if (c > EPSILON) {
            b = Math['sqrt'](c);
            var e = this['attribute']['VELOCITY'];
            if (this['state_dash'] > 0x0)
                e += DASH_VELOCITY;
            if (b > EPSILON && b > e) {
                a['x'] /= b;
                a['y'] /= b;
                this['vx'] += a['x'] * e;
                this['vy'] += a['y'] * e;
            } else {
                this['vx'] += a['x'];
                this['vy'] += a['y'];
            }
        }
    }
    ['forceForEnemy'](c, e, f, g) {
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
        if (f < TOO_NEAR_ENEMY) {
            if (f < EPSILON)
                return this['resolveEpsilon']();
            k = Math['sqrt'](f);
            if (k < EPSILON)
                return this['resolveEpsilon']();
            c['x'] += g * i / k;
            c['y'] += g * j / k;
            return;
        }
        k = Math['sqrt'](f);
        if (k < EPSILON)
            return this['resolveEpsilon']();
        if (h['state_runaway'] > 0x0 || f < h['attribute']['ENEMY_RANGE_SQUARE']) {
            c['x'] += g * i / k;
            c['y'] += g * j / k;
            return;
        }
        if (h['life'] > h['attribute']['RUNAWAY_LIFE']) {
            c['x'] -= g * i / k;
            c['y'] -= g * j / k;
            return;
        }
        if (f < h['attribute']['ENEMY_RANGE_SQUARE'] + e['attribute']['ENEMY_RANGE_SQUARE']) {
            c['x'] += g * i / k;
            c['y'] += g * j / k;
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
            if (f < EPSILON)
                return this['resolveEpsilon']();
            k = Math['sqrt'](f);
            if (k < EPSILON)
                return this['resolveEpsilon']();
            c['x'] += g * i / k;
            c['y'] += g * j / k;
            return;
        }
        k = Math['sqrt'](f);
        if (k < EPSILON)
            return this['resolveEpsilon']();
        if (f > this['attribute']['FRIEND_RANGE_SQUARE']) {
            c['x'] -= g * i / k;
            c['y'] -= g * j / k;
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
        if (c['f0'] === e || c['f1'] === e || c['f2'] === e)
            return;
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
    ['tryRunaway'](a) {
        if (this['state_runaway'] > 0x0)
            return;
        if (this['life'] > this['attribute']['RUNAWAY_LIFE'])
            return;
        this['effects']['push'](new Effect_runaway(this, a, RUNAWAY_DURATION));
    }
}
class Entity0 extends Entity {
    ['setupSkill']() {
        this['skill0'] = new Skill_attack(this['attribute']['skill0']['INIT_ENERGY']);
    }
}