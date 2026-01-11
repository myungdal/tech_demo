class VirusEffectComponent extends Component {
    static ['componentId'] = -0x1;
    ['power'] = 0x0;
    ['duration'] = 0x0;
    ['max_duration'] = 0x0;
    ['start'](a) {
        this['max_duration'] = this['duration'] = a;
    }
    ['team'] = -0x1;
    ['characterType'] = -0x1;
}