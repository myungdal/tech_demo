class Engine {
    ['enabled'] = !![];
    ['eventManager'] = new EventManager();
    ['entityManager'] = new EntityManager();
    ['componentManager'] = new ComponentManager();
    ['systemManager'] = new SystemManager();
    ['attributeManager'] = new AttributeManager();
    constructor() {
    }
    ['update']() {
        if (!this['enabled'])
            return;
        this['eventManager']['dispatchEvents']();
        this['entityManager']['destroyCheckedEntities'](this['componentManager'], this['systemManager']);
        this['systemManager']['updateComponents'](this['entityManager'], this['componentManager']);
        this['systemManager']['updateDataViews'](this['entityManager'], this['componentManager']);
        this['systemManager']['updateSystems'](this);
    }
    ['regAttribute'](a, b) {
        return this['attributeManager']['regAttribute'](a, b);
    }
    ['getAttribute'](a, b) {
        return this['attributeManager']['getAttribute'](a, b);
    }
    ['regComponentTemplate'](a) {
        return this['componentManager']['regComponentTemplate'](a);
    }
    ['hasComponent'](a, b) {
        if (a === -0x1)
            return ![];
        var c = this['entityManager']['getIdx'](a);
        if (c === -0x1)
            return ![];
        var d = this['entityManager']['entities']['objs'][c];
        return d['hasComponent'](b);
    }
    ['getComponent'](a, b) {
        if (a === -0x1)
            return null;
        var c = this['entityManager']['getIdx'](a);
        if (c === -0x1)
            return null;
        var d = this['entityManager']['entities']['objs'][c];
        if (!d['hasComponent'](b))
            return null;
        var e = this['componentManager']['getComponent'](c, b);
        return e;
    }
    ['regSystem'](a) {
        return this['systemManager']['regSystem'](a);
    }
    ['getSystem'](a) {
        return this['systemManager']['systems'][a];
    }
    ['setupSystemUpdateOrder'](a, b) {
        this['systemManager']['setupSystemUpdateOrder'](a, b);
    }
    ['createEntity']() {
        var a = this['entityManager']['newEntity']();
        return a;
    }
    ['destroyEntity'](a) {
        if (a === -0x1)
            return;
        var b = this['entityManager']['getIdx'](a);
        if (b === -0x1)
            return;
        var c = this['entityManager']['entities']['objs'][b];
        c['checkedDestroy'] = !![];
    }
    ['attachComponent'](a, b) {
        var c = this['entityManager']['getIdx'](a);
        if (c === -0x1)
            return null;
        var d = this['entityManager']['entities']['objs'][c];
        if (d['hasComponent'](b))
            return null;
        if (d['prevTagHasComponent'](b))
            return null;
        d['addTag'](b);
        this['systemManager']['checkDataViewsToBeRenew'](d['prevTag'], d['tag']);
        d['addPrevTag'](b);
        var e = this['componentManager']['attachComponent'](c, b);
        return e;
    }
    ['detachComponent'](a, b) {
        var c = this['entityManager']['getIdx'](a);
        if (c === -0x1)
            return;
        var d = this['entityManager']['entities']['objs'][c];
        d['removeTag'](b);
        this['systemManager']['checkComponentsToBeRenew'](b);
    }
}