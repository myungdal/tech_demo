class BloodSkillSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](EnemyComponent['componentId'])['includeComponent'](BloodSkillComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](RunawayEffectComponent['componentId'])['exceptComponent'](BloodEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
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
            l['energy'] += h['characterAttribute']['charging'];
            if (l['energy'] > l['skillAttribute']['max_energy'])
                l['energy'] = l['skillAttribute']['max_energy'];
            if (l['energy'] < l['skillAttribute']['require_energy'])
                continue;
            if (!k['e'] || k['dd'] > l['skillAttribute']['range'] * l['skillAttribute']['range'])
                continue;
            l['energy'] -= l['skillAttribute']['require_energy'];
            l['target'] = k['e']['id'];
            this['onAction'](g, j, l, b);
        }
    }
    ['onAction'](a, b, c, d) {
        var f = d['getComponent'](c['target'], PositionComponent['componentId']);
        if (!f)
            return;
        if (d['hasComponent'](c['target'], BloodEffectComponent['componentId']))
            return;
        var g = d['attachComponent'](c['target'], StunEffectComponent['componentId']);
        if (!g)
            return;
        g['start'](ConstAttribute['BLOOD_DURATION']);
        var h = d['attachComponent'](a['id'], BloodEffectComponent['componentId']);
        if (!h)
            return;
        h['target'] = c['target'];
        h['x'] = b['x'];
        h['y'] = b['y'];
        h['power'] = c['skillAttribute']['power'];
        h['duration'] = ConstAttribute['BLOOD_DURATION'];
        b['x'] = f['x'] + 0xa;
        b['y'] = f['y'] + 0xa;
    }
}