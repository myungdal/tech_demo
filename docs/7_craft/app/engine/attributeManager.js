class AttributeManager {
    ['attributes'] = [];
    ['regAttribute'](a, b) {
        var c = this['attributes'][a];
        if (c === undefined) {
            c = this['attributes'][a] = [];
        }
        c['push'](b);
        return c['length'] - 0x1;
    }
    ['getAttribute'](a, b) {
        var c = this['attributes'][a];
        if (c === undefined)
            return null;
        return c[b];
    }
}