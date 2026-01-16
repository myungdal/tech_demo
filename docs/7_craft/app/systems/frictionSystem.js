class FrictionSystem extends System {
    ['scale'] = 0x0;
    ['colCnt'] = 0x0;
    ['rowCnt'] = 0x0;
    ['cells'] = null;
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](MovementComponent['componentId']);
        return super['init'](a);
    }
    ['initGrid'](a) {
        this['scale'] = a;
        this['colCnt'] = ConstAttribute['WIDTH'] >> this['scale'];
        this['rowCnt'] = ConstAttribute['HEIGHT'] >> this['scale'];
        this['cells'] = new Array(this['colCnt']);
        var b = 0x0;
        var d = 0x0;
        for (b = 0x0; b < this['colCnt']; ++b) {
            this['cells'][b] = new Array(this['rowCnt']);
            for (d = 0x0; d < this['rowCnt']; ++d)
                this['cells'][b][d] = ConstAttribute['DEFAULT_FRICTION'];
        }
    }
    ['setFriction'](a, b, d) {
        this['cells'][a][b] = d;
    }
    ['update'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var e = this['getDataValue'](a, d, 0x1);
            var f = this['getDataValue'](a, d, 0x2);
            var g = this['pick'](e['x'], e['y']);
            if (!g)
                continue;
            f['vx'] = f['vx'] * g;
            f['vy'] = f['vy'] * g;
        }
    }
    ['pick'](a, b) {
        var d = Math['floor'](a) >> this['scale'];
        var e = Math['floor'](b) >> this['scale'];
        return this['getCell'](d, e);
    }
    ['getCell'](a, b) {
        if (a < 0x0)
            a = 0x0;
        else if (a >= this['colCnt'])
            a = this['colCnt'] - 0x1;
        if (b < 0x0)
            b = 0x0;
        else if (b >= this['rowCnt'])
            b = this['rowCnt'] - 0x1;
        return this['cells'][a][b];
    }
}