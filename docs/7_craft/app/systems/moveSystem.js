class MoveSystem extends System {
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](MovementComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var e = 0x0; e < c; ++e) {
            var f = this['getDataValue'](a, e, 0x1);
            var g = this['getDataValue'](a, e, 0x2);
            var h = g['fx'] * g['fx'] + g['fy'] * g['fy'];
            if (h > ConstAttribute['EPSILON']) {
                var j = Math['sqrt'](h);
                if (j > ConstAttribute['EPSILON']) {
                    g['vx'] += g['fx'] * g['speed'] / j;
                    g['vy'] += g['fy'] * g['speed'] / j;
                }
            }
            g['px'] = f['x'];
            g['py'] = f['y'];
            f['x'] += g['vx'];
            f['y'] += g['vy'];
        }
    }
}