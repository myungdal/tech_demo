class Effect {
    ['duration'] = 0x0;
    ['max_duration'] = 0x0;
    constructor(a, b, c) {
        this['max_duration'] = this['duration'] = c;
        this['onStart'](a, b);
    }
    ['update'](a, b) {
        if (this['duration'] > 0x0) {
            this['duration'] -= 0x1;
            if (this['duration'] > 0x0)
                return !![];
        }
        this['duration'] = 0x0;
        this['onFinish'](a, b);
        return ![];
    }
    ['onStart'](a, b) {
    }
    ['onFinish'](a, b) {
    }
}
class Effect_fear extends Effect {
    ['onStart'](a, b) {
        ++a['state_fear'];
    }
    ['onFinish'](a, b) {
        --a['state_fear'];
        if (a['state_fear'] < 0x0)
            a['state_fear'] = 0x0;
    }
}
class Effect_stun extends Effect {
    ['onStart'](a, b) {
        ++a['state_stun'];
    }
    ['onFinish'](a, b) {
        --a['state_stun'];
        if (a['state_stun'] < 0x0)
            a['state_stun'] = 0x0;
    }
}
class Effect_dash extends Effect {
    ['onStart'](a, b) {
        ++a['state_dash'];
    }
    ['onFinish'](a, b) {
        --a['state_dash'];
        if (a['state_dash'] < 0x0)
            a['state_dash'] = 0x0;
    }
}
class Effect_electricShock extends Effect {
    ['enemy'] = null;
    ['victims'] = [];
    ['electricShockCommunicationDelay'] = 0x0;
    constructor(a, b, c, d, e, f) {
        super(a, b, c);
        this['enemy'] = d;
        this['victims'] = e;
        this['victims']['push'](a);
        this['electricShockCommunicationDelay'] = ELECTRICSHOCK_COMMUNICATIONDELAY;
    }
    ['update'](a, b) {
        var c = super['update'](a, b);
        if (c === !![] && this['electricShockCommunicationDelay'] > 0x0) {
            --this['electricShockCommunicationDelay'];
            if (this['electricShockCommunicationDelay'] === 0x0) {
                if (this['victims']['length'] > 0x5)
                    return;
                if (a['f0'] && this['victims']['findIndex']((d, e) => {
                        return d === a['f0'];
                    }) === -0x1)
                    a['f0']['tryElectricShock'](b, this['max_duration'], this['enemy'], this['victims'], a);
                if (a['f1'] && this['victims']['findIndex']((d, e) => {
                        return d === a['f1'];
                    }) === -0x1)
                    a['f1']['tryElectricShock'](b, this['max_duration'], this['enemy'], this['victims'], a);
                if (a['f2'] && this['victims']['findIndex']((d, e) => {
                        return d === a['f2'];
                    }) === -0x1)
                    a['f2']['tryElectricShock'](b, this['max_duration'], this['enemy'], this['victims'], a);
            }
        }
    }
    ['onStart'](a, b) {
        ++a['state_electricShock'];
    }
    ['onFinish'](a, b) {
        --a['state_electricShock'];
        if (a['state_electricShock'] < 0x0)
            a['state_electricShock'] = 0x0;
    }
}