/**
 * 스프라이트 이미지 보정 스크립트
 * 
 * 기능:
 * 1. 그림자 제거 (하단 보라색/갈색 영역)
 * 2. 핑크 프린지 제거 (마젠타 배경 잔여물)
 * 
 * 사용법:
 *   node fix_sprites.js                    # individual/ 폴더 전체 처리
 *   node fix_sprites.js warrior.png        # 특정 파일만
 */

const sharp = require('sharp');
const fs = require('fs');
const path = require('path');

const INPUT_DIR = path.join(__dirname, 'individual');
const OUTPUT_DIR = path.join(__dirname, 'individual_fixed');

// ═══════════════════════════════════════════════════════════════════════════
// 색상 유틸리티
// ═══════════════════════════════════════════════════════════════════════════

function rgbToHsl(r, g, b) {
	r /= 255; g /= 255; b /= 255;
	const max = Math.max(r, g, b), min = Math.min(r, g, b);
	let h, s, l = (max + min) / 2;

	if (max === min) {
		h = s = 0;
	} else {
		const d = max - min;
		s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
		switch (max) {
			case r: h = ((g - b) / d + (g < b ? 6 : 0)) / 6; break;
			case g: h = ((b - r) / d + 2) / 6; break;
			case b: h = ((r - g) / d + 4) / 6; break;
		}
	}
	return { h: h * 360, s: s * 100, l: l * 100 };
}

// 마젠타/핑크 계열인지 확인 (색상 범위: 280-340도)
function isMagentaPink(r, g, b, a) {
	if (a < 128) return false; // 투명한 픽셀 무시
	const { h, s, l } = rgbToHsl(r, g, b);
	// 마젠타/핑크: 색상 280-360 또는 0-20, 채도 높음
	const isMagentaHue = (h >= 280 && h <= 360) || (h >= 0 && h <= 20);
	return isMagentaHue && s > 30 && l > 20 && l < 90;
}

// 그림자 색상인지 확인 (어두운 보라/갈색)
function isShadowColor(r, g, b, a) {
	if (a < 128) return false;
	const { h, s, l } = rgbToHsl(r, g, b);
	// 어두운 보라/갈색: 명도 낮고, 보라~갈색 범위
	const isShadowHue = (h >= 260 && h <= 340) || (h >= 0 && h <= 40);
	return isShadowHue && l < 35 && l > 5;
}

// 주변 픽셀에서 적절한 대체 색상 찾기
function findReplacementColor(data, width, x, y, radius = 3) {
	const candidates = [];
	
	for (let dy = -radius; dy <= radius; dy++) {
		for (let dx = -radius; dx <= radius; dx++) {
			if (dx === 0 && dy === 0) continue;
			const nx = x + dx;
			const ny = y + dy;
			if (nx < 0 || nx >= width || ny < 0) continue;
			
			const idx = (ny * width + nx) * 4;
			const r = data[idx], g = data[idx + 1], b = data[idx + 2], a = data[idx + 3];
			
			// 투명하지 않고, 핑크/마젠타가 아닌 픽셀
			if (a > 200 && !isMagentaPink(r, g, b, a)) {
				candidates.push({ r, g, b, dist: Math.abs(dx) + Math.abs(dy) });
			}
		}
	}
	
	if (candidates.length === 0) return null;
	
	// 가장 가까운 유효 픽셀 사용
	candidates.sort((a, b) => a.dist - b.dist);
	return candidates[0];
}

// ═══════════════════════════════════════════════════════════════════════════
// 이미지 처리
// ═══════════════════════════════════════════════════════════════════════════

async function processImage(inputPath, outputPath) {
	console.log(`\n처리 중: ${path.basename(inputPath)}`);
	
	// 이미지 로드
	const image = sharp(inputPath);
	const metadata = await image.metadata();
	const { width, height } = metadata;
	
	// raw 픽셀 데이터 추출
	const { data, info } = await image
		.ensureAlpha()
		.raw()
		.toBuffer({ resolveWithObject: true });
	
	const pixels = Buffer.from(data);
	let pinkFixed = 0;
	let shadowRemoved = 0;
	
	// 1차: 그림자 제거 (하단 25% 영역의 어두운 픽셀)
	const shadowThreshold = Math.floor(height * 0.75);
	
	for (let y = 0; y < height; y++) {
		for (let x = 0; x < width; x++) {
			const idx = (y * width + x) * 4;
			const r = pixels[idx], g = pixels[idx + 1], b = pixels[idx + 2], a = pixels[idx + 3];
			
			// 하단 영역의 그림자 색상 제거
			if (y > shadowThreshold && isShadowColor(r, g, b, a)) {
				// 위쪽 픽셀 참조하여 캐릭터 일부인지 확인
				const upperIdx = ((y - 5) * width + x) * 4;
				const upperA = pixels[upperIdx + 3];
				
				// 위에 불투명 픽셀이 없으면 그림자로 판단
				if (upperA < 50) {
					pixels[idx + 3] = 0; // 투명하게
					shadowRemoved++;
				}
			}
		}
	}
	
	// 2차: 핑크 프린지 수정
	for (let y = 0; y < height; y++) {
		for (let x = 0; x < width; x++) {
			const idx = (y * width + x) * 4;
			const r = pixels[idx], g = pixels[idx + 1], b = pixels[idx + 2], a = pixels[idx + 3];
			
			if (isMagentaPink(r, g, b, a)) {
				const replacement = findReplacementColor(pixels, width, x, y);
				
				if (replacement) {
					// 인접 색상으로 교체
					pixels[idx] = replacement.r;
					pixels[idx + 1] = replacement.g;
					pixels[idx + 2] = replacement.b;
					pinkFixed++;
				} else {
					// 대체 색상 없으면 투명화
					pixels[idx + 3] = 0;
					pinkFixed++;
				}
			}
		}
	}
	
	// 결과 저장
	await sharp(pixels, {
		raw: {
			width: info.width,
			height: info.height,
			channels: 4
		}
	})
		.png()
		.toFile(outputPath);
	
	console.log(`   ✅ 완료: 핑크 수정 ${pinkFixed}px, 그림자 제거 ${shadowRemoved}px`);
	return { pinkFixed, shadowRemoved };
}

// ═══════════════════════════════════════════════════════════════════════════
// 메인
// ═══════════════════════════════════════════════════════════════════════════

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   스프라이트 이미지 보정');
	console.log('   - 그림자 제거');
	console.log('   - 핑크 프린지 수정');
	console.log('═══════════════════════════════════════════════════════');
	
	// 출력 디렉토리 생성
	if (!fs.existsSync(OUTPUT_DIR)) {
		fs.mkdirSync(OUTPUT_DIR, { recursive: true });
	}
	
	// 대상 파일 결정
	const targetFile = process.argv[2];
	let files;
	
	if (targetFile) {
		files = [targetFile];
	} else {
		files = fs.readdirSync(INPUT_DIR).filter(f => f.endsWith('.png'));
	}
	
	console.log(`\n📁 입력: ${INPUT_DIR}`);
	console.log(`📁 출력: ${OUTPUT_DIR}`);
	console.log(`📊 대상: ${files.length}개 파일`);
	
	let totalPink = 0, totalShadow = 0;
	
	for (const file of files) {
		const inputPath = path.join(INPUT_DIR, file);
		const outputPath = path.join(OUTPUT_DIR, file);
		
		try {
			const result = await processImage(inputPath, outputPath);
			totalPink += result.pinkFixed;
			totalShadow += result.shadowRemoved;
		} catch (err) {
			console.error(`   ❌ 실패: ${err.message}`);
		}
	}
	
	console.log('\n═══════════════════════════════════════════════════════');
	console.log(`   완료! 총 핑크 수정: ${totalPink}px, 그림자 제거: ${totalShadow}px`);
	console.log(`   결과: ${OUTPUT_DIR}`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
