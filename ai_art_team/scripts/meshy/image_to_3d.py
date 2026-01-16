"""Meshy API: 2D 이미지 → 3D 모델 변환"""
import base64
import time
import requests
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.parent.parent
SECRET_FILE = PROJECT_ROOT / "secret" / "txt" / "meshy.txt"
CHARACTER_INPUT = PROJECT_ROOT / "ai_art_team" / "character" / "individual"
OUTPUT_DIR = PROJECT_ROOT / "ai_art_team" / "character" / "3d_models"

# API 키 로드
API_KEY = SECRET_FILE.read_text().strip()
API_BASE = "https://api.meshy.ai/openapi/v1"

HEADERS = {
    "Authorization": f"Bearer {API_KEY}",
    "Content-Type": "application/json"
}


def image_to_base64(image_path: Path) -> str:
    """이미지를 base64 data URI로 변환"""
    with open(image_path, "rb") as f:
        data = base64.b64encode(f.read()).decode("utf-8")
    return f"data:image/png;base64,{data}"


def create_task(image_path: Path) -> str | None:
    """Image-to-3D 태스크 생성"""
    image_uri = image_to_base64(image_path)
    
    payload = {
        "image_url": image_uri,
        "enable_pbr": True,
        "should_remesh": True,
        "should_texture": True
    }
    
    resp = requests.post(
        f"{API_BASE}/image-to-3d",
        headers=HEADERS,
        json=payload,
        timeout=60
    )
    
    if resp.status_code not in [200, 202]:
        print(f"[ERROR] {resp.status_code}: {resp.text[:300]}")
        return None
    
    task_id = resp.json().get("result")
    print(f"[OK] 태스크 생성: {task_id}")
    return task_id


def poll_task(task_id: str, timeout: int = 600) -> dict | None:
    """태스크 완료까지 폴링"""
    start = time.time()
    
    while time.time() - start < timeout:
        resp = requests.get(
            f"{API_BASE}/image-to-3d/{task_id}",
            headers=HEADERS,
            timeout=30
        )
        
        if resp.status_code != 200:
            print(f"[ERROR] 상태 조회 실패: {resp.status_code}")
            time.sleep(10)
            continue
        
        data = resp.json()
        status = data.get("status")
        progress = data.get("progress", 0)
        
        print(f"  상태: {status} | 진행: {progress}%")
        
        if status == "SUCCEEDED":
            return data
        elif status == "FAILED":
            error = data.get("task_error", {}).get("message", "Unknown")
            print(f"[FAILED] {error}")
            return None
        
        time.sleep(10)
    
    print("[TIMEOUT]")
    return None


def download_model(task_data: dict, output_name: str) -> bool:
    """3D 모델 다운로드"""
    model_urls = task_data.get("model_urls", {})
    
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    
    # GLB와 FBX 다운로드
    for fmt in ["glb", "fbx"]:
        url = model_urls.get(fmt)
        if not url:
            continue
        
        print(f"  다운로드: {fmt}...")
        resp = requests.get(url, timeout=120)
        if resp.status_code == 200:
            output_file = OUTPUT_DIR / f"{output_name}.{fmt}"
            with open(output_file, "wb") as f:
                f.write(resp.content)
            print(f"  -> {output_file.relative_to(PROJECT_ROOT)}")
    
    # 썸네일 다운로드
    thumb_url = task_data.get("thumbnail_url")
    if thumb_url:
        resp = requests.get(thumb_url, timeout=60)
        if resp.status_code == 200:
            thumb_file = OUTPUT_DIR / f"{output_name}_preview.png"
            with open(thumb_file, "wb") as f:
                f.write(resp.content)
            print(f"  -> {thumb_file.relative_to(PROJECT_ROOT)}")
    
    return True


def convert_character(char_id: str, char_name: str):
    """캐릭터 2D → 3D 변환"""
    image_path = CHARACTER_INPUT / f"{char_id}.png"
    
    if not image_path.exists():
        print(f"[ERROR] 이미지 없음: {image_path}")
        return False
    
    print(f"\n=== {char_name} ({char_id}) ===")
    print(f"입력: {image_path.relative_to(PROJECT_ROOT)}")
    
    # 1. 태스크 생성
    task_id = create_task(image_path)
    if not task_id:
        return False
    
    # 2. 완료 대기
    print("변환 중...")
    task_data = poll_task(task_id)
    if not task_data:
        return False
    
    # 3. 모델 다운로드
    print("다운로드 중...")
    download_model(task_data, char_name)
    
    print(f"[완료] {char_name}")
    return True


def main():
    print("=== Meshy Image-to-3D ===")
    print("warrior 캐릭터 테스트")
    
    # warrior (01) 테스트
    convert_character("01", "warrior")
    
    print(f"\n결과: {OUTPUT_DIR.relative_to(PROJECT_ROOT)}")


if __name__ == "__main__":
    main()
