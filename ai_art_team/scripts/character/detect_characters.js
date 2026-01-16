/**
 * 12characters_clean.png에서 캐릭터 AABB 자동 감지
 * - 투명하지 않은 연결된 픽셀 덩어리를 각각 하나의 캐릭터로 인식
 * - Flood fill 알고리즘으로 덩어리 분리
 * 
 * 입력: character/12characters_clean.png
 * 출력: 콘솔에 AABB 좌표 출력
 * 
 * 사용법: node detect_characters.js
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const BASE_DIR = path.join(__dirname, '..', '..', 'character');
const INPUT_PATH = path.join(BASE_DIR, '12characters_clean.png');

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   캐릭터 AABB 자동 감지 (Flood Fill)');
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
	console.log(`📄 입력: ${width}x${height}\n`);

	// visited 배열 (어느 덩어리에 속하는지 기록, 0=미방문, -1=투명, 1~N=덩어리번호)
	const visited = new Int32Array(width * height);
	
	// 투명 픽셀 먼저 마킹
	for (let i = 0; i < width * height; i++) {
		const a = data[i * 4 + 3];
		if (a < 128) {
			visited[i] = -1; // 투명
		}
	}

	// Flood fill로 덩어리 찾기
	const blobs = []; // { minX, minY, maxX, maxY, pixelCount }
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
			if (visited[idx] !== 0) continue; // 이미 방문 또는 투명
			
			visited[idx] = blobId;
			pixelCount++;
			
			// AABB 업데이트
			if (x < minX) minX = x;
			if (x > maxX) maxX = x;
			if (y < minY) minY = y;
			if (y > maxY) maxY = y;
			
			// 8방향 탐색 (대각선 포함)
			stack.push([x - 1, y]);
			stack.push([x + 1, y]);
			stack.push([x, y - 1]);
			stack.push([x, y + 1]);
			stack.push([x - 1, y - 1]);
			stack.push([x + 1, y - 1]);
			stack.push([x - 1, y + 1]);
			stack.push([x + 1, y + 1]);
		}
		
		return { minX, minY, maxX, maxY, pixelCount };
	}

	// 모든 픽셀 스캔하여 덩어리 찾기
	console.log('🔍 덩어리 탐색 중...\n');
	
	for (let y = 0; y < height; y++) {
		for (let x = 0; x < width; x++) {
			const idx = y * width + x;
			if (visited[idx] === 0) { // 미방문 불투명 픽셀
				const blob = floodFill(x, y);
				if (blob.pixelCount > 50) { // 노이즈 제거 (50픽셀 이상만)
					blobs.push(blob);
				}
			}
		}
	}

	// 결과 정렬 (좌상단→우하단, 행 우선)
	blobs.sort((a, b) => {
		const rowA = Math.floor(a.minY / (height / 3));
		const rowB = Math.floor(b.minY / (height / 3));
		if (rowA !== rowB) return rowA - rowB;
		return a.minX - b.minX;
	});

	// 결과 출력
	console.log(`📊 감지된 캐릭터: ${blobs.length}개\n`);
	console.log('┌─────┬────────────────────────────────┬───────────┬────────┐');
	console.log('│  #  │           AABB (x,y,w,h)       │   크기    │ 픽셀수 │');
	console.log('├─────┼────────────────────────────────┼───────────┼────────┤');
	
	for (let i = 0; i < blobs.length; i++) {
		const b = blobs[i];
		const w = b.maxX - b.minX + 1;
		const h = b.maxY - b.minY + 1;
		const num = String(i + 1).padStart(3);
		const aabb = `(${b.minX}, ${b.minY}, ${w}, ${h})`.padEnd(28);
		const size = `${w}x${h}`.padStart(9);
		const pixels = String(b.pixelCount).padStart(6);
		console.log(`│ ${num} │ ${aabb} │ ${size} │ ${pixels} │`);
	}
	
	console.log('└─────┴────────────────────────────────┴───────────┴────────┘');

	// JSON 형태로도 출력
	console.log('\n📋 JSON 형태:');
	console.log('const CHARACTERS = [');
	for (let i = 0; i < blobs.length; i++) {
		const b = blobs[i];
		const w = b.maxX - b.minX + 1;
		const h = b.maxY - b.minY + 1;
		console.log(`  { x: ${b.minX}, y: ${b.minY}, w: ${w}, h: ${h} },`);
	}
	console.log('];');

	console.log('\n═══════════════════════════════════════════════════════');
	console.log('   완료!');
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
