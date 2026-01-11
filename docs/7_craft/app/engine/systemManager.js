class SystemManager {
    ['systems'] = [];
    ['systemUpdateOrder'] = [];
    ['dataViews'] = {};
    ['dataViews_noNeedRenew'] = [];
    ['dataViews_needRenew'] = [];
    ['toBeUpdateComponents'] = ![];
    ['regSystem'](a) {
        this['systems']['push'](a);
        var b = this['dataViews_needRenew']['find'](function (c) {
            return c['key'] === a['componentsInfo']['key'];
        });
        if (!b) {
            this['dataViews_needRenew']['push'](a['componentsInfo']);
        }
        if (!this['dataViews'][a['componentsInfo']['key']])
            this['dataViews'][a['componentsInfo']['key']] = [];
        a['systemId'] = this['systems']['length'] - 0x1;
        return a['systemId'];
    }
    ['setupSystemUpdateOrder'](a, b) {
        var c = this['systems'][a];
        var d = {
            'system': c,
            'updater': null
        };
        if (b === System['updateType']['update'])
            d['updater'] = c['update'];
        else if (b === System['updateType']['update0'])
            d['updater'] = c['update0'];
        else if (b === System['updateType']['update1'])
            d['updater'] = c['update1'];
        else if (b === System['updateType']['update2'])
            d['updater'] = c['update2'];
        else if (b === System['updateType']['update3'])
            d['updater'] = c['update3'];
        this['systemUpdateOrder']['push'](d);
    }
    ['updateSystems'](a) {
        var b = null;
        for (var c of this['systemUpdateOrder']) {
            if (!c['system']['enabled'])
                continue;
            b = this['dataViews'][c['system']['componentsInfo']['key']];
            c['updater']['call'](c['system'], b, a);
        }
    }
    ['updateComponents'](a, b) {
        if (this['toBeUpdateComponents'] === ![])
            return;
        var c = b['componentGroup']['length'];
        var d = a['entities']['objCnt'];
        for (var e = 0x0; e < d; ++e) {
            var f = a['entities']['objs'][e];
            if (f['prevTag']['is'](f['tag']))
                continue;
            for (var g = 0x0; g < c; ++g) {
                if (f['hasComponent'](g) || !f['prevTagHasComponent'](g))
                    continue;
                b['detachComponent'](e, g);
            }
            this['checkDataViewsToBeRenew'](f['prevTag'], f['tag']);
            f['prevTag']['copyFrom'](f['tag']);
        }
        this['toBeUpdateComponents'] = ![];
    }
    ['updateDataViews'](a, b) {
        var c = this['dataViews_needRenew']['length'];
        for (var d = 0x0; d < c; ++d) {
            var e = this['dataViews_needRenew'][d];
            this['dataViews'][e['key']] = this['makeData'](e, a, b);
            this['dataViews_noNeedRenew']['push'](e);
        }
        this['dataViews_needRenew']['length'] = 0x0;
    }
    ['checkComponentsToBeRenew'](a) {
        this['toBeUpdateComponents'] = !![];
    }
    ['checkDataViewsToBeRenew'](a, b) {
        var c = a['xor'](b, BitField['Common0']);
        var d = this['dataViews_noNeedRenew']['length'];
        for (var e = d; e--;) {
            var f = this['dataViews_noNeedRenew'][e];
            if (f['including']['and'](c, BitField['Common1'])['isZero']() && f['exception']['and'](c, BitField['Common1'])['isZero']()) {
                continue;
            }
            this['dataViews_noNeedRenew']['splice'](e, 0x1);
            this['dataViews_needRenew']['push'](f);
        }
    }
    ['makeData'](a, b, c) {
        var d = [];
        var e = c['componentGroup']['length'];
        for (var f = 0x0; f < e; ++f) {
            if (a['including']['isOff'](f))
                continue;
            d['push'](c['entityComponentViews'][f]);
        }
        var g = [];
        e = d['length'];
        var h = b['entities']['objCnt'];
        for (var j = 0x0; j < h; ++j) {
            var k = !![];
            for (f = 0x0; f < e; ++f) {
                let q = d[f];
                if (!q['has'](j)) {
                    k = ![];
                    break;
                }
            }
            if (!k)
                continue;
            var l = b['entities']['objs'][j];
            if (l['tag']['and'](a['exception'], BitField['Common'])['isNotZero']())
                continue;
            g['push'](l);
            for (f = 0x0; f < e; ++f) {
                let r = d[f];
                var m = r['get'](j);
                var o = c['componentGroup'][r['componentId']];
                var p = o['objs'][m];
                g['push'](p);
            }
        }
        return g;
    }
}