"""
Warrior 8종 애니메이션 전체 파이프라인
- 6개 병렬 동영상 생성 (ATTACK 완료, TOMBSTONE 별도)
- 프레임 추출 + 스프라이트 시트 조립
"""

import fal_client
import os
import requests
import subprocess
import math
from PIL import Image
from concurrent.futures import ThreadPoolExecutor, as_completed
import time

# ============================================================
# 설정
# ============================================================

# API Key 로드
with open("C:/dev/nearest3/secret/txt/fal.txt", "r") as f:
    api_key = f.read().strip()
os.environ["FAL_KEY"] = api_key

# 경로
CHARACTER_IMAGE = "C:/dev/nearest3/ai_art_team/character/individual/01.png"
OUTPUT_DIR = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior"
FINAL_SPRITESHEET = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior_full_spritesheet.png"

# 애니메이션 정의 (8종 × 8프레임)
ANIMATIONS = {
    "idle": {
        "prompt": "pixel art warrior breathing idle animation, subtle movement, standing pose, looping",
        "frames": 8,
        "row": 0,
    },
    "run": {
        "prompt": "pixel art warrior running animation, legs moving, dynamic sprint cycle",
        "frames": 8,
        "row": 1,
    },
    "summon": {
        "prompt": "pixel art warrior appearing summoning animation, magical entrance, fade in glow",
        "frames": 8,
        "row": 2,
    },
    "dash": {
        "prompt": "pixel art warrior dashing forward, quick dash movement, speed blur motion",
        "frames": 8,
        "row": 3,
    },
    "hit": {
        "prompt": "pixel art warrior surprised reaction, startled flinch, step back motion",
        "frames": 8,
        "row": 4,
    },
    "die": {
        "prompt": "pixel art warrior falling down, laying on ground, sleep pose animation",
        "frames": 8,
        "row": 5,
    },
    "tombstone": {
        "prompt": "pixel art tombstone gravestone subtle wobble animation, eerie glow flicker",
        "frames": 8,
        "row": 6,
    },
    "attack": {
        "prompt": "pixel art warrior swinging sword attack animation, powerful slash motion",
        "frames": 8,
        "row": 7,
        "existing": True,  # 이미 생성됨
    },
}

# ============================================================
# 유틸리티 함수
# ============================================================

def upscale_image(image_path: str, scale: int = 8) -> str:
    """이미지를 업스케일 (NEAREST로 픽셀아트 보존)"""
    img = Image.open(image_path)
    new_size = (img.width * scale, img.height * scale)
    upscaled = img.resize(new_size, Image.Resampling.NEAREST)
    
    temp_path = os.path.join(OUTPUT_DIR, "upscaled_8x.png")
    upscaled.save(temp_path)
    print(f"  Upscaled: {img.width}x{img.height} -> {new_size[0]}x{new_size[1]}")
    return temp_path


def remove_white_background(img, threshold=240):
    """흰색 배경을 투명하게 변환"""
    img = img.convert('RGBA')
    data = list(img.getdata())
    
    new_data = []
    for item in data:
        if item[0] > threshold and item[1] > threshold and item[2] > threshold:
            new_data.append((255, 255, 255, 0))
        else:
            new_data.append(item)
    
    img.putdata(new_data)
    return img


# ============================================================
# 동영상 생성
# ============================================================

def generate_video(anim_name: str, prompt: str, image_url: str) -> dict:
    """단일 애니메이션 동영상 생성"""
    print(f"\n[{anim_name}] Generating video...")
    print(f"  Prompt: {prompt[:60]}...")
    
    try:
        result = fal_client.subscribe(
            "fal-ai/vidu/image-to-video",
            arguments={
                "image_url": image_url,
                "prompt": prompt,
                "aspect_ratio": "1:1",
                "duration": "4",
            },
            with_logs=False,
        )
        
        if "video" in result and "url" in result["video"]:
            video_url = result["video"]["url"]
            output_path = os.path.join(OUTPUT_DIR, f"{anim_name}.mp4")
            
            response = requests.get(video_url)
            with open(output_path, "wb") as f:
                f.write(response.content)
            
            print(f"[{anim_name}] OK Saved: {output_path}")
            return {"name": anim_name, "success": True, "path": output_path}
        else:
            print(f"[{anim_name}] FAIL No video in result")
            return {"name": anim_name, "success": False, "error": "No video URL"}
            
    except Exception as e:
        print(f"[{anim_name}] FAIL Error: {e}")
        return {"name": anim_name, "success": False, "error": str(e)}


def generate_all_videos():
    """6개 애니메이션 병렬 생성"""
    print("\n" + "=" * 60)
    print("STEP 1: Generate Videos (parallel)")
    print("=" * 60)
    
    # 출력 폴더 생성
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    
    # 이미지 업스케일 및 업로드
    print("\nPreparing image...")
    upscaled_path = upscale_image(CHARACTER_IMAGE)
    image_url = fal_client.upload_file(upscaled_path)
    print(f"  Uploaded: {image_url[:50]}...")
    
    # 생성할 애니메이션 목록 (attack 제외)
    to_generate = [
        (name, data["prompt"])
        for name, data in ANIMATIONS.items()
        if not data.get("existing", False)
    ]
    
    print(f"\nGenerating {len(to_generate)} animations in parallel...")
    
    results = []
    with ThreadPoolExecutor(max_workers=6) as executor:
        futures = {
            executor.submit(generate_video, name, prompt, image_url): name
            for name, prompt in to_generate
        }
        
        for future in as_completed(futures):
            result = future.result()
            results.append(result)
    
    # 결과 요약
    print("\n" + "-" * 40)
    print("Video Generation Results:")
    success = sum(1 for r in results if r["success"])
    print(f"  Success: {success}/{len(to_generate)}")
    for r in results:
        status = "OK" if r["success"] else "FAIL"
        print(f"  [{status}] {r['name']}")
    
    return results


# ============================================================
# 프레임 추출
# ============================================================

def extract_frames(video_path: str, output_dir: str, num_frames: int = 40):
    """동영상에서 프레임 추출 (ffmpeg)"""
    os.makedirs(output_dir, exist_ok=True)
    
    cmd = [
        "ffmpeg", "-y",
        "-i", video_path,
        "-vf", f"fps={num_frames}/4",  # 4초에서 num_frames개 추출
        os.path.join(output_dir, "frame_%03d.png")
    ]
    
    subprocess.run(cmd, capture_output=True)
    
    frames = sorted([f for f in os.listdir(output_dir) if f.endswith('.png')])
    return frames


def extract_all_frames():
    """모든 동영상에서 프레임 추출"""
    print("\n" + "=" * 60)
    print("STEP 2: Extract Frames")
    print("=" * 60)
    
    for anim_name in ANIMATIONS.keys():
        if anim_name == "attack":
            # 기존 attack 프레임 사용
            frames_dir = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior_attack_frames"
            if os.path.exists(frames_dir):
                print(f"[{anim_name}] Using existing frames")
                continue
        
        video_path = os.path.join(OUTPUT_DIR, f"{anim_name}.mp4")
        if not os.path.exists(video_path):
            print(f"[{anim_name}] SKIP Video not found")
            continue
        
        frames_dir = os.path.join(OUTPUT_DIR, f"{anim_name}_frames")
        frames = extract_frames(video_path, frames_dir)
        print(f"[{anim_name}] OK Extracted {len(frames)} frames")


# ============================================================
# 스프라이트 시트 생성
# ============================================================

def create_animation_row(frames_dir: str, num_frames: int = 8, frame_size: int = 64) -> Image.Image:
    """프레임들을 한 행으로 조립"""
    frame_files = sorted([f for f in os.listdir(frames_dir) if f.endswith('.png')])
    
    if len(frame_files) == 0:
        # 빈 행 반환
        return Image.new('RGBA', (frame_size * num_frames, frame_size), (0, 0, 0, 0))
    
    # 균등 샘플링
    if len(frame_files) > num_frames:
        step = len(frame_files) / num_frames
        indices = [int(i * step) for i in range(num_frames)]
        frame_files = [frame_files[i] for i in indices]
    
    # 행 이미지 생성
    row_img = Image.new('RGBA', (frame_size * num_frames, frame_size), (0, 0, 0, 0))
    
    for i, frame_file in enumerate(frame_files[:num_frames]):
        frame_path = os.path.join(frames_dir, frame_file)
        frame = Image.open(frame_path).convert('RGBA')
        
        # 흰색 배경 제거
        frame = remove_white_background(frame)
        
        # 다운스케일
        frame_resized = frame.resize((frame_size, frame_size), Image.Resampling.NEAREST)
        
        # 배치
        row_img.paste(frame_resized, (i * frame_size, 0), frame_resized)
    
    return row_img


def create_full_spritesheet():
    """8행 × 8열 통합 스프라이트 시트 생성"""
    print("\n" + "=" * 60)
    print("STEP 3: Create Full Spritesheet")
    print("=" * 60)
    
    frame_size = 64
    num_frames = 8
    num_rows = 8
    
    # 전체 스프라이트 시트 (512 x 512)
    full_sheet = Image.new('RGBA', (frame_size * num_frames, frame_size * num_rows), (0, 0, 0, 0))
    
    for anim_name, data in ANIMATIONS.items():
        row_idx = data["row"]
        
        # 프레임 폴더 경로
        if anim_name == "attack":
            frames_dir = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior_attack_frames"
        else:
            frames_dir = os.path.join(OUTPUT_DIR, f"{anim_name}_frames")
        
        if not os.path.exists(frames_dir):
            print(f"[Row {row_idx}] {anim_name}: SKIP Frames not found")
            continue
        
        # 행 생성
        row_img = create_animation_row(frames_dir, num_frames, frame_size)
        
        # 전체 시트에 배치
        full_sheet.paste(row_img, (0, row_idx * frame_size))
        print(f"[Row {row_idx}] {anim_name}: OK")
    
    # 저장
    full_sheet.save(FINAL_SPRITESHEET)
    print(f"\nSaved: {FINAL_SPRITESHEET}")
    print(f"  Size: {full_sheet.width}x{full_sheet.height}")
    
    # 128x128 버전도 생성
    full_sheet_128 = full_sheet.resize(
        (frame_size * 2 * num_frames, frame_size * 2 * num_rows),
        Image.Resampling.NEAREST
    )
    sheet_128_path = FINAL_SPRITESHEET.replace(".png", "_128.png")
    full_sheet_128.save(sheet_128_path)
    print(f"Saved: {sheet_128_path}")
    
    return full_sheet


# ============================================================
# 메인
# ============================================================

def main():
    print("=" * 60)
    print("WARRIOR ANIMATION PIPELINE")
    print("8 animations × 8 frames = 64 total frames")
    print("=" * 60)
    
    start_time = time.time()
    
    # Step 1: 동영상 생성 (병렬)
    generate_all_videos()
    
    # Step 2: 프레임 추출
    extract_all_frames()
    
    # Step 3: 스프라이트 시트 조립
    create_full_spritesheet()
    
    elapsed = time.time() - start_time
    print("\n" + "=" * 60)
    print(f"COMPLETE! Total time: {elapsed:.1f}s ({elapsed/60:.1f}min)")
    print("=" * 60)


if __name__ == "__main__":
    main()
