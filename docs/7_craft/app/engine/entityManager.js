class EntityManager {
    ['entities'] = new Objs();
    ['ids'] = new FastId();
    constructor() {
    }
    ['getEntity'](a) {
        var b = this['ids']['idxes'][a];
        return this['entities']['objs'][b];
    }
    ['getIdx'](a) {
        var b = this['ids']['idxes'][a];
        return b;
    }
    ['newEntity']() {
        var a = this['ids']['pop']();
        var b = this['entities']['newObj'](Entity);
        var c = this['entities']['objs'][b];
        c['init'](a);
        this['ids']['setIdx'](a, b);
        return a;
    }
    ['destroyCheckedEntities'](a, b) {
        var c = this['ids']['idxes']['length'];
        for (var d = c; d--;) {
            var e = this['getEntity'](d);
            if (!e || !e['checkedDestroy'])
                continue;
            this['destroyEntity'](d, a, b);
        }
    }
    ['destroyEntity'](a, b, c) {
        var d = this['getIdx'](a);
        var e = b['detachComponentAll'](d);
        c['checkDataViewsToBeRenew'](e, BitField['Zero']);
        var f = this['ids']['idxes'][a];
        var g = this['entities']['objCnt'] - 0x1;
        this['entities']['deleteObjCustom'](f, Entity['copy']);
        this['ids']['push'](a);
        if (f !== g) {
            var h = this['entities']['objs'][g]['id'];
            this['ids']['setIdx'](h, f);
        }
        b['deleteEntity'](g, f);
    }
}