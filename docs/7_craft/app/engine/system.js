class System {
    static ['updateType'] = {
        'update': 0x0,
        'update0': 0x1,
        'update1': 0x2,
        'update2': 0x3,
        'update3': 0x4
    };
    ['systemId'] = -0x1;
    ['enabled'] = !![];
    ['componentsInfo'] = {
        'including': new BitField(),
        'exception': new BitField(),
        'key': ''
    };
    ['componentCnt'] = 0x0;
    ['init'](a) {
        this['enabled'] = a;
        return this;
    }
    ['includeComponent'](a) {
        this['componentsInfo']['including']['on'](a);
        this['componentsInfo']['key'] = this['componentsInfo']['including']['makeString']() + '_' + this['componentsInfo']['exception']['makeString']();
        ++this['componentCnt'];
        return this;
    }
    ['exceptComponent'](a) {
        this['componentsInfo']['exception']['on'](a);
        this['componentsInfo']['key'] = this['componentsInfo']['including']['makeString']() + '_' + this['componentsInfo']['exception']['makeString']();
        return this;
    }
    ['getDataRowCnt'](a) {
        if (!a)
            return 0x0;
        return a['length'] / (0x1 + this['componentCnt']);
    }
    ['getDataValue'](a, b, c) {
        return a[b * (0x1 + this['componentCnt']) + c];
    }
    ['update'](a, b) {
    }
    ['update0'](a, b) {
    }
    ['update1'](a, b) {
    }
    ['update2'](a, b) {
    }
    ['update3'](a, b) {
    }
}