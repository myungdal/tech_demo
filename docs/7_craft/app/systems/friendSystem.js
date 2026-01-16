class FriendSystem extends System {
    ['odd'] = 0x0;
    ['indices'] = [];
    ['getSquare0'] = null;
    ['getSquare1'] = null;
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](FriendComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
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
            f['m'] = null;
            f['idx'] = -0x1;
        }
        for (c = 0x0; c < e; ++c)
            this['indices'][c] = c;
        if (this['odd'] === 0x0) {
            this['indices']['sort'](function (p, q) {
                var r = this['getDataValue'](a, p, 0x2);
                var s = this['getDataValue'](a, q, 0x2);
                var t = r['x'] - s['x'];
                return t !== 0x0 ? t : p - q;
            }['bind'](this));
        } else if (this['odd'] === 0x1) {
            this['indices']['sort'](function (p, q) {
                var r = this['getDataValue'](a, q, 0x2);
                var s = this['getDataValue'](a, p, 0x2);
                var t = r['x'] - s['x'];
                return t !== 0x0 ? t : q - p;
            }['bind'](this));
        } else if (this['odd'] === 0x2) {
            this['indices']['sort'](function (p, q) {
                var r = this['getDataValue'](a, p, 0x2);
                var s = this['getDataValue'](a, q, 0x2);
                var t = r['y'] - s['y'];
                return t !== 0x0 ? t : p - q;
            }['bind'](this));
        } else if (this['odd'] === 0x3) {
            this['indices']['sort'](function (p, q) {
                var r = this['getDataValue'](a, q, 0x2);
                var s = this['getDataValue'](a, p, 0x2);
                var t = r['y'] - s['y'];
                return t !== 0x0 ? t : q - p;
            }['bind'](this));
        }
        if (this['odd'] === 0x0 || this['odd'] === 0x1) {
            this['getSquare0'] = this['getSquareX'];
            this['getSquare1'] = this['getSquareY'];
        } else {
            this['getSquare0'] = this['getSquareY'];
            this['getSquare1'] = this['getSquareX'];
        }
        for (c = 0x0; c < e; ++c) {
            var g = this['indices'][c];
            var h = this['getDataValue'](a, g, 0x0);
            var k = this['getDataValue'](a, g, 0x1);
            var l = this['getDataValue'](a, g, 0x2);
            var o = this['getDataValue'](a, g, 0x3);
            o['idx'] = c;
            for (d = c + 0x1; d < e; ++d) {
                if (!this['processFriend'](d, a, h, k, l, o))
                    break;
            }
            for (d = c - 0x1; d >= 0x0; --d) {
                if (!this['processFriend'](d, a, h, k, l, o))
                    break;
            }
        }
    }
    ['processFriend'](a, b, c, d, e, f) {
        var g = this['indices'][a];
        var h = this['getDataValue'](b, g, 0x1);
        if (d['team'] !== h['team'])
            return !![];
        var i = this['getDataValue'](b, g, 0x0);
        var k = this['getDataValue'](b, g, 0x2);
        var l = this['getDataValue'](b, g, 0x3);
        var m = this['getSquare0'](e, k);
        if (m > f['dd'])
            return ![];
        var n = this['getSquare1'](e, k);
        var o = m + n;
        this['tryConnectToFriend'](c, f, i, h, k, l, o);
        this['tryConnectToFriend'](i, l, c, d, e, f, o);
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
    ['tryConnectToFriend'](a, b, c, d, e, f, g) {
        if (f['e'] === a)
            return;
        if (b['dd'] > g) {
            b['dd'] = g;
            b['e'] = c;
            b['c'] = d;
            b['p'] = e;
            b['m'] = f;
        }
    }
}