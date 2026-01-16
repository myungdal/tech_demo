"""T-pose 이미지 생성 후 Meshy 3D 변환"""
import base64
import json
import time
import requests
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.parent.parent
SECRET_DIR = PROJECT_ROOT / "secret"
OUTPUT_DIR = PROJECT_ROOT / "ai_art_team" / "character" / "3d_models"
TPOSE_DIR = PROJECT_ROOT / "ai_art_team" / "character" / "tpose_reference"

# API 키
OPENAI_KEY = json.load(open(SECRET_DIR / "ai_api_key.json"))["OPENAI_API_KEY"]
MESHY_KEY = (SECRET_DIR / "txt" / "meshy.txt").read_text().strip()

def generate_tpose_image(char_name: str, char_desc: str) -> Path | None:
    """OpenAI로 T-pose 이미지 생성"""
    prompt = f"""Create a T-pose character reference image for 3D rigging:

Character: {char_name} - {char_desc}

CRITICAL REQUIREMENTS:
- T-POSE: Arms straight out horizontally, legs slightly apart
- Front facing view
- Full body visible from head to feet
- Simple solid color background (white or light gray)
- Clear silhouette for 3D scanning
- LOW POLY style, minimal details, flat colors
- No weapons in hands (hands open, palms facing forward)
- Standing straight, symmetrical pose

Style: LOW POLY game character, simple geometric shapes, flat shading, minimal polygons, mobile game style"""

    print(f"[1/3] T-pose 이미지 생성 중...")
    
    headers = {
        "Authorization": f"Bearer {OPENAI_KEY}",
        "Content-Type": "application/json"
    }
    
    resp = requests.post(
        "https://api.openai.com/v1/images/generations",
        headers=headers,
        json={
            "model": "gpt-image-1",
            "prompt": prompt,
            "n": 1,
            "size": "1024x1024",
            "quality": "high"
        },
        timeout=120
    )
    
    if resp.status_code != 200:
        print(f"  [ERROR] {resp.status_code}: {resp.text[:200]}")
        return None
    
    result = resp.json()
    TPOSE_DIR.mkdir(parents=True, exist_ok=True)
    output_file = TPOSE_DIR / f"{char_name}_tpose.png"
    
    if "data" in result and result["data"]:
        img_data = result["data"][0]
        if "b64_json" in img_data:
            img_bytes = base64.b64decode(img_data["b64_json"])
        elif "url" in img_data:
            img_bytes = requests.get(img_data["url"], timeout=60).content
        else:
            print("  [ERROR] 이미지 데이터 없음")
            return None
        
        output_file.write_bytes(img_bytes)
        print(f"  -> {output_file.relative_to(PROJECT_ROOT)}")
        return output_file
    
    return None


def convert_to_3d(image_path: Path, char_name: str) -> bool:
    """Meshy로 3D 변환"""
    print(f"[2/3] Meshy 3D 변환 중...")
    
    # base64 인코딩
    img_b64 = base64.b64encode(image_path.read_bytes()).decode()
    image_uri = f"data:image/png;base64,{img_b64}"
    
    headers = {
        "Authorization": f"Bearer {MESHY_KEY}",
        "Content-Type": "application/json"
    }
    
    # 태스크 생성
    resp = requests.post(
        "https://api.meshy.ai/openapi/v1/image-to-3d",
        headers=headers,
        json={
            "image_url": image_uri,
            "enable_pbr": True,
            "should_remesh": True,
            "should_texture": True
        },
        timeout=60
    )
    
    if resp.status_code not in [200, 202]:
        print(f"  [ERROR] {resp.status_code}: {resp.text[:200]}")
        return False
    
    task_id = resp.json().get("result")
    print(f"  태스크: {task_id}")
    
    # 폴링
    print(f"[3/3] 변환 대기 중...")
    for i in range(60):
        time.sleep(10)
        resp = requests.get(
            f"https://api.meshy.ai/openapi/v1/image-to-3d/{task_id}",
            headers=headers
        )
        data = resp.json()
        status = data.get("status")
        progress = data.get("progress", 0)
        print(f"  {status} - {progress}%")
        
        if status == "SUCCEEDED":
            # 다운로드
            OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
            for fmt in ["glb", "fbx"]:
                url = data.get("model_urls", {}).get(fmt)
                if url:
                    content = requests.get(url, timeout=120).content
                    out_file = OUTPUT_DIR / f"{char_name}_tpose.{fmt}"
                    out_file.write_bytes(content)
                    print(f"  -> {out_file.relative_to(PROJECT_ROOT)}")
            
            # 썸네일
            thumb = data.get("thumbnail_url")
            if thumb:
                content = requests.get(thumb, timeout=60).content
                out_file = OUTPUT_DIR / f"{char_name}_tpose_preview.png"
                out_file.write_bytes(content)
                print(f"  -> {out_file.relative_to(PROJECT_ROOT)}")
            
            return True
        
        elif status == "FAILED":
            print(f"  [FAILED] {data.get('task_error', {})}")
            return False
    
    print("  [TIMEOUT]")
    return False


def main():
    print("=== T-pose 3D 모델 생성 ===")
    print()
    
    # warrior 캐릭터
    char_name = "warrior"
    char_desc = "medieval knight warrior with armor, sword and shield, strong muscular build"
    
    # 1. T-pose 이미지 생성
    tpose_image = generate_tpose_image(char_name, char_desc)
    if not tpose_image:
        return
    
    # 2. 3D 변환
    success = convert_to_3d(tpose_image, char_name)
    
    if success:
        print(f"\n[완료] {OUTPUT_DIR.relative_to(PROJECT_ROOT)}")
    else:
        print("\n[실패]")


if __name__ == "__main__":
    main()
