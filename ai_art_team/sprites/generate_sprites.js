/**
 * 캐릭터 스프라이트 시트 자동 생성 스크립트
 * OpenAI (DALL-E), Google (Gemini/Imagen), xAI (Grok) 지원
 */

const fs = require('fs');
const path = require('path');

// 설정값 기본값
let PROVIDER = 'google';
let OPENAI_API_KEY = '';
let GOOGLE_API_KEY = '';
let GROK_API_KEY = '';
let OPENAI_MODEL = 'dall-e-3';
let GOOGLE_MODEL = 'gemini-2.5-flash-image';
let GROK_MODEL = 'grok-2-vision';
let SIZE = '1024x1024';

// config.json 로드
const configPath = path.join(__dirname, '..', '캐릭터', 'config.json');
try {
	const config = JSON.parse(fs.readFileSync(configPath, 'utf8'));
	if (config.PROVIDER) PROVIDER = config.PROVIDER;
	if (config.OPENAI_API_KEY) OPENAI_API_KEY = config.OPENAI_API_KEY;
	if (config.GOOGLE_API_KEY) GOOGLE_API_KEY = config.GOOGLE_API_KEY;
	if (config.GROK_API_KEY) GROK_API_KEY = config.GROK_API_KEY;
	if (config.OPENAI_MODEL) OPENAI_MODEL = config.OPENAI_MODEL;
	if (config.GOOGLE_MODEL) GOOGLE_MODEL = config.GOOGLE_MODEL;
	if (config.GROK_MODEL) GROK_MODEL = config.GROK_MODEL;
	if (config.SIZE) SIZE = config.SIZE;
	console.log('✅ 설정 로드 완료');
} catch (e) {
	console.error('⚠️ config.json 로드 실패:', e.message);
}

// 현재 모델 설정
let CURRENT_MODEL = '';
let CURRENT_API_KEY = '';
if (PROVIDER === 'google') { CURRENT_MODEL = GOOGLE_MODEL; CURRENT_API_KEY = GOOGLE_API_KEY; }
else if (PROVIDER === 'openai') { CURRENT_MODEL = OPENAI_MODEL; CURRENT_API_KEY = OPENAI_API_KEY; }
else if (PROVIDER === 'grok') { CURRENT_MODEL = GROK_MODEL; CURRENT_API_KEY = GROK_API_KEY; }

console.log(`   Provider: ${PROVIDER.toUpperCase()}`);
console.log(`   모델: ${CURRENT_MODEL}\n`);

// 참조 이미지 로드
function loadReferenceImage() {
	const refPath = path.join(__dirname, '..', '캐릭터', '썸네일', '12characters.png');
	if (fs.existsSync(refPath)) {
		return fs.readFileSync(refPath).toString('base64');
	}
	return null;
}
const REFERENCE_IMAGE_BASE64 = loadReferenceImage();

// 프롬프트 생성
function createPrompt(character, style) {
	return `TECHNICAL SPECIFICATION: 
Create a strict 8x8 Grid Sprite Sheet.

SUBJECT: ${character.nameKo} (${character.description})

VISUAL STYLE (STRICT):
- ${style || "Flat 16-bit pixel art"}
- UNIFORM style for all characters.
- NO artistic variations.
- Solid Black Outline (1px).
- Flat colors, minimal shading.

LAYOUT (MANDATORY):
- Canvas: 1024x1024 px.
- Background: SOLID MAGENTA (#FF00FF).
- Grid: 8 evenly spaced rows.
- Orientation: All frames face RIGHT.

ANIMATION ROWS:
1. Idle
2. Run
3. Attack
4. Hit
5. Die
6. Skill
7. Dash
8. Win

CONSTRAINT: Keep characters within 64x64px bounds inside the 128px row height.
OUTPUT: A technical game asset, NOT an illustration.`;
}

// Google 생성 함수
async function generateWithGoogle(character) {
	// 1. [최초 1회] 참조 이미지 스타일 분석 (Gemini Vision)
	if (!global.STYLE_PROMPT && REFERENCE_IMAGE_BASE64) {
		console.log('   🔍 참조 이미지 스타일 정밀 분석 중 (Gemini Vision)...');
		try {
			const analyzeUrl = `https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=${CURRENT_API_KEY}`;
			const analyzeBody = {
				contents: [{
					role: 'user',
					parts: [
						{ inline_data: { mime_type: 'image/png', data: REFERENCE_IMAGE_BASE64 } },
						{ text: `Analyze this pixel art sprite sheet in extreme detail. 
Describe the visual style so an AI artist can replicate it exactly.
Focus on:
1. Pixel scale and outlining technique (e.g. black outline?).
2. Color palette (saturation, specific tones?).
3. Body proportions (head to body ratio).
4. Shading technique.
5. Background color.
Output a concise paragraph describing the STYLE only.` }
					]
				}]
			};
			const res = await fetch(analyzeUrl, { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(analyzeBody) });
			const data = await res.json();
			if (data.candidates && data.candidates[0]?.content?.parts[0]?.text) {
				global.STYLE_PROMPT = data.candidates[0].content.parts[0].text;
				console.log('\n🔍 [Gemini Vision 분석 결과 (STYLE_PROMPT)]');
				console.log('---------------------------------------------------');
				console.log(global.STYLE_PROMPT);
				console.log('---------------------------------------------------\n');
			}
		} catch (e) {
			console.error('   ⚠️ 스타일 분석 실패 (기본값 사용):', e.message);
		}
	}

	// 2. 프롬프트 조합
	const baseStyle = global.STYLE_PROMPT || "Retro 16-bit JRPG pixel art, flat colors, black outline.";
	const finalPrompt = createPrompt(character, baseStyle);

	console.log(`\n🎨 생성 중: ${character.nameKo}...`);
	console.log('🔍 [최종 프롬프트 (Imagen에 전송됨)]');
	console.log('---------------------------------------------------');
	console.log(finalPrompt);
	console.log('---------------------------------------------------\n');

	// 3. Imagen 4.0 호출 (predict)
	if (CURRENT_MODEL.includes('imagen')) {
		console.log('   📎 Google Imagen 4.0 API 호출');
		const url = `https://generativelanguage.googleapis.com/v1beta/models/${CURRENT_MODEL}:predict?key=${CURRENT_API_KEY}`;
		const body = {
			instances: [{ prompt: finalPrompt }],
			parameters: { 
				sampleCount: 1, 
				aspectRatio: "1:1"
			}
		};

		try {
			const res = await fetch(url, { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(body) });
			const data = await res.json();
			
			if (data.predictions && data.predictions[0]?.bytesBase64Encoded) {
				saveImage(character.name, Buffer.from(data.predictions[0].bytesBase64Encoded, 'base64'));
				return true;
			}
			throw new Error('Imagen 이미지 데이터 없음: ' + JSON.stringify(data).substring(0, 200));
		} catch (e) {
			console.error('   ❌ Imagen 오류:', e.message);
			return false;
		}
	}
    
    // ... (Gemini fallback 생략)
}

// OpenAI 생성 함수
async function generateWithOpenAI(character) {
	// ... (이전과 동일, 생략하지 않고 포함)
	const prompt = createPrompt(character);
	console.log(`\n🎨 생성 중: ${character.nameKo}...`);
	
	try {
		const body = {
			model: CURRENT_MODEL,
			prompt: prompt,
			n: 1,
			size: SIZE
		};
		
		const res = await fetch('https://api.openai.com/v1/images/generations', {
			method: 'POST',
			headers: { 'Content-Type': 'application/json', 'Authorization': `Bearer ${CURRENT_API_KEY}` },
			body: JSON.stringify(body)
		});
		
		const data = await res.json();
		if (data.data && data.data[0]?.url) {
			console.log('   📥 다운로드 중...');
			const imgRes = await fetch(data.data[0].url);
			saveImage(character.name, Buffer.from(await imgRes.arrayBuffer()));
			return true;
		}
		throw new Error(data.error?.message || 'OpenAI 응답 오류');
	} catch (e) {
		console.error('   ❌ OpenAI 오류:', e.message);
		return false;
	}
}

// Grok 생성 함수 (Placeholder)
async function generateWithGrok(character) {
	console.log(`\n🎨 생성 중: ${character.nameKo}...`);
	console.log('   ⚠️ Grok 이미지 생성은 아직 지원되지 않습니다.');
	return false;
}

function saveImage(name, buffer) {
	const dir = path.join(__dirname, '..', '캐릭터', '애니메이션');
	if (!fs.existsSync(dir)) fs.mkdirSync(dir, { recursive: true });
	fs.writeFileSync(path.join(dir, `${name}.png`), buffer);
	console.log(`   ✅ 저장됨: ${name}.png`);
}

// 메인
const CHARACTERS = [
	{ name: 'archer', nameKo: '궁수', description: 'Archer with bow' },
	{ name: 'bat', nameKo: '박쥐', description: 'Flying bat' },
	{ name: 'bear', nameKo: '곰', description: 'Brown bear' },
    // ... (나머지 캐릭터 생략 가능, 테스트용으로 앞부분만)
];

async function main() {
	console.log('🚀 스프라이트 생성 시작');
	for (const char of CHARACTERS) {
		if (PROVIDER === 'google') await generateWithGoogle(char);
		else if (PROVIDER === 'openai') await generateWithOpenAI(char);
		else if (PROVIDER === 'grok') await generateWithGrok(char);
		
		await new Promise(r => setTimeout(r, 2000));
	}
}

main();
