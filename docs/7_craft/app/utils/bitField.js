class BitField {
    static ['Common'] = new BitField();
    static ['Common0'] = new BitField();
    static ['Common1'] = new BitField();
    static ['Zero'] = new BitField();
    ['data'] = [];
    ['clear']() {
        var a = this['data']['length'];
        for (var b = 0x0; b < a; ++b) {
            this['data'][b] = 0x0;
        }
        return this;
    }
    ['on'](a) {
        var b = a >> 0x4;
        this['setDataLength'](b + 0x1);
        this['data'][b] |= 0x1 << (a & 0xf);
        return this;
    }
    ['off'](a) {
        var b = a >> 0x4;
        this['setDataLength'](b + 0x1);
        this['data'][b] &= ~(0x1 << (a & 0xf));
        return this;
    }
    ['isOn'](a) {
        var b = a >> 0x4;
        return this['data']['length'] > b && (this['data'][b] & 0x1 << (a & 0xf)) > 0x0 ? !![] : ![];
    }
    ['isOff'](a) {
        return this['isOn'](a) ? ![] : !![];
    }
    ['and'](a, b) {
        var c = this['data']['length'];
        var d = a['data']['length'];
        var e = Math['min'](c, d);
        b['setDataLength'](e);
        b['clear']();
        for (var f = 0x0; f < e; ++f) {
            b['data'][f] = this['data'][f] & a['data'][f];
        }
        return b;
    }
    ['or'](a, b) {
        var c = this['data']['length'];
        var d = a['data']['length'];
        var e = Math['max'](c, d);
        this['setDataLength'](e);
        a['setDataLength'](e);
        b['setDataLength'](e);
        b['clear']();
        for (var f = 0x0; f < e; ++f) {
            b['data'][f] = this['data'][f] | a['data'][f];
        }
        return b;
    }
    ['not'](a) {
        var b = this['data']['length'];
        a['setDataLength'](b);
        a['clear']();
        for (var c = 0x0; c < b; ++c) {
            a['data'][c] = ~this['data'][c];
        }
        return a;
    }
    ['xor'](a, b) {
        var c = this['data']['length'];
        var d = a['data']['length'];
        var e = Math['max'](c, d);
        this['setDataLength'](e);
        a['setDataLength'](e);
        b['setDataLength'](e);
        b['clear']();
        for (var f = 0x0; f < e; ++f) {
            b['data'][f] = this['data'][f] ^ a['data'][f];
        }
        return b;
    }
    ['setDataLength'](a) {
        while (this['data']['length'] < a)
            this['data']['push'](0x0);
    }
    ['isZero']() {
        return this['isNotZero']() ? ![] : !![];
    }
    ['isNotZero']() {
        var a = this['data']['length'];
        for (var b = 0x0; b < a; ++b) {
            if (this['data'][b] !== 0x0)
                return !![];
        }
        return ![];
    }
    ['is'](a) {
        var b = this['data']['length'];
        var c = a['data']['length'];
        var d = Math['min'](b, c);
        var e = 0x0;
        for (e = 0x0; e < d; ++e) {
            if (this['data'][e] !== a['data'][e])
                return ![];
        }
        if (b > c) {
            for (e = c; e < b; ++e) {
                if (this['data'][e] !== 0x0)
                    return ![];
            }
        } else if (b < c) {
            for (e = b; e < c; ++e) {
                if (a['data'][e] !== 0x0)
                    return ![];
            }
        }
        return !![];
    }
    ['isNot'](a) {
        return this['is'](a) ? ![] : !![];
    }
    ['copyFrom'](a) {
        var b = a['data']['length'];
        this['setDataLength'](b);
        this['clear']();
        for (var c = 0x0; c < b; ++c) {
            this['data'][c] = a['data'][c];
        }
        return this;
    }
    ['makeString']() {
        var a = '';
        var b = 0x0;
        var c = ![];
        var d = this['data']['length'];
        for (var e = 0x0; e < d; ++e) {
            b = this['data'][e];
            if (!c && b === 0x0)
                continue;
            c = !![];
            if (b <= 0xf)
                a = a + '000' + b['toString'](0x10);
            else if (b <= 0xff)
                a = a + '00' + b['toString'](0x10);
            else if (b <= 0xfff)
                a = a + '0' + b['toString'](0x10);
            else if (b <= 0xffff)
                a = a + b['toString'](0x10);
        }
        return a;
    }
}