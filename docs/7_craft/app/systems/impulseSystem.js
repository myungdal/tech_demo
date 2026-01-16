class ImpulseSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](MovementComponent['componentId'])['includeComponent'](ImpulseComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            var k = this['getDataValue'](a, f, 0x3);
            j['vx'] += k['fx'] / h['characterAttribute']['weight'];
            j['vy'] += k['fy'] / h['characterAttribute']['weight'];
            --k['duration'];
            if (k['duration'] > 0x0)
                continue;
            b['detachComponent'](g['id'], ImpulseComponent['componentId']);
        }
    }
}