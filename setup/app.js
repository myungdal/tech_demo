"use strict";
// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
Object.defineProperty(exports, "__esModule", { value: true });
const log_1 = require("./Util/log");
const db_1 = require("./Util/db");
const fs = require("fs");
const path = require("path");
const sql_to_db_1 = require("./Tool/sql_to_db");
const static_db_backup_1 = require("./Tool/static_db_backup");
const static_db_restore_1 = require("./Tool/static_db_restore");
const db_to_code_1 = require("./Tool/db_to_code");
const setup_config_1 = require("./Tool/setup_config");
const setup_enum_generation_1 = require("./Tool/setup_enum_generation");
const setup_packet_generation_1 = require("./Tool/setup_packet_generation");
const mapeditor_export_db_1 = require("./Tool/mapeditor_export_db");
const mapeditor_import_db_1 = require("./Tool/mapeditor_import_db");
const client_code_auto_generator_event_1 = require("./Tool/client_code_auto_generator_event");
const client_code_auto_generator_component_printer_1 = require("./Tool/client_code_auto_generator_component_printer");
if (!process.argv[2])
    process.stdin.resume();
process.on('exit', function (exitCode) {
    log_1.log.info(`exit: ${exitCode}`);
});
process.on('SIGINT', function () {
    log_1.log.info(`SIGINT`);
});
process.on('SIGUSR1', function () {
    log_1.log.info(`SIGUSR1`);
});
process.on('SIGUSR2', function () {
    log_1.log.info(`SIGUSR2`);
});
process.on('uncaughtException', function (err) {
    log_1.log.fatal(`uncaughtException: ${err}`);
    // fail-fast: setup 占쌘듸옙화 占쏙옙占쏙옙 占쏙옙占쌤곤옙 占쌩삼옙占싹몌옙 占쏙옙占?占쏙옙占쏙옙 占쏙옙占쏙옙
    process.exit(1);
});
process.on('unhandledRejection', function (reason) {
    log_1.log.fatal(`unhandledRejection: ${reason}`);
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
    const origCopyFileSync = fs.copyFileSync ? fs.copyFileSync.bind(fs) : undefined;
    const origAppendFileSync = fs.appendFileSync.bind(fs);
    function toAbsFilePath(p) {
        if (typeof p === 'number')
            return null; // fd
        const s = String(p);
        if (path.isAbsolute(s))
            return s;
        return path.resolve(process.cwd(), s);
    }
    function isSamePath(a, b) {
        return path.resolve(a).toLowerCase() === path.resolve(b).toLowerCase();
    }
    function shouldLog(absPath) {
        if (isSamePath(absPath, setupLogPath))
            return false;
        if (absPath.toLowerCase().endsWith('.tmp'))
            return false;
        return true;
    }
    function relOrAbs(absPath) {
        const rel = path.relative(repoRoot, absPath);
        if (!rel || rel.startsWith('..'))
            return absPath;
        return rel.replace(/\\/g, '/');
    }
    function appendLogLine(op, absPath) {
        try {
            fs.mkdirSync(setupLogDir, { recursive: true });
            const ts = new Date().toISOString();
            const line = `${ts}\t${op}\t${relOrAbs(absPath)}\r\n`;
            origAppendFileSync(setupLogPath, line, { encoding: 'utf8' });
        }
        catch (e) {
            // 占싸깍옙 占쏙옙占싻곤옙 setup 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占십듸옙占쏙옙 占싼댐옙.
        }
    }
    fs.writeFileSync = function patchedWriteFileSync(file, data, options) {
        const absPath = toAbsFilePath(file);
        const result = origWriteFileSync(file, data, options);
        if (absPath && shouldLog(absPath)) {
            appendLogLine('WRITE', absPath);
        }
        return result;
    };
    if (origCopyFileSync) {
        fs.copyFileSync = function patchedCopyFileSync(src, dest, flags) {
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
(function run() {
    return __awaiter(this, void 0, void 0, function* () {
        if ('sql_to_db_sp' === runType) {
            yield (new sql_to_db_1.default()).Run('sp');
        }
        else if ('sql_to_db_table' === runType) {
            yield (new sql_to_db_1.default()).Run('table');
        }
        else if ('dumpStaticDb' === runType) {
            yield (new static_db_backup_1.default()).Run('STATIC');
        }
        else if ('restoreStaticDb' === runType) {
            yield (new static_db_restore_1.default()).Run('STATIC');
        }
        else if ('dumpUserDb' === runType) {
            yield (new static_db_backup_1.default()).Run('USER');
        }
        else if ('restoreUserDb' === runType) {
            yield (new static_db_restore_1.default()).Run('USER');
        }
        else if ('dumpMainDb' === runType) {
            yield (new static_db_backup_1.default()).Run('MAIN');
        }
        else if ('restoreMainDb' === runType) {
            yield (new static_db_restore_1.default()).Run('MAIN');
        }
        else if ('db_to_code_cpp' === runType) {
            yield (new db_to_code_1.default()).Run('cpp');
        }
        else if ('db_to_code_sql' === runType) {
            yield (new db_to_code_1.default()).Run('sql');
        }
        else if ('db_to_code_txt' === runType) {
            yield (new db_to_code_1.default()).Run('txt');
        }
        else if ('setup_config' === runType) {
            yield (new setup_config_1.default()).Run();
        }
        else if ('setup_enum_generation' === runType) {
            yield (new setup_enum_generation_1.default()).Run();
        }
        else if ('setup_packet_generation' === runType) {
            yield (new setup_packet_generation_1.default()).Run();
        }
        else if ('mapeditor_export_db' === runType) {
            yield (new mapeditor_export_db_1.default()).Run();
        }
        else if ('mapeditor_import_db' === runType) {
            yield (new mapeditor_import_db_1.default()).Run();
        }
        else if ('client_code_auto_generator' === runType) {
            yield (new client_code_auto_generator_event_1.default()).Run();
            yield (new client_code_auto_generator_component_printer_1.default()).Run();
        }
        db_1.default.destroyAll();
    });
})();
//# sourceMappingURL=app.js.map