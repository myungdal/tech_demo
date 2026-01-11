class BloodEffectSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](BloodEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x0);
            var g = this['getDataValue'](a, d, 0x1);
            var h = this['getDataValue'](a, d, 0x2);
            var j = this['getDataValue'](a, d, 0x3);
            var k = b['getComponent'](j['target'], CharacterComponent['componentId']);
            var l = b['getComponent'](j['target'], PositionComponent['componentId']);
            if (!k || !l) {
                b['detachComponent'](f['id'], BloodEffectComponent['componentId']);
                continue;
            }
            h['x'] = l['x'] + 0xa;
            h['y'] = l['y'] + 0xa;
            var m = k['life'];
            k['life'] -= j['power'] / ConstAttribute['BLOOD_DURATION'];
            if (k['life'] <= 0x0) {
                k['life'] = 0x0;
                b['detachComponent'](f['id'], BloodEffectComponent['componentId']);
                var o = b['attachComponent'](j['target'], DyingEffectComponent['componentId']);
                if (o) {
                    o['killerTeam'] = g['team'];
                    o['killerCharacterType'] = g['type'];
                    o['start'](ConstAttribute['DYING_DURATION']);
                }
            }
            m = m - k['life'];
            g['life'] += m;
            if (g['life'] > g['characterAttribute']['max_life']) {
                g['life'] = g['characterAttribute']['max_life'];
            }
            --j['duration'];
            if (j['duration'] > 0x0)
                continue;
            b['detachComponent'](f['id'], BloodEffectComponent['componentId']);
        }
    }
}