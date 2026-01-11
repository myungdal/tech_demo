class DyingEffectComponent extends Component {
    static ['componentId'] = -0x1;
    ['duration'] = 0x0;
    ['max_duration'] = 0x0;
    ['start'](a) {
        this['max_duration'] = this['duration'] = a;
    }
    ['killerTeam'] = -0x1;
    ['killerCharacterType'] = -0x1;
}