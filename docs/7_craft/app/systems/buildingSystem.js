class BuildingSystem extends System {
    ['barrierSystem'] = null;
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](BuildingComponent['componentId'])['exceptComponent'](MovementComponent['componentId']);
        return super['init'](a);
    }
    ['update0'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var e = this['getDataValue'](a, d, 0x1);
            var f = barrierSystem['pick'](e['x'], e['y']);
            if (!f)
                continue;
            if (f['b'])
                continue;
            barrierSystem['setBarrier'](f['c'], f['r'], 0x1);
        }
    }
    ['update1'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x0);
            var g = this['getDataValue'](a, d, 0x1);
            if (!f['checkedDestroy'])
                continue;
            var h = barrierSystem['pick'](g['x'], g['y']);
            if (!h)
                continue;
            barrierSystem['setBarrier'](h['c'], h['r'], 0x0);
        }
    }
}