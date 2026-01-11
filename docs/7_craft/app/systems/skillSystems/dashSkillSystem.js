class DashSkillSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](EnemyComponent['componentId'])['includeComponent'](DashSkillComponent['componentId'])['exceptComponent'](SkillMotionComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](RunawayEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
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
        }
    }
    ['onAction'](a, b, c, d) {
        var f = d['attachComponent'](a['id'], DashEffectComponent['componentId']);
        if (f)
            f['start'](ConstAttribute['DASH_DURATION']);
    }
}