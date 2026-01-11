class DamageSystem extends System {
    ['enemySystem'] = null;
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](DamageComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = 0x0;
        var f = this['enemySystem']['indices']['length'];
        var g = b['systemManager']['dataViews'][this['enemySystem']['componentsInfo']['key']];
        var h = this['getDataRowCnt'](a);
        for (var k = 0x0; k < h; ++k) {
            var l = this['getDataValue'](a, k, 0x0);
            var m = this['getDataValue'](a, k, 0x1);
            var o = this['getDataValue'](a, k, 0x2);
            var q = this['getDataValue'](a, k, 0x3);
            b['detachComponent'](l['id'], DamageComponent['componentId']);
            if (q['range'] > 0x0 && f > 0x0) {
                var r = this['enemySystem']['binarySearch'](g, o);
                if (r < 0x0)
                    continue;
                for (d = r; d >= 0x0; --d) {
                    if (!this['processRangedDamage'](d, g, o, q, b))
                        break;
                }
                for (d = r + 0x1; d < f; ++d) {
                    if (!this['processRangedDamage'](d, g, o, q, b))
                        break;
                }
            } else {
                this['processDamage'](l, m, q, b);
            }
        }
    }
    ['processRangedDamage'](a, b, c, d, e) {
        var f = this['enemySystem']['indices'][a];
        var g = this['enemySystem']['getDataValue'](b, f, 0x1);
        if (d['team'] === g['team'])
            return !![];
        var h = this['enemySystem']['getDataValue'](b, f, 0x2);
        var i = this['enemySystem']['getSquare0'](c, h);
        if (i > d['range'] * d['range'])
            return ![];
        var k = this['enemySystem']['getSquare1'](c, h);
        var l = i + k;
        if (l > d['range'] * d['range'])
            return !![];
        var m = this['enemySystem']['getDataValue'](b, f, 0x0);
        this['processDamage'](m, g, d, e);
    }
    ['processDamage'](a, b, d, f) {
        b['life'] -= (0x1 - b['characterAttribute']['armor']) * d['power'];
        if (b['life'] <= 0x0) {
            b['life'] = 0x0;
            f['detachComponent'](a['id'], DamageComponent['componentId']);
            var g = f['attachComponent'](a['id'], DyingEffectComponent['componentId']);
            if (g) {
                g['killerTeam'] = d['team'];
                g['killerCharacterType'] = d['characterType'];
                g['start'](ConstAttribute['DYING_DURATION']);
            }
            return;
        }
        if ((0x1 - b['characterAttribute']['stun_resistance']) * d['stun_chance'] > Math['random']()) {
            var h = f['attachComponent'](a['id'], StunEffectComponent['componentId']);
            if (h)
                h['start'](ConstAttribute['STUN_DURATION']);
        } else if (b['life'] < b['characterAttribute']['runaway_life']) {
            var i = f['attachComponent'](a['id'], RunawayEffectComponent['componentId']);
            if (i)
                i['start'](ConstAttribute['RUNAWAY_DURATION']);
        }
    }
}