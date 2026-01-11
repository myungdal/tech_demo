// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import * as log4js from 'log4js';
import * as fs from 'fs';
import * as path from 'path';

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

export const filelog = log4js.getLogger;
export const log = log4js.getLogger('console');

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup automation log (Build/output/setup.log)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

export function setupLog(op: string, detail: string) {
    try {
        const repoRoot = path.resolve(__dirname, '..', '..');
        const logPath = path.join(repoRoot, 'Build', 'output', 'setup.log');
        fs.mkdirSync(path.dirname(logPath), { recursive: true } as any);

        const ts = new Date().toISOString();
        const line = `${ts}\t${op}\t${detail}\r\n`;
        fs.appendFileSync(logPath, line, { encoding: 'utf8' } as any);

        // 콘솔에도 출력
        log.info(`[${op}] ${detail}`);
    } catch (e) {
        // logging failure should not break setup pipeline
    }
}
