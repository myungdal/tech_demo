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
const log_1 = require("../Util/log");
const _ = require("lodash");
const mysql = require("mysql");
class DbPool {
    constructor(dbConnInfo) {
        this.dbConnInfo = { host: '', port: 0, user: '', password: '', database: '' };
        this.dbPool = mysql.createPool(dbConnInfo);
    }
    query(context, sql) {
        return __awaiter(this, void 0, void 0, function* () {
            return yield new Promise(function (resolve) {
                this.dbPool.query(sql, function (error, rows, cols) {
                    if (error) {
                        log_1.log.fatal(`query: ${sql}`);
                        throw error;
                    }
                    const resultObj = { error: error, context: context, r: rows, c: cols };
                    resolve(resultObj);
                }.bind(this));
            }.bind(this));
        });
    }
    queryWithRollback(context, sql) {
        return __awaiter(this, void 0, void 0, function* () {
            return yield new Promise(function (resolve) {
                this.dbPool.getConnection(function (connErr, conn) {
                    if (connErr)
                        throw connErr;
                    conn.beginTransaction(function () {
                        conn.query(sql, function (error, rows, cols) {
                            if (error) {
                                log_1.log.fatal(`queryWithRollback: ${sql}`);
                                throw error;
                            }
                            const resultObj = { error: error, context: context, r: rows, c: cols };
                            resolve(resultObj);
                            conn.rollback();
                            conn.release();
                        }.bind(this));
                    }.bind(this));
                }.bind(this));
            }.bind(this));
        });
    }
    destroy() {
        this.dbPool.end();
    }
}
class Db {
    static connect(dbConnInfo) {
        let dbPool = Db.dbPools[dbConnInfo.database];
        if (dbPool === undefined) {
            dbPool = Db.dbPools[dbConnInfo.database] = new DbPool(dbConnInfo);
        }
        return dbPool;
    }
    static get(database) {
        return Db.dbPools[database];
    }
    static destroyAll() {
        _.forEach(Db.dbPools, function (dbPool) {
            dbPool.destroy();
        });
        Db.dbPools = {};
    }
}
Db.dbPools = {};
exports.default = Db;
//# sourceMappingURL=db.js.map