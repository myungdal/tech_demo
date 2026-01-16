/**
 * 마젠타 색상 제거 (모든 마젠타를 주변색/갈색으로 대체)
 * 
 * 입력: character/12characters_clean.png
 * 출력: character/12characters_clean.png (덮어쓰기)
 * 
 * 사용법: node fix_magenta.js
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const BASE_DIR = path.join(__dirname, '..', '..', 'character');
const INPUT_PATH = path.join(BASE_DIR, '12characters_clean.png');
const OUTPUT_PATH = path.join(BASE_DIR, '12characters_clean.png');

// 마젠타/보라/핑크 계열 판별 (더 넓은 범위)
function isMagentaish(r, g, b) {
	// 순수 마젠타
	if (r > 200 && g < 50 && b > 200) return true;
	
	// 보라/마젠타 계열: R과 B가 높고 비슷하며, G가 상대적으로 낮음
	// R > G, B > G, R과 B 차이가 크지 않음
	if (r > 100 && b > 100 && g < Math.min(r, b) * 0.7) {
		// R과 B가 비슷한 범위
		if (Math.abs(r - b) < 80) return true;
	}
	
	// 핑크 계열: R이 높고 B가 중간, G가 낮음
	if (r > 150 && b > 80 && g < 100 && r > b) return true;
	
	// 연한 보라/핑크
	if (r > 120 && b > 120 && g < 80) return true;
	
	return false;
}

// 유효한 색상인지 (마젠타 아닌 색)
function isValidColor(r, g, b, a) {
	if (a < 128) return false; // 반투명 이하 제외
	if (isMagentaish(r, g, b)) return false;
	return true;
}

// 주변에서 유효한 색상 찾기 (더 넓은 범위 탐색)
function findReplacementColor(data, width, height, x, y) {
	const validColors = [];
	
	// 5x5 범위에서 유효한 색상 수집
	for (let dy = -2; dy <= 2; dy++) {
		for (let dx = -2; dx <= 2; dx++) {
			if (dx === 0 && dy === 0) continue;
			
			const nx = x + dx;
			const ny = y + dy;
			
			if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
			
			const idx = (ny * width + nx) * 4;
			const r = data[idx];
			const g = data[idx + 1];
			const b = data[idx + 2];
			const a = data[idx + 3];
			
			if (isValidColor(r, g, b, a)) {
				// 거리 가중치 (가까울수록 높은 가중치)
				const dist = Math.sqrt(dx * dx + dy * dy);
				validColors.push({ r, g, b, weight: 1 / dist });
			}
		}
	}
	
	if (validColors.length === 0) {
		// 주변에 유효한 색상 없으면 기본 갈색 반환
		return { r: 139, g: 90, b: 43 }; // 갈색
	}
	
	// 가중 평균 계산
	let totalWeight = 0;
	let sumR = 0, sumG = 0, sumB = 0;
	
	for (const c of validColors) {
		sumR += c.r * c.weight;
		sumG += c.g * c.weight;
		sumB += c.b * c.weight;
		totalWeight += c.weight;
	}
	
	return {
		r: Math.round(sumR / totalWeight),
		g: Math.round(sumG / totalWeight),
		b: Math.round(sumB / totalWeight)
	};
}

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   마젠타 색상 제거');
	console.log('   - 모든 마젠타/보라/핑크 → 주변색 또는 갈색');
	console.log('═══════════════════════════════════════════════════════\n');

	if (!fs.existsSync(INPUT_PATH)) {
		console.error(`❌ 입력 파일 없음: ${INPUT_PATH}`);
		process.exit(1);
	}

	const { data, info } = await sharp(INPUT_PATH)
		.ensureAlpha()
		.raw()
		.toBuffer({ resolveWithObject: true });
	
	const { width, height } = info;
	console.log(`📄 입력: ${width}x${height}`);

	const outputData = Buffer.from(data);
	
	// 여러 패스로 마젠타 제거 (한 번에 안 될 수 있음)
	let totalFixed = 0;
	
	for (let pass = 1; pass <= 5; pass++) {
		console.log(`\n🔄 패스 ${pass}: 마젠타 탐색 중...`);
		
		const tempData = Buffer.from(outputData);
		let fixedThisPass = 0;
		
		for (let y = 0; y < height; y++) {
			for (let x = 0; x < width; x++) {
				const idx = (y * width + x) * 4;
				const a = tempData[idx + 3];
				
				if (a < 128) continue; // 투명/반투명 스킵
				
				const r = tempData[idx];
				const g = tempData[idx + 1];
				const b = tempData[idx + 2];
				
				if (isMagentaish(r, g, b)) {
					const replacement = findReplacementColor(tempData, width, height, x, y);
					outputData[idx] = replacement.r;
					outputData[idx + 1] = replacement.g;
					outputData[idx + 2] = replacement.b;
					fixedThisPass++;
				}
			}
		}
		
		console.log(`   ✅ ${fixedThisPass} 픽셀 수정`);
		totalFixed += fixedThisPass;
		
		if (fixedThisPass === 0) {
			console.log('   → 더 이상 마젠타 없음, 종료');
			break;
		}
	}

	// 저장
	console.log('\n💾 저장 중...');
	await sharp(outputData, { raw: { width, height, channels: 4 } })
		.png()
		.toFile(OUTPUT_PATH);
	
	console.log(`   ✅ 저장 완료: ${OUTPUT_PATH}`);
	
	console.log('\n═══════════════════════════════════════════════════════');
	console.log(`   완료! 총 ${totalFixed} 픽셀 수정`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
