class Entity {
    static ['copy'](a, b) {
        b['id'] = a['id'];
        b['tag']['copyFrom'](a['tag']);
        b['prevTag']['copyFrom'](a['prevTag']);
        b['checkedDestroy'] = a['checkedDestroy'];
    }
    ['id'] = -0x1;
    ['tag'] = new BitField();
    ['prevTag'] = new BitField();
    ['checkedDestroy'] = ![];
    ['init'](a) {
        this['id'] = a;
        this['tag']['clear']();
        this['prevTag']['clear']();
        this['checkedDestroy'] = ![];
    }
    ['hasComponent'](a) {
        return this['tag']['isOn'](a);
    }
    ['prevTagHasComponent'](a) {
        return this['prevTag']['isOn'](a);
    }
    ['addTag'](a) {
        this['tag']['on'](a);
    }
    ['removeTag'](a) {
        this['tag']['off'](a);
    }
    ['addPrevTag'](a) {
        this['prevTag']['on'](a);
    }
    ['removePrevTag'](a) {
        this['prevTag']['off'](a);
    }
}