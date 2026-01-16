class MovementToEnemySystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](MovementComponent['componentId'])['includeComponent'](EnemyComponent['componentId'])['exceptComponent'](SkillMotionComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId'])['exceptComponent'](PatrolComponent['componentId']);
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
            if (l['p'])
                this['forceForEnemy'](g, h, j, k, l['p'], l['dd'], h['characterAttribute']['force_for_enemy']);
        }
    }
    ['forceForEnemy'](a, b, f, g, h, i, j) {
        if (i < ConstAttribute['EPSILON'])
            return this['resolveEpsilon'](g);
        var k = 0x0;
        if (i < ConstAttribute['TOO_NEAR'] * ConstAttribute['TOO_NEAR'])
            k = j + g['speed'];
        else if (a['hasComponent'](DashEffectComponent['componentId']))
            k = -j;
        else if (a['hasComponent'](RunawayEffectComponent['componentId']))
            k = j;
        else if (i < b['characterAttribute']['enemy_range_near'] * b['characterAttribute']['enemy_range_near'])
            k = j;
        else if (i > b['characterAttribute']['enemy_range_far'] * b['characterAttribute']['enemy_range_far'])
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