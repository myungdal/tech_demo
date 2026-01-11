class ParasiteSkillSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](EnemyComponent['componentId'])['includeComponent'](ParasiteSkillComponent['componentId'])['exceptComponent'](SkillMotionComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](RunawayEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            var k = this['getDataValue'](a, f, 0x3);
            if (k['pre_delay'] > 0x0) {
                k['pre_delay'] -= 0x1;
                if (k['pre_delay'] <= 0x0) {
                    k['pre_delay'] = 0x0;
                    this['onAction'](g, h, k, b);
                }
                continue;
            }
            if (k['post_delay'] > 0x0) {
                k['post_delay'] -= 0x1;
                if (k['post_delay'] <= 0x0) {
                    k['post_delay'] = 0x0;
                    k['target'] = 0x0;
                }
                continue;
            }
            k['energy'] += h['characterAttribute']['charging'];
            if (k['energy'] > k['skillAttribute']['max_energy'])
                k['energy'] = k['skillAttribute']['max_energy'];
            if (k['energy'] < k['skillAttribute']['require_energy'])
                continue;
            if (!j['e'] || j['dd'] > k['skillAttribute']['range'] * k['skillAttribute']['range'])
                continue;
            k['energy'] -= k['skillAttribute']['require_energy'];
            k['target'] = j['e']['id'];
            k['pre_delay'] = k['skillAttribute']['pre_delay'];
            k['post_delay'] = k['skillAttribute']['post_delay'];
            var l = b['attachComponent'](g['id'], SkillMotionComponent['componentId']);
            if (l)
                l['start'](k['pre_delay'] + k['post_delay']);
        }
    }
    ['onAction'](a, b, c, d) {
        var e = d['getComponent'](c['target'], CharacterComponent['componentId']);
        if (!e)
            return;
        var f = d['attachComponent'](c['target'], ParasiteEffectComponent['componentId']);
        if (!f)
            return;
        f['originalTeam'] = e['team'];
        if (e['team'] === 0x0)
            d['detachComponent'](c['target'], TeamComponent0['componentId']);
        else if (e['team'] === 0x1)
            d['detachComponent'](c['target'], TeamComponent1['componentId']);
        else if (e['team'] === 0x2)
            d['detachComponent'](c['target'], TeamComponent2['componentId']);
        else if (e['team'] === 0x3)
            d['detachComponent'](c['target'], TeamComponent3['componentId']);
        if (b['team'] === 0x0)
            d['attachComponent'](c['target'], TeamComponent0['componentId']);
        else if (b['team'] === 0x1)
            d['attachComponent'](c['target'], TeamComponent1['componentId']);
        else if (b['team'] === 0x2)
            d['attachComponent'](c['target'], TeamComponent2['componentId']);
        else if (b['team'] === 0x3)
            d['attachComponent'](c['target'], TeamComponent3['componentId']);
        e['team'] = b['team'];
        f['duration'] = ConstAttribute['PARASITE_DURATION'];
    }
}