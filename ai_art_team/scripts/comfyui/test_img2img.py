"""img2img 테스트"""
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
OUTPUT_DIR = PROJECT_ROOT / "ai_art_team" / "character" / "animation_frames" / "test_img2img"

def load_workflow():
    with open(Path(__file__).parent / "workflow_img2img.json", "r") as f:
        return json.load(f)

def test_img2img(denoise: float = 0.3):
    """denoise 값으로 테스트"""
    # 캐릭터 이미지 복사
    src = CHARACTER_INPUT / "01.png"
    dest = COMFYUI_INPUT / "test_char.png"
    shutil.copy(src, dest)
    print(f"[COPY] {src.name} -> ComfyUI/input/test_char.png")
    
    # 워크플로우 설정
    workflow = load_workflow()
    workflow["2"]["inputs"]["image"] = "test_char.png"
    workflow["5"]["inputs"]["text"] = "pixel art warrior sprite, idle pose breathing, facing right, game asset"
    workflow["6"]["inputs"]["text"] = "blurry, 3d, realistic, different character"
    workflow["7"]["inputs"]["seed"] = 42
    workflow["7"]["inputs"]["denoise"] = denoise
    workflow["9"]["inputs"]["filename_prefix"] = f"test_img2img/denoise_{int(denoise*100):02d}"
    
    print(f"\n[TEST] denoise = {denoise}")
    
    # 실행
    resp = requests.post(f"{COMFYUI_API}/prompt", json={"prompt": workflow}, timeout=30)
    if resp.status_code != 200:
        print(f"[ERROR] {resp.status_code}: {resp.text[:200]}")
        return
    
    prompt_id = resp.json().get("prompt_id")
    print(f"  큐 ID: {prompt_id}")
    
    # 대기
    for _ in range(60):
        time.sleep(2)
        hist = requests.get(f"{COMFYUI_API}/history/{prompt_id}").json()
        if prompt_id in hist:
            status = hist[prompt_id].get("status", {}).get("status_str")
            if status == "success":
                print(f"  [OK] 완료")
                outputs = hist[prompt_id].get("outputs", {}).get("9", {}).get("images", [])
                if outputs:
                    print(f"  출력: {outputs[0]['subfolder']}/{outputs[0]['filename']}")
                return
            elif status == "error":
                print(f"  [ERROR] 생성 실패")
                return
    print("  [TIMEOUT]")

if __name__ == "__main__":
    print("=== img2img 테스트 ===")
    print("denoise 값별 결과 비교")
    print()
    
    # 여러 denoise 값으로 테스트
    for d in [0.2, 0.3, 0.4]:
        test_img2img(d)
        time.sleep(2)
    
    print("\n완료! 결과 확인:")
    print(f"  {COMFYUI_OUTPUT / 'test_img2img'}")
