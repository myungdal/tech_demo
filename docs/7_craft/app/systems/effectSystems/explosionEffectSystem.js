class ExplosionEffectSystem extends System {
    ['enemySystem'] = null;
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](ExplosionEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var c = 0x0;
        var d = this['enemySystem']['indices']['length'];
        var f = b['systemManager']['dataViews'][this['enemySystem']['componentsInfo']['key']];
        var g = this['getDataRowCnt'](a);
        for (var h = 0x0; h < g; ++h) {
            var k = this['getDataValue'](a, h, 0x0);
            var l = this['getDataValue'](a, h, 0x1);
            var m = this['getDataValue'](a, h, 0x2);
            --m['duration'];
            if (m['duration'] > 0x0)
                continue;
            b['destroyEntity'](k['id']);
            if (d > 0x0) {
                var o = this['enemySystem']['binarySearch'](f, l);
                if (o < 0x0)
                    continue;
                for (c = o; c >= 0x0; --c) {
                    if (!this['processExplosion'](c, f, l, m))
                        break;
                }
                for (c = o + 0x1; c < d; ++c) {
                    if (!this['processExplosion'](c, f, l, m))
                        break;
                }
            }
        }
    }
    ['processExplosion'](a, b, c, e) {
        var f = this['enemySystem']['indices'][a];
        var g = this['enemySystem']['getDataValue'](b, f, 0x2);
        var h = this['enemySystem']['getSquare0'](c, g);
        if (h > ConstAttribute['EXPLOSION_RANGE'] * ConstAttribute['EXPLOSION_RANGE'])
            return ![];
        var i = this['enemySystem']['getSquare1'](c, g);
        var k = h + i;
        if (k > ConstAttribute['EXPLOSION_RANGE'] * ConstAttribute['EXPLOSION_RANGE'])
            return !![];
        var l = g['x'] - c['x'];
        var m = g['y'] - c['y'];
        if (k > ConstAttribute['EPSILON']) {
            var n = Math['sqrt'](k);
            if (n > ConstAttribute['EPSILON']) {
                l = l / n;
                m = m / n;
            } else {
                l = Math['random']() * 0x2 - 0x1;
                m = Math['random']() * 0x2 - 0x1;
            }
        } else {
            l = Math['random']() * 0x2 - 0x1;
            m = Math['random']() * 0x2 - 0x1;
        }
        var o = this['enemySystem']['getDataValue'](b, f, 0x0);
        var p = engine['attachComponent'](o['id'], ImpulseComponent['componentId']);
        if (p) {
            p['fx'] = 0x0;
            p['fy'] = 0x0;
        } else {
            p = engine['getComponent'](o['id'], ImpulseComponent['componentId']);
        }
        if (p) {
            p['duration'] = 0x1;
            p['fx'] += l * ConstAttribute['EXPLOSION_FORCE'];
            p['fy'] += m * ConstAttribute['EXPLOSION_FORCE'];
        }
        var q = engine['attachComponent'](o['id'], DamageComponent['componentId']);
        if (q) {
            q['power'] = 0x0;
            q['range'] = 0x0;
            q['stun_chance'] = 0x0;
        } else {
            q = engine['getComponent'](o['id'], DamageComponent['componentId']);
        }
        if (q) {
            q['power'] += e['power'];
            q['range'] = 0x0;
            q['stun_chance'] = 0x1;
            q['team'] = e['team'];
            q['characterType'] = e['characterType'];
        }
        return !![];
    }
}