// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import * as path from 'path';
import * as fs from 'fs';
import * as _ from 'lodash';

// 저작권 헤더 캐시 (한 번만 읽기)
let _copyrightHeaderCache: string | null = null;

/**
 * 저작권 헤더를 읽어옵니다.
 * resource/copyright_header.txt 파일에서 읽어와 CRLF로 정규화합니다.
 * @returns 저작권 헤더 문자열 (파일 없으면 빈 문자열)
 */
export function getCopyrightHeader(): string {
    if (_copyrightHeaderCache !== null) {
        return _copyrightHeaderCache;
    }

    const repoRoot = path.resolve(__dirname, '../..');
    const headerPath = path.join(repoRoot, 'power_shell', 'copyright_header.txt');

    if (fs.existsSync(headerPath)) {
        let content = fs.readFileSync(headerPath, 'utf8');
        // BOM 제거
        if (content.charCodeAt(0) === 0xFEFF) {
            content = content.slice(1);
        }
        // CRLF로 정규화
        content = content.replace(/\r?\n/g, '\r\n');
        // 끝 개행 정리 후 하나 추가
        content = content.replace(/\s+$/, '') + '\r\n';
        _copyrightHeaderCache = content;
        return content;
    }

    _copyrightHeaderCache = '';
    return '';
}

function _mkdirp(dir) {
    var dirname = path.dirname(dir);

    if (!fs.existsSync(dirname)) {
        _mkdirp(dirname);
    }

    if (!fs.existsSync(dir)) {
        fs.mkdirSync(dir);
    }
}

function _rmdirp(folderPath) {
    _mkdirp(folderPath);

    const filenameList = fs.readdirSync(folderPath);
    _.forEach(filenameList, function (filename) {
        if ('.sql' !== path.extname(filename))
            return;
        const filePath = path.join(folderPath, filename);
        fs.unlink(filePath, err => {
            if (err) {
                console.error('Error deleting file:', err);
            }
        });
    }.bind(this));
}

export const mkdirp = function (filepath) {
    var dir = path.parse(filepath).dir;
    _mkdirp(dir);
}


export const rmdirp = function (dir) {
    _rmdirp(dir);
}
