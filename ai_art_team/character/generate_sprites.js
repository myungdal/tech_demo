/**
 * 캐릭터 스프라이트 시트 자동 생성 스크립트
 * 
 * 사용법:
 *   1. config.json에 OpenAI API 키 설정
 *   2. 실행: node generate_sprites.js [캐릭터명]
 *      - 전체 생성: node generate_sprites.js
 *      - 개별 생성: node generate_sprites.js warrior
 * 
 * 요구사항:
 *   - Node.js 18+
 *   - npm install openai (선택사항)
 */

const fs = require('fs');
const path = require('path');

// ═══════════════════════════════════════════════════════════════════════════
// 설정 로드
// ═══════════════════════════════════════════════════════════════════════════

let OPENAI_API_KEY = process.env.OPENAI_API_KEY;
let MODEL = 'gpt-image-1';
let SIZE = '1024x1024';
let QUALITY = 'medium';

const configPath = path.join(__dirname, 'config.json');
if (fs.existsSync(configPath)) {
	try {
		const config = JSON.parse(fs.readFileSync(configPath, 'utf8'));
		if (config.OPENAI_API_KEY && !config.OPENAI_API_KEY.includes('여기에')) {
			OPENAI_API_KEY = config.OPENAI_API_KEY;
		}
		if (config.MODEL) MODEL = config.MODEL;
		if (config.SIZE) SIZE = config.SIZE;
		if (config.QUALITY) QUALITY = config.QUALITY;
		console.log('✅ config.json 로드 완료');
	} catch (e) {
		console.warn('⚠️  config.json 파싱 오류:', e.message);
	}
}

if (!OPENAI_API_KEY || OPENAI_API_KEY.includes('여기에')) {
	console.error('❌ OpenAI API 키가 설정되지 않았습니다.');
	console.error('   config.json 파일에 API 키를 입력하세요.');
	console.error('   API 키 발급: https://platform.openai.com/api-keys');
	process.exit(1);
}

// ═══════════════════════════════════════════════════════════════════════════
// 작업지시서 파싱
// ═══════════════════════════════════════════════════════════════════════════

const specPath = path.join(__dirname, '애니메이션규격및작업지시서.md');

function parseSpec() {
	if (!fs.existsSync(specPath)) {
		console.error('❌ 작업지시서를 찾을 수 없습니다:', specPath);
		process.exit(1);
	}
	
	const content = fs.readFileSync(specPath, 'utf8');
	
	// 캐릭터 목록 파싱 (섹션 3 테이블)
	const characters = parseCharacterTable(content);
	
	// 프롬프트 템플릿 파싱 (섹션 4 코드 블록)
	const promptTemplate = parsePromptTemplate(content);
	
	// 캐릭터별 추가 지시 파싱
	const additionalInstructions = parseAdditionalInstructions(content);
	
	return { characters, promptTemplate, additionalInstructions };
}

function parseCharacterTable(content) {
	// "## 3. 캐릭터 작업 현황" 섹션에서 테이블 파싱
	const tableMatch = content.match(/## 3\. 캐릭터 작업 현황[\s\S]*?\|[\s\S]*?\|[\s\S]*?\|([\s\S]*?)(?=\n---|\n## )/);
	if (!tableMatch) {
		console.error('❌ 캐릭터 테이블을 찾을 수 없습니다.');
		return [];
	}
	
	const characters = [];
	const lines = tableMatch[0].split('\n');
	
	for (const line of lines) {
		// 테이블 행 파싱: | # | 파일명 | 캐릭터 | 상태 | 특징 |
		const match = line.match(/\|\s*\d+\s*\|\s*(\w+)\.png\s*\|\s*([^|]+)\s*\|\s*([⬜✅])\s*\|\s*([^|]+)\s*\|/);
		if (match) {
			characters.push({
				name: match[1].trim(),
				nameKo: match[2].trim(),
				status: match[3].trim(),
				features: match[4].trim()
			});
		}
	}
	
	return characters;
}

function parsePromptTemplate(content) {
	// "### 기본 프롬프트" 섹션에서 코드 블록 추출
	const promptMatch = content.match(/### 기본 프롬프트[\s\S]*?```([\s\S]*?)```/);
	if (!promptMatch) {
		console.warn('⚠️  프롬프트 템플릿을 찾을 수 없습니다. 기본값 사용.');
		return getDefaultPromptTemplate();
	}
	
	return promptMatch[1].trim();
}

function parseAdditionalInstructions(content) {
	// "### 캐릭터별 추가 지시" 테이블 파싱
	const instructions = {};
	const tableMatch = content.match(/### 캐릭터별 추가 지시[\s\S]*?\|[\s\S]*?\|[\s\S]*?\|([\s\S]*?)(?=\n---|\n## )/);
	
	if (tableMatch) {
		const lines = tableMatch[0].split('\n');
		for (const line of lines) {
			const match = line.match(/\|\s*(\w+)\s*\|\s*([^|]+)\s*\|/);
			if (match && match[1] !== '캐릭터' && match[1] !== ':---') {
				instructions[match[1].trim()] = match[2].trim();
			}
		}
	}
	
	return instructions;
}

function getDefaultPromptTemplate() {
	return `Create a pixel art sprite sheet for THIS EXACT CHARACTER.
IMPORTANT: Match the character design, colors, and style EXACTLY from the reference.

Layout:
- 1024x1024 pixels PNG with transparent background
- 8 rows × 8 columns grid (each cell 128×128 pixels)
- Character facing RIGHT (→) in ALL frames
- Character centered within each 128×128 cell
- Clear outline, high saturation colors
- NO anti-aliasing, hard pixel edges only

Animation Rows:
Row 1: IDLE (6 frames) - subtle breathing, weapon ready
Row 2: RUN (6 frames) - running cycle
Row 3: SUMMON (8 frames) - appearing/spawning effect
Row 4: DASH (5 frames) - quick dodge movement
Row 5: HIT (3 frames) - taking damage reaction
Row 6: DIE (8 frames) - falling down death
Row 7: TOMBSTONE (1 frame) - gravestone or remains
Row 8: ATTACK (7 frames) - attacking motion

Style: 16-bit retro pixel art, matching the reference image exactly
Character only - NO projectiles, NO effects, NO text labels
Empty cells should be completely transparent.`;
}

// ═══════════════════════════════════════════════════════════════════════════
// 프롬프트 생성
// ═══════════════════════════════════════════════════════════════════════════

function createPrompt(character, promptTemplate, additionalInstructions) {
	// 캐릭터 설명 생성
	const charDescription = `${character.nameKo} (${character.name}): ${character.features}`;
	
	// 추가 지시 가져오기
	const extra = additionalInstructions[character.name] || '';
	
	// 프롬프트 조합
	let prompt = promptTemplate;
	
	// {캐릭터} 플레이스홀더 치환
	prompt = prompt.replace(/\{캐릭터\}/g, character.name);
	prompt = prompt.replace(/\{캐릭터명\}/g, character.name);
	
	// 캐릭터 설명 추가
	prompt += `\n\nCharacter: ${charDescription}`;
	
	if (extra) {
		prompt += `\nAdditional: ${extra}`;
	}
	
	return prompt;
}

// ═══════════════════════════════════════════════════════════════════════════
// 썸네일 로드 (Base64)
// ═══════════════════════════════════════════════════════════════════════════

function loadThumbnail(characterName) {
	const thumbnailPath = path.join(__dirname, 'thumbnail', 'individual', `${characterName}.png`);
	
	if (!fs.existsSync(thumbnailPath)) {
		console.warn(`   ⚠️  썸네일 없음: ${thumbnailPath}`);
		return null;
	}
	
	const imageBuffer = fs.readFileSync(thumbnailPath);
	return imageBuffer.toString('base64');
}

// ═══════════════════════════════════════════════════════════════════════════
// GPT-4 Vision으로 썸네일 분석
// ═══════════════════════════════════════════════════════════════════════════

async function analyzeThumbnail(thumbnailBase64, characterName) {
	console.log('   🔍 썸네일 분석 중 (GPT-4 Vision)...');
	
	try {
		const response = await fetch('https://api.openai.com/v1/chat/completions', {
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
				'Authorization': `Bearer ${OPENAI_API_KEY}`
			},
			body: JSON.stringify({
				model: 'gpt-4o',
				messages: [
					{
						role: 'user',
						content: [
							{
								type: 'text',
								text: `Analyze this pixel art character sprite and describe it in detail for recreating in a sprite sheet.

Describe:
1. Overall appearance (body shape, proportions, pose)
2. Colors used (specific colors for each part)
3. Clothing/armor details
4. Weapons or accessories
5. Art style (pixel density, outline style)

Be very specific about colors (e.g., "dark brown #5C4033" not just "brown").
Output should be a single paragraph description suitable for image generation prompt.`
							},
							{
								type: 'image_url',
								image_url: {
									url: `data:image/png;base64,${thumbnailBase64}`
								}
							}
						]
					}
				],
				max_tokens: 500
			})
		});

		if (!response.ok) {
			const error = await response.json();
			throw new Error(error.error?.message || `HTTP ${response.status}`);
		}

		const data = await response.json();
		return data.choices[0].message.content;
		
	} catch (error) {
		console.warn(`   ⚠️  썸네일 분석 실패: ${error.message}`);
		return null;
	}
}

// ═══════════════════════════════════════════════════════════════════════════
// 이미지 생성
// ═══════════════════════════════════════════════════════════════════════════

async function generateImage(character, promptTemplate, additionalInstructions) {
	let prompt = createPrompt(character, promptTemplate, additionalInstructions);
	const thumbnailBase64 = loadThumbnail(character.name);
	
	console.log(`\n🎨 생성 중: ${character.nameKo} (${character.name})...`);
	
	try {
		// 썸네일이 있으면 GPT-4 Vision으로 분석
		if (thumbnailBase64) {
			const analysis = await analyzeThumbnail(thumbnailBase64, character.name);
			if (analysis) {
				prompt += `\n\nREFERENCE CHARACTER DESCRIPTION (match this exactly):\n${analysis}`;
				console.log('   ✅ 썸네일 분석 완료');
			}
		}
		
		let requestBody;
		
		if (MODEL === 'gpt-image-1' || MODEL.startsWith('gpt-image')) {
			requestBody = {
				model: 'gpt-image-1',
				prompt: prompt,
				n: 1,
				size: SIZE,
				quality: QUALITY
			};
		} else {
			// DALL-E 모델
			requestBody = {
				model: MODEL,
				prompt: prompt,
				n: 1,
				size: SIZE,
				quality: MODEL === 'dall-e-3' ? 'hd' : 'standard',
				response_format: 'b64_json'
			};
		}
		
		console.log('   🖼️  이미지 생성 중...');
		
		const response = await fetch('https://api.openai.com/v1/images/generations', {
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
				'Authorization': `Bearer ${OPENAI_API_KEY}`
			},
			body: JSON.stringify(requestBody)
		});

		if (!response.ok) {
			const error = await response.json();
			throw new Error(error.error?.message || `HTTP ${response.status}`);
		}

		const data = await response.json();
		
		// 출력 디렉토리 (animation 폴더)
		const outputDir = path.join(__dirname, 'animation');
		if (!fs.existsSync(outputDir)) {
			fs.mkdirSync(outputDir, { recursive: true });
		}
		
		const outputPath = path.join(outputDir, `${character.name}.png`);
		
		// 응답 형식에 따라 처리
		if (data.data[0].b64_json) {
			fs.writeFileSync(outputPath, Buffer.from(data.data[0].b64_json, 'base64'));
		} else if (data.data[0].url) {
			console.log('   📥 이미지 다운로드 중...');
			const imageResponse = await fetch(data.data[0].url);
			const imageBuffer = Buffer.from(await imageResponse.arrayBuffer());
			fs.writeFileSync(outputPath, imageBuffer);
		}
		
		console.log(`   ✅ 저장됨: animation/${character.name}.png`);
		
		// 작업지시서 상태 업데이트
		updateCharacterStatus(character.name, '✅');
		
		return true;
		
	} catch (error) {
		console.error(`   ❌ 오류: ${error.message}`);
		return false;
	}
}

// ═══════════════════════════════════════════════════════════════════════════
// 작업지시서 상태 업데이트
// ═══════════════════════════════════════════════════════════════════════════

function updateCharacterStatus(characterName, newStatus) {
	try {
		let content = fs.readFileSync(specPath, 'utf8');
		
		// 해당 캐릭터 행 찾아서 상태 업데이트
		const regex = new RegExp(`(\\|\\s*\\d+\\s*\\|\\s*${characterName}\\.png\\s*\\|[^|]+\\|)\\s*⬜\\s*(\\|)`, 'g');
		content = content.replace(regex, `$1 ${newStatus} $2`);
		
		fs.writeFileSync(specPath, content, 'utf8');
	} catch (e) {
		// 상태 업데이트 실패해도 계속 진행
	}
}

// ═══════════════════════════════════════════════════════════════════════════
// 메인
// ═══════════════════════════════════════════════════════════════════════════

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   캐릭터 스프라이트 시트 생성기');
	console.log('═══════════════════════════════════════════════════════');
	
	// 작업지시서 파싱
	console.log('\n📄 작업지시서 로드 중...');
	const { characters, promptTemplate, additionalInstructions } = parseSpec();
	
	console.log(`   캐릭터 수: ${characters.length}개`);
	console.log(`   프롬프트 길이: ${promptTemplate.length}자`);
	console.log(`   추가 지시: ${Object.keys(additionalInstructions).length}개`);
	
	// 대상 캐릭터 결정
	const targetName = process.argv[2];
	let targetCharacters;
	
	if (targetName) {
		// 특정 캐릭터만
		const found = characters.find(c => c.name === targetName);
		if (!found) {
			console.error(`\n❌ 캐릭터를 찾을 수 없습니다: ${targetName}`);
			console.log('   사용 가능한 캐릭터:', characters.map(c => c.name).join(', '));
			process.exit(1);
		}
		targetCharacters = [found];
	} else {
		// 대기 상태인 캐릭터만
		targetCharacters = characters.filter(c => c.status === '⬜');
		if (targetCharacters.length === 0) {
			console.log('\n✅ 모든 캐릭터가 이미 완료되었습니다.');
			return;
		}
	}
	
	console.log(`\n🎯 대상: ${targetCharacters.map(c => c.name).join(', ')}`);
	console.log(`   모델: ${MODEL}, 크기: ${SIZE}, 품질: ${QUALITY}`);
	
	// 비용 경고
	const costPerImage = MODEL.includes('gpt-image') ? 0.02 : 0.12;
	const totalCost = (costPerImage * targetCharacters.length).toFixed(2);
	console.log(`\n⚠️  예상 비용: $${costPerImage} × ${targetCharacters.length} = $${totalCost}`);
	console.log('   5초 후 시작... (Ctrl+C로 취소)\n');
	
	await new Promise(resolve => setTimeout(resolve, 5000));
	
	let successCount = 0;
	let failCount = 0;
	
	for (let i = 0; i < targetCharacters.length; i++) {
		const character = targetCharacters[i];
		const success = await generateImage(character, promptTemplate, additionalInstructions);
		
		if (success) successCount++;
		else failCount++;
		
		// API 속도 제한 방지
		if (i < targetCharacters.length - 1) {
			console.log('   ⏳ 15초 대기...');
			await new Promise(resolve => setTimeout(resolve, 15000));
		}
	}
	
	console.log('\n═══════════════════════════════════════════════════════');
	console.log(`   완료! 성공: ${successCount}, 실패: ${failCount}`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
