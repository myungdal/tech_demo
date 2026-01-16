class RecoverySystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](RecoveryComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            if (h['life'] <= 0x0)
                continue;
            if (h['life'] <= h['characterAttribute']['recovery_max_life']) {
                h['life'] += h['characterAttribute']['recovery'];
            }
        }
    }
}