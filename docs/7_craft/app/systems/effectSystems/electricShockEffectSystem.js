class ElectricShockEffectSystem extends System {
    ['init'](a) {
        this['includeComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x0);
            var g = this['getDataValue'](a, d, 0x1);
            --g['duration'];
            if (g['duration'] > 0x0 && g['contact_cnt'] > 0x0 && g['max_duration'] - g['duration'] === ConstAttribute['ELECTRICSHOCK_CONTACT_DURATION']) {
                var h = b['getComponent'](f['id'], FriendComponent['componentId']);
                if (h && h['e'] && h['dd'] < ConstAttribute['ELECTRICSHOCK_CONTACT_RANGE'] * ConstAttribute['ELECTRICSHOCK_CONTACT_RANGE'] && !h['e']['hasComponent'](ElectricShockEffectComponent['componentId'])) {
                    g['target'] = h['e']['id'];
                    var j = b['attachComponent'](g['target'], ElectricShockEffectComponent['componentId']);
                    if (j) {
                        j['start'](g['duration'], g['contact_cnt'] - 0x1);
                    }
                }
            }
            if (g['duration'] > 0x0)
                continue;
            b['detachComponent'](f['id'], ElectricShockEffectComponent['componentId']);
        }
    }
}