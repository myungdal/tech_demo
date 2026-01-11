class BombSkillSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](BombSkillComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            var k = this['getDataValue'](a, f, 0x3);
            if (!g['checkedDestroy'])
                continue;
            var l = b['createEntity']();
            var m = b['attachComponent'](l, PositionComponent['componentId']);
            m['x'] = j['x'];
            m['y'] = j['y'];
            var o = b['attachComponent'](l, ExplosionEffectComponent['componentId']);
            o['power'] = k['skillAttribute']['power'];
            o['max_duration'] = o['duration'] = ConstAttribute['EXPLOSION_DURATION'];
            o['team'] = h['team'];
            o['characterType'] = h['type'];
        }
    }
}