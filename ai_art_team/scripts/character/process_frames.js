/**
 * 프레임 후처리 스크립트
 * 
 * 기능:
 * 1. 마젠타 배경 제거 (→ 투명)
 * 2. 128x128로 리사이즈 (nearest neighbor)
 * 3. 캐릭터 센터링
 * 
 * 사용법:
 *   node process_frames.js warrior           # warrior 전체 처리
 *   node process_frames.js warrior idle      # warrior의 idle만
 */

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const INPUT_DIR = path.join(__dirname, 'animation_frames');
const OUTPUT_DIR = path.join(__dirname, 'animation_processed');
const TARGET_SIZE = 128;

// ═══════════════════════════════════════════════════════════════════════════
// 마젠타 제거 (split_thumbnails_v3.js와 동일 로직)
// ═══════════════════════════════════════════════════════════════════════════

async function removeBackground(inputPath) {
	const { data, info } = await sharp(inputPath)
		.ensureAlpha()
		.raw()
		.toBuffer({ resolveWithObject: true });
	
	const { width, height, channels } = info;
	const rgbaData = Buffer.alloc(width * height * 4);
	
	for (let i = 0; i < width * height; i++) {
		const idx = i * channels;
		const outIdx = i * 4;
		const r = data[idx];
		const g = data[idx + 1];
		const b = data[idx + 2];
		const a = channels >= 4 ? data[idx + 3] : 255;
		
		// 마젠타 계열 감지
		const isExactMagenta = (r === 255 && g === 0 && b === 255);
		const isNearMagenta = (r >= 250 && g <= 10 && b >= 250);
		const isAntiAliasedMagenta = (
			(r >= 150 && r <= 170 && g <= 70 && b >= 100 && b <= 130) ||
			(r > 150 && b > 120 && g < 80 && Math.abs(r - b) < 80) ||
			(r > 200 && g < 50 && b > 200)
		);
		
		if (isExactMagenta || isNearMagenta || isAntiAliasedMagenta) {
			rgbaData[outIdx] = 0;
			rgbaData[outIdx + 1] = 0;
			rgbaData[outIdx + 2] = 0;
			rgbaData[outIdx + 3] = 0;
		} else {
			rgbaData[outIdx] = r;
			rgbaData[outIdx + 1] = g;
			rgbaData[outIdx + 2] = b;
			rgbaData[outIdx + 3] = a;
		}
	}
	
	return sharp(rgbaData, { raw: { width, height, channels: 4 } })
		.png()
		.toBuffer();
}

// ═══════════════════════════════════════════════════════════════════════════
// 프레임 처리
// ═══════════════════════════════════════════════════════════════════════════

async function processFrame(inputPath, outputPath) {
	try {
		// 1. 마젠타 배경 제거
		const transparent = await removeBackground(inputPath);
		
		// 2. 128x128로 리사이즈 (contain 모드로 비율 유지, 중앙 정렬)
		await sharp(transparent)
			.resize(TARGET_SIZE, TARGET_SIZE, {
				kernel: sharp.kernel.nearest,
				fit: 'contain',
				background: { r: 0, g: 0, b: 0, alpha: 0 }
			})
			.png()
			.toFile(outputPath);
		
		return true;
	} catch (err) {
		console.error(`   ❌ 오류: ${err.message}`);
		return false;
	}
}

// ═══════════════════════════════════════════════════════════════════════════
// 메인
// ═══════════════════════════════════════════════════════════════════════════

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   프레임 후처리');
	console.log('═══════════════════════════════════════════════════════\n');

	const charName = process.argv[2];
	const animFilter = process.argv[3];

	if (!charName) {
		console.error('사용법: node process_frames.js <캐릭터> [애니메이션]');
		process.exit(1);
	}

	const inputCharDir = path.join(INPUT_DIR, charName);
	const outputCharDir = path.join(OUTPUT_DIR, charName);

	if (!fs.existsSync(inputCharDir)) {
		console.error(`❌ 입력 폴더 없음: ${inputCharDir}`);
		process.exit(1);
	}

	// 출력 폴더 생성
	if (!fs.existsSync(outputCharDir)) {
		fs.mkdirSync(outputCharDir, { recursive: true });
	}

	// 처리할 파일 목록
	let files = fs.readdirSync(inputCharDir).filter(f => f.endsWith('.png'));
	if (animFilter) {
		files = files.filter(f => f.startsWith(animFilter + '_'));
	}

	console.log(`🎯 캐릭터: ${charName}`);
	console.log(`📊 처리 대상: ${files.length}개 파일\n`);

	let success = 0, fail = 0;

	for (const file of files) {
		const inputPath = path.join(inputCharDir, file);
		const outputPath = path.join(outputCharDir, file);
		
		process.stdout.write(`   🔧 ${file}...`);
		
		if (await processFrame(inputPath, outputPath)) {
			console.log(' ✅');
			success++;
		} else {
			console.log(' ❌');
			fail++;
		}
	}

	console.log('\n═══════════════════════════════════════════════════════');
	console.log(`   완료! 성공: ${success}, 실패: ${fail}`);
	console.log(`   출력: animation_processed/${charName}/`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
