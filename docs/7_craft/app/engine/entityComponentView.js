class EntityComponentView {
    ['componentId'] = -0x1;
    ['idxes'] = new Map();
    constructor(a) {
        this['componentId'] = a;
    }
    ['set'](a, b) {
        this['idxes']['set'](a, b);
    }
    ['get'](a) {
        var b = this['idxes']['get'](a);
        return b === undefined ? -0x1 : b;
    }
    ['has'](a) {
        return this['idxes']['has'](a);
    }
    ['delete'](a) {
        this['idxes']['delete'](a);
    }
    ['modify'](a, b) {
        var c = -0x1;
        for (var [d, e] of this['idxes']) {
            if (e === a) {
                c = d;
                break;
            }
        }
        if (c === -0x1)
            return;
        this['idxes']['set'](c, b);
    }
}