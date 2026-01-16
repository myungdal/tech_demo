class MovementToFriendSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](MovementComponent['componentId'])['includeComponent'](EnemyComponent['componentId'])['includeComponent'](FriendComponent['componentId'])['exceptComponent'](SkillMotionComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            var k = this['getDataValue'](a, f, 0x3);
            var l = this['getDataValue'](a, f, 0x4);
            var q = this['getDataValue'](a, f, 0x5);
            if (!q['p'])
                continue;
            this['forceForFriend'](g, h, j, k, q['p'], q['dd'], h['characterAttribute']['force_for_friend0'], l['p']);
        }
    }
    ['checkEnemyDirection'](a, b, c) {
        if (!b || !c)
            return ![];
        var d = b['x'] - a['x'];
        var e = b['y'] - a['y'];
        var f = d * d + e * e;
        if (f < ConstAttribute['EPSILON'])
            return ![];
        var g = Math['sqrt'](f);
        if (g < ConstAttribute['EPSILON'])
            return ![];
        d = d / g;
        e = e / g;
        var h = c['x'] - a['x'];
        var i = c['y'] - a['y'];
        var j = h * h + i * i;
        if (j < ConstAttribute['EPSILON'])
            return ![];
        var k = Math['sqrt'](j);
        if (k < ConstAttribute['EPSILON'])
            return ![];
        h = h / k;
        i = i / k;
        var l = d * h + e * i;
        return l > 0x1 - ConstAttribute['PENETRATE_FRIEND'] ? !![] : ![];
    }
    ['checkEnemyDirection'](a, b, c) {
        if (!b || !c)
            return ![];
        var d = c['x'] - a['x'];
        var e = c['y'] - a['y'];
        var f = b['x'] - a['x'];
        var g = b['y'] - a['y'];
        var h = d * g - e * g;
        return h > 0x0 ? 0x1 : -0x1;
    }
    ['forceForFriend'](a, b, f, g, h, i, j, k) {
        if (i < ConstAttribute['EPSILON'])
            return this['resolveEpsilon'](g);
        var l = 0x0;
        var m = 0x0;
        if (i < ConstAttribute['TOO_NEAR'] * ConstAttribute['TOO_NEAR'])
            l = j + g['speed'];
        else if (i < b['characterAttribute']['friend_range_near'] * b['characterAttribute']['friend_range_near']) {
            l = j;
        } else if (i > b['characterAttribute']['friend_range_far'] * b['characterAttribute']['friend_range_far'])
            l = -j;
        if (l === 0x0)
            return;
        var n = Math['sqrt'](i);
        if (n < ConstAttribute['EPSILON'])
            return this['resolveEpsilon'](g);
        var o = f['x'] - h['x'];
        var p = f['y'] - h['y'];
        this['apllyForce'](g, o, p, n, l, m);
    }
    ['resolveEpsilon'](a) {
        a['fx'] += Math['random']() * (ConstAttribute['RESOLVE_EPSILON'] * 0x2) - ConstAttribute['RESOLVE_EPSILON'];
        a['fy'] += Math['random']() * (ConstAttribute['RESOLVE_EPSILON'] * 0x2) - ConstAttribute['RESOLVE_EPSILON'];
    }
    ['apllyForce'](a, b, c, e, f, g) {
        if (g > 0x0) {
            a['fx'] += f * c / e;
            a['fy'] += f * -b / e;
        } else if (g < 0x0) {
            a['fx'] += f * -c / e;
            a['fy'] += f * b / e;
        } else {
            a['fx'] += f * b / e;
            a['fy'] += f * c / e;
        }
    }
}