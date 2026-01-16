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
    ['geometry'] = null;
    ['grid'] = null;
    constructor(a, b) {
        this['width'] = a;
        this['height'] = b;
        this['grid'] = new Grid(a, b, CELL_SIZE);
        this['geometry'] = new Geometry();
    }
    ['add'](a) {
        var b = a['length'];
        var c = new Array(b);
        var d = new Attribute0();
        for (var e = b; e--;) {
            if (a[e][0x3] === 0x0)
                c[e] = new Entity0(a[e][0x0], a[e][0x1], a[e][0x2], a[e][0x3], d);
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
        var e = this['entities']['length'];
        var f = 0x0;
        this['grid']['follow'](this['entities']);
        for (f = e; f--;)
            this['entities'][f]['move'](this['width'], this['height']);
        this['geometry']['updateConnections'](this['entities']);
        for (f = e; f--;)
            this['entities'][f]['behavior'](this['events']);
        this['processDestroyEntities']();
        this['processRemoveEvents']();
        this['calcScore']();
        this['grid']['wave'](this['entities'], this['scores']);
        this['grid']['update']();
    }
}