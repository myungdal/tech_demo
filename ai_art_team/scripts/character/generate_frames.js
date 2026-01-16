/**
 * 개별 애니메이션 프레임 생성 스크립트
 * 
 * 사용법:
 *   node generate_frames.js warrior           # warrior 전체 (44프레임)
 *   node generate_frames.js warrior idle      # warrior의 idle만 (6프레임)
 *   node generate_frames.js warrior idle 1-3  # warrior idle 1~3번만
 */

const fs = require('fs');
const path = require('path');

// ═══════════════════════════════════════════════════════════════════════════
// 설정
// ═══════════════════════════════════════════════════════════════════════════

let OPENAI_API_KEY = process.env.OPENAI_API_KEY;
const configPath = path.join(__dirname, 'config.json');
if (fs.existsSync(configPath)) {
	const config = JSON.parse(fs.readFileSync(configPath, 'utf8'));
	if (config.OPENAI_API_KEY) OPENAI_API_KEY = config.OPENAI_API_KEY;
}

if (!OPENAI_API_KEY) {
	console.error('❌ OpenAI API 키가 필요합니다. config.json 확인');
	process.exit(1);
}

const THUMBNAIL_DIR = path.join(__dirname, 'thumbnail', 'individual');
const OUTPUT_DIR = path.join(__dirname, 'animation_frames');

// ═══════════════════════════════════════════════════════════════════════════
// 애니메이션 정의
// ═══════════════════════════════════════════════════════════════════════════

const ANIMATIONS = {
	idle: {
		frames: 6,
		poses: [
			'Standing neutral pose, relaxed, slight inhale beginning',
			'Chest slightly raised, breathing in',
			'Peak of breath, chest fully expanded',
			'Beginning to exhale, chest starting to lower',
			'Chest lowering, relaxed exhale',
			'Return to neutral standing pose'
		]
	},
	run: {
		frames: 6,
		poses: [
			'Contact pose - front foot just touching ground, back leg extended',
			'Recoil - front knee bent absorbing impact, arms swinging',
			'Passing - legs crossing mid-stride, body upright',
			'High point - body at highest, back leg swinging forward',
			'Forward fall - leaning forward, front leg reaching',
			'Contact opposite - other foot landing, cycle complete'
		]
	},
	summon: {
		frames: 8,
		poses: [
			'Magical particles gathering, character fading in 10%',
			'More particles, character 25% visible, translucent',
			'Swirling energy, character 40% visible',
			'Bright flash, character 60% visible',
			'Energy condensing, character 75% visible',
			'Almost solid, character 90% visible',
			'Fully materialized, energy dispersing',
			'Landing pose, ready stance, summon complete'
		]
	},
	dash: {
		frames: 5,
		poses: [
			'Crouch preparation, coiled to spring',
			'Launch - blur effect, body stretched horizontal',
			'Mid-dash - maximum speed, motion blur',
			'Deceleration - body tilting back',
			'Recovery - returning to standing stance'
		]
	},
	hit: {
		frames: 3,
		poses: [
			'Impact moment - head snapping back, body recoiling',
			'Pain reaction - grimace, body bent backward',
			'Recovery start - beginning to stand straight'
		]
	},
	die: {
		frames: 8,
		poses: [
			'Fatal hit - extreme recoil, arms flying up',
			'Falling backward - body arching',
			'Mid-fall - legs leaving ground',
			'Falling continued - body horizontal',
			'About to hit ground - body crumpling',
			'Ground impact - dust/debris',
			'Settling - body on ground, limbs falling',
			'Final rest - completely still, defeated pose'
		]
	},
	tombstone: {
		frames: 1,
		poses: [
			'Gravestone or skeleton remains marking death spot'
		]
	},
	attack: {
		frames: 7,
		poses: [
			'Ready stance - weapon held, preparing to strike',
			'Wind up - weapon raised high, body coiled',
			'Peak of swing - weapon at highest point',
			'Strike - weapon swinging down/forward with force',
			'Impact - weapon fully extended, maximum reach',
			'Follow through - weapon continuing motion',
			'Return - bringing weapon back to ready position'
		]
	}
};

// 캐릭터 정보
const CHARACTERS = {
	warrior: { name: '전사', weapon: 'sword and shield', style: 'Roman soldier with plumed helmet, armor, red cape' },
	archer: { name: '궁수', weapon: 'bow', style: 'hooded ranger with green cloak' },
	assassin: { name: '암살자', weapon: 'dual daggers', style: 'dark hooded figure, ninja-like' },
	bat: { name: '박쥐', weapon: 'claws/fangs', style: 'flying bat creature, always airborne' },
	bear: { name: '곰', weapon: 'claws', style: 'large brown bear, heavy movements' },
	spider: { name: '거미', weapon: 'fangs', style: 'eight-legged spider, creepy' },
	zeus: { name: '제우스', weapon: 'lightning bolt', style: 'old man with white beard, white toga' },
	hades: { name: '하데스', weapon: 'trident', style: 'dark robed figure with trident' },
	cupid: { name: '큐피드', weapon: 'bow', style: 'small angel with wings, always floating' },
	golem: { name: '골렘', weapon: 'fists', style: 'stone creature, slow and heavy' },
	phoenix: { name: '피닉스', weapon: 'fire', style: 'flaming bird, always flying' },
	dragon: { name: '드래곤', weapon: 'claws/fire', style: 'green dragon with wings' }
};

// ═══════════════════════════════════════════════════════════════════════════
// 프롬프트 생성
// ═══════════════════════════════════════════════════════════════════════════

function createPrompt(charInfo, animName, frameNum, totalFrames, poseDesc) {
	return `Create a single pixel art character animation frame.
CRITICAL: Match the character design EXACTLY from the attached reference image.

=== TECHNICAL SPECS ===
- Output: 1024x1024 pixels (will be resized to 128x128)
- Single character, single pose
- Character facing RIGHT (→)
- Character centered in frame
- Background: Solid MAGENTA (#FF00FF)
- Style: 16-bit retro pixel art
- Hard pixel edges only - NO anti-aliasing, NO smoothing, NO blur

=== CHARACTER ===
${charInfo.style}
Weapon/Tool: ${charInfo.weapon}

=== ANIMATION: ${animName.toUpperCase()} ===
Frame ${frameNum} of ${totalFrames}

=== POSE DESCRIPTION ===
${poseDesc}

=== STRICT RULES ===
1. Match reference character EXACTLY (colors, proportions, details)
2. Character must face RIGHT
3. NO projectiles, NO magic effects, NO particles
4. NO text, NO labels, NO UI elements
5. Background must be solid magenta #FF00FF
6. Character should fill about 60-70% of frame height`;
}

// ═══════════════════════════════════════════════════════════════════════════
// 썸네일 로드
// ═══════════════════════════════════════════════════════════════════════════

function loadThumbnail(charName) {
	const thumbPath = path.join(THUMBNAIL_DIR, `${charName}.png`);
	if (!fs.existsSync(thumbPath)) {
		console.error(`❌ 썸네일 없음: ${thumbPath}`);
		return null;
	}
	return fs.readFileSync(thumbPath).toString('base64');
}

// ═══════════════════════════════════════════════════════════════════════════
// 이미지 생성
// ═══════════════════════════════════════════════════════════════════════════

async function generateFrame(charName, charInfo, animName, frameNum, thumbnail) {
	const anim = ANIMATIONS[animName];
	const poseDesc = anim.poses[frameNum - 1];
	const prompt = createPrompt(charInfo, animName, frameNum, anim.frames, poseDesc);
	
	// 출력 디렉토리
	const charDir = path.join(OUTPUT_DIR, charName);
	if (!fs.existsSync(charDir)) {
		fs.mkdirSync(charDir, { recursive: true });
	}
	
	const outputPath = path.join(charDir, `${animName}_${frameNum}.png`);
	
	// 이미 존재하면 스킵
	if (fs.existsSync(outputPath)) {
		console.log(`   ⏭️  ${animName}_${frameNum}.png (이미 존재)`);
		return true;
	}
	
	console.log(`   🎨 ${animName}_${frameNum}.png 생성 중...`);
	
	try {
		const response = await fetch('https://api.openai.com/v1/images/generations', {
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
				'Authorization': `Bearer ${OPENAI_API_KEY}`
			},
			body: JSON.stringify({
				model: 'gpt-image-1',
				prompt: prompt,
				n: 1,
				size: '1024x1024',
				quality: 'medium'
			})
		});

		if (!response.ok) {
			const error = await response.json();
			throw new Error(error.error?.message || `HTTP ${response.status}`);
		}

		const data = await response.json();
		
		if (data.data[0].b64_json) {
			fs.writeFileSync(outputPath, Buffer.from(data.data[0].b64_json, 'base64'));
		} else if (data.data[0].url) {
			const imgRes = await fetch(data.data[0].url);
			fs.writeFileSync(outputPath, Buffer.from(await imgRes.arrayBuffer()));
		}
		
		console.log(`   ✅ ${animName}_${frameNum}.png 저장됨`);
		return true;
		
	} catch (err) {
		console.error(`   ❌ ${animName}_${frameNum}: ${err.message}`);
		return false;
	}
}

// ═══════════════════════════════════════════════════════════════════════════
// 메인
// ═══════════════════════════════════════════════════════════════════════════

async function main() {
	console.log('═══════════════════════════════════════════════════════');
	console.log('   애니메이션 프레임 생성기');
	console.log('═══════════════════════════════════════════════════════\n');

	const charName = process.argv[2];
	const animFilter = process.argv[3];
	const frameRange = process.argv[4];

	if (!charName || !CHARACTERS[charName]) {
		console.error('사용법: node generate_frames.js <캐릭터> [애니메이션] [프레임범위]');
		console.error('캐릭터:', Object.keys(CHARACTERS).join(', '));
		console.error('애니메이션:', Object.keys(ANIMATIONS).join(', '));
		process.exit(1);
	}

	const charInfo = CHARACTERS[charName];
	const thumbnail = loadThumbnail(charName);
	
	console.log(`🎯 캐릭터: ${charInfo.name} (${charName})`);

	// 대상 애니메이션 결정
	let targetAnims = Object.keys(ANIMATIONS);
	if (animFilter) {
		if (!ANIMATIONS[animFilter]) {
			console.error(`❌ 알 수 없는 애니메이션: ${animFilter}`);
			process.exit(1);
		}
		targetAnims = [animFilter];
	}

	// 프레임 범위 파싱
	let frameStart = 1, frameEnd = 999;
	if (frameRange) {
		if (frameRange.includes('-')) {
			[frameStart, frameEnd] = frameRange.split('-').map(Number);
		} else {
			frameStart = frameEnd = parseInt(frameRange);
		}
	}

	// 총 프레임 계산
	let totalFrames = 0;
	for (const anim of targetAnims) {
		const animFrames = ANIMATIONS[anim].frames;
		for (let f = 1; f <= animFrames; f++) {
			if (f >= frameStart && f <= frameEnd) totalFrames++;
		}
	}

	console.log(`📊 생성 대상: ${totalFrames}프레임`);
	console.log(`💰 예상 비용: $${(totalFrames * 0.02).toFixed(2)}`);
	console.log(`\n3초 후 시작...\n`);
	await new Promise(r => setTimeout(r, 3000));

	let success = 0, fail = 0;

	for (const animName of targetAnims) {
		const anim = ANIMATIONS[animName];
		console.log(`\n📂 ${animName.toUpperCase()} (${anim.frames}프레임)`);
		
		for (let f = 1; f <= anim.frames; f++) {
			if (f < frameStart || f > frameEnd) continue;
			
			const ok = await generateFrame(charName, charInfo, animName, f, thumbnail);
			if (ok) success++; else fail++;
			
			// API 속도 제한
			if (f < anim.frames || targetAnims.indexOf(animName) < targetAnims.length - 1) {
				await new Promise(r => setTimeout(r, 3000));
			}
		}
	}

	console.log('\n═══════════════════════════════════════════════════════');
	console.log(`   완료! 성공: ${success}, 실패: ${fail}`);
	console.log(`   출력: animation_frames/${charName}/`);
	console.log('═══════════════════════════════════════════════════════');
}

main().catch(console.error);
