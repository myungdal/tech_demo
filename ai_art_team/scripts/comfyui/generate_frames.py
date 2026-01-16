"""
ComfyUI 캐릭터 애니메이션 프레임 생성기

사용법:
    python generate_frames.py warrior           # warrior 전체 애니메이션
    python generate_frames.py warrior idle      # warrior idle만
    python generate_frames.py warrior idle 1-3  # warrior idle 1~3 프레임만
    python generate_frames.py --test            # 연결 테스트
"""

import os
import sys
import json
import time
import shutil
import requests
import argparse
from pathlib import Path

# === 경로 설정 ===
PROJECT_ROOT = Path(__file__).parent.parent.parent.parent  # nearest3
AI_ART_ROOT = PROJECT_ROOT / "ai_art_team"
COMFYUI_ROOT = PROJECT_ROOT / "tools" / "ComfyUI"

CHARACTER_INPUT = AI_ART_ROOT / "character" / "individual"
FRAMES_OUTPUT = AI_ART_ROOT / "character" / "animation_frames"
COMFYUI_INPUT = COMFYUI_ROOT / "input"
COMFYUI_OUTPUT = COMFYUI_ROOT / "output"

COMFYUI_API = "http://localhost:8188"

# === 애니메이션 정의 ===
ANIMATIONS = {
    "idle": {
        "frames": 6,
        "poses": [
            "standing neutral pose, slight inhale, weapon ready",
            "chest slightly raised, breathing in",
            "peak of breath, alert stance",
            "beginning exhale, relaxed",
            "chest lowering, calm",
            "return to neutral standing pose"
        ]
    },
    "run": {
        "frames": 6,
        "poses": [
            "running contact pose, front foot touching ground",
            "running recoil, body absorbing impact",
            "running passing position, legs crossing",
            "running high point, body at highest",
            "running forward fall, leaning forward",
            "running contact opposite, other foot landing"
        ]
    },
    "summon": {
        "frames": 8,
        "poses": [
            "appearing from magic portal, faint silhouette",
            "materializing, body forming",
            "half materialized, glowing outline",
            "mostly formed, energy dissipating",
            "almost complete, slight glow",
            "fully formed, energy fading",
            "landing pose, touching ground",
            "ready stance after summoning"
        ]
    },
    "dash": {
        "frames": 5,
        "poses": [
            "dash preparation, crouching",
            "dash launch, explosive movement",
            "dash mid-air, horizontal motion blur",
            "dash landing preparation",
            "dash recovery, skidding stop"
        ]
    },
    "hit": {
        "frames": 3,
        "poses": [
            "hit reaction, recoiling backward",
            "hit stagger, off balance",
            "hit recovery, regaining stance"
        ]
    },
    "die": {
        "frames": 8,
        "poses": [
            "fatal hit, dramatic reaction",
            "falling backward, losing balance",
            "mid-fall, arms flailing",
            "hitting ground, impact pose",
            "bouncing slightly, limp body",
            "settling on ground",
            "final position, motionless",
            "completely still, dead pose"
        ]
    },
    "tombstone": {
        "frames": 1,
        "poses": [
            "gravestone or corpse remains on ground"
        ]
    },
    "attack": {
        "frames": 7,
        "poses": [
            "attack ready stance, weapon raised",
            "attack wind up, pulling back weapon",
            "attack peak, weapon at highest point",
            "attack strike, weapon swinging forward",
            "attack impact, weapon fully extended",
            "attack follow through, momentum carrying",
            "attack recovery, returning to ready"
        ]
    }
}

# === 캐릭터 정보 ===
CHARACTERS = {
    "01": "warrior with armor sword and shield",
    "02": "archer with bow and quiver",
    "03": "assassin with black mask and daggers",
    "04": "bat creature with wings flying",
    "05": "brown bear with claws",
    "06": "spider with eight legs",
    "07": "zeus god with lightning and white beard",
    "08": "hades god with trident and black robe",
    "09": "cupid angel with wings and bow",
    "10": "stone golem large and heavy",
    "11": "phoenix fire bird with red feathers",
    "12": "green dragon with wings and tail"
}

# 파일명 매핑 (01.png -> warrior)
CHARACTER_NAMES = {
    "01": "warrior", "02": "archer", "03": "assassin", "04": "bat",
    "05": "bear", "06": "spider", "07": "zeus", "08": "hades",
    "09": "cupid", "10": "golem", "11": "phoenix", "12": "dragon"
}


def check_comfyui_connection():
    """ComfyUI 연결 확인"""
    try:
        resp = requests.get(f"{COMFYUI_API}/system_stats", timeout=5)
        if resp.status_code == 200:
            print("[OK] ComfyUI 연결됨")
            return True
    except Exception as e:
        print(f"[ERROR] ComfyUI 연결 실패: {e}")
    return False


def copy_image_to_comfyui(src_path: Path, dest_name: str) -> bool:
    """캐릭터 이미지를 ComfyUI input 폴더로 복사"""
    dest_path = COMFYUI_INPUT / dest_name
    try:
        shutil.copy(src_path, dest_path)
        print(f"[COPY] {src_path.name} -> ComfyUI/input/{dest_name}")
        return True
    except Exception as e:
        print(f"[ERROR] 이미지 복사 실패: {e}")
        return False


def build_prompt(char_desc: str, pose_desc: str) -> tuple[str, str]:
    """프롬프트 생성 - 캐릭터 일관성 우선"""
    # IP-Adapter가 캐릭터를 유지하므로 프롬프트는 최소화
    positive = f"""same character, {pose_desc}, pixel art sprite, facing right, game asset"""

    negative = """different character, different style, blurry, 3d render, realistic, multiple characters, text, background"""

    return positive.strip(), negative.strip()


def load_workflow_template() -> dict:
    """워크플로우 템플릿 로드"""
    template_path = Path(__file__).parent / "workflow_ipadapter.json"
    with open(template_path, "r", encoding="utf-8") as f:
        return json.load(f)


def create_workflow(char_image: str, positive: str, negative: str, 
                    output_prefix: str, seed: int) -> dict:
    """워크플로우 생성"""
    workflow = load_workflow_template()
    
    # 템플릿 값 치환 (노드 번호 확인!)
    # 3: LoadImage, 5: positive prompt, 6: negative prompt, 8: KSampler, 10: SaveImage
    workflow["3"]["inputs"]["image"] = char_image
    workflow["5"]["inputs"]["text"] = positive
    workflow["6"]["inputs"]["text"] = negative
    workflow["8"]["inputs"]["seed"] = seed
    workflow["10"]["inputs"]["filename_prefix"] = output_prefix
    
    return workflow


def queue_prompt(workflow: dict) -> str | None:
    """ComfyUI에 프롬프트 큐 추가"""
    try:
        resp = requests.post(
            f"{COMFYUI_API}/prompt",
            json={"prompt": workflow},
            timeout=30
        )
        if resp.status_code == 200:
            result = resp.json()
            if "error" in result:
                print(f"[ERROR] API 에러: {result['error']}")
                if "node_errors" in result:
                    for node_id, err in result["node_errors"].items():
                        print(f"  노드 {node_id}: {err}")
                return None
            return result.get("prompt_id")
        else:
            print(f"[ERROR] HTTP {resp.status_code}: {resp.text[:500]}")
    except Exception as e:
        print(f"[ERROR] 프롬프트 큐 실패: {e}")
    return None


def wait_for_completion(prompt_id: str, timeout: int = 300) -> dict | None:
    """프롬프트 완료 대기, 출력 정보 반환"""
    start = time.time()
    while time.time() - start < timeout:
        try:
            resp = requests.get(f"{COMFYUI_API}/history/{prompt_id}", timeout=10)
            if resp.status_code == 200:
                history = resp.json()
                if prompt_id in history:
                    data = history[prompt_id]
                    status = data.get("status", {})
                    if status.get("status_str") == "success":
                        return data.get("outputs", {})
                    elif status.get("status_str") == "error":
                        return None
        except:
            pass
        time.sleep(2)
    return None


def copy_output_to_frames(outputs: dict, char_name: str, anim_name: str, frame_num: int) -> bool:
    """ComfyUI 출력을 animation_frames 폴더로 복사"""
    # SaveImage 노드(10번)의 출력 찾기
    save_output = outputs.get("10", {})
    images = save_output.get("images", [])
    
    if not images:
        print(f"  [WARN] 출력 이미지 없음")
        return False
    
    for img_info in images:
        src_file = COMFYUI_OUTPUT / img_info.get("subfolder", "") / img_info["filename"]
        dest_dir = FRAMES_OUTPUT / char_name
        dest_dir.mkdir(parents=True, exist_ok=True)
        dest_file = dest_dir / f"{anim_name}_{frame_num:02d}.png"
        
        try:
            shutil.copy(src_file, dest_file)
            print(f"  [COPY] -> {dest_file.relative_to(AI_ART_ROOT)}")
            return True
        except Exception as e:
            print(f"  [ERROR] 복사 실패: {e}")
            return False
    
    return False


def generate_frame(char_id: str, anim_name: str, frame_num: int, seed: int = 42) -> bool:
    """단일 프레임 생성"""
    if char_id not in CHARACTERS:
        print(f"[ERROR] 알 수 없는 캐릭터: {char_id}")
        return False
    
    if anim_name not in ANIMATIONS:
        print(f"[ERROR] 알 수 없는 애니메이션: {anim_name}")
        return False
    
    anim = ANIMATIONS[anim_name]
    if frame_num < 1 or frame_num > anim["frames"]:
        print(f"[ERROR] 프레임 범위 초과: {frame_num} (1-{anim['frames']})")
        return False
    
    char_desc = CHARACTERS[char_id]
    char_name = CHARACTER_NAMES[char_id]
    pose_desc = anim["poses"][frame_num - 1]
    
    # 이미지 복사
    src_image = CHARACTER_INPUT / f"{char_id}.png"
    if not src_image.exists():
        print(f"[ERROR] 캐릭터 이미지 없음: {src_image}")
        return False
    
    comfyui_image_name = f"char_{char_id}.png"
    if not copy_image_to_comfyui(src_image, comfyui_image_name):
        return False
    
    # 출력 폴더 생성
    output_dir = FRAMES_OUTPUT / char_name
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # 프롬프트 생성
    positive, negative = build_prompt(char_desc, pose_desc)
    output_prefix = f"{char_name}/{anim_name}_{frame_num:02d}"
    
    print(f"\n[GEN] {char_name} / {anim_name} / frame {frame_num}")
    print(f"  포즈: {pose_desc[:50]}...")
    
    # 워크플로우 생성 및 실행
    workflow = create_workflow(
        char_image=comfyui_image_name,
        positive=positive,
        negative=negative,
        output_prefix=output_prefix,
        seed=seed + frame_num
    )
    
    prompt_id = queue_prompt(workflow)
    if not prompt_id:
        return False
    
    print(f"  큐 ID: {prompt_id}")
    
    # 완료 대기
    outputs = wait_for_completion(prompt_id)
    if outputs:
        print(f"  [OK] 생성 완료")
        # animation_frames 폴더로 복사
        copy_output_to_frames(outputs, char_name, anim_name, frame_num)
        return True
    else:
        print(f"  [FAIL] 생성 실패 또는 시간 초과")
        return False


def generate_animation(char_id: str, anim_name: str, frame_range: str = None):
    """애니메이션 전체 또는 일부 프레임 생성"""
    if anim_name not in ANIMATIONS:
        print(f"[ERROR] 알 수 없는 애니메이션: {anim_name}")
        return
    
    anim = ANIMATIONS[anim_name]
    total = anim["frames"]
    
    # 프레임 범위 파싱
    if frame_range:
        if "-" in frame_range:
            start, end = map(int, frame_range.split("-"))
        else:
            start = end = int(frame_range)
    else:
        start, end = 1, total
    
    print(f"\n=== {CHARACTER_NAMES.get(char_id, char_id)} / {anim_name} ===")
    print(f"프레임: {start} ~ {end} (총 {end - start + 1}개)")
    
    for frame in range(start, end + 1):
        success = generate_frame(char_id, anim_name, frame)
        if not success:
            print(f"[WARN] 프레임 {frame} 생성 실패, 계속 진행")
        time.sleep(1)  # API 쿨다운


def generate_character(char_id: str):
    """캐릭터의 모든 애니메이션 생성"""
    print(f"\n{'='*50}")
    print(f"캐릭터: {CHARACTER_NAMES.get(char_id, char_id)} ({char_id})")
    print(f"{'='*50}")
    
    for anim_name in ANIMATIONS:
        generate_animation(char_id, anim_name)
        time.sleep(5)  # 애니메이션 간 쿨다운


def find_char_id(name: str) -> str | None:
    """캐릭터 이름으로 ID 찾기"""
    # 숫자 ID인 경우
    if name in CHARACTERS:
        return name
    
    # 이름으로 검색
    for cid, cname in CHARACTER_NAMES.items():
        if cname.lower() == name.lower():
            return cid
    
    return None


def main():
    parser = argparse.ArgumentParser(description="ComfyUI 애니메이션 프레임 생성기")
    parser.add_argument("character", nargs="?", help="캐릭터 이름/ID (예: warrior, 01)")
    parser.add_argument("animation", nargs="?", help="애니메이션 (예: idle, run)")
    parser.add_argument("frames", nargs="?", help="프레임 범위 (예: 1-3, 5)")
    parser.add_argument("--test", action="store_true", help="연결 테스트만")
    parser.add_argument("--list", action="store_true", help="캐릭터/애니메이션 목록")
    
    args = parser.parse_args()
    
    if args.test:
        check_comfyui_connection()
        return
    
    if args.list:
        print("\n=== 캐릭터 목록 ===")
        for cid, cname in CHARACTER_NAMES.items():
            print(f"  {cid}: {cname}")
        print("\n=== 애니메이션 목록 ===")
        for aname, ainfo in ANIMATIONS.items():
            print(f"  {aname}: {ainfo['frames']} frames")
        return
    
    if not args.character:
        parser.print_help()
        return
    
    # ComfyUI 연결 확인
    if not check_comfyui_connection():
        return
    
    # 캐릭터 ID 찾기
    char_id = find_char_id(args.character)
    if not char_id:
        print(f"[ERROR] 알 수 없는 캐릭터: {args.character}")
        print("--list 옵션으로 목록 확인")
        return
    
    # 실행
    if args.animation:
        generate_animation(char_id, args.animation, args.frames)
    else:
        generate_character(char_id)


if __name__ == "__main__":
    main()
