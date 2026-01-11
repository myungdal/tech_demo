class VirusEffectSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](FriendComponent['componentId'])['includeComponent'](VirusEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            var k = this['getDataValue'](a, f, 0x3);
            h['life'] -= k['power'] / k['max_duration'];
            if (h['life'] <= 0x0) {
                h['life'] = 0x0;
                b['detachComponent'](g['id'], ElectricShockEffectComponent['componentId']);
                var l = b['attachComponent'](g['id'], DyingEffectComponent['componentId']);
                if (l) {
                    l['killerTeam'] = k['team'];
                    l['killerCharacterType'] = k['characterType'];
                    l['start'](ConstAttribute['DYING_DURATION']);
                }
                continue;
            }
            --k['duration'];
            if (k['duration'] > 0x0 && k['duration'] % ConstAttribute['VIRUS_CONTACT_DURATION'] === 0x0 && j['e'] && j['dd'] < ConstAttribute['VIRUS_CONTACT_RANGE'] * ConstAttribute['VIRUS_CONTACT_RANGE'] && !j['e']['hasComponent'](VirusEffectComponent['componentId'])) {
                k['target'] = j['e']['id'];
                var o = b['attachComponent'](k['target'], VirusEffectComponent['componentId']);
                if (o) {
                    o['power'] = k['power'];
                    o['max_duration'] = o['duration'] = k['duration'];
                }
            }
            if (k['duration'] > 0x0)
                continue;
            b['detachComponent'](g['id'], VirusEffectComponent['componentId']);
        }
    }
}