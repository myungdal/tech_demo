/**
 * 12characters.png 배경 처리
 * 1. 검정색 배경 → 투명
 * 2. 마젠타 프린지(테두리) → 인접 픽셀 색상으로 자연스럽게 보정
 * 
 * 입력: character/12characters.png
 * 출력: character/12characters.png (덮어쓰기)
 * 
 * 사용법: node clean_background.js
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const BASE_DIR = path.join(__dirname, '..', '..', 'character');
const INPUT_PATH = path.join(BASE_DIR, '12characters.png');
const OUTPUT_PATH = path.join(BASE_DIR, '12characters_clean.png');

// 검정색 배경 판별
function isBlackBackground(r, g, b) {
	return r < 10 && g < 10 && b < 10;
}

// 마젠타/보라 프린지 판별
function isMagentaFringe(r, g, b) {
	// 마젠타: R높음, G낮음, B높음
	// 보라 프린지: R중간, G낮음, B중간~높음
	const isMagenta = (r > 150 && g < 80 && b > 150);
	const isPurpleFringe = (r > 100 && r < 200 && g < 60 && b > 80 && b < 180);
	const isPinkFringe = (r > 150 && g < 100 && b > 100 && Math.abs(r - b) < 80);
	return isMagenta || isPurpleFringe || isPinkFringe;
}

// 인접 픽셀에서 유효한 색상 찾기 (배경/프린지 아닌 색)
function findNearestValidColor(data, width, height, x, y) {
	const directions = [
		[-1, 0], [1, 0], [0, -1], [0, 1],  // 4방향
		[-1, -1], [1, -1], [-1, 1], [1, 1]  // 대각선
	];
	
	// 가까운 순으로 탐색 (최대 3픽셀 거리)
	for (let dist = 1; dist <= 3; dist++) {
		for (const [dx, dy] of directions) {
			const nx = x + dx * dist;
			const ny = y + dy * dist;
			
			if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
			
			const idx = (ny * width + nx) * 4;
			const r = data[idx];
			const g = data[idx + 1];
			const b = data[idx + 2];
			const a = data[idx + 3];
			
			// 투명, 배경, 프린지가 아닌 유효한 색상
			if (a > 200 && !isBlackBackground(r, g, b) && !isMagentaFringe(r, g, b)) {
				return { r, g, b };
			}
		}
	}
	
	return null; // 유효한 색상 없음
}

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   12characters.png 배경 정리');
	console.log('   - 검정 배경 → 투명');
	console.log('   - 마젠타 프린지 → 자연스러운 색상');
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
	
	let bgRemoved = 0;
	let fringeFixed = 0;

	// 1단계: 검정 배경 → 투명
	console.log('\n🔄 1단계: 검정 배경 제거...');
	for (let i = 0; i < width * height; i++) {
		const idx = i * 4;
		const r = data[idx];
		const g = data[idx + 1];
		const b = data[idx + 2];
		
		if (isBlackBackground(r, g, b)) {
			outputData[idx] = 0;
			outputData[idx + 1] = 0;
			outputData[idx + 2] = 0;
			outputData[idx + 3] = 0;
			bgRemoved++;
		}
	}
	console.log(`   ✅ ${bgRemoved} 픽셀 투명화`);

	// 2단계: 마젠타 프린지 보정
	console.log('\n🔄 2단계: 마젠타 프린지 보정...');
	const tempData = Buffer.from(outputData);
	
	for (let y = 0; y < height; y++) {
		for (let x = 0; x < width; x++) {
			const idx = (y * width + x) * 4;
			const a = tempData[idx + 3];
			
			if (a === 0) continue; // 이미 투명
			
			const r = tempData[idx];
			const g = tempData[idx + 1];
			const b = tempData[idx + 2];
			
			if (isMagentaFringe(r, g, b)) {
				// 인접한 투명 픽셀이 있는지 확인 (테두리인지)
				let hasTransparentNeighbor = false;
				for (let dy = -1; dy <= 1; dy++) {
					for (let dx = -1; dx <= 1; dx++) {
						if (dx === 0 && dy === 0) continue;
						const nx = x + dx;
						const ny = y + dy;
						if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
							hasTransparentNeighbor = true;
							continue;
						}
						const nIdx = (ny * width + nx) * 4;
						if (tempData[nIdx + 3] === 0) {
							hasTransparentNeighbor = true;
							break;
						}
					}
					if (hasTransparentNeighbor) break;
				}
				
				if (hasTransparentNeighbor) {
					// 테두리의 프린지 → 인접 유효 색상으로 대체
					const validColor = findNearestValidColor(tempData, width, height, x, y);
					if (validColor) {
						outputData[idx] = validColor.r;
						outputData[idx + 1] = validColor.g;
						outputData[idx + 2] = validColor.b;
						fringeFixed++;
					} else {
						// 유효한 색상 없으면 투명화
						outputData[idx + 3] = 0;
						fringeFixed++;
					}
				}
			}
		}
	}
	console.log(`   ✅ ${fringeFixed} 픽셀 보정`);

	// 저장
	console.log('\n💾 저장 중...');
	await sharp(outputData, { raw: { width, height, channels: 4 } })
		.png()
		.toFile(OUTPUT_PATH);
	
	console.log(`   ✅ 저장 완료: ${OUTPUT_PATH}`);
	
	console.log('\n═══════════════════════════════════════════════════════');
	console.log('   완료!');
	console.log(`   - 배경 제거: ${bgRemoved} 픽셀`);
	console.log(`   - 프린지 보정: ${fringeFixed} 픽셀`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
