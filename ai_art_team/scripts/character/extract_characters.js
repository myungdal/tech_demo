/**
 * 12characters_clean.png에서 캐릭터 개별 추출
 * - Flood fill로 덩어리 감지
 * - 동일 크기 (2의 자승), 중앙 정렬
 * 
 * 입력: character/12characters_clean.png
 * 출력: character/individual/{번호}.png
 * 
 * 사용법: node extract_characters.js
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const BASE_DIR = path.join(__dirname, '..', '..', 'character');
const INPUT_PATH = path.join(BASE_DIR, '12characters_clean.png');
const OUTPUT_DIR = path.join(BASE_DIR, 'individual');

// 2의 자승으로 올림
function nextPowerOf2(n) {
	let p = 1;
	while (p < n) p *= 2;
	return p;
}

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   캐릭터 개별 추출');
	console.log('   - 자동 AABB 감지');
	console.log('   - 동일 크기 (2의 자승), 중앙 정렬');
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

	const { data, info } = await sharp(INPUT_PATH)
		.ensureAlpha()
		.raw()
		.toBuffer({ resolveWithObject: true });
	
	const { width, height } = info;
	console.log(`📄 입력: ${width}x${height}\n`);

	// visited 배열
	const visited = new Int32Array(width * height);
	
	// 투명 픽셀 마킹
	for (let i = 0; i < width * height; i++) {
		const a = data[i * 4 + 3];
		if (a < 128) {
			visited[i] = -1;
		}
	}

	// Flood fill로 덩어리 찾기
	const blobs = [];
	let blobId = 0;

	function floodFill(startX, startY) {
		const stack = [[startX, startY]];
		let minX = startX, maxX = startX;
		let minY = startY, maxY = startY;
		let pixelCount = 0;
		
		blobId++;
		
		while (stack.length > 0) {
			const [x, y] = stack.pop();
			const idx = y * width + x;
			
			if (x < 0 || x >= width || y < 0 || y >= height) continue;
			if (visited[idx] !== 0) continue;
			
			visited[idx] = blobId;
			pixelCount++;
			
			if (x < minX) minX = x;
			if (x > maxX) maxX = x;
			if (y < minY) minY = y;
			if (y > maxY) maxY = y;
			
			// 8방향 탐색
			stack.push([x - 1, y], [x + 1, y], [x, y - 1], [x, y + 1]);
			stack.push([x - 1, y - 1], [x + 1, y - 1], [x - 1, y + 1], [x + 1, y + 1]);
		}
		
		return { minX, minY, maxX, maxY, pixelCount, id: blobId };
	}

	// 덩어리 탐색
	console.log('🔍 캐릭터 탐색 중...\n');
	
	for (let y = 0; y < height; y++) {
		for (let x = 0; x < width; x++) {
			const idx = y * width + x;
			if (visited[idx] === 0) {
				const blob = floodFill(x, y);
				if (blob.pixelCount > 50) {
					blobs.push(blob);
				}
			}
		}
	}

	// 좌상단→우하단 정렬 (행 우선)
	blobs.sort((a, b) => {
		const rowA = Math.floor((a.minY + a.maxY) / 2 / (height / 3));
		const rowB = Math.floor((b.minY + b.maxY) / 2 / (height / 3));
		if (rowA !== rowB) return rowA - rowB;
		return a.minX - b.minX;
	});

	console.log(`📊 감지된 캐릭터: ${blobs.length}개\n`);

	// 최대 크기 계산
	let maxW = 0, maxH = 0;
	for (const b of blobs) {
		const w = b.maxX - b.minX + 1;
		const h = b.maxY - b.minY + 1;
		if (w > maxW) maxW = w;
		if (h > maxH) maxH = h;
	}

	// 2의 자승으로 출력 크기 결정
	const outputSize = nextPowerOf2(Math.max(maxW, maxH));
	console.log(`📏 최대 캐릭터: ${maxW}x${maxH}`);
	console.log(`📐 출력 크기: ${outputSize}x${outputSize} (2^${Math.log2(outputSize)})\n`);

	// 각 캐릭터 추출 및 저장
	console.log('✂️ 추출 중...\n');
	
	for (let i = 0; i < blobs.length; i++) {
		const b = blobs[i];
		const w = b.maxX - b.minX + 1;
		const h = b.maxY - b.minY + 1;
		const num = String(i + 1).padStart(2, '0');
		const outputPath = path.join(OUTPUT_DIR, `${num}.png`);
		
		try {
			// AABB 영역 추출
			const charBuffer = await sharp(INPUT_PATH)
				.extract({ left: b.minX, top: b.minY, width: w, height: h })
				.png()
				.toBuffer();
			
			// 중앙 정렬
			const padLeft = Math.floor((outputSize - w) / 2);
			const padTop = Math.floor((outputSize - h) / 2);
			
			await sharp({
				create: {
					width: outputSize,
					height: outputSize,
					channels: 4,
					background: { r: 0, g: 0, b: 0, alpha: 0 }
				}
			})
			.composite([{ input: charBuffer, left: padLeft, top: padTop }])
			.png()
			.toFile(outputPath);
			
			console.log(`  ✅ ${num}.png (${w}x${h} → ${outputSize}x${outputSize})`);
		} catch (err) {
			console.error(`  ❌ ${num}: ${err.message}`);
		}
	}

	console.log('\n═══════════════════════════════════════════════════════');
	console.log(`   완료! ${blobs.length}개 캐릭터 추출`);
	console.log(`   출력: character/individual/`);
	console.log(`   크기: ${outputSize}x${outputSize} (동일)`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
