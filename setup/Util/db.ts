// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import { filelog, log } from '../Util/log';
import * as _ from 'lodash';
import * as mysql from 'mysql';

class DbPool {
    public dbConnInfo = { host: '', port: 0, user: '', password: '', database: '' };
    public dbPool: any;
    public constructor(dbConnInfo: {connectionLimit: number, host: string, port: number, user: string, password: string, database: string }) {
        this.dbPool = mysql.createPool(dbConnInfo);
    }
    public async query(context: object, sql: string) {        
        return await new Promise(function (resolve) {
            this.dbPool.query(sql, function (error, rows, cols) {
                if (error) {
                    log.fatal(`query: ${sql}`);
                    throw error;
                }
                const resultObj = { error: error, context: context, r: rows, c: cols };
                resolve(resultObj);
            }.bind(this));
        }.bind(this));
    }
    public async queryWithRollback(context: object, sql: string) {        
        return await new Promise(function (resolve) {
            this.dbPool.getConnection(function (connErr, conn) {
                if (connErr) throw connErr;
                conn.beginTransaction(function () {
                    conn.query(sql, function (error, rows, cols) {
                        if (error) {
                            log.fatal(`queryWithRollback: ${sql}`);
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
    }
    public destroy() {
        this.dbPool.end();
    }
}

export default class Db {
    private static dbPools = {};
    public static connect(dbConnInfo: { connectionLimit: number, host: string, port: number, user: string, password: string, database: string }) {
        let dbPool = Db.dbPools[dbConnInfo.database];
        if (dbPool === undefined) {
            dbPool = Db.dbPools[dbConnInfo.database] = new DbPool(dbConnInfo);
        }
        return dbPool;
    }
    public static get(database: string) {
        return Db.dbPools[database];
    }
    public static destroyAll() {
        _.forEach(Db.dbPools, function (dbPool: DbPool) {
            dbPool.destroy();
        });
        Db.dbPools = {};
    }
}
