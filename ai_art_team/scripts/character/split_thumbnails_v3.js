/**
 * 12characters.png를 개별 캐릭터 썸네일로 분리 (v3 - 정밀 좌표)
 * 
 * 각 캐릭터의 중심 좌표와 바운딩 박스 기반으로 정확히 추출
 * 
 * 입력: thumbnail/12characters.png (1600x1600)
 * 출력: thumbnail/individual/{캐릭터}.png (128x128, 투명 배경)
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const INPUT_PATH = path.join(__dirname, 'thumbnail', '12characters.png');
const OUTPUT_DIR = path.join(__dirname, 'thumbnail', 'individual');
const TARGET_SIZE = 128;

// 각 캐릭터의 정밀 좌표 (1600x1600 기준)
// center_x, center_y: 캐릭터 중심
// bbox_w, bbox_h: 실제 바운딩 박스 크기
// 추출 시 가장 큰 변 + 패딩으로 정사각형 영역 추출
const CHARACTERS = [
	{ name: 'warrior',  cx: 242,  cy: 248,  bw: 241, bh: 276 },
	{ name: 'archer',   cx: 587,  cy: 252,  bw: 238, bh: 269 },
	{ name: 'assassin', cx: 923,  cy: 258,  bw: 182, bh: 254 },
	{ name: 'bat',      cx: 1251, cy: 247,  bw: 213, bh: 168 },
	{ name: 'bear',     cx: 192,  cy: 714,  bw: 241, bh: 255 },
	{ name: 'spider',   cx: 546,  cy: 737,  bw: 238, bh: 213 },
	{ name: 'zeus',     cx: 878,  cy: 716,  bw: 222, bh: 272 },
	{ name: 'hades',    cx: 1219, cy: 712,  bw: 183, bh: 277 },
	{ name: 'cupid',    cx: 316,  cy: 1252, bw: 225, bh: 279 },
	{ name: 'golem',    cx: 592,  cy: 1259, bw: 239, bh: 289 },
	{ name: 'phoenix',  cx: 898,  cy: 1259, bw: 222, bh: 290 },
	{ name: 'dragon',   cx: 1216, cy: 1262, bw: 232, bh: 303 }
];

const IMAGE_SIZE = 1600;
const PADDING = 20; // 여유 패딩

// 마젠타 배경 → 투명 변환 (정확한 #FF00FF + 유사색)
async function removeBackground(buffer) {
	const { data, info } = await sharp(buffer)
		.ensureAlpha()
		.raw()
		.toBuffer({ resolveWithObject: true });
	
	const { width, height } = info;
	const rgbaData = Buffer.alloc(width * height * 4);
	
	// 1단계: 정확한 마젠타 제거
	for (let i = 0; i < width * height; i++) {
		const idx = i * 4;
		const r = data[idx];
		const g = data[idx + 1];
		const b = data[idx + 2];
		const a = data[idx + 3];
		
		// 정확한 마젠타 (#FF00FF) 또는 거의 근접한 색
		const isExactMagenta = (r === 255 && g === 0 && b === 255);
		const isNearMagenta = (r >= 250 && g <= 5 && b >= 250);
		
		if (isExactMagenta || isNearMagenta) {
			rgbaData[idx] = 0;
			rgbaData[idx + 1] = 0;
			rgbaData[idx + 2] = 0;
			rgbaData[idx + 3] = 0;
		} else {
			rgbaData[idx] = r;
			rgbaData[idx + 1] = g;
			rgbaData[idx + 2] = b;
			rgbaData[idx + 3] = a;
		}
	}
	
	// 2단계: 투명 픽셀에 인접한 마젠타 계열 제거 (프린지 제거)
	const tempData = Buffer.from(rgbaData);
	for (let y = 1; y < height - 1; y++) {
		for (let x = 1; x < width - 1; x++) {
			const idx = (y * width + x) * 4;
			const a = tempData[idx + 3];
			
			if (a === 0) continue; // 이미 투명
			
			const r = tempData[idx];
			const g = tempData[idx + 1];
			const b = tempData[idx + 2];
			
			// 마젠타 계열인지 체크 (R과 B가 높고 비슷, G가 낮음)
			const isMagentaish = (r > 150 && b > 150 && g < 100 && Math.abs(r - b) < 50);
			
			if (!isMagentaish) continue;
			
			// 인접 픽셀 중 투명이 있는지 확인
			let hasTransparentNeighbor = false;
			for (let dy = -1; dy <= 1; dy++) {
				for (let dx = -1; dx <= 1; dx++) {
					if (dx === 0 && dy === 0) continue;
					const nIdx = ((y + dy) * width + (x + dx)) * 4;
					if (tempData[nIdx + 3] === 0) {
						hasTransparentNeighbor = true;
						break;
					}
				}
				if (hasTransparentNeighbor) break;
			}
			
			// 투명 픽셀에 인접한 마젠타 계열 → 투명으로
			if (hasTransparentNeighbor) {
				rgbaData[idx] = 0;
				rgbaData[idx + 1] = 0;
				rgbaData[idx + 2] = 0;
				rgbaData[idx + 3] = 0;
			}
		}
	}
	
	return sharp(rgbaData, { raw: { width, height, channels: 4 } })
		.png()
		.toBuffer();
}

async function extractCharacter(char) {
	// 정사각형 크기 계산 (바운딩 박스의 큰 변 + 패딩)
	const size = Math.max(char.bw, char.bh) + PADDING * 2;
	
	// 중심 기준 좌상단 좌표 계산
	let left = Math.round(char.cx - size / 2);
	let top = Math.round(char.cy - size / 2);
	
	// 이미지 경계 클램핑
	if (left < 0) left = 0;
	if (top < 0) top = 0;
	if (left + size > IMAGE_SIZE) left = IMAGE_SIZE - size;
	if (top + size > IMAGE_SIZE) top = IMAGE_SIZE - size;
	
	const outputPath = path.join(OUTPUT_DIR, `${char.name}.png`);
	
	try {
		// 1. 원본에서 배경 먼저 제거 (리사이즈 전에!)
		const fullImage = await sharp(INPUT_PATH)
			.ensureAlpha()
			.raw()
			.toBuffer({ resolveWithObject: true });
		
		const { data, info } = fullImage;
		const { width, height, channels } = info;
		
		// 전체 이미지에서 마젠타 및 마젠타 계열 제거
		const cleanData = Buffer.alloc(width * height * 4);
		for (let i = 0; i < width * height; i++) {
			const idx = i * channels;
			const outIdx = i * 4;
			const r = data[idx];
			const g = data[idx + 1];
			const b = data[idx + 2];
			const a = channels === 4 ? data[idx + 3] : 255;
			
			// 마젠타 계열 감지:
			// 1. 정확한 마젠타 (#FF00FF)
			// 2. 안티앨리어싱된 마젠타/보라 (R높음, G낮음, B중간)
			const isExactMagenta = (r === 255 && g === 0 && b === 255);
			const isAntiAliasedMagenta = (
				// 보라/마젠타 프린지: R~150-170, G~20-70, B~100-125
				(r >= 150 && r <= 170 && g <= 70 && b >= 100 && b <= 130) ||
				// 기존 조건도 유지
				(r > 150 && b > 120 && g < 80 && Math.abs(r - b) < 80)
			);
			
			if (isExactMagenta || isAntiAliasedMagenta) {
				cleanData[outIdx] = 0;
				cleanData[outIdx + 1] = 0;
				cleanData[outIdx + 2] = 0;
				cleanData[outIdx + 3] = 0;
			} else {
				cleanData[outIdx] = r;
				cleanData[outIdx + 1] = g;
				cleanData[outIdx + 2] = b;
				cleanData[outIdx + 3] = a;
			}
		}
		
		// 2. 클린 이미지에서 영역 추출
		const cleanImage = sharp(cleanData, { raw: { width, height, channels: 4 } });
		const extracted = await cleanImage
			.extract({ left, top, width: size, height: size })
			.png()
			.toBuffer();
		
		// 3. 128x128로 리사이즈 (nearest neighbor 필수!)
		await sharp(extracted)
			.resize(TARGET_SIZE, TARGET_SIZE, {
				kernel: sharp.kernel.nearest,
				fit: 'contain',
				background: { r: 0, g: 0, b: 0, alpha: 0 }
			})
			.png()
			.toFile(outputPath);
		
		console.log(`✅ ${char.name}.png (${size}px → ${TARGET_SIZE}px, center: ${char.cx},${char.cy})`);
		return true;
		
	} catch (err) {
		console.error(`❌ ${char.name}: ${err.message}`);
		return false;
	}
}

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   캐릭터 썸네일 분리 (v3 - 정밀 좌표)');
	console.log('═══════════════════════════════════════════════════════\n');

	if (!fs.existsSync(INPUT_PATH)) {
		console.error(`❌ 입력 파일 없음: ${INPUT_PATH}`);
		process.exit(1);
	}

	// 기존 출력 폴더 삭제 후 재생성
	if (fs.existsSync(OUTPUT_DIR)) {
		fs.rmSync(OUTPUT_DIR, { recursive: true });
	}
	fs.mkdirSync(OUTPUT_DIR, { recursive: true });

	const metadata = await sharp(INPUT_PATH).metadata();
	console.log(`📄 입력: 12characters.png (${metadata.width}x${metadata.height})`);
	console.log(`📁 출력: thumbnail/individual/\n`);

	let success = 0;
	for (const char of CHARACTERS) {
		if (await extractCharacter(char)) success++;
	}

	console.log('\n═══════════════════════════════════════════════════════');
	console.log(`   완료! ${success}/${CHARACTERS.length} 캐릭터 추출`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
