/**
 * 마젠타(255, 0, 255) 근처 색을 무채색으로 변환하는 스크립트
 * 
 * 사용법: node remove_magenta.js
 */

const fs = require('fs');
const path = require('path');
const { Jimp } = require('jimp');

async function processImage(inputPath, outputPath, tolerance) {
	const image = await Jimp.read(inputPath);
	
	const width = image.width;
	const height = image.height;
	let changedPixels = 0;
	
	for (let y = 0; y < height; y++) {
		for (let x = 0; x < width; x++) {
			const color = image.getPixelColor(x, y);
			
			// unsigned 처리를 위해 >>> 0 사용
			const r = (color >>> 24) & 0xFF;
			const g = (color >>> 16) & 0xFF;
			const b = (color >>> 8) & 0xFF;
			const a = color & 0xFF;
			
			// 마젠타(255, 0, 255) 근처인지 확인
			// 빨간색이 높고, 초록색이 낮고, 파란색이 높은 색상
			const isMagenta = 
				r >= (255 - tolerance) &&
				g <= tolerance &&
				b >= (255 - tolerance);
			
			if (isMagenta) {
				// 무채색(검은색)으로 설정, 알파 유지
				const newColor = ((0 << 24) | (0 << 16) | (0 << 8) | a) >>> 0;
				image.setPixelColor(newColor, x, y);
				changedPixels++;
			}
		}
	}
	
	await image.write(outputPath);
	return changedPixels;
}

async function main() {
	const tolerance = 110;  // 색상 허용 범위 (0~255) - RGB(145, 110, 145) 커버
	const animationDir = __dirname;
	
	console.log('=== 마젠타 배경 → 무채색 변환 스크립트 ===');
	console.log(`허용 범위: ${tolerance}`);
	console.log(`대상: R >= ${255 - tolerance}, G <= ${tolerance}, B >= ${255 - tolerance}`);
	console.log(`변환 색상: 검은색 (0, 0, 0)\n`);
	
	// PNG 파일 목록 (백업 파일 제외)
	const files = fs.readdirSync(animationDir)
		.filter(f => f.endsWith('.png') && !f.includes('_backup'));
	
	console.log(`총 ${files.length}개의 PNG 파일 발견\n`);
	
	for (const file of files) {
		const inputPath = path.join(animationDir, file);
		const backupPath = path.join(animationDir, file.replace('.png', '_backup.png'));
		
		console.log(`처리 중: ${file}`);
		
		// 백업이 있으면 백업에서 읽기 (원본 보존)
		const sourcePath = fs.existsSync(backupPath) ? backupPath : inputPath;
		
		// 백업 생성 (없으면)
		if (!fs.existsSync(backupPath)) {
			fs.copyFileSync(inputPath, backupPath);
			console.log(`  ✓ 백업 생성: ${file.replace('.png', '_backup.png')}`);
		}
		
		try {
			const changed = await processImage(sourcePath, inputPath, tolerance);
			console.log(`  ✓ 완료! (${changed.toLocaleString()}개 픽셀 변환됨)`);
		} catch (err) {
			console.error(`  ✗ 오류: ${err.message}`);
		}
	}
	
	console.log('\n=== 처리 완료 ===');
}

main().catch(console.error);
