"""
fal.ai Video Generation Test
- 캐릭터 이미지 → 애니메이션 비디오
"""

import fal_client
import os
import requests
import time
from PIL import Image
import io

# API Key 로드
with open("C:/dev/nearest3/secret/txt/fal.txt", "r") as f:
    api_key = f.read().strip()

os.environ["FAL_KEY"] = api_key

def list_available_models():
    """사용 가능한 비디오 모델 확인"""
    print("=== fal.ai Video Models ===")
    models = [
        "fal-ai/stable-video",                    # Stable Video Diffusion
        "fal-ai/vidu/image-to-video",             # Vidu (고품질)
        "fal-ai/decart/lucy-5b/image-to-video",   # Lucy-5B (빠름)
    ]
    for m in models:
        print(f"  - {m}")
    return models

def upscale_image(image_path: str, scale: int = 8) -> str:
    """이미지를 업스케일 (nearest neighbor로 픽셀아트 보존)"""
    img = Image.open(image_path)
    new_size = (img.width * scale, img.height * scale)
    # NEAREST로 픽셀아트 스타일 유지
    upscaled = img.resize(new_size, Image.Resampling.NEAREST)
    
    # 임시 파일로 저장
    temp_path = image_path.replace(".png", f"_upscaled_{scale}x.png")
    upscaled.save(temp_path)
    print(f"Upscaled: {img.width}x{img.height} -> {new_size[0]}x{new_size[1]}")
    return temp_path

def generate_video_from_image(
    image_path: str,
    prompt: str,
    model: str = "fal-ai/vidu/image-to-video",
    output_path: str = None,
    upscale: int = 8
):
    """
    이미지에서 비디오 생성
    
    Args:
        image_path: 입력 이미지 경로
        prompt: 애니메이션 설명
        model: 사용할 모델
        output_path: 출력 비디오 경로
        upscale: 이미지 업스케일 배율
    """
    print(f"\n=== Generating Video ===")
    print(f"Model: {model}")
    print(f"Image: {image_path}")
    print(f"Prompt: {prompt}")
    
    # 이미지 업스케일 (64x64 -> 512x512)
    if upscale > 1:
        print(f"\nUpscaling image {upscale}x...")
        image_path = upscale_image(image_path, upscale)
    
    # 이미지를 fal에 업로드
    print("\nUploading image...")
    image_url = fal_client.upload_file(image_path)
    print(f"Uploaded: {image_url}")
    
    # 비디오 생성 요청
    print("\nGenerating video (this may take 1-3 minutes)...")
    
    try:
        # 모델별 파라미터 설정
        if "stable-video" in model:
            arguments = {
                "image_url": image_url,
            }
        elif "vidu" in model:
            arguments = {
                "image_url": image_url,
                "prompt": prompt,
                "aspect_ratio": "1:1",
                "duration": "4",  # 4초
            }
        elif "lucy" in model:
            arguments = {
                "image_url": image_url,
                "prompt": prompt,
            }
        else:
            arguments = {
                "image_url": image_url,
                "prompt": prompt,
            }
        
        print(f"\nArguments: {arguments}")
        
        result = fal_client.subscribe(
            model,
            arguments=arguments,
            with_logs=True,
        )
        
        print("\n=== Result ===")
        print(result)
        
        # 비디오 다운로드
        if "video" in result and "url" in result["video"]:
            video_url = result["video"]["url"]
            print(f"\nVideo URL: {video_url}")
            
            if output_path:
                print(f"Downloading to: {output_path}")
                response = requests.get(video_url)
                with open(output_path, "wb") as f:
                    f.write(response.content)
                print(f"Saved: {output_path} ({len(response.content)} bytes)")
        
        return result
        
    except Exception as e:
        print(f"Error: {e}")
        return None

def test_with_warrior():
    """warrior 캐릭터로 테스트"""
    image_path = "C:/dev/nearest3/ai_art_team/character/individual/01.png"
    
    # 출력 폴더 생성
    os.makedirs("C:/dev/nearest3/ai_art_team/character/animation_test", exist_ok=True)
    
    # Attack 애니메이션
    output_path = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior_attack.mp4"
    prompt = "pixel art warrior swinging sword attack animation, powerful slash motion, combat action"
    
    result = generate_video_from_image(
        image_path=image_path,
        prompt=prompt,
        model="fal-ai/vidu/image-to-video",
        output_path=output_path,
        upscale=8  # 64x64 -> 512x512
    )
    
    return result

if __name__ == "__main__":
    print("fal.ai Video Generation Test")
    print("=" * 50)
    
    # 사용 가능한 모델 목록
    list_available_models()
    
    # warrior 캐릭터 테스트
    print("\n" + "=" * 50)
    print("Testing with warrior character...")
    result = test_with_warrior()
