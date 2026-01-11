class PatrolCell {
    ['c'] = 0x0;
    ['r'] = 0x0;
    ['pathIdx'] = -0x1;
    ['nextPathIdx'] = -0x1;
    ['duration'] = -0x1;
}
class PatrolSystem extends System {
    ['scale'] = 0x0;
    ['pathList'] = [];
    ['firstPath'] = ![];
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](MovementComponent['componentId'])['includeComponent'](PatrolComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['initGrid'](a) {
        this['scale'] = a;
        this['createPath'](0x0, 0x0);
        var b = this['pathList'][0x0];
        b['pathIdx'] = 0x1;
        b['nextPathIdx'] = 0x1;
    }
    ['createPath'](a, b) {
        var d = new PatrolCell();
        d['c'] = a;
        d['r'] = b;
        d['pathIdx'] = this['pathList']['length'];
        d['nextPathIdx'] = -0x1;
        d['duration'] = -0x1;
        this['pathList']['push'](d);
        return d['pathIdx'];
    }
    ['linkPath'](a, b) {
        this['pathList'][a]['nextPathIdx'] = this['pathList'][b]['pathIdx'];
    }
    ['setFirstPath'](a, b) {
        var d = this['pathList'][0x0];
        d['c'] = a;
        d['r'] = b;
        d['pathIdx'] = 0x0;
        d['nextPathIdx'] = 0x1;
        d['duration'] = 0x3e8;
        this['firstPath'] = !![];
    }
    ['update'](a, b) {
        var f = this['pathList'][0x0];
        if (f['duration'] > 0x0) {
            --f['duration'];
            if (f['duration'] === 0x0) {
                f['pathIdx'] = 0x1;
                f['nextPathIdx'] = 0x1;
                f['duration'] = -0x1;
            }
        }
        var g = this['getDataRowCnt'](a);
        if (this['firstPath']) {
            this['firstPath'] = ![];
            for (var h = 0x0; h < g; ++h) {
                var j = this['getDataValue'](a, h, 0x3);
                j['pathIdx'] = 0x0;
            }
        }
        for (var h = 0x0; h < g; ++h) {
            var k = this['getDataValue'](a, h, 0x0);
            var l = this['getDataValue'](a, h, 0x1);
            var m = this['getDataValue'](a, h, 0x2);
            var q = this['getDataValue'](a, h, 0x3);
            if (q['pathIdx'] === -0x1)
                continue;
            if (q['idleDuration'] > 0x0) {
                --q['idleDuration'];
                continue;
            }
            var s = this['pathList'][q['pathIdx']];
            while (s && s['pathIdx'] !== q['pathIdx']) {
                q['pathIdx'] = s['nextPathIdx'];
                s = this['pathList'][s['nextPathIdx']];
            }
            if (!s)
                continue;
            var t = Math['floor'](l['x']) >> this['scale'];
            var u = Math['floor'](l['y']) >> this['scale'];
            if (s['c'] === t && s['r'] === u) {
                if (s['nextPathIdx'] < 0x0) {
                    b['detachComponent'](k['id'], PatrolComponent['componentId']);
                } else {
                    q['pathIdx'] = s['nextPathIdx'];
                    q['idleDuration'] = ConstAttribute['PATROL_IDLE_DURATION'];
                }
                continue;
            }
            s = this['pathList'][q['pathIdx']];
            if (!s)
                continue;
            var v = (s['c'] << this['scale']) + (0x1 << this['scale'] - 0x1);
            var w = (s['r'] << this['scale']) + (0x1 << this['scale'] - 0x1);
            var z = v - l['x'];
            var A = w - l['y'];
            var B = z * z + A * A;
            if (B < ConstAttribute['EPSILON'])
                continue;
            var C = Math['sqrt'](B);
            if (C < ConstAttribute['EPSILON'])
                continue;
            m['vx'] += z * m['speed'] / C;
            m['vy'] += A * m['speed'] / C;
        }
    }
}