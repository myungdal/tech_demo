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
    ['hull0'] = [];
    ['hull1'] = [];
    ['hull2'] = [];
    ['hull3'] = [];
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
            --c['ttl'];
            if (c['ttl'] <= 0x0)
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
        var f = this['entities']['length'];
        var g = 0x0;
        this['grid']['follow'](this['entities']);
        for (g = f; g--;)
            this['entities'][g]['move'](this['width'], this['height']);
        this['geometry']['updateConnections'](this['entities']);
        for (g = f; g--;)
            this['entities'][g]['behavior'](this['events']);
        this['processDestroyEntities']();
        this['processRemoveEvents']();
        this['calcScore']();
        this['grid']['wave'](this['entities'], this['scores']);
        this['grid']['update']();
        this['hull0']['length'] = 0x0;
        this['hull1']['length'] = 0x0;
        this['hull2']['length'] = 0x0;
        this['hull3']['length'] = 0x0;
        var h = [];
        var j = null;
        for (g = f; g--;) {
            j = this['entities'][g];
            if (j['team'] === 0x0)
                h['push'](j);
        }
        this['hull0']['length'] = this['geometry']['chainHull'](h, this['hull0']);
        h['length'] = 0x0;
        for (g = f; g--;) {
            j = this['entities'][g];
            if (j['team'] === 0x1)
                h['push'](j);
        }
        this['hull1']['length'] = this['geometry']['chainHull'](h, this['hull1']);
        h['length'] = 0x0;
        for (g = f; g--;) {
            j = this['entities'][g];
            if (j['team'] === 0x2)
                h['push'](j);
        }
        this['hull2']['length'] = this['geometry']['chainHull'](h, this['hull2']);
        h['length'] = 0x0;
        for (g = f; g--;) {
            j = this['entities'][g];
            if (j['team'] === 0x3)
                h['push'](j);
        }
        this['hull3']['length'] = this['geometry']['chainHull'](h, this['hull3']);
    }
}