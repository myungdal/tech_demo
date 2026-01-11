class HealingEffectSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](HealingEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            h['life'] += j['power'] / ConstAttribute['HEALING_DURATION'];
            if (h['life'] > h['characterAttribute']['max_life']) {
                h['life'] = h['characterAttribute']['max_life'];
            }
            --j['duration'];
            if (j['duration'] > 0x0)
                continue;
            b['detachComponent'](g['id'], HealingEffectComponent['componentId']);
        }
    }
}