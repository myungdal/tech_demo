"""공격 애니메이션 8프레임 생성 (img2img)"""
import json
import time
import shutil
import requests
from pathlib import Path

COMFYUI_API = "http://localhost:8188"
PROJECT_ROOT = Path(__file__).parent.parent.parent.parent
COMFYUI_INPUT = PROJECT_ROOT / "tools" / "ComfyUI" / "input"
COMFYUI_OUTPUT = PROJECT_ROOT / "tools" / "ComfyUI" / "output"
CHARACTER_INPUT = PROJECT_ROOT / "ai_art_team" / "character" / "individual"
OUTPUT_DIR = PROJECT_ROOT / "ai_art_team" / "character" / "animation_frames" / "warrior"

ATTACK_POSES = [
    "ready stance, sword raised, preparing to attack",
    "wind up, pulling sword back, weight shifting",
    "swing start, sword moving forward, aggressive",
    "mid swing, sword horizontal, full force",
    "strike impact, sword extended forward, hit",
    "follow through, sword past target, momentum",
    "recovery, pulling sword back, rebalancing",
    "return to ready, sword up, alert stance",
]

DENOISE = 0.35

def load_workflow():
    with open(Path(__file__).parent / "workflow_img2img.json", "r") as f:
        return json.load(f)

def generate_frame(frame_num: int, pose: str):
    """단일 프레임 생성"""
    # 캐릭터 이미지 복사
    src = CHARACTER_INPUT / "01.png"
    dest = COMFYUI_INPUT / "char_01.png"
    shutil.copy(src, dest)
    
    # 워크플로우 설정
    workflow = load_workflow()
    workflow["2"]["inputs"]["image"] = "char_01.png"
    workflow["5"]["inputs"]["text"] = f"pixel art warrior sprite, {pose}, facing right, game asset, attack animation frame"
    workflow["6"]["inputs"]["text"] = "blurry, 3d, realistic, different character, idle, standing still"
    workflow["7"]["inputs"]["seed"] = 1000 + frame_num
    workflow["7"]["inputs"]["denoise"] = DENOISE
    workflow["9"]["inputs"]["filename_prefix"] = f"warrior/attack_{frame_num:02d}"
    
    print(f"[{frame_num}/8] {pose[:40]}...")
    
    # 실행
    resp = requests.post(f"{COMFYUI_API}/prompt", json={"prompt": workflow}, timeout=30)
    if resp.status_code != 200:
        print(f"  [ERROR] {resp.status_code}")
        return None
    
    prompt_id = resp.json().get("prompt_id")
    
    # 대기
    for _ in range(60):
        time.sleep(2)
        hist = requests.get(f"{COMFYUI_API}/history/{prompt_id}").json()
        if prompt_id in hist:
            status = hist[prompt_id].get("status", {}).get("status_str")
            if status == "success":
                outputs = hist[prompt_id].get("outputs", {}).get("9", {}).get("images", [])
                if outputs:
                    # 결과 복사
                    src_file = COMFYUI_OUTPUT / outputs[0]["subfolder"] / outputs[0]["filename"]
                    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
                    dest_file = OUTPUT_DIR / f"attack_{frame_num:02d}.png"
                    shutil.copy(src_file, dest_file)
                    print(f"  -> {dest_file.relative_to(PROJECT_ROOT / 'ai_art_team')}")
                    return True
            elif status == "error":
                print(f"  [ERROR]")
                return False
    print("  [TIMEOUT]")
    return False

def main():
    print("=== warrior 공격 애니메이션 8프레임 ===")
    print(f"denoise: {DENOISE}")
    print()
    
    for i, pose in enumerate(ATTACK_POSES, 1):
        generate_frame(i, pose)
        time.sleep(1)
    
    print(f"\n완료! 결과: {OUTPUT_DIR.relative_to(PROJECT_ROOT)}")

if __name__ == "__main__":
    main()
