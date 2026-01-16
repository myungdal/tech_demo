class BehaviorSystem extends System {
    ['init'](a) {
        this['includeComponent'](EnemyComponent['componentId'])['includeComponent'](BehaviorComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var c = this['getDataRowCnt'](a);
        for (var d = 0x0; d < c; ++d) {
            var f = this['getDataValue'](a, d, 0x0);
            var g = this['getDataValue'](a, d, 0x1);
            if (g['e'] && f['hasComponent'](PatrolComponent['componentId'])) {
                b['detachComponent'](f['id'], PatrolComponent['componentId']);
            } else if (!g['e'] && !f['hasComponent'](PatrolComponent['componentId'])) {
                var h = b['attachComponent'](f['id'], PatrolComponent['componentId']);
                if (h) {
                    h['pathIdx'] = 0x0;
                    h['idleDuration'] = ConstAttribute['PATROL_IDLE_DURATION'];
                }
            }
        }
    }
}