var characterAttribute_warrior_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_warrior);
var characterAttribute_archer_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_archer);
var characterAttribute_healer_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_healer);
var characterAttribute_bird_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_bird);
var characterAttribute_bomber_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_bomber);
var characterAttribute_bazooka_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_bazooka);
var characterAttribute_electrician_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_electrician);
var characterAttribute_giant_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_giant);
var characterAttribute_hammer_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_hammer);
var characterAttribute_laser_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_laser);
var characterAttribute_magnet_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_magnet);
var characterAttribute_parasite_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_parasite);
var characterAttribute_sniper_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_sniper);
var characterAttribute_dracula_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_dracula);
var characterAttribute_spider_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_spider);
var characterAttribute_virus_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_virus);
var characterAttribute_snake_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_snake);
var characterAttribute_tower_idx = engine['regAttribute'](CharacterAttribute['attributeType'], characterAttribute_tower);
var skillAttribute_warrior_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_warrior0);
var skillAttribute_warrior_idx1 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_warrior1);
var skillAttribute_archer_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_archer0);
var skillAttribute_healer_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_healer0);
var skillAttribute_bird_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_bird0);
var skillAttribute_bomber_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_bomber0);
var skillAttribute_bazooka_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_bazooka0);
var skillAttribute_electrician_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_electrician0);
var skillAttribute_giant_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_giant0);
var skillAttribute_hammer_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_hammer0);
var skillAttribute_laser_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_laser0);
var skillAttribute_magnet_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_magnet0);
var skillAttribute_parasite_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_parasite0);
var skillAttribute_sniper_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_sniper0);
var skillAttribute_dracula_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_dracula0);
var skillAttribute_dracula_idx1 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_dracula1);
var skillAttribute_spider_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_spider0);
var skillAttribute_spider_idx1 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_spider1);
var skillAttribute_virus_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_virus0);
var skillAttribute_snake_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_snake0);
var skillAttribute_tower_idx0 = engine['regAttribute'](SkillAttribute['attributeType'], skillAttribute_tower0);
var canvas = document['getElementById']('canvas');
var ctx = canvas['getContext']('2d');
var characterImgs = document['getElementById']('img');
var render = new Render(engine, canvas, ctx, characterImgs);
render['initCharacterRenderFncs']();
render['initBuildingRenderFncs']();
var getRandomX = function (a) {
    if (a === 0x0)
        return canvas['width'] * 0.25 * (0x1 + Math['random']());
    if (a === 0x1)
        return canvas['width'] * 0.25 * (0x3 + Math['random']());
    if (a === 0x2)
        return canvas['width'] * 0.25 * (0x0 + Math['random']());
    if (a === 0x3)
        return canvas['width'] * 0.25 * (0x2 + Math['random']());
};
var getRandomY = function (a) {
    if (a === 0x0)
        return canvas['height'] * 0.25 * (0x0 + Math['random']());
    if (a === 0x1)
        return canvas['height'] * 0.25 * (0x1 + Math['random']());
    if (a === 0x2)
        return canvas['height'] * 0.25 * (0x2 + Math['random']());
    if (a === 0x3)
        return canvas['height'] * 0.25 * (0x3 + Math['random']());
};
var createRandomEntities = function (a) {
    var b;
    for (b = 0x0; b < 0x1; ++b)
        createWarrior(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createArcher(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createHealer(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createBird(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createBomber(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createElectrician(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createGiant(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createHammer(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createLaser(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createParasite(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createSniper(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createDracula(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createSpider(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createVirus(engine, getRandomX(a), getRandomY(a), a);
    for (b = 0x0; b < 0x1; ++b)
        createSnake(engine, getRandomX(a), getRandomY(a), a);
};
creationCnt = 0x0;
maxEntityCnt = 0x0;
lockStep = new LockStep(Date['now']() + 0x3e8, 0xf, 0xbb8);
function update() {
    if (creationCnt !== 0x0 && engine['entityManager']['entities']['objCnt'] < maxEntityCnt) {
        --creationCnt;
        createRandomEntities(0x0);
        createRandomEntities(0x1);
        createRandomEntities(0x2);
        createRandomEntities(0x3);
    }
    var a = lockStep ? lockStep['getStepCntForNow']() : 0x1;
    if (a < 0x0) {
        lockStep = new LockStep(Date['now'](), 0x10, 0xbb8);
        setTimeout(update, 0x0);
        return;
    } else if (a === 0x0) {
        setTimeout(update, 0x0);
        return;
    }
    var b = 0x0;
    for (b = 0x0; b < a; ++b) {
        engine['update']();
    }
    render['update']();
    setTimeout(update, 0x0);
}
update();
var mousePos = {
    'x': 0x0,
    'y': 0x0
};
canvas['addEventListener']('mousemove', onMouseMove, ![]);
document['addEventListener']('keydown', onKeyDown, ![]);
function onKeyDown(a) {
    a['preventDefault']();
    a['stopPropagation']();
    var b = -0x1;
    var c = mousePos['x'];
    var d = mousePos['y'];
    if (c < canvas['width'] / 0x2 && d < canvas['height'] / 0x2)
        b = 0x0;
    else if (c > canvas['width'] / 0x2 && d < canvas['height'] / 0x2)
        b = 0x1;
    else if (c < canvas['width'] / 0x2 && d > canvas['height'] / 0x2)
        b = 0x2;
    else
        b = 0x3;
    var f = 0x0;
    var g = 0x0;
    var h = 0x44;
    if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createWarrior(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createArcher(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createHealer(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createBird(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createBomber(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createBazooka(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createElectrician(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createGiant(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createHammer(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createLaser(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createMagnet(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createParasite(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createSniper(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createDracula(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createSpider(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createVirus(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createSnake(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createTower(engine, c, d, b);
    else if (a['keyCode'] === h++)
        for (g = 0x0; g < 0x1; ++g)
            createSpawner(engine, c, d, b);
    else if (a['keyCode'] === 0x41) {
        createRandomEntities(0x0);
        createRandomEntities(0x1);
        createRandomEntities(0x2);
        createRandomEntities(0x3);
    } else if (a['keyCode'] === 0x42) {
        createRandomEntities(b);
    } else if (a['keyCode'] === 0x30) {
        var j = engine['systemManager']['dataViews_needRenew'];
        engine['systemManager']['dataViews_needRenew'] = engine['systemManager']['dataViews_noNeedRenew'];
        engine['systemManager']['dataViews_noNeedRenew'] = j;
    } else if (a['keyCode'] === 0x31) {
        patrolSystem0['setFirstPath'](c >> 0x5, d >> 0x5);
    } else if (a['keyCode'] === 0x32) {
        patrolSystem1['setFirstPath'](c >> 0x5, d >> 0x5);
    } else if (a['keyCode'] === 0x33) {
        patrolSystem2['setFirstPath'](c >> 0x5, d >> 0x5);
    } else if (a['keyCode'] === 0x34) {
        patrolSystem3['setFirstPath'](c >> 0x5, d >> 0x5);
    } else if (a['keyCode'] === 0x35) {
        var k = engine['createEntity']();
        f = engine['attachComponent'](k, PositionComponent['componentId']);
        var l = engine['attachComponent'](k, ExplosionEffectComponent['componentId']);
        f['x'] = c;
        f['y'] = d;
        f['vx'] = 0x0;
        f['vy'] = 0x0;
        f['speed'] = 0x0;
        l['power'] = 0x1388;
        l['max_duration'] = l['duration'] = ConstAttribute['EXPLOSION_DURATION'];
    } else if (a['keyCode'] === 0x36) {
        var m = engine['createEntity']();
        f = engine['attachComponent'](m, PositionComponent['componentId']);
        var o = engine['attachComponent'](m, MagnetEffectComponent['componentId']);
        f['x'] = c;
        f['y'] = d;
        f['vx'] = 0x0;
        f['vy'] = 0x0;
        f['speed'] = 0x0;
        o['power'] = 0x0;
        o['timer'] = ConstAttribute['MAGNET_TIMER'];
        o['max_duration'] = o['duration'] = ConstAttribute['MAGNET_DURATION'];
    } else if (a['keyCode'] === 0x37) {
        var q = engine['entityManager']['entities']['objCnt'];
        var r = engine['entityManager']['entities']['objs'];
        for (var g = 0x0; g < q; ++g) {
            var a = r[g];
            console['log'](a['id']);
            for (var j = 0x0; j < 0x40; ++j) {
                if (!a['tag']['isOn'](j))
                    continue;
                var s = engine['componentManager']['templates'][j];
                console['log'](a['id'] + '\x09' + s['name']);
            }
        }
    } else {
        console['log']({
            'x': c,
            'y': d,
            'c': c >> 0x5,
            'r': d >> 0x5
        });
    }
}
function onMouseMove(a) {
    a['preventDefault']();
    a['stopPropagation']();
    mousePos['x'] = a['pageX'] - canvas['offsetLeft'];
    mousePos['y'] = a['pageY'] - canvas['offsetTop'];
}