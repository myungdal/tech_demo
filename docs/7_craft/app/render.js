class Render {
    ['engine'] = null;
    ['canvas'] = null;
    ['ctx'] = null;
    ['characterImgs'] = null;
    ['teamColors'] = [
        '#FF0000',
        '#00FF00',
        '#0000FF',
        '#000000'
    ];
    ['characterRendererFncs'] = {};
    ['buildingRendererFncs'] = {};
    constructor(a, b, c, d) {
        this['engine'] = a;
        this['canvas'] = b;
        this['ctx'] = c;
        this['ctx']['lineWidth'] = 0x1;
        this['ctx']['font'] = '16px\x20Verdana';
        this['characterImgs'] = d;
    }
    ['initCharacterRenderFncs']() {
        this['characterRendererFncs'][WarriorTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x2, 0x14 * 0x1, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][ArcherTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x9, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][HealerTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x1, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][BirdTypeComponent['characterType']] = function (a, b, d, e) {
            a['moveTo'](d['x'], d['y']);
            a['lineTo'](d['x'] + e['vx'] * 0x3 + Math['random']() - 0.5, d['y'] + e['vy'] * 0x3 + Math['random']() - 0.5);
            return !![];
        }['bind'](this);
        this['characterRendererFncs'][BomberTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x4, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][BazookaTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x5, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][ElectricianTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x6, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][GiantTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x7, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][HammerTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x8, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][LaserTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x9, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][MagnetTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0xa, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][ParasiteTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0xb, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][SniperTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x0, 0x14 * 0x0, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][DraculaTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x1, 0x14 * 0x1, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][SpiderTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x2, 0x14 * 0x1, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][VirusTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x3, 0x14 * 0x1, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['characterRendererFncs'][SnakeTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0x4, 0x14 * 0x1, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
    }
    ['initBuildingRenderFncs']() {
        this['buildingRendererFncs'][TowerTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0xa, 0x14 * 0x7, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
        this['buildingRendererFncs'][SpawnerTypeComponent['characterType']] = function (a, b, d, e) {
            a['drawImage'](this['characterImgs'], 0x14 * 0xb, 0x14 * 0x7, 0x14, 0x14, d['x'] - 0xa, d['y'] - 0xa, 0x14, 0x14);
            return ![];
        }['bind'](this);
    }
    ['characterRenderer'](a, b, d, e) {
        var f = a['getSystem'](d);
        var g = a['systemManager']['dataViews'][f['componentsInfo']['key']];
        f['sort'](g, a);
        var h = f['getDataRowCnt'](g);
        for (var j = 0x0; j < h; ++j) {
            var k = f['getDataValue'](g, f['indices'][j], 0x1);
            var l = f['getDataValue'](g, f['indices'][j], 0x2);
            var m = f['getDataValue'](g, f['indices'][j], 0x3);
            b['strokeStyle'] = this['teamColors'][k['team']];
            if (k['life'] > k['characterAttribute']['max_life'] * 0.9)
                b['globalAlpha'] = 0x1;
            else if (k['life'] > k['characterAttribute']['max_life'] * 0.6)
                b['globalAlpha'] = 0.8;
            else if (k['life'] > k['characterAttribute']['max_life'] * 0.4)
                b['globalAlpha'] = 0.6;
            else if (k['life'] > k['characterAttribute']['max_life'] * 0.1)
                b['globalAlpha'] = 0.4;
            else
                b['globalAlpha'] = 0.2;
            b['beginPath']();
            if (!e[k['type']](b, k, l, m)) {
                b['moveTo'](l['x'] - 0xa, l['y'] - 0xa);
                b['lineTo'](l['x'] + 0xa, l['y'] - 0xa);
                b['lineTo'](l['x'] + 0xa, l['y'] + 0xa);
                b['lineTo'](l['x'] - 0xa, l['y'] + 0xa);
            }
            b['closePath']();
            b['stroke']();
        }
        b['globalAlpha'] = 0x1;
    }
    ['buildingRenderer'](a, b, d, e) {
        var f = a['getSystem'](d);
        var g = a['systemManager']['dataViews'][f['componentsInfo']['key']];
        f['sort'](g, a);
        var h = f['getDataRowCnt'](g);
        for (var j = 0x0; j < h; ++j) {
            var k = f['getDataValue'](g, f['indices'][j], 0x1);
            var l = f['getDataValue'](g, f['indices'][j], 0x2);
            var m = f['getDataValue'](g, f['indices'][j], 0x3);
            b['strokeStyle'] = this['teamColors'][k['team']];
            if (k['life'] > k['characterAttribute']['max_life'] * 0.9)
                b['globalAlpha'] = 0x1;
            else if (k['life'] > k['characterAttribute']['max_life'] * 0.6)
                b['globalAlpha'] = 0.8;
            else if (k['life'] > k['characterAttribute']['max_life'] * 0.4)
                b['globalAlpha'] = 0.6;
            else if (k['life'] > k['characterAttribute']['max_life'] * 0.1)
                b['globalAlpha'] = 0.4;
            else
                b['globalAlpha'] = 0.2;
            b['beginPath']();
            if (!e[k['type']](b, k, l, m)) {
                b['moveTo'](l['x'] - 0xa, l['y'] - 0xa);
                b['lineTo'](l['x'] + 0xa, l['y'] - 0xa);
                b['lineTo'](l['x'] + 0xa, l['y'] + 0xa);
                b['lineTo'](l['x'] - 0xa, l['y'] + 0xa);
            }
            b['closePath']();
            b['stroke']();
        }
        b['globalAlpha'] = 0x1;
    }
    ['skillRenderer'](a, b, c, d) {
        var e = a['getSystem'](c);
        var f = a['systemManager']['dataViews'][e['componentsInfo']['key']];
        e['sort'](f, a);
        var g = e['getDataRowCnt'](f);
        for (var h = 0x0; h < g; ++h) {
            var j = e['getDataValue'](f, e['indices'][h], 0x1);
            var k = e['getDataValue'](f, e['indices'][h], 0x2);
            if (k['post_delay'] === 0x0)
                continue;
            var l = a['getComponent'](k['target'], PositionComponent['componentId']);
            if (!l)
                continue;
            b['beginPath']();
            d(b, j, l, k);
            b['closePath']();
            b['stroke']();
        }
    }
    ['damageRenderer'](a, b, c, d) {
        var e = a['getSystem'](c);
        var f = a['systemManager']['dataViews'][e['componentsInfo']['key']];
        e['sort'](f, a);
        var g = e['getDataRowCnt'](f);
        b['fillStyle'] = 'red';
        for (var h = 0x0; h < g; ++h) {
            var j = e['getDataValue'](f, e['indices'][h], 0x1);
            var k = e['getDataValue'](f, e['indices'][h], 0x2);
            b['beginPath']();
            d(b, j, k);
            b['closePath']();
            b['stroke']();
            b['fillText'](k['power'], j['x'], j['y']);
        }
    }
    ['effectRenderer'](a, b, c, d) {
        var e = a['getSystem'](c);
        var g = a['systemManager']['dataViews'][e['componentsInfo']['key']];
        e['sort'](g, a);
        var h = e['getDataRowCnt'](g);
        for (var j = 0x0; j < h; ++j) {
            var k = e['getDataValue'](g, e['indices'][j], 0x1);
            var l = e['getDataValue'](g, e['indices'][j], 0x2);
            b['beginPath']();
            d(b, k, l);
            b['closePath']();
            b['stroke']();
        }
    }
    ['update']() {
        var a = this['engine'];
        var b = this['canvas'];
        var d = this['ctx'];
        d['clearRect'](0x0, 0x0, b['width'], b['height']);
        var e = null;
        var f = 0x0;
        var g = 0x0;
        var h = null;
        this['characterRenderer'](a, d, CharacterRenderSystem['characterRenderSystemId'], this['characterRendererFncs']);
        this['buildingRenderer'](a, d, BuildingRenderSystem['buildingRenderSystemId'], this['buildingRendererFncs']);
        d['strokeStyle'] = '#99000033';
        this['skillRenderer'](a, d, SkillRenderSystem['attackSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['movingAttackSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['dashSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['electricShockSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['healSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['bombSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['bazookaSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['bazookaSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['magnetSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['parasiteSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['bloodSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['webSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['virusSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['poisonSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        this['skillRenderer'](a, d, SkillRenderSystem['hammerSkillRenderSystemId'], function (w, z, A, B) {
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](A['x'], A['y']);
        }['bind'](this));
        d['strokeStyle'] = '#FF0000FF';
        this['damageRenderer'](a, d, DamageRenderSystem['damageRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#FF00FFFF';
        this['effectRenderer'](a, d, EffectRenderSystem['dashEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#0000FFFF';
        this['effectRenderer'](a, d, EffectRenderSystem['electricShockEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
            var B = a['getComponent'](A['target'], PositionComponent['componentId']);
            if (!B)
                return;
            w['closePath']();
            w['stroke']();
            w['beginPath']();
            w['moveTo'](z['x'], z['y']);
            w['lineTo'](B['x'], B['y']);
        }['bind'](this));
        d['strokeStyle'] = '#00FF00FF';
        this['effectRenderer'](a, d, EffectRenderSystem['healingEffectRenderSystemId'], function (w, z, A) {
            w['moveTo'](z['x'] + 0x16, z['y']);
            w['lineTo'](z['x'] - 0x16, z['y']);
            w['moveTo'](z['x'], z['y'] - 0x16);
            w['lineTo'](z['x'], z['y'] + 0x16);
        }['bind'](this));
        d['strokeStyle'] = '#99999999';
        this['effectRenderer'](a, d, EffectRenderSystem['runawayEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#99999999';
        this['effectRenderer'](a, d, EffectRenderSystem['stunEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#FF00FFFF';
        this['effectRenderer'](a, d, EffectRenderSystem['explosionEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0x10, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#00FFFFFF';
        this['effectRenderer'](a, d, EffectRenderSystem['magnetEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0x10, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#99999999';
        this['effectRenderer'](a, d, EffectRenderSystem['parasiteEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#990000FF';
        this['effectRenderer'](a, d, EffectRenderSystem['bloodEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#009900FF';
        this['effectRenderer'](a, d, EffectRenderSystem['webEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#FF9999FF';
        this['effectRenderer'](a, d, EffectRenderSystem['virusEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#99999999';
        this['effectRenderer'](a, d, EffectRenderSystem['poisonEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        d['strokeStyle'] = '#99999999';
        this['effectRenderer'](a, d, EffectRenderSystem['dyingEffectRenderSystemId'], function (w, z, A) {
            w['arc'](z['x'], z['y'], 0xc, 0x0, Math['PI'] * 0x2, !![]);
        }['bind'](this));
        h = a['getSystem'](MotionRenderSystem['skillMotionRenderSystemId']);
        e = a['systemManager']['dataViews'][h['componentsInfo']['key']];
        f = h['getDataRowCnt'](e);
        d['fillStyle'] = 'black';
        for (g = 0x0; g < f; ++g) {
            var j = h['getDataValue'](e, g, 0x1);
            var k = h['getDataValue'](e, g, 0x2);
            d['fillText'](Math['floor'](k['duration'] / k['max_duration'] * 0x64), j['x'], j['y']);
        }
        h = a['getSystem'](MotionRenderSystem['movingSkillMotionRenderSystemId']);
        e = a['systemManager']['dataViews'][h['componentsInfo']['key']];
        f = h['getDataRowCnt'](e);
        d['strokeStyle'] = '#0000FFFF';
        for (g = 0x0; g < f; ++g) {
            var j = h['getDataValue'](e, g, 0x1);
            var l = h['getDataValue'](e, g, 0x2);
            d['fillText'](Math['floor'](l['duration'] / l['max_duration'] * 0x64), j['x'], j['y']);
        }
        var m = 0x0;
        var o = 0x0;
        var q = 0x1 << barrierSystem['scale'];
        d['strokeStyle'] = '#883333FF';
        var s = 0x1 << barrierSystem['scale'] - 0x1;
        for (m = 0x0; m < barrierSystem['colCnt']; ++m) {
            for (o = 0x0; o < barrierSystem['rowCnt']; ++o) {
                var t = barrierSystem['getCell'](m, o);
                var u = q * m + q * 0.5;
                var v = q * o + q * 0.5;
                if (t['b']) {
                    d['strokeStyle'] = '#883333FF';
                    d['beginPath']();
                    d['arc'](u, v, 0x2, 0x0, Math['PI'] * 0x2, !![]);
                    d['moveTo'](u - s, v - s);
                    d['lineTo'](u + s, v - s);
                    d['lineTo'](u + s, v + s);
                    d['lineTo'](u - s, v + s);
                    d['closePath']();
                    d['stroke']();
                }
                d['strokeStyle'] = '#88333333';
                if (t['nextCell']) {
                    if (t['nextCell'] === t['c4']) {
                        d['beginPath']();
                        d['moveTo'](u, v);
                        d['lineTo'](u - s, v);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['nextCell'] === t['c6']) {
                        d['beginPath']();
                        d['moveTo'](u, v);
                        d['lineTo'](u + s, v);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['nextCell'] === t['c8']) {
                        d['beginPath']();
                        d['moveTo'](u, v);
                        d['lineTo'](u, v - s);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['nextCell'] === t['c2']) {
                        d['beginPath']();
                        d['moveTo'](u, v);
                        d['lineTo'](u, v + s);
                        d['closePath']();
                        d['stroke']();
                    }
                }
                if (t['detourDir7'] || t['detourDir9'] || t['detourDir1'] || t['detourDir3']) {
                    d['strokeStyle'] = '#33888833';
                    d['beginPath']();
                    d['arc'](u, v, 0x2, 0x0, Math['PI'] * 0x2, !![]);
                    d['moveTo'](u - s, v - s);
                    d['lineTo'](u + s, v - s);
                    d['lineTo'](u + s, v + s);
                    d['lineTo'](u - s, v + s);
                    d['closePath']();
                    d['stroke']();
                }
                if (t['detourDir7']) {
                    let w = u - s * 0.5;
                    let z = v - s * 0.5;
                    let A = s * 0.5;
                    if (t['detourDir7'] === t['c4']) {
                        d['beginPath']();
                        d['moveTo'](w, z);
                        d['lineTo'](w - A, z);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir7'] === t['c6']) {
                        d['beginPath']();
                        d['moveTo'](w, z);
                        d['lineTo'](w + A, z);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir7'] === t['c8']) {
                        d['beginPath']();
                        d['moveTo'](w, z);
                        d['lineTo'](w, z - A);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir7'] === t['c2']) {
                        d['beginPath']();
                        d['moveTo'](w, z);
                        d['lineTo'](w, z + A);
                        d['closePath']();
                        d['stroke']();
                    }
                }
                if (t['detourDir9']) {
                    let B = u + s * 0.5;
                    let C = v - s * 0.5;
                    let D = s * 0.5;
                    if (t['detourDir9'] === t['c4']) {
                        d['beginPath']();
                        d['moveTo'](B, C);
                        d['lineTo'](B - D, C);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir9'] === t['c6']) {
                        d['beginPath']();
                        d['moveTo'](B, C);
                        d['lineTo'](B + D, C);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir9'] === t['c8']) {
                        d['beginPath']();
                        d['moveTo'](B, C);
                        d['lineTo'](B, C - D);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir9'] === t['c2']) {
                        d['beginPath']();
                        d['moveTo'](B, C);
                        d['lineTo'](B, C + D);
                        d['closePath']();
                        d['stroke']();
                    }
                }
                if (t['detourDir1']) {
                    let E = u - s * 0.5;
                    let F = v + s * 0.5;
                    let G = s * 0.5;
                    if (t['detourDir1'] === t['c4']) {
                        d['beginPath']();
                        d['moveTo'](E, F);
                        d['lineTo'](E - G, F);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir1'] === t['c6']) {
                        d['beginPath']();
                        d['moveTo'](E, F);
                        d['lineTo'](E + G, F);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir1'] === t['c8']) {
                        d['beginPath']();
                        d['moveTo'](E, F);
                        d['lineTo'](E, F - G);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir1'] === t['c2']) {
                        d['beginPath']();
                        d['moveTo'](E, F);
                        d['lineTo'](E, F + G);
                        d['closePath']();
                        d['stroke']();
                    }
                }
                if (t['detourDir3']) {
                    let H = u + s * 0.5;
                    let I = v + s * 0.5;
                    let J = s * 0.5;
                    if (t['detourDir3'] === t['c4']) {
                        d['beginPath']();
                        d['moveTo'](H, I);
                        d['lineTo'](H - J, I);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir3'] === t['c6']) {
                        d['beginPath']();
                        d['moveTo'](H, I);
                        d['lineTo'](H + J, I);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir3'] === t['c8']) {
                        d['beginPath']();
                        d['moveTo'](H, I);
                        d['lineTo'](H, I - J);
                        d['closePath']();
                        d['stroke']();
                    }
                    if (t['detourDir3'] === t['c2']) {
                        d['beginPath']();
                        d['moveTo'](H, I);
                        d['lineTo'](H, I + J);
                        d['closePath']();
                        d['stroke']();
                    }
                }
            }
        }
    }
}