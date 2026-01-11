class ParasiteEffectSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](ParasiteEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            --j['duration'];
            if (j['duration'] > 0x0)
                continue;
            if (h['team'] === 0x0)
                b['detachComponent'](g['id'], TeamComponent0['componentId']);
            else if (h['team'] === 0x1)
                b['detachComponent'](g['id'], TeamComponent1['componentId']);
            else if (h['team'] === 0x2)
                b['detachComponent'](g['id'], TeamComponent2['componentId']);
            else if (h['team'] === 0x3)
                b['detachComponent'](g['id'], TeamComponent3['componentId']);
            if (j['originalTeam'] === 0x0)
                b['attachComponent'](g['id'], TeamComponent0['componentId']);
            else if (j['originalTeam'] === 0x1)
                b['attachComponent'](g['id'], TeamComponent1['componentId']);
            else if (j['originalTeam'] === 0x2)
                b['attachComponent'](g['id'], TeamComponent2['componentId']);
            else if (j['originalTeam'] === 0x3)
                b['attachComponent'](g['id'], TeamComponent3['componentId']);
            h['team'] = j['originalTeam'];
            b['detachComponent'](g['id'], ParasiteEffectComponent['componentId']);
        }
    }
}