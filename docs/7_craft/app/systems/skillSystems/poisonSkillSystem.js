class PoisonSkillSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](EnemyComponent['componentId'])['includeComponent'](PoisonSkillComponent['componentId'])['exceptComponent'](SkillMotionComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](RunawayEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
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
        var f = d['getComponent'](c['target'], CharacterComponent['componentId']);
        if (!f)
            return;
        if (c['skillAttribute']['accuracy'] < Math['random']())
            return;
        if (f['characterAttribute']['dodge'] > Math['random']())
            return;
        var g = d['attachComponent'](c['target'], PoisonEffectComponent['componentId']);
        if (g) {
            g['power'] = 0x0;
            g['duration'] = 0x0;
        } else {
            g = d['getComponent'](c['target'], PoisonEffectComponent['componentId']);
        }
        if (g) {
            g['power'] += c['skillAttribute']['power'];
            g['duration'] = Math['max'](g['duration'], ConstAttribute['POISON_DURATION']);
            g['max_duration'] = g['duration'];
            g['team'] = b['team'];
            g['characterType'] = b['characterType'];
        }
    }
}