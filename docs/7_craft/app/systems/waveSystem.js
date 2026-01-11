class WaveCell {
    ['vx'] = 0x0;
    ['vy'] = 0x0;
    ['pressure'] = 0x0;
    ['c'] = 0x0;
    ['r'] = 0x0;
    ['c7'] = null;
    ['c8'] = null;
    ['c9'] = null;
    ['c4'] = null;
    ['c6'] = null;
    ['c1'] = null;
    ['c2'] = null;
    ['c3'] = null;
    ['updatePressure']() {
        var a = (this['c7']['vx'] + this['c1']['vx'] - this['c9']['vx'] - this['c3']['vx']) * 0.5 + this['c4']['vx'] - this['c6']['vx'];
        var b = (this['c7']['vy'] + this['c9']['vy'] - this['c1']['vy'] - this['c3']['vy']) * 0.5 + this['c8']['vy'] - this['c2']['vy'];
        this['pressure'] = (a + b) * 0.25;
        if (this['pressure'] < ConstAttribute['WAVE_STOP_PRESSURE'])
            this['pressure'] = 0x0;
    }
    ['adaptVelocity']() {
        this['vx'] += ((this['c7']['pressure'] + this['c1']['pressure'] - this['c9']['pressure'] - this['c3']['pressure']) * 0.5 + this['c4']['pressure'] - this['c6']['pressure']) * 0.25;
        this['vy'] += ((this['c7']['pressure'] + this['c9']['pressure'] - this['c1']['pressure'] - this['c3']['pressure']) * 0.5 + this['c8']['pressure'] - this['c2']['pressure']) * 0.25;
        this['vx'] *= ConstAttribute['WAVE_RESISTANCE'];
        this['vy'] *= ConstAttribute['WAVE_RESISTANCE'];
    }
}
class WaveSystem extends System {
    ['scale'] = 0x0;
    ['colCnt'] = 0x0;
    ['rowCnt'] = 0x0;
    ['cells'] = null;
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](MovementComponent['componentId'])['includeComponent'](FlockComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
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
                this['cells'][b][d] = new WaveCell();
        }
        for (b = 0x0; b < this['colCnt']; ++b) {
            for (d = 0x0; d < this['rowCnt']; ++d) {
                var e = this['cells'][b][d];
                e['c'] = b;
                e['r'] = d;
                e['c7'] = this['getCell'](b - 0x1, d - 0x1);
                e['c8'] = this['getCell'](b, d - 0x1);
                e['c9'] = this['getCell'](b + 0x1, d - 0x1);
                e['c4'] = this['getCell'](b - 0x1, d);
                e['c6'] = this['getCell'](b + 0x1, d);
                e['c1'] = this['getCell'](b - 0x1, d + 0x1);
                e['c2'] = this['getCell'](b, d + 0x1);
                e['c3'] = this['getCell'](b + 0x1, d + 0x1);
            }
        }
    }
    ['force'](a, b, e, f, g) {
        var h = this['pick'](a, b);
        for (var i = 0x0; i < this['colCnt']; ++i) {
            for (var j = 0x0; j < this['rowCnt']; ++j) {
                var k = i - h['c'];
                var l = j - h['r'];
                var m = k * k + l * l;
                var n = this['cells'][i][j];
                n['vx'] += m < ConstAttribute['EPSILON'] ? e * f : e * f / m;
                n['vy'] += m < ConstAttribute['EPSILON'] ? e * g : e * g / m;
            }
        }
    }
    ['pressure'](a, b, c) {
        var d = this['pick'](a, b);
        d['pressure'] += c;
    }
    ['update0'](a, b) {
        var d = 0x0;
        var e = 0x0;
        for (d = 0x0; d < this['colCnt']; ++d)
            for (e = 0x0; e < this['rowCnt']; ++e)
                this['cells'][d][e]['updatePressure']();
    }
    ['update1'](a, b) {
        var d = 0x0;
        var e = 0x0;
        for (d = 0x0; d < this['colCnt']; ++d)
            for (e = 0x0; e < this['rowCnt']; ++e)
                this['cells'][d][e]['adaptVelocity']();
    }
    ['update2'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x0);
            var g = this['getDataValue'](a, d, 0x1);
            var h = this['getDataValue'](a, d, 0x2);
            var j = this['pick'](g['x'], g['y']);
            h['vx'] += j['vx'] * ConstAttribute['WAVE_FORCE'];
            h['vy'] += j['vy'] * ConstAttribute['WAVE_FORCE'];
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