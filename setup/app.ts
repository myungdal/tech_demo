// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import { filelog, log } from './Util/log';
import Db from './Util/db';

import * as fs from 'fs';
import * as path from 'path';

import sql_to_db from './Tool/sql_to_db';
import static_db_backup from './Tool/static_db_backup';
import static_db_restore from './Tool/static_db_restore';
import db_to_code from './Tool/db_to_code';
import setup_config from './Tool/setup_config';
import setup_enum_generation from './Tool/setup_enum_generation';
import setup_packet_generation from './Tool/setup_packet_generation';
import mapeditor_export_db from './Tool/mapeditor_export_db';
import mapeditor_import_db from './Tool/mapeditor_import_db';
import client_code_auto_generator_event from './Tool/client_code_auto_generator_event';
import client_code_auto_generator_component_printer from './Tool/client_code_auto_generator_component_printer';


if (!process.argv[2])
    process.stdin.resume();

process.on('exit', function (exitCode) {
    log.info(`exit: ${exitCode}`);
});
process.on('SIGINT', function () {
    log.info(`SIGINT`);
});
process.on('SIGUSR1', function () {
    log.info(`SIGUSR1`);
});
process.on('SIGUSR2', function () {
    log.info(`SIGUSR2`);
});
process.on('uncaughtException', function (err) {
    log.fatal(`uncaughtException: ${err}`);
    // fail-fast: setup 占쌘듸옙화 占쏙옙占쏙옙 占쏙옙占쌤곤옙 占쌩삼옙占싹몌옙 占쏙옙占?占쏙옙占쏙옙 占쏙옙占쏙옙
    process.exit(1);
});
process.on('unhandledRejection', function (reason) {
    log.fatal(`unhandledRejection: ${reason}`);
    // fail-fast
    process.exit(1);
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setup auto-generated file logging
// - 占쏙옙 Tool(ts)占쏙옙占쏙옙 占쏙옙占쏙옙/占쏙옙占쏙옙占싹댐옙 占쏙옙占쏙옙占쏙옙 Build/output/setup.log 占싸깍옙
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

(function setupGeneratedFileLogger() {
    const repoRoot = path.resolve(__dirname, '..');
    const setupLogPath = path.join(repoRoot, 'Build', 'output', 'setup.log');
    const setupLogDir = path.dirname(setupLogPath);

    const origWriteFileSync = fs.writeFileSync.bind(fs);
    const origCopyFileSync = (fs as any).copyFileSync ? (fs as any).copyFileSync.bind(fs) : undefined;
    const origAppendFileSync = fs.appendFileSync.bind(fs);

    function toAbsFilePath(p: any): string | null {
        if (typeof p === 'number') return null; // fd
        const s = String(p);
        if (path.isAbsolute(s)) return s;
        return path.resolve(process.cwd(), s);
    }

    function isSamePath(a: string, b: string): boolean {
        return path.resolve(a).toLowerCase() === path.resolve(b).toLowerCase();
    }

    function shouldLog(absPath: string): boolean {
        if (isSamePath(absPath, setupLogPath)) return false;
        if (absPath.toLowerCase().endsWith('.tmp')) return false;
        return true;
    }

    function relOrAbs(absPath: string): string {
        const rel = path.relative(repoRoot, absPath);
        if (!rel || rel.startsWith('..')) return absPath;
        return rel.replace(/\\/g, '/');
    }

    function appendLogLine(op: string, absPath: string) {
        try {
            fs.mkdirSync(setupLogDir, { recursive: true } as any);
            const ts = new Date().toISOString();
            const line = `${ts}\t${op}\t${relOrAbs(absPath)}\r\n`;
            origAppendFileSync(setupLogPath, line, { encoding: 'utf8' } as any);
        } catch (e) {
            // 占싸깍옙 占쏙옙占싻곤옙 setup 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占십듸옙占쏙옙 占싼댐옙.
        }
    }

    (fs as any).writeFileSync = function patchedWriteFileSync(file: any, data: any, options?: any) {
        const absPath = toAbsFilePath(file);
        const result = origWriteFileSync(file as any, data as any, options as any);
        if (absPath && shouldLog(absPath)) {
            appendLogLine('WRITE', absPath);
        }
        return result;
    };

    if (origCopyFileSync) {
        (fs as any).copyFileSync = function patchedCopyFileSync(src: any, dest: any, flags?: any) {
            const absDest = toAbsFilePath(dest);
            const result = origCopyFileSync(src, dest, flags);
            if (absDest && shouldLog(absDest)) {
                appendLogLine('COPY', absDest);
            }
            return result;
        };
    }
})();


const runType = (process.argv[2]) ? process.argv[2] : 'db_to_txt';

(async function run() {
    if ('sql_to_db_sp' === runType) {
        await (new sql_to_db()).Run('sp');
    }
    else if ('sql_to_db_table' === runType) {
        await (new sql_to_db()).Run('table');
    }
    else if ('dumpStaticDb' === runType) {
        await (new static_db_backup()).Run('STATIC');
    }
    else if ('restoreStaticDb' === runType) {
        await (new static_db_restore()).Run('STATIC');
    }
    else if ('dumpUserDb' === runType) {
        await (new static_db_backup()).Run('USER');
    }
    else if ('restoreUserDb' === runType) {
        await (new static_db_restore()).Run('USER');
    }
    else if ('dumpMainDb' === runType) {
        await (new static_db_backup()).Run('MAIN');
    }
    else if ('restoreMainDb' === runType) {
        await (new static_db_restore()).Run('MAIN');
    }
    else if ('db_to_code_cpp' === runType) {
        await (new db_to_code()).Run('cpp');
    }
    else if ('db_to_code_sql' === runType) {
        await (new db_to_code()).Run('sql');
    }
    else if ('db_to_code_txt' === runType) {
        await (new db_to_code()).Run('txt');
    }
    else if ('setup_config' === runType) {
        await (new setup_config()).Run();
    }
    else if ('setup_enum_generation' === runType) {
        await (new setup_enum_generation()).Run();
    }
    else if ('setup_packet_generation' === runType) {
        await (new setup_packet_generation()).Run();
    }
    else if ('mapeditor_export_db' === runType) {
        await (new mapeditor_export_db()).Run();
    }
    else if ('mapeditor_import_db' === runType) {
        await (new mapeditor_import_db()).Run();
    }
    else if ('client_code_auto_generator' === runType) {
        await (new client_code_auto_generator_event()).Run();
        await (new client_code_auto_generator_component_printer()).Run();
    }

    Db.destroyAll();
})();

