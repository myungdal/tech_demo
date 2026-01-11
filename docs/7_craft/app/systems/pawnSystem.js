class PawnSystem extends System {
    ['init'](a) {
        this['includeComponent'](PawnComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x0);
            var g = this['getDataValue'](a, d, 0x1);
            if (g['spawnerEntityId'] === -0x1)
                continue;
            if (f['checkedDestroy']) {
                var h = b['getComponent'](g['spawnerEntityId'], SpawnComponent['componentId']);
                --h['pawnCnt'];
            } else if (!b['entityManager']['getEntity'](g['spawnerEntityId'])) {
                g['spawnerEntityId'] = -0x1;
            }
        }
    }
}