const https = require('https');
const fs = require('fs');
const path = require('path');

// config.json에서 API 키 읽기
let API_KEY = '';
try {
    const configContent = fs.readFileSync(path.join(__dirname, '캐릭터', 'config.json'), 'utf8');
    const config = JSON.parse(configContent);
    API_KEY = config.GOOGLE_API_KEY;
} catch (e) {
    console.error('config.json 읽기 실패:', e.message);
    process.exit(1);
}

if (!API_KEY || API_KEY.includes('여기에')) {
    console.error('GOOGLE_API_KEY가 설정되지 않았습니다.');
    process.exit(1);
}

const url = `https://generativelanguage.googleapis.com/v1beta/models?key=${API_KEY}`;

https.get(url, (res) => {
    let data = '';
    res.on('data', (chunk) => data += chunk);
    res.on('end', () => {
        try {
            const json = JSON.parse(data);
            if (json.models) {
                console.log('✅ 사용 가능한 모델 목록:');
                json.models.forEach(model => {
                    if (model.name.includes('gemini') || model.name.includes('imagen')) {
                        console.log(`- ${model.name} (${model.supportedGenerationMethods})`);
                    }
                });
            } else {
                console.log('모델 목록을 가져올 수 없습니다:', json);
            }
        } catch (e) {
            console.error('응답 파싱 실패:', e.message);
        }
    });
}).on('error', (e) => {
    console.error('요청 실패:', e.message);
});
