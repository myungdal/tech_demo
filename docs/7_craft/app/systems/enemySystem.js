class EnemySystem extends System {
    ['odd'] = 0x0;
    ['indices'] = [];
    ['getSquare0'] = null;
    ['getSquare1'] = null;
    ['binarySearch'] = null;
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](EnemyComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        ++this['odd'];
        if (this['odd'] === 0x4)
            this['odd'] = 0x0;
        var c = 0x0;
        var d = 0x0;
        var e = this['getDataRowCnt'](a);
        this['indices']['length'] = e;
        for (c = 0x0; c < e; ++c) {
            var f = this['getDataValue'](a, c, 0x3);
            f['dd'] = Number['MAX_VALUE'];
            f['e'] = null;
            f['c'] = null;
            f['p'] = null;
            f['y'] = null;
            f['idx'] = -0x1;
        }
        for (c = 0x0; c < e; ++c)
            this['indices'][c] = c;
        if (this['odd'] === 0x0) {
            this['indices']['sort'](function (o, p) {
                var q = this['getDataValue'](a, o, 0x2);
                var r = this['getDataValue'](a, p, 0x2);
                var s = q['x'] - r['x'];
                return s !== 0x0 ? s : o - p;
            }['bind'](this));
        } else if (this['odd'] === 0x1) {
            this['indices']['sort'](function (o, p) {
                var q = this['getDataValue'](a, p, 0x2);
                var r = this['getDataValue'](a, o, 0x2);
                var s = q['x'] - r['x'];
                return s !== 0x0 ? s : p - o;
            }['bind'](this));
        } else if (this['odd'] === 0x2) {
            this['indices']['sort'](function (o, p) {
                var q = this['getDataValue'](a, o, 0x2);
                var r = this['getDataValue'](a, p, 0x2);
                var s = q['y'] - r['y'];
                return s !== 0x0 ? s : o - p;
            }['bind'](this));
        } else if (this['odd'] === 0x3) {
            this['indices']['sort'](function (o, p) {
                var q = this['getDataValue'](a, p, 0x2);
                var r = this['getDataValue'](a, o, 0x2);
                var s = q['y'] - r['y'];
                return s !== 0x0 ? s : p - o;
            }['bind'](this));
        }
        if (this['odd'] === 0x0 || this['odd'] === 0x1) {
            this['getSquare0'] = this['getSquareX'];
            this['getSquare1'] = this['getSquareY'];
        } else {
            this['getSquare0'] = this['getSquareY'];
            this['getSquare1'] = this['getSquareX'];
        }
        if (this['odd'] === 0x0)
            this['binarySearch'] = this['binarySearch0'];
        else if (this['odd'] === 0x1)
            this['binarySearch'] = this['binarySearch1'];
        else if (this['odd'] === 0x2)
            this['binarySearch'] = this['binarySearch2'];
        else if (this['odd'] === 0x3)
            this['binarySearch'] = this['binarySearch3'];
        for (c = 0x0; c < e; ++c) {
            var g = this['indices'][c];
            var h = this['getDataValue'](a, g, 0x0);
            var k = this['getDataValue'](a, g, 0x1);
            var l = this['getDataValue'](a, g, 0x2);
            var m = this['getDataValue'](a, g, 0x3);
            m['idx'] = c;
            for (d = c + 0x1; d < e; ++d) {
                if (!this['processEnemy'](d, a, h, k, l, m))
                    break;
            }
            for (d = c - 0x1; d >= 0x0; --d) {
                if (!this['processEnemy'](d, a, h, k, l, m))
                    break;
            }
            if (!m['e']) {
                m['range'] = ConstAttribute['ENEMY_DETECTION_RANGE'];
            } else if (m['range'] === ConstAttribute['ENEMY_DETECTION_RANGE']) {
                m['range'] = ConstAttribute['ENEMY_MISS_RANGE'];
            }
        }
    }
    ['processEnemy'](a, b, c, d, e, f) {
        var g = this['indices'][a];
        var h = this['getDataValue'](b, g, 0x1);
        if (d['team'] === h['team'])
            return !![];
        var i = this['getDataValue'](b, g, 0x2);
        var k = this['getDataValue'](b, g, 0x3);
        var l = this['getSquare0'](e, i);
        if (l > f['dd'])
            return ![];
        if (l > f['range'] * f['range'])
            return ![];
        var m = this['getSquare1'](e, i);
        var n = l + m;
        if (n > f['range'] * f['range'])
            return !![];
        var o = this['getDataValue'](b, g, 0x0);
        this['tryConnectToEnemy'](f, o, h, i, k, n);
        this['tryConnectToEnemy'](k, c, d, e, f, n);
        return !![];
    }
    ['getSquareX'](c, e) {
        var f = e['x'] - c['x'];
        return f * f;
    }
    ['getSquareY'](c, e) {
        var f = e['y'] - c['y'];
        return f * f;
    }
    ['tryConnectToEnemy'](a, b, c, d, e, f) {
        if (a['dd'] > f) {
            a['dd'] = f;
            a['e'] = b;
            a['c'] = c;
            a['p'] = d;
            a['y'] = e;
        }
    }
    ['binarySearch0'](a, b) {
        var c = 0x0;
        var d = 0x0;
        var e = this['indices']['length'] - 0x1;
        while (d <= e) {
            c = Math['floor']((d + e) / 0x2);
            var f = this['getDataValue'](a, this['indices'][c], 0x2);
            if (f['x'] < b['x'])
                d = c + 0x1;
            else
                e = c - 0x1;
        }
        return c;
    }
    ['binarySearch1'](a, b) {
        var c = 0x0;
        var d = 0x0;
        var e = this['indices']['length'] - 0x1;
        while (d <= e) {
            c = Math['floor']((d + e) / 0x2);
            var f = this['getDataValue'](a, this['indices'][c], 0x2);
            if (f['x'] < b['x'])
                d = c + 0x1;
            else
                e = c - 0x1;
        }
        return c;
    }
    ['binarySearch2'](a, b) {
        var c = 0x0;
        var d = 0x0;
        var e = this['indices']['length'] - 0x1;
        while (d <= e) {
            c = Math['floor']((d + e) / 0x2);
            var f = this['getDataValue'](a, this['indices'][c], 0x2);
            if (f['x'] < b['x'])
                d = c + 0x1;
            else
                e = c - 0x1;
        }
        return c;
    }
    ['binarySearch3'](a, b) {
        var c = 0x0;
        var d = 0x0;
        var e = this['indices']['length'] - 0x1;
        while (d <= e) {
            c = Math['floor']((d + e) / 0x2);
            var f = this['getDataValue'](a, this['indices'][c], 0x2);
            if (f['x'] < b['x'])
                d = c + 0x1;
            else
                e = c - 0x1;
        }
        return c;
    }
}