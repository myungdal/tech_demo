class HealSkillSystem extends System {
    ['friendSystem'] = null;
    ['init'](a) {
        this['includeComponent'](PositionComponent['componentId'])['includeComponent'](CharacterComponent['componentId'])['includeComponent'](FriendComponent['componentId'])['includeComponent'](HealSkillComponent['componentId'])['exceptComponent'](MovingSkillMotionComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](ElectricShockEffectComponent['componentId'])['exceptComponent'](StunEffectComponent['componentId'])['exceptComponent'](RunawayEffectComponent['componentId'])['exceptComponent'](DyingEffectComponent['componentId']);
        return super['init'](a);
    }
    ['update'](a, b) {
        var d = 0x0;
        var f = this['friendSystem']['indices']['length'];
        var g = b['systemManager']['dataViews'][this['friendSystem']['componentsInfo']['key']];
        var h = this['getDataRowCnt'](a);
        for (var k = 0x0; k < h; ++k) {
            var l = this['getDataValue'](a, k, 0x0);
            var o = this['getDataValue'](a, k, 0x1);
            var q = this['getDataValue'](a, k, 0x2);
            var r = this['getDataValue'](a, k, 0x3);
            var t = this['getDataValue'](a, k, 0x4);
            if (t['pre_delay'] > 0x0) {
                t['pre_delay'] -= 0x1;
                if (t['pre_delay'] <= 0x0) {
                    t['pre_delay'] = 0x0;
                    this['onAction'](t, b);
                }
                continue;
            }
            if (t['post_delay'] > 0x0) {
                t['post_delay'] -= 0x1;
                if (t['post_delay'] <= 0x0) {
                    t['post_delay'] = 0x0;
                    t['target'] = 0x0;
                }
                continue;
            }
            t['energy'] += o['characterAttribute']['charging'];
            if (t['energy'] > t['skillAttribute']['max_energy'])
                t['energy'] = t['skillAttribute']['max_energy'];
            if (t['energy'] < t['skillAttribute']['require_energy'])
                continue;
            t['energy'] -= t['skillAttribute']['require_energy'];
            if (this['tryHeal'](l, r['e'], r['c'], r['dd'], t, b))
                continue;
            if (r['idx'] === -0x1)
                continue;
            var u = {
                'dd': Number['MAX_VALUE'],
                'e': null,
                'c': null,
                'p': null
            };
            for (d = r['idx'] - 0x1; d >= 0x0; --d) {
                if (!this['processFriend'](d, g, l, o, q, u, t))
                    break;
            }
            for (d = r['idx'] + 0x1; d < f; ++d) {
                if (!this['processFriend'](d, g, l, o, q, u, t))
                    break;
            }
            if (this['tryHeal'](l, u['e'], u['c'], u['dd'], t, b))
                continue;
        }
    }
    ['tryHeal'](a, b, c, d, e, f) {
        if (!b)
            return ![];
        if (c['life'] >= c['characterAttribute']['max_life'])
            return ![];
        if (d > e['skillAttribute']['range'] * e['skillAttribute']['range'])
            return ![];
        this['startHeal'](a, b['id'], e, f);
        return !![];
    }
    ['startHeal'](a, b, c, d) {
        c['target'] = b;
        c['pre_delay'] = c['skillAttribute']['pre_delay'];
        c['post_delay'] = c['skillAttribute']['post_delay'];
        var f = d['attachComponent'](a['id'], MovingSkillMotionComponent['componentId']);
        if (f)
            f['start'](c['pre_delay'] + c['post_delay']);
    }
    ['onAction'](a, b) {
        var c = b['getComponent'](a['target'], CharacterComponent['componentId']);
        if (!c || c['life'] <= 0x0)
            return;
        var d = b['attachComponent'](a['target'], HealingEffectComponent['componentId']);
        if (d) {
            d['power'] = 0x0;
            d['duration'] = 0x0;
        } else {
            d = b['getComponent'](a['target'], HealingEffectComponent['componentId']);
        }
        if (d) {
            d['power'] = a['skillAttribute']['power'];
            d['duration'] += ConstAttribute['HEALING_DURATION'];
            d['max_duration'] = d['duration'];
        }
    }
    ['processFriend'](a, b, c, d, e, f, g) {
        var h = this['friendSystem']['indices'][a];
        var i = this['friendSystem']['getDataValue'](b, h, 0x0);
        var k = this['friendSystem']['getDataValue'](b, h, 0x1);
        var l = this['friendSystem']['getDataValue'](b, h, 0x2);
        if (d['team'] !== k['team'])
            return !![];
        if (k['life'] >= k['characterAttribute']['max_life'])
            return !![];
        var m = this['friendSystem']['getSquare0'](e, l);
        if (m > f['dd'])
            return ![];
        if (m > g['skillAttribute']['range'] * g['skillAttribute']['range'])
            return ![];
        var n = this['friendSystem']['getSquare1'](e, l);
        var o = m + n;
        if (o > g['skillAttribute']['range'] * g['skillAttribute']['range'])
            return !![];
        this['tryConnectToFriend'](c, f, i, k, l, o);
        return !![];
    }
    ['tryConnectToFriend'](a, b, c, d, e, f) {
        if (b['dd'] > f) {
            b['dd'] = f;
            b['e'] = c;
            b['c'] = d;
            b['p'] = e;
        }
    }
}