class ElectricShockEffectComponent extends Component {
    static ['componentId'] = -0x1;
    ['duration'] = 0x0;
    ['max_duration'] = 0x0;
    ['contact_cnt'] = 0x0;
    ['target'] = -0x1;
    ['start'](a, b) {
        this['max_duration'] = this['duration'] = a;
        this['contact_cnt'] = b;
        this['target'] = -0x1;
    }
}