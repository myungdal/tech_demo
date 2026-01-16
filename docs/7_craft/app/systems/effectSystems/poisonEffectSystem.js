class PoisonEffectSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PoisonEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            h['life'] -= j['power'] / j['max_duration'];
            if (h['life'] <= 0x0) {
                h['life'] = 0x0;
                b['detachComponent'](g['id'], PoisonEffectComponent['componentId']);
                var k = b['attachComponent'](g['id'], DyingEffectComponent['componentId']);
                if (k) {
                    k['killerTeam'] = j['team'];
                    k['killerCharacterType'] = j['characterType'];
                    k['start'](ConstAttribute['DYING_DURATION']);
                }
                continue;
            }
            --j['duration'];
            if (j['duration'] > 0x0)
                continue;
            b['detachComponent'](g['id'], PoisonEffectComponent['componentId']);
        }
    }
}