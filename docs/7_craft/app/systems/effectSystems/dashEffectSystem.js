class DashEffectSystem extends System {
    ['init'](a) {
        this['includeComponent'](MovementComponent['componentId'])['includeComponent'](DashEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x0);
            var g = this['getDataValue'](a, d, 0x1);
            var h = this['getDataValue'](a, d, 0x2);
            g['speed'] += ConstAttribute['DASH_SPEED'];
            --h['duration'];
            if (h['duration'] > 0x0)
                continue;
            b['detachComponent'](f['id'], DashEffectComponent['componentId']);
        }
    }
}