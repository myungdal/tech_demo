"""공격 애니메이션 생성 v2: 업스케일 + IP-Adapter + ControlNet OpenPose"""
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
POSE_REF_DIR = PROJECT_ROOT / "ai_art_team" / "character" / "pose_reference" / "attack"
OUTPUT_DIR = PROJECT_ROOT / "ai_art_team" / "character" / "animation_frames" / "warrior"

# 워크플로우: 업스케일 캐릭터 + IP-Adapter + ControlNet OpenPose
WORKFLOW = {
    "1": {
        "class_type": "CheckpointLoaderSimple",
        "inputs": {"ckpt_name": "sd_xl_base_1.0.safetensors"}
    },
    "2": {
        "class_type": "IPAdapterUnifiedLoader",
        "inputs": {"model": ["1", 0], "preset": "VIT-G (medium strength)"}
    },
    # 캐릭터 이미지 로드 및 업스케일
    "3": {
        "class_type": "LoadImage",
        "inputs": {"image": "CHAR_IMAGE"}
    },
    "4": {
        "class_type": "ImageScale",
        "inputs": {
            "image": ["3", 0],
            "upscale_method": "nearest-exact",
            "width": 512, "height": 512, "crop": "center"
        }
    },
    # IP-Adapter 적용
    "5": {
        "class_type": "IPAdapter",
        "inputs": {
            "model": ["2", 0], "ipadapter": ["2", 1], "image": ["4", 0],
            "weight": 0.85, "start_at": 0.0, "end_at": 1.0,
            "weight_type": "style transfer"
        }
    },
    # 포즈 레퍼런스 로드 및 OpenPose 추출
    "6": {
        "class_type": "LoadImage",
        "inputs": {"image": "POSE_IMAGE"}
    },
    "7": {
        "class_type": "OpenposePreprocessor",
        "inputs": {
            "image": ["6", 0],
            "detect_hand": "enable", "detect_body": "enable", "detect_face": "disable",
            "resolution": 512
        }
    },
    # ControlNet
    "8": {
        "class_type": "ControlNetLoader",
        "inputs": {"control_net_name": "controlnet-openpose-sdxl.safetensors"}
    },
    # 프롬프트
    "9": {
        "class_type": "CLIPTextEncode",
        "inputs": {"clip": ["1", 1], "text": "POSITIVE"}
    },
    "10": {
        "class_type": "CLIPTextEncode",
        "inputs": {"clip": ["1", 1], "text": "NEGATIVE"}
    },
    # ControlNet 적용
    "11": {
        "class_type": "ControlNetApplyAdvanced",
        "inputs": {
            "positive": ["9", 0], "negative": ["10", 0],
            "control_net": ["8", 0], "image": ["7", 0],
            "strength": 0.7, "start_percent": 0.0, "end_percent": 1.0
        }
    },
    # 생성
    "12": {
        "class_type": "EmptyLatentImage",
        "inputs": {"width": 512, "height": 512, "batch_size": 1}
    },
    "13": {
        "class_type": "KSampler",
        "inputs": {
            "model": ["5", 0],
            "positive": ["11", 0], "negative": ["11", 1],
            "latent_image": ["12", 0],
            "seed": 42, "steps": 25, "cfg": 7.0,
            "sampler_name": "euler", "scheduler": "normal", "denoise": 1.0
        }
    },
    "14": {
        "class_type": "VAEDecode",
        "inputs": {"vae": ["1", 2], "samples": ["13", 0]}
    },
    "15": {
        "class_type": "SaveImage",
        "inputs": {"images": ["14", 0], "filename_prefix": "OUTPUT"}
    }
}

def generate_frame(frame_num: int):
    """단일 프레임 생성"""
    # 파일 복사
    char_src = CHARACTER_INPUT / "01.png"
    char_dest = COMFYUI_INPUT / "char_01.png"
    shutil.copy(char_src, char_dest)
    
    pose_src = POSE_REF_DIR / f"attack_{frame_num:02d}.png"
    pose_dest = COMFYUI_INPUT / f"pose_{frame_num:02d}.png"
    shutil.copy(pose_src, pose_dest)
    
    # 워크플로우 설정
    workflow = json.loads(json.dumps(WORKFLOW))
    workflow["3"]["inputs"]["image"] = "char_01.png"
    workflow["6"]["inputs"]["image"] = f"pose_{frame_num:02d}.png"
    workflow["9"]["inputs"]["text"] = "pixel art warrior sprite, attack pose, sword swing, facing right, game asset, 16-bit style"
    workflow["10"]["inputs"]["text"] = "blurry, 3d, realistic, photo, different character, idle, standing"
    workflow["13"]["inputs"]["seed"] = 1000 + frame_num
    workflow["15"]["inputs"]["filename_prefix"] = f"warrior_attack/frame_{frame_num:02d}"
    
    print(f"[{frame_num}/8] 생성 중...")
    
    # 실행
    resp = requests.post(f"{COMFYUI_API}/prompt", json={"prompt": workflow}, timeout=30)
    if resp.status_code != 200:
        print(f"  [ERROR] {resp.status_code}: {resp.text[:200]}")
        return False
    
    prompt_id = resp.json().get("prompt_id")
    
    # 대기
    for _ in range(90):
        time.sleep(2)
        hist = requests.get(f"{COMFYUI_API}/history/{prompt_id}").json()
        if prompt_id in hist:
            status = hist[prompt_id].get("status", {}).get("status_str")
            if status == "success":
                outputs = hist[prompt_id].get("outputs", {}).get("15", {}).get("images", [])
                if outputs:
                    src_file = COMFYUI_OUTPUT / outputs[0]["subfolder"] / outputs[0]["filename"]
                    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
                    dest_file = OUTPUT_DIR / f"attack_v2_{frame_num:02d}.png"
                    shutil.copy(src_file, dest_file)
                    print(f"  -> {dest_file.relative_to(PROJECT_ROOT / 'ai_art_team')}")
                    return True
            elif status == "error":
                msgs = hist[prompt_id].get("status", {}).get("messages", [])
                for msg in msgs:
                    if msg[0] == "execution_error":
                        print(f"  [ERROR] {msg[1].get('exception_message', '')[:100]}")
                return False
    print("  [TIMEOUT]")
    return False

def main():
    print("=== warrior 공격 애니메이션 v2 ===")
    print("업스케일 + IP-Adapter + ControlNet OpenPose")
    print()
    
    for i in range(1, 9):
        generate_frame(i)
        time.sleep(2)
    
    print(f"\n완료! 결과: {OUTPUT_DIR.relative_to(PROJECT_ROOT)}")

if __name__ == "__main__":
    main()
