"""OpenAI API로 공격 포즈 레퍼런스 생성"""
import os
import json
import base64
import requests
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.parent.parent
SECRET_FILE = PROJECT_ROOT / "secret" / "ai_api_key.json"
OUTPUT_DIR = PROJECT_ROOT / "ai_art_team" / "character" / "pose_reference" / "attack"

# API 키 로드
with open(SECRET_FILE) as f:
    config = json.load(f)
    API_KEY = config["OPENAI_API_KEY"]

ATTACK_POSES = [
    "warrior standing ready, sword raised above head, preparing to strike, side view facing right",
    "warrior winding up, sword pulled back behind shoulder, weight on back foot, side view facing right",
    "warrior starting swing, sword moving forward, aggressive stance, side view facing right",
    "warrior mid-swing, sword horizontal at chest level, full power, side view facing right",
    "warrior striking, sword fully extended forward, impact moment, side view facing right",
    "warrior follow through, sword past target, momentum carrying, side view facing right",
    "warrior recovering, pulling sword back, regaining balance, side view facing right",
    "warrior returning to ready stance, sword raised, alert, side view facing right",
]

def generate_pose(frame_num: int, pose_desc: str):
    """단일 포즈 레퍼런스 생성"""
    prompt = f"""Simple illustration of a human figure in this pose: {pose_desc}

Requirements:
- Full body visible
- Clear silhouette
- Simple clothing (t-shirt and pants)
- White or plain background
- No weapons, just the pose
- Side view, facing right
- Standing pose for OpenPose detection"""

    print(f"[{frame_num}/8] 생성 중...")
    
    headers = {
        "Authorization": f"Bearer {API_KEY}",
        "Content-Type": "application/json"
    }
    
    data = {
        "model": "gpt-image-1",
        "prompt": prompt,
        "n": 1,
        "size": "1024x1024",
        "quality": "medium"
    }
    
    resp = requests.post(
        "https://api.openai.com/v1/images/generations",
        headers=headers,
        json=data,
        timeout=120
    )
    
    if resp.status_code != 200:
        print(f"  [ERROR] {resp.status_code}: {resp.text[:200]}")
        return False
    
    result = resp.json()
    
    # 이미지 저장
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    
    if "data" in result and len(result["data"]) > 0:
        img_data = result["data"][0]
        
        if "b64_json" in img_data:
            img_bytes = base64.b64decode(img_data["b64_json"])
            output_file = OUTPUT_DIR / f"attack_{frame_num:02d}.png"
            with open(output_file, "wb") as f:
                f.write(img_bytes)
            print(f"  -> {output_file.relative_to(PROJECT_ROOT / 'ai_art_team')}")
            return True
        elif "url" in img_data:
            img_url = img_data["url"]
            img_resp = requests.get(img_url, timeout=60)
            if img_resp.status_code == 200:
                output_file = OUTPUT_DIR / f"attack_{frame_num:02d}.png"
                with open(output_file, "wb") as f:
                    f.write(img_resp.content)
                print(f"  -> {output_file.relative_to(PROJECT_ROOT / 'ai_art_team')}")
                return True
    
    print(f"  [ERROR] 이미지 데이터 없음")
    return False

def main():
    print("=== 공격 포즈 레퍼런스 생성 (OpenAI API) ===")
    print(f"출력: {OUTPUT_DIR.relative_to(PROJECT_ROOT)}")
    print()
    
    for i, pose in enumerate(ATTACK_POSES, 1):
        generate_pose(i, pose)
    
    print("\n완료!")

if __name__ == "__main__":
    main()
