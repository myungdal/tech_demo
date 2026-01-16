class MagnetEffectSystem extends System {
    ['enemySystem'] = null;
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](MagnetEffectComponent['componentId']);
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
            --m['timer'];
            if (m['timer'] > 0x0)
                continue;
            if (d > 0x0) {
                var o = this['enemySystem']['binarySearch'](f, l);
                if (o < 0x0)
                    continue;
                for (c = o; c >= 0x0; --c) {
                    if (!this['processMagnet'](c, f, l, m))
                        break;
                }
                for (c = o + 0x1; c < d; ++c) {
                    if (!this['processMagnet'](c, f, l, m))
                        break;
                }
            }
            --m['duration'];
            if (m['duration'] > 0x0)
                continue;
            b['destroyEntity'](k['id']);
        }
    }
    ['processMagnet'](a, b, c, e) {
        var f = this['enemySystem']['indices'][a];
        var g = this['enemySystem']['getDataValue'](b, f, 0x0);
        var h = this['enemySystem']['getDataValue'](b, f, 0x1);
        var i = this['enemySystem']['getDataValue'](b, f, 0x2);
        var k = this['enemySystem']['getSquare0'](c, i);
        if (k > ConstAttribute['MAGNET_RANGE'] * ConstAttribute['MAGNET_RANGE'])
            return ![];
        var l = this['enemySystem']['getSquare1'](c, i);
        var m = k + l;
        if (m > ConstAttribute['MAGNET_RANGE'] * ConstAttribute['MAGNET_RANGE'])
            return !![];
        var n = i['x'] - c['x'];
        var o = i['y'] - c['y'];
        if (m > ConstAttribute['EPSILON']) {
            var p = Math['sqrt'](m);
            if (p > ConstAttribute['EPSILON']) {
                n = n / p;
                o = o / p;
            } else {
                n = Math['random']() * 0x2 - 0x1;
                o = Math['random']() * 0x2 - 0x1;
            }
        } else {
            n = Math['random']() * 0x2 - 0x1;
            o = Math['random']() * 0x2 - 0x1;
        }
        if (g['hasComponent'](MovementComponent['componentId'])) {
            var q = engine['getComponent'](g['id'], MovementComponent['componentId']);
            if (q) {
                q['vx'] -= n * ConstAttribute['MAGNET_FORCE'] / h['characterAttribute']['weight'];
                q['vy'] -= o * ConstAttribute['MAGNET_FORCE'] / h['characterAttribute']['weight'];
            }
        }
        return !![];
    }
}