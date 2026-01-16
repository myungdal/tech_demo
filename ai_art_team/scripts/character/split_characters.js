/**
 * 12characters.png를 개별 캐릭터 이미지로 분리
 * - 자동 AABB 감지로 캐릭터 영역 추출
 * - 모든 캐릭터 동일 크기, 중앙 정렬
 * - 캐릭터 짤림 방지
 * 
 * 입력: character/12characters.png (255x255)
 * 출력: character/individual/{캐릭터}.png
 * 
 * 사용법: node split_characters.js
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const BASE_DIR = path.join(__dirname, '..', '..', 'character');
const INPUT_PATH = path.join(BASE_DIR, '12characters.png');
const OUTPUT_DIR = path.join(BASE_DIR, 'individual');

// 4열 x 3행 그리드
const COLS = 4;
const ROWS = 3;

// 캐릭터 이름 (좌→우, 상→하 순서)
const CHARACTERS = [
	'warrior', 'archer', 'assassin', 'bat',
	'bear', 'spider', 'zeus', 'hades',
	'cupid', 'golem', 'phoenix', 'dragon'
];

// 배경색 감지 (투명 또는 특정 색상)
function isBackground(r, g, b, a) {
	// 투명
	if (a < 10) return true;
	// 검정색 배경
	if (r < 5 && g < 5 && b < 5) return true;
	// 흰색 배경
	if (r > 250 && g > 250 && b > 250) return true;
	// 마젠타 배경
	if (r > 250 && g < 10 && b > 250) return true;
	return false;
}

// 이미지에서 캐릭터의 AABB (바운딩 박스) 찾기
async function findAABB(buffer, width, height) {
	const { data } = await sharp(buffer)
		.ensureAlpha()
		.raw()
		.toBuffer({ resolveWithObject: true });
	
	let minX = width, minY = height, maxX = 0, maxY = 0;
	let found = false;
	
	for (let y = 0; y < height; y++) {
		for (let x = 0; x < width; x++) {
			const idx = (y * width + x) * 4;
			const r = data[idx];
			const g = data[idx + 1];
			const b = data[idx + 2];
			const a = data[idx + 3];
			
			if (!isBackground(r, g, b, a)) {
				found = true;
				if (x < minX) minX = x;
				if (x > maxX) maxX = x;
				if (y < minY) minY = y;
				if (y > maxY) maxY = y;
			}
		}
	}
	
	if (!found) {
		return null;
	}
	
	return {
		x: minX,
		y: minY,
		width: maxX - minX + 1,
		height: maxY - minY + 1,
		centerX: (minX + maxX) / 2,
		centerY: (minY + maxY) / 2
	};
}

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   12characters.png → 개별 캐릭터 분리 (AABB 자동 감지)');
	console.log('═══════════════════════════════════════════════════════\n');

	if (!fs.existsSync(INPUT_PATH)) {
		console.error(`❌ 입력 파일 없음: ${INPUT_PATH}`);
		process.exit(1);
	}

	// 출력 폴더 생성/초기화
	if (fs.existsSync(OUTPUT_DIR)) {
		fs.rmSync(OUTPUT_DIR, { recursive: true });
	}
	fs.mkdirSync(OUTPUT_DIR, { recursive: true });

	const metadata = await sharp(INPUT_PATH).metadata();
	const { width, height } = metadata;
	console.log(`📄 입력: 12characters.png (${width}x${height})`);
	console.log(`📁 출력: character/individual/\n`);

	const cellWidth = Math.floor(width / COLS);
	const cellHeight = Math.floor(height / ROWS);
	console.log(`📐 그리드 셀: ${cellWidth}x${cellHeight}\n`);

	// 1단계: 각 캐릭터의 AABB 수집
	console.log('🔍 AABB 감지 중...\n');
	const aabbList = [];
	
	for (let row = 0; row < ROWS; row++) {
		for (let col = 0; col < COLS; col++) {
			const idx = row * COLS + col;
			const charName = CHARACTERS[idx];
			
			const left = col * cellWidth;
			const top = row * cellHeight;
			
			// 셀 영역 추출
			const cellBuffer = await sharp(INPUT_PATH)
				.extract({ left, top, width: cellWidth, height: cellHeight })
				.png()
				.toBuffer();
			
			const aabb = await findAABB(cellBuffer, cellWidth, cellHeight);
			
			if (aabb) {
				aabbList.push({
					name: charName,
					cellLeft: left,
					cellTop: top,
					...aabb
				});
				console.log(`  ${charName}: ${aabb.width}x${aabb.height} at (${aabb.x},${aabb.y})`);
			} else {
				console.log(`  ${charName}: ⚠️ 캐릭터 감지 실패`);
				aabbList.push({ name: charName, cellLeft: left, cellTop: top, width: 0, height: 0 });
			}
		}
	}

	// 2단계: 최대 크기 결정 (모든 캐릭터 동일 크기)
	const maxWidth = Math.max(...aabbList.map(a => a.width));
	const maxHeight = Math.max(...aabbList.map(a => a.height));
	const outputSize = Math.max(maxWidth, maxHeight) + 4; // 여백 추가
	
	console.log(`\n📏 최대 캐릭터 크기: ${maxWidth}x${maxHeight}`);
	console.log(`📐 출력 크기: ${outputSize}x${outputSize}\n`);

	// 3단계: 각 캐릭터 추출 및 중앙 정렬
	console.log('✂️ 캐릭터 추출 중...\n');
	let success = 0;
	
	for (const char of aabbList) {
		if (char.width === 0) {
			console.log(`  ❌ ${char.name}: 스킵 (감지 실패)`);
			continue;
		}
		
		const outputPath = path.join(OUTPUT_DIR, `${char.name}.png`);
		
		try {
			// 원본에서 AABB 영역 추출 (전체 이미지 기준 좌표)
			const globalX = char.cellLeft + char.x;
			const globalY = char.cellTop + char.y;
			
			const charBuffer = await sharp(INPUT_PATH)
				.extract({ 
					left: globalX, 
					top: globalY, 
					width: char.width, 
					height: char.height 
				})
				.png()
				.toBuffer();
			
			// 정사각형 캔버스에 중앙 정렬
			const padLeft = Math.floor((outputSize - char.width) / 2);
			const padTop = Math.floor((outputSize - char.height) / 2);
			
			await sharp({
				create: {
					width: outputSize,
					height: outputSize,
					channels: 4,
					background: { r: 0, g: 0, b: 0, alpha: 0 }
				}
			})
			.composite([{
				input: charBuffer,
				left: padLeft,
				top: padTop
			}])
			.png()
			.toFile(outputPath);
			
			console.log(`  ✅ ${char.name}.png (${char.width}x${char.height} → ${outputSize}x${outputSize})`);
			success++;
		} catch (err) {
			console.error(`  ❌ ${char.name}: ${err.message}`);
		}
	}

	console.log('\n═══════════════════════════════════════════════════════');
	console.log(`   완료! ${success}/${CHARACTERS.length} 캐릭터 추출`);
	console.log(`   출력 크기: ${outputSize}x${outputSize} (동일)`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
