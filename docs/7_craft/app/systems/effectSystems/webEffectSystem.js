class WebEffectSystem extends System {
    ['init'](a) {
        this['includeComponent'](WebEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x0);
            var g = this['getDataValue'](a, d, 0x1);
            --g['duration'];
            if (g['duration'] > 0x0)
                continue;
            b['detachComponent'](f['id'], WebEffectComponent['componentId']);
        }
    }
}