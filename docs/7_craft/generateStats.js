const XLSX = require('XLSX');
var fs = require('fs');
var stats = XLSX['readFile']('./app/stats.xlsx');
var characters = XLSX['utils']['sheet_to_json'](stats['Sheets']['characters'], new Object({ 'row': !![] }));
var result = {};
var className = '';
var classIdx = '';
var key = '';
for (var row of characters) {
    for (var [k, v] of Object['entries'](row)) {
        if (k === 'className') {
            className = v;
            continue;
        }
        if (k === 'classIdx') {
            classIdx = v;
            continue;
        }
        if (k === 'key') {
            key = v;
            continue;
        }
        var classStr = className + '_' + k + classIdx;
        if (!result[classStr])
            result[classStr] = {};
        result[classStr][key] = v;
    }
}
;
var str = '';
for (var [classStr, kv] of Object['entries'](result)) {
    str += 'var\x20' + classStr + '\x20=\x20{\x0d\x0a';
    for (var [k, v] of Object['entries'](kv)) {
        str += k + ':\x20' + v + ',\x0d\x0a';
    }
    str += '};';
    str += '\x0d\x0a';
}
fs['writeFile']('./app/stats.js', str, function (a) {
    if (a)
        throw a;
});
console['log']('generateStats\x20completed');