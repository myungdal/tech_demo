/**
 * 개별 캐릭터를 정확한 간격의 그리드로 재조립
 * 
 * 입력: character/individual/01~12.png (64x64)
 * 출력: character/12characters_grid.png (512x512, 4x3 그리드)
 * 
 * 사용법: node assemble_grid.js
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const BASE_DIR = path.join(__dirname, '..', '..', 'character');
const INPUT_DIR = path.join(BASE_DIR, 'individual');
const OUTPUT_PATH = path.join(BASE_DIR, '12characters_grid.png');

// 그리드 설정
const COLS = 4;
const ROWS = 3;
const CELL_SIZE = 128;  // 각 셀 크기 (64 → 128 업스케일)
const OUTPUT_SIZE = 512; // 출력 크기 (2의 자승)

async function main() {
	console.log('=======================================================');
	console.log('   캐릭터 그리드 조립 (512x512)');
	console.log('=======================================================\n');

	// 출력 크기 계산
	const gridWidth = COLS * CELL_SIZE;   // 4 * 128 = 512
	const gridHeight = ROWS * CELL_SIZE;  // 3 * 128 = 384

	console.log(`Grid: ${COLS}x${ROWS} = ${gridWidth}x${gridHeight}`);
	console.log(`Cell: ${CELL_SIZE}x${CELL_SIZE}`);
	console.log(`Output: ${OUTPUT_SIZE}x${OUTPUT_SIZE}\n`);

	// 캐릭터 파일 로드
	const composites = [];
	
	for (let i = 1; i <= 12; i++) {
		const num = String(i).padStart(2, '0');
		const filePath = path.join(INPUT_DIR, `${num}.png`);
		
		if (!fs.existsSync(filePath)) {
			console.log(`  [SKIP] ${num}.png not found`);
			continue;
		}
		
		// 64x64 → 128x128 업스케일 (nearest neighbor로 픽셀아트 유지)
		const resized = await sharp(filePath)
			.resize(CELL_SIZE, CELL_SIZE, { kernel: sharp.kernel.nearest })
			.png()
			.toBuffer();
		
		// 그리드 위치 계산
		const col = (i - 1) % COLS;
		const row = Math.floor((i - 1) / COLS);
		const x = col * CELL_SIZE;
		const y = row * CELL_SIZE;
		
		composites.push({
			input: resized,
			left: x,
			top: y
		});
		
		console.log(`  [OK] ${num}.png -> (${x}, ${y})`);
	}

	// 512x512 캔버스에 조립
	const paddingTop = Math.floor((OUTPUT_SIZE - gridHeight) / 2);  // (512-384)/2 = 64
	
	// 패딩 적용된 위치로 조정
	for (const c of composites) {
		c.top += paddingTop;
	}

	console.log(`\n  Padding top: ${paddingTop}px`);
	console.log('\n  Assembling...');

	await sharp({
		create: {
			width: OUTPUT_SIZE,
			height: OUTPUT_SIZE,
			channels: 4,
			background: { r: 0, g: 0, b: 0, alpha: 0 }
		}
	})
	.composite(composites)
	.png()
	.toFile(OUTPUT_PATH);

	console.log(`\n  [OK] Saved: ${OUTPUT_PATH}`);
	console.log('\n=======================================================');
	console.log('   Done!');
	console.log('=======================================================');
}

main().catch(console.error);
