/**
 * 스프라이트 시트 조합 스크립트
 * 
 * 기능:
 * 1. animation_processed/{캐릭터}/ 폴더의 프레임 로드
 * 2. 8x8 그리드 (1024x1024) 캔버스 생성
 * 3. 각 프레임을 올바른 위치에 배치
 * 4. 빈 셀은 투명 유지
 * 5. animation/{캐릭터}.png로 저장
 * 
 * 사용법:
 *   node assemble_sheet.js warrior           # warrior 시트 생성
 *   node assemble_sheet.js warrior --partial # 있는 프레임만으로 부분 시트 생성
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const INPUT_DIR = path.join(__dirname, 'animation_processed');
const OUTPUT_DIR = path.join(__dirname, 'animation');

const GRID_SIZE = 8;       // 8x8 그리드
const CELL_SIZE = 128;     // 각 셀 128x128
const SHEET_SIZE = 1024;   // 전체 시트 1024x1024

// ═══════════════════════════════════════════════════════════════════════════
// 애니메이션 레이아웃 정의
// ═══════════════════════════════════════════════════════════════════════════

const ANIMATIONS = [
	{ name: 'idle',      row: 0, frames: 6 },
	{ name: 'run',       row: 1, frames: 6 },
	{ name: 'summon',    row: 2, frames: 8 },
	{ name: 'dash',      row: 3, frames: 5 },
	{ name: 'hit',       row: 4, frames: 3 },
	{ name: 'die',       row: 5, frames: 8 },
	{ name: 'tombstone', row: 6, frames: 1 },
	{ name: 'attack',    row: 7, frames: 7 },
];

// ═══════════════════════════════════════════════════════════════════════════
// 메인 로직
// ═══════════════════════════════════════════════════════════════════════════

async function assembleSheet(charName, partialMode = false) {
	const inputDir = path.join(INPUT_DIR, charName);
	const outputPath = path.join(OUTPUT_DIR, `${charName}.png`);

	if (!fs.existsSync(inputDir)) {
		console.error(`   ❌ 입력 폴더 없음: ${inputDir}`);
		return false;
	}

	// 출력 폴더 생성
	if (!fs.existsSync(OUTPUT_DIR)) {
		fs.mkdirSync(OUTPUT_DIR, { recursive: true });
	}

	// 사용 가능한 프레임 파일 목록
	const availableFiles = new Set(
		fs.readdirSync(inputDir).filter(f => f.endsWith('.png'))
	);

	console.log(`   📂 프레임 폴더: ${inputDir}`);
	console.log(`   📊 발견된 프레임: ${availableFiles.size}개`);

	// 필요한 프레임 체크
	const requiredFrames = [];
	const missingFrames = [];

	for (const anim of ANIMATIONS) {
		for (let i = 1; i <= anim.frames; i++) {
			const filename = `${anim.name}_${i}.png`;
			requiredFrames.push({
				filename,
				row: anim.row,
				col: i - 1,
				exists: availableFiles.has(filename)
			});
			if (!availableFiles.has(filename)) {
				missingFrames.push(filename);
			}
		}
	}

	if (missingFrames.length > 0 && !partialMode) {
		console.error(`   ❌ 누락된 프레임 ${missingFrames.length}개:`);
		missingFrames.forEach(f => console.error(`      - ${f}`));
		console.error(`   💡 --partial 옵션으로 부분 시트 생성 가능`);
		return false;
	}

	// 투명 캔버스 생성
	console.log(`\n   🎨 1024x1024 투명 캔버스 생성...`);

	// 합성할 이미지 목록 생성
	const composites = [];

	for (const frame of requiredFrames) {
		if (!frame.exists) continue;

		const framePath = path.join(inputDir, frame.filename);
		const x = frame.col * CELL_SIZE;
		const y = frame.row * CELL_SIZE;

		composites.push({
			input: framePath,
			left: x,
			top: y
		});
	}

	console.log(`   🔧 ${composites.length}개 프레임 배치 중...`);

	// 투명 캔버스에 프레임들 합성
	await sharp({
		create: {
			width: SHEET_SIZE,
			height: SHEET_SIZE,
			channels: 4,
			background: { r: 0, g: 0, b: 0, alpha: 0 }
		}
	})
		.composite(composites)
		.png()
		.toFile(outputPath);

	console.log(`   ✅ 저장 완료: ${outputPath}`);

	// 결과 요약
	const placedCount = composites.length;
	const totalRequired = requiredFrames.length;
	const completeness = Math.round((placedCount / totalRequired) * 100);

	console.log(`\n   📊 결과:`);
	console.log(`      - 배치된 프레임: ${placedCount}/${totalRequired} (${completeness}%)`);
	
	if (missingFrames.length > 0) {
		console.log(`      - 누락된 프레임: ${missingFrames.length}개`);
		console.log(`      - 누락 목록:`);
		missingFrames.forEach(f => console.log(`         • ${f}`));
	}

	return true;
}

// ═══════════════════════════════════════════════════════════════════════════
// CLI 엔트리포인트
// ═══════════════════════════════════════════════════════════════════════════

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   스프라이트 시트 조합');
	console.log('═══════════════════════════════════════════════════════\n');

	const charName = process.argv[2];
	const partialMode = process.argv.includes('--partial');

	if (!charName) {
		console.error('사용법: node assemble_sheet.js <캐릭터> [--partial]');
		console.error('');
		console.error('옵션:');
		console.error('  --partial  누락된 프레임이 있어도 부분 시트 생성');
		process.exit(1);
	}

	console.log(`🎯 캐릭터: ${charName}`);
	console.log(`📋 모드: ${partialMode ? '부분 시트' : '전체 시트'}\n`);

	const success = await assembleSheet(charName, partialMode);

	console.log('\n═══════════════════════════════════════════════════════');
	if (success) {
		console.log(`   ✅ 완료! animation/${charName}.png 생성됨`);
	} else {
		console.log('   ❌ 실패');
	}
	console.log('═══════════════════════════════════════════════════════');

	process.exit(success ? 0 : 1);
}

main().catch(err => {
	console.error('오류:', err);
	process.exit(1);
});
