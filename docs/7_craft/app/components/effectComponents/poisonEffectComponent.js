class PoisonEffectComponent extends Component {
    static ['componentId'] = -0x1;
    ['duration'] = 0x0;
    ['max_duration'] = 0x0;
    ['start'](a) {
        this['max_duration'] = this['duration'] = a;
    }
    ['team'] = -0x1;
    ['characterType'] = -0x1;
}