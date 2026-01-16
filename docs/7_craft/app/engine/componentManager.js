class ComponentManager {
    ['templates'] = [];
    ['componentGroup'] = [];
    ['entityComponentViews'] = [];
    constructor() {
    }
    ['regComponentTemplate'](a) {
        var b = this['componentGroup']['length'];
        this['templates']['push'](a);
        var c = new Objs();
        this['componentGroup']['push'](c);
        var d = new EntityComponentView(b);
        this['entityComponentViews']['push'](d);
        return b;
    }
    ['getComponent'](a, b) {
        var c = this['entityComponentViews'][b];
        var d = c['get'](a);
        var e = this['componentGroup'][b];
        var f = e['objs'][d];
        return f;
    }
    ['attachComponent'](a, b) {
        var c = this['templates'][b];
        var d = this['componentGroup'][b];
        var e = d['newObj'](c);
        var f = this['entityComponentViews'][b];
        f['set'](a, e);
        var g = d['objs'][e];
        return g;
    }
    ['detachComponentAll'](a) {
        var b = BitField['Common'];
        var c = this['componentGroup']['length'];
        for (var d = 0x0; d < c; ++d) {
            if (!this['detachComponent'](a, d))
                continue;
            b['on'](d);
        }
        return b;
    }
    ['detachComponent'](a, b) {
        var c = this['entityComponentViews'][b];
        var d = c['get'](a);
        if (d < 0x0)
            return ![];
        var e = this['componentGroup'][b];
        var f = e['objCnt'] - 0x1;
        e['deleteObj'](d);
        c['delete'](a);
        c['modify'](f, d);
        return !![];
    }
    ['deleteEntity'](a, b) {
        var c = this['componentGroup']['length'];
        for (var d = 0x0; d < c; ++d) {
            var e = this['entityComponentViews'][d];
            if (a === b) {
                e['delete'](a);
                continue;
            }
            var f = e['get'](a);
            if (f < 0x0) {
                continue;
            }
            e['delete'](a);
            e['set'](b, f);
        }
    }
}