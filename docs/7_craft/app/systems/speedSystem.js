class SpeedSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](MovementComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var e = 0x0; e < d; ++e) {
            var f = this['getDataValue'](a, e, 0x1);
            var g = this['getDataValue'](a, e, 0x2);
            g['fx'] = 0x0;
            g['fy'] = 0x0;
            g['speed'] = f['characterAttribute']['speed'];
        }
    }
}