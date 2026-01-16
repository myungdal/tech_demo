class MagnetSkillSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](EnemyComponent['componentId'])['includeComponent'](MagnetSkillComponent['componentId'])['exceptComponent'](SkillMotionComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](RunawayEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            var k = this['getDataValue'](a, f, 0x3);
            var l = this['getDataValue'](a, f, 0x4);
            if (l['pre_delay'] > 0x0) {
                l['pre_delay'] -= 0x1;
                if (l['pre_delay'] <= 0x0) {
                    l['pre_delay'] = 0x0;
                    this['onAction'](g, h, j, l, b);
                }
                continue;
            }
            if (l['post_delay'] > 0x0) {
                l['post_delay'] -= 0x1;
                if (l['post_delay'] <= 0x0) {
                    l['post_delay'] = 0x0;
                    l['target'] = 0x0;
                }
                continue;
            }
            l['energy'] += h['characterAttribute']['charging'];
            if (l['energy'] > l['skillAttribute']['max_energy'])
                l['energy'] = l['skillAttribute']['max_energy'];
            if (l['energy'] < l['skillAttribute']['require_energy'])
                continue;
            if (!k['e'] || k['dd'] > l['skillAttribute']['range'] * l['skillAttribute']['range'])
                continue;
            l['energy'] -= l['skillAttribute']['require_energy'];
            l['target'] = k['e']['id'];
            l['pre_delay'] = l['skillAttribute']['pre_delay'];
            l['post_delay'] = l['skillAttribute']['post_delay'];
            var m = b['attachComponent'](g['id'], SkillMotionComponent['componentId']);
            if (m)
                m['start'](l['pre_delay'] + l['post_delay']);
        }
    }
    ['onAction'](a, b, d, f, g) {
        var h = g['getComponent'](f['target'], PositionComponent['componentId']);
        if (!h)
            return;
        var i = g['createEntity']();
        var j = g['attachComponent'](i, PositionComponent['componentId']);
        j['x'] = h['x'];
        j['y'] = h['y'];
        var k = g['attachComponent'](i, MagnetEffectComponent['componentId']);
        k['power'] = f['skillAttribute']['power'];
        k['timer'] = ConstAttribute['MAGNET_TIMER'];
        k['max_duration'] = k['duration'] = ConstAttribute['MAGNET_DURATION'];
    }
}