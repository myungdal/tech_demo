class MovementToFlockSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](MovementComponent['componentId'])['includeComponent'](FlockComponent['componentId'])['exceptComponent'](SkillMotionComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
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
            if (l['p0'])
                this['forceForFriend'](g, h, j, k, l['p0'], l['dd0'], h['characterAttribute']['force_for_friend0']);
            if (l['p1'])
                this['forceForFriend'](g, h, j, k, l['p1'], l['dd1'], h['characterAttribute']['force_for_friend1']);
            if (l['p2'])
                this['forceForFriend'](g, h, j, k, l['p2'], l['dd2'], h['characterAttribute']['force_for_friend2']);
        }
    }
    ['forceForFriend'](a, b, f, g, h, i, j) {
        if (i < ConstAttribute['EPSILON'])
            return this['resolveEpsilon'](g);
        var k = 0x0;
        if (i < ConstAttribute['FLOCK_TOO_NEAR'] * ConstAttribute['FLOCK_TOO_NEAR'])
            k = j + g['speed'];
        else if (i < b['characterAttribute']['friend_range_near'] * b['characterAttribute']['friend_range_near'])
            k = j;
        else if (i > b['characterAttribute']['friend_range_far'] * b['characterAttribute']['friend_range_far'])
            k = -j;
        if (k === 0x0)
            return;
        var l = Math['sqrt'](i);
        if (l < ConstAttribute['EPSILON'])
            return this['resolveEpsilon'](g);
        var m = f['x'] - h['x'];
        var n = f['y'] - h['y'];
        this['apllyForce'](g, m, n, l, k);
    }
    ['resolveEpsilon'](a) {
        a['fx'] += Math['random']() * (ConstAttribute['RESOLVE_EPSILON'] * 0x2) - ConstAttribute['RESOLVE_EPSILON'];
        a['fy'] += Math['random']() * (ConstAttribute['RESOLVE_EPSILON'] * 0x2) - ConstAttribute['RESOLVE_EPSILON'];
    }
    ['apllyForce'](a, b, c, e, f) {
        a['fx'] += f * b / e;
        a['fy'] += f * c / e;
    }
}