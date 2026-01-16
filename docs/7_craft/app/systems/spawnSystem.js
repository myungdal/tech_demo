class SpawnSystem extends System {
    ['init'](a) {
        this['includeComponent'](CharacterComponent['componentId'])['includeComponent'](PositionComponent['componentId'])['includeComponent'](SpawnComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = this['getDataRowCnt'](a);
        for (var f = 0x0; f < d; ++f) {
            var g = this['getDataValue'](a, f, 0x0);
            var h = this['getDataValue'](a, f, 0x1);
            var j = this['getDataValue'](a, f, 0x2);
            var k = this['getDataValue'](a, f, 0x3);
            if (k['pawnCnt'] >= k['max_pawnCnt']) {
                continue;
            }
            --k['duration'];
            if (k['duration'] > 0x0)
                continue;
            k['duration'] = k['max_duration'];
            var l = k['spawn'](b, j['x'], j['y'], h['team']);
            var m = b['attachComponent'](l, PawnComponent['componentId']);
            m['spawnerEntityId'] = g['id'];
            ++k['pawnCnt'];
        }
    }
}