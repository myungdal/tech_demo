"use strict";
// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================
Object.defineProperty(exports, "__esModule", { value: true });
exports.log = exports.filelog = void 0;
exports.setupLog = setupLog;
const log4js = require("log4js");
const fs = require("fs");
const path = require("path");
const configure = {
    appenders: {
        file: {
            type: 'multiFile',
            base: 'logs/',
            property: 'categoryName',
            extension: '.log',
            maxLogSize: 10485760,
            backups: 1000,
        },
        console: {
            type: 'console'
        }
    },
    categories: {
        default: {
            appenders: ['file'],
            level: 'trace'
        },
        console: {
            appenders: ['console'],
            level: 'trace'
        }
    }
};
log4js.configure(configure);
exports.filelog = log4js.getLogger;
exports.log = log4js.getLogger('console');
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup automation log (Build/output/setup.log)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function setupLog(op, detail) {
    try {
        const repoRoot = path.resolve(__dirname, '..', '..');
        const logPath = path.join(repoRoot, 'Build', 'output', 'setup.log');
        fs.mkdirSync(path.dirname(logPath), { recursive: true });
        const ts = new Date().toISOString();
        const line = `${ts}\t${op}\t${detail}\r\n`;
        fs.appendFileSync(logPath, line, { encoding: 'utf8' });
        // 콘솔에도 출력
        exports.log.info(`[${op}] ${detail}`);
    }
    catch (e) {
        // logging failure should not break setup pipeline
    }
}
//# sourceMappingURL=log.js.map