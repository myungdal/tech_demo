class World {
    ['entities'] = [];
    ['scores'] = [
        0x0,
        0x0,
        0x0,
        0x0
    ];
    ['events'] = [];
    ['width'] = 0x0;
    ['height'] = 0x0;
    ['geometry'] = new Geometry();
    constructor(a, b) {
        this['width'] = a;
        this['height'] = b;
    }
    ['add'](a) {
        var b = a['length'];
        var c = new Array(b);
        var d = new Attribute0();
        var e = new Attribute1();
        var f = new Attribute2();
        for (var g = b; g--;) {
            if (a[g][0x3] === 0x0)
                c[g] = new Entity0(a[g][0x0], a[g][0x1], a[g][0x2], a[g][0x3], d);
            else if (a[g][0x3] === 0x1)
                c[g] = new Entity1(a[g][0x0], a[g][0x1], a[g][0x2], a[g][0x3], e);
            else if (a[g][0x3] === 0x2)
                c[g] = new Entity2(a[g][0x0], a[g][0x1], a[g][0x2], a[g][0x3], f);
        }
        this['entities'] = this['entities']['concat'](c);
    }
    ['processDestroyEntities']() {
        var a = this['entities']['length'];
        for (var b = a; b--;)
            if (this['entities'][b]['life'] <= 0x0)
                this['entities']['splice'](b, 0x1);
    }
    ['processRemoveEvents']() {
        var a = this['events']['length'];
        for (var b = a; b--;) {
            var c = this['events'][b];
            --c['life'];
            if (c['life'] <= 0x0)
                this['events']['splice'](b, 0x1);
        }
    }
    ['calcScore']() {
        this['scores'][0x0] = this['scores'][0x1] = this['scores'][0x2] = this['scores'][0x3] = 0x0;
        var a = this['entities']['length'];
        for (var b = a; b--;) {
            ++this['scores'][this['entities'][b]['team']];
        }
    }
    ['update'](a, b, c, d) {
        this['geometry']['updateConnections'](this['entities']);
        var e = this['entities']['length'];
        var f = 0x0;
        for (f = e; f--;)
            this['entities'][f]['behavior'](this['events']);
        for (f = e; f--;)
            this['entities'][f]['preMove'](this['width'], this['height']);
        for (f = e; f--;)
            this['entities'][f]['resolveOverlap']();
        for (f = e; f--;)
            this['entities'][f]['move'](this['width'], this['height']);
        this['processDestroyEntities']();
        this['processRemoveEvents']();
        this['calcScore']();
    }
}